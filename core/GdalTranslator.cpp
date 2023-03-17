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

// Includes Estandar
#include <vector>
#include <string>
// Includes Suri
#include "GdalTranslator.h"
#include "suri/GcpList.h"
#include <suri/RasterSpatialModel.h>
// Includes Wx
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
// forwards

namespace suri {
   
const std::string GdalTranslator::kDefaultDriver("GTiff");

std::vector<GDAL_GCP> GcpListToGdalGcp(const GcpList& gcpList);

GdalTranslator::GdalTranslator(const std::string& SrcFilename,
                         const std::string& DestFilename,
                         const std::vector<int>& SelectedBands) :
                    srcFilename(SrcFilename), destFilename(DestFilename),
                    selectedBands(SelectedBands){}

void GdalTranslator::Translate(const GcpList& gcps) {
   GDALDatasetH srcds, destds;
   
   std::vector<GDAL_GCP> gdalGcps(GcpListToGdalGcp(gcps));
   
   srcds = GDALOpen(this->srcFilename.c_str(), GA_ReadOnly);
   double xSizeSrc = GDALGetRasterXSize(srcds);
   double ySizeSrc = GDALGetRasterYSize(srcds);
   const char* pszProjection = GDALGetProjectionRef(srcds);
   VRTDataset* pvirtds = NULL;
   
   pvirtds = reinterpret_cast<VRTDataset *>(VRTCreate(xSizeSrc, ySizeSrc));
   
   double adfGeoTransform[6] = {0, 1, 0, 0, 0, 1};
   pvirtds->SetGeoTransform(adfGeoTransform);
   
   char** ppmetadata = CSLDuplicate(((GDALDataset*)srcds)->GetMetadata());
   pvirtds->SetMetadata(ppmetadata);
   CSLDestroy(ppmetadata);

   const char* pinterleave = GDALGetMetadataItem(srcds, "INTERLEAVE",
                                                   "IMAGE_STRUCTURE");
   if (pinterleave){
      pvirtds->SetMetadataItem("INTERLEAVE", pinterleave, "IMAGE_STRUCTURE");
   }

   // Mantengo datos espaciales.
   char** ppmd = ((GDALDataset*) srcds)->GetMetadata("RPC");
   if (ppmd != NULL) pvirtds->SetMetadata(ppmd, "RPC");

   ppmd = ((GDALDataset*) srcds)->GetMetadata("GEOLOCATION");
   if (ppmd != NULL) pvirtds->SetMetadata(ppmd, "GEOLOCATION");
   
   if (pszProjection != NULL && strlen(pszProjection) > 0) {
      pvirtds->SetProjection(pszProjection);
   }
   if (gdalGcps.size() > 0){
      const char *pgcpprojection = GDALGetGCPProjection(srcds);
      pvirtds->SetGCPs(gdalGcps.size(), &gdalGcps.front(), pgcpprojection);
   }
   
   for (unsigned int ix = 0; ix < selectedBands.size(); ++ix) {
      VRTSourcedRasterBand* pvirtband;
      GDALRasterBand* psrcband;

      int nSrcBand = selectedBands[ix] + 1;

      psrcband = ((GDALDataset *) srcds)->GetRasterBand(ABS(nSrcBand));

      pvirtds->AddBand(psrcband->GetRasterDataType(), NULL);
      pvirtband = (VRTSourcedRasterBand *) pvirtds->GetRasterBand(ix + 1);
      if (nSrcBand < 0) {
         pvirtband->AddMaskBandSource(psrcband);
         continue;
      }

      pvirtband->AddSimpleSource(psrcband);

      CopyBandInfo(psrcband, pvirtband);
   }

   // Mando al disco.
   GDALDriverH hDriver = GDALGetDriverByName(kDefaultDriver.c_str());
   destds = GDALCreateCopy(hDriver, this->destFilename.c_str(),
                           (GDALDatasetH) pvirtds, FALSE, NULL,
                           NULL, NULL);

   if (destds != NULL) {
      int hasgoterr = FALSE;
      CPLErrorReset();
      GDALFlushCache(destds);
      if (CPLGetLastErrorType() != CE_None) hasgoterr = TRUE;
      GDALClose(destds);
      if (hasgoterr) destds = NULL;
   }

   GDALClose((GDALDatasetH) pvirtds);
   GDALClose(srcds);
}

/**
 * Copia los datos de las bandas
 */
void GdalTranslator::CopyBandInfo(GDALRasterBand* SrcBand, GDALRasterBand* DstBand) {
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

bool GdalTranslator::IsSelectedBand(int nBand) const{
   std::vector<int>::const_iterator it = this->selectedBands.begin();
   for (; it != this->selectedBands.end(); ++it) {
      if (((*it)+1) == nBand) {
         return true;
      }
   }
   return false;
}

std::vector<GDAL_GCP> GcpListToGdalGcp(const GcpList& gcpList){
   unsigned int size = gcpList.Size();
   std::vector<GDAL_GCP> gdalGcps(size);
   std::vector<GroundControlPoint> surigcps = gcpList.CreateAdaptedGcps();
   
   RasterSpatialModel* pRasterModel = RasterSpatialModel::Create(
      gcpList.GetRasterModelSource());
   
   for (int ix = 0, lenix = size; ix < lenix; ++ix) {
      Coordinates src = surigcps[ix].GetSource();
      Coordinates dest = surigcps[ix].GetDestination();

      if (pRasterModel)
         pRasterModel->Transform(src);

      char* pszid = new char[11];
      sprintf(pszid, "%d", ix + 1);

      gdalGcps[ix].pszId = pszid;
      gdalGcps[ix].pszInfo = const_cast<char*>(std::string("").c_str());
      gdalGcps[ix].dfGCPPixel = dest.x_;
      gdalGcps[ix].dfGCPLine = dest.y_;
      gdalGcps[ix].dfGCPX = src.x_;
      gdalGcps[ix].dfGCPY = src.y_;
      gdalGcps[ix].dfGCPZ = 0.0;
   }
   RasterSpatialModel::Destroy(pRasterModel);
   return gdalGcps;
}

}