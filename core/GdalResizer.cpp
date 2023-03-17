/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#include "GdalResizer.h"
// Includes Estandar
#include <algorithm>
#include <string>
// Includes Suri
#include "suri/RasterProcess.h"
// Includes Wx
#include "wx/progdlg.h"
#include "wx/filename.h"
// Include GDAL
#include "cpl_conv.h"
#include "cpl_multiproc.h"
#include "cpl_string.h"
#include "cpl_vsi.h"
#include "gdal_priv.h"
#include "gdal.h"
#include "gdalwarper.h"
#include "ogr_spatialref.h"
#include "vrtdataset.h"
// Defines
// forwards
namespace suri {
   
const std::string GdalResizer::kDefaultDriver("GTiff");
   
/**
 * Funcion callback para mostrar avance del proceso.
 */
int CPL_DLL CPL_STDCALL ProgressProc(double Complete, const char* pMessage, void* pProgressArg) {
   if (pProgressArg != NULL) {
      wxProgressDialog* pprogress = static_cast<wxProgressDialog*>(pProgressArg);
      pprogress->Update(static_cast<int>(Complete * 10));
   }
   return TRUE;
}

GdalResizer::GdalResizer(const std::string& SrcFilename,
                         const std::string& DestFilename,
                         const std::vector<int>& SelectedBands) :
                    srcFilename(SrcFilename), destFilename(DestFilename),
                    selectedBands(SelectedBands){}

void GdalResizer::Resize(const Subset& outputSubset,
                         int outXSize, int outYSize,
                         double pixelXSize, double pixelYSize){
   GDALDatasetH srcds, destds;

   srcds = GDALOpen(this->srcFilename.c_str(), GA_ReadOnly);
   const char* pszProjection = GDALGetProjectionRef(srcds);
   double xSizeSrc = GDALGetRasterXSize(srcds);
   double ySizeSrc = GDALGetRasterYSize(srcds);
   VRTDataset* pvirtds = NULL;
   pvirtds = reinterpret_cast<VRTDataset *>(VRTCreate(outXSize, outYSize));
   if (pszProjection != NULL && strlen(pszProjection) > 0)
      pvirtds->SetProjection(pszProjection);
   double adfGeoTransform[6] = { 0 };
   GDALGetGeoTransform(srcds, adfGeoTransform);
   int ofx = static_cast<int>(
              floor((outputSubset.ul_.x_- adfGeoTransform[0]) / adfGeoTransform[1] + 0.001));
   int ofy = static_cast<int>(
               floor((outputSubset.ul_.y_ - adfGeoTransform[3]) / adfGeoTransform[5] + 0.001));
   if (ofx > 0) {
      xSizeSrc-= ofx;
   }
   if (ofy > 0) {
      ySizeSrc-= ofy;
   }
   adfGeoTransform[0]=outputSubset.ul_.x_;
   adfGeoTransform[3]=outputSubset.ul_.y_;
   adfGeoTransform[1]=pixelXSize;
   adfGeoTransform[5]=pixelYSize;
   pvirtds->SetGeoTransform(adfGeoTransform);
   char** ppmetadata = CSLDuplicate((reinterpret_cast<GDALDataset*>(srcds))->GetMetadata());
   pvirtds->SetMetadata(ppmetadata);
   CSLDestroy(ppmetadata);
   const char* pinterleave = GDALGetMetadataItem(srcds, "INTERLEAVE",
                                                  "IMAGE_STRUCTURE");
   if (pinterleave)
      pvirtds->SetMetadataItem("INTERLEAVE", pinterleave, "IMAGE_STRUCTURE");
   // Mantengo datos espaciales.
   char** ppmd = (reinterpret_cast<GDALDataset*>(srcds))->GetMetadata("RPC");
   if (ppmd != NULL) pvirtds->SetMetadata(ppmd, "RPC");
   ppmd = (reinterpret_cast<GDALDataset*>(srcds))->GetMetadata("GEOLOCATION");
   if (ppmd != NULL) pvirtds->SetMetadata(ppmd, "GEOLOCATION");
   // Copio las bandas.
   int virtbands = 0;
   for (int ix = 0; ix < GDALGetRasterCount(srcds); ++ix) {
      VRTSourcedRasterBand* pvirtband;
      GDALRasterBand* psrcband;
      int nSrcBand = ix + 1;
      if (IsSelectedBand(nSrcBand)) {
         psrcband = (reinterpret_cast<GDALDataset*>(srcds))->GetRasterBand(ABS(nSrcBand));
         pvirtds->AddBand(psrcband->GetRasterDataType(), NULL);
         virtbands+=1;
         pvirtband = reinterpret_cast<VRTSourcedRasterBand*>(pvirtds->GetRasterBand(virtbands));
         if (nSrcBand < 0) {
            pvirtband->AddMaskBandSource(psrcband);
            continue;
         }
         pvirtband->AddSimpleSource(psrcband, ofx, ofy, xSizeSrc,
                                    ySizeSrc, 0, 0, outXSize, outYSize);
         CopyBandInfo(psrcband, pvirtband);
      }
   }
   wxProgressDialog pprogress(_(caption_OPERATION_IN_PROGRESS), "", 10);
   pprogress.SetSize(wxSize(400, -1));
   pprogress.Centre(wxCENTRE_ON_SCREEN);
   GDALDriverH hDriver = GDALGetDriverByName(kDefaultDriver.c_str());
   destds = GDALCreateCopy(hDriver, this->destFilename.c_str(), (GDALDatasetH) pvirtds, FALSE, NULL,
                           ProgressProc, &pprogress);
   if (destds != NULL) {
      CPLErrorReset();
      GDALFlushCache(destds);
      GDALClose(destds);
   }
   GDALClose((GDALDatasetH) pvirtds);
   GDALClose(srcds);
   CPLCleanupTLS();
}

void GdalResizer::Resize(const World* pOutputWorld){
   Subset outputSubset;
   pOutputWorld->GetWindow(outputSubset);
   int outXSize = 0, outYSize = 0;
   pOutputWorld->GetViewport(outXSize, outYSize);
   double pixelXSize = 0, pixelYSize = 0;
   pOutputWorld->GetPixelSize(pixelXSize, pixelYSize);
   Resize(outputSubset, outXSize, outYSize, pixelXSize, pixelYSize);
}

/**
 * Copia informacion de una banda.
 */
void GdalResizer::CopyBandInfo(GDALRasterBand* SrcBand, GDALRasterBand* DstBand) {
   char** metadata = SrcBand->GetMetadata();
   char** metadatanew = NULL;
   for (int i = 0; metadata != NULL && metadata[i] != NULL; i++) {
      if (strncmp(metadata[i], "STATISTICS_", 11) != 0)
         metadatanew = CSLAddString(metadatanew, metadata[i]);
   }
   DstBand->SetMetadata(metadatanew);
   CSLDestroy(metadatanew);
   DstBand->SetColorTable(SrcBand->GetColorTable());
   DstBand->SetColorInterpretation(SrcBand->GetColorInterpretation());
   if (strlen(SrcBand->GetDescription()) > 0){
      DstBand->SetDescription(SrcBand->GetDescription());
   }
   DstBand->SetOffset(SrcBand->GetOffset());
   DstBand->SetScale(SrcBand->GetScale());
   DstBand->SetCategoryNames(SrcBand->GetCategoryNames());
   if (!EQUAL(SrcBand->GetUnitType(), "")){
      DstBand->SetUnitType(SrcBand->GetUnitType());
   }
}

bool GdalResizer::IsSelectedBand(int nBand) const{
   std::vector<int>::const_iterator it = this->selectedBands.begin();
   for (; it != this->selectedBands.end(); ++it) {
      if (((*it)+1) == nBand) {
         return true;
      }
   }
   return false;
}

} // namespace suri
