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

#include <string>
#include <vector>
#include <cstring>

#include "SRSHSVSharpening.h"
#include "SRSSharpeningFactory.h"
#include "SRSSharpeningUtils.h"

#include "gdal.h"
#include "gdalwarper.h"
#include "cpl_vsi.h"
#include "cpl_conv.h"
#include "cpl_string.h"
#include "cpl_multiproc.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"

#define SHARPENING_NATIVE_TYPE   unsigned char
#define SHARPENING_LIB_TYPE      GDT_Byte
#define SHARPENING_LIB_DRIVER    "GTiff"

namespace suri {
namespace raster {
namespace sharpening {

REGISTER_SHARPENING(HSVSharpening)

/**
 * Ctor.
 */
HSVSharpening::HSVSharpening(const std::string& SrcFilename,
                             const std::string& PanFilename,
                             const std::string& DestFilename,
                             const std::vector<int>& SrcBands, int PanBand) :
      Sharpening(SrcFilename, PanFilename, DestFilename, SrcBands, PanBand) {

}

/**
 * Dtor.
 */
HSVSharpening::~HSVSharpening() {

}

/**
 * Implementa el procedimiento de fusion de bandas con pancromatica por HSV.
 */
void HSVSharpening::Transform(CBSHARPENINGPROGRESS ProgressCB, void* pProgressCBData) {
   namespace SRSU = suri::raster::sharpening::utils;

   GDALDatasetH srcds, pands, destds;
   GDALRasterBandH panband;
   int nXSize = 0, nYSize = 0;

   // entrada (ambos tienen que estar transformados valores entre 0 y 255)
   srcds = GDALOpen(srcFilename_.c_str(), GA_Update);
   pands = GDALOpen(panFilename_.c_str(), GA_ReadOnly);

   nXSize = GDALGetRasterXSize(pands);
   nYSize = GDALGetRasterYSize(pands);

   GDALDriverH hDriver = GDALGetDriverByName(SHARPENING_LIB_DRIVER);
   destds = GDALCreate(hDriver, destFilename_.c_str(), nXSize, nYSize, kDestBandCount,
                       SHARPENING_LIB_TYPE, NULL);

   CopyStandardData(pands, destds);

   GDALRasterBandH hRedBand = GDALGetRasterBand(srcds, srcBands_[0] + 1);
   GDALRasterBandH hGreenBand = GDALGetRasterBand(srcds, srcBands_[1] + 1);
   GDALRasterBandH hBlueBand = GDALGetRasterBand(srcds, srcBands_[2] + 1);
   panband = GDALGetRasterBand(pands, panBand_ + 1);

   GDALRasterBandH hRedBandPan = GDALGetRasterBand(destds, 1);
   GDALRasterBandH hGreenBandPan = GDALGetRasterBand(destds, 2);
   GDALRasterBandH hBlueBandPan = GDALGetRasterBand(destds, 3);

   SHARPENING_NATIVE_TYPE* pafSLRed = (SHARPENING_NATIVE_TYPE*) CPLMalloc(sizeof(SHARPENING_NATIVE_TYPE) * nXSize);
   SHARPENING_NATIVE_TYPE* pafSLGreen = (SHARPENING_NATIVE_TYPE*) CPLMalloc(sizeof(SHARPENING_NATIVE_TYPE) * nXSize);
   SHARPENING_NATIVE_TYPE* pafSLBlue = (SHARPENING_NATIVE_TYPE*) CPLMalloc(sizeof(SHARPENING_NATIVE_TYPE) * nXSize);
   SHARPENING_NATIVE_TYPE* pafPan = (SHARPENING_NATIVE_TYPE*) CPLMalloc(sizeof(SHARPENING_NATIVE_TYPE) * nXSize);

   SRSU::RGB* prgb = new SRSU::RGB[nXSize];
   SRSU::HSV* phsv = new SRSU::HSV[nXSize];

   for (int Yix = 0; Yix < nYSize; ++Yix) {
      GDALRasterIO(hRedBand, GF_Read, 0, Yix, nXSize, 1, pafSLRed, nXSize, 1, SHARPENING_LIB_TYPE,
                   0, 0);
      GDALRasterIO(hGreenBand, GF_Read, 0, Yix, nXSize, 1, pafSLGreen, nXSize, 1,
                   SHARPENING_LIB_TYPE, 0, 0);
      GDALRasterIO(hBlueBand, GF_Read, 0, Yix, nXSize, 1, pafSLBlue, nXSize, 1,
                   SHARPENING_LIB_TYPE, 0, 0);
      GDALRasterIO(panband, GF_Read, 0, Yix, nXSize, 1, pafPan, nXSize, 1,
                   SHARPENING_LIB_TYPE, 0, 0);

      for (int pix = 0; pix < nXSize; ++pix) {
         prgb[pix].red = pafSLRed[pix];
         prgb[pix].green = pafSLGreen[pix];
         prgb[pix].blue = pafSLBlue[pix];
      }

      for (int pix = 0; pix < nXSize; ++pix) {
         SRSU::RGB2HSV(prgb[pix].red, prgb[pix].green, prgb[pix].blue, phsv[pix].hue,
                 phsv[pix].saturation, phsv[pix].value);
         phsv[pix].value = (((1.0f - 0.0f) * ((float) pafPan[pix] - 0.0f))
               / (255.0f - 0.0f));
         SRSU::HSV2RGB(phsv[pix].hue, phsv[pix].saturation, phsv[pix].value, prgb[pix].red,
                 prgb[pix].green, prgb[pix].blue);
         pafSLRed[pix] = prgb[pix].red;
         pafSLGreen[pix] = prgb[pix].green;
         pafSLBlue[pix] = prgb[pix].blue;
      }

      GDALRasterIO(hRedBandPan, GF_Write, 0, Yix, nXSize, 1, pafSLRed, nXSize, 1,
                   SHARPENING_LIB_TYPE, 0, 0);
      GDALRasterIO(hGreenBandPan, GF_Write, 0, Yix, nXSize, 1, pafSLGreen, nXSize, 1,
                   SHARPENING_LIB_TYPE, 0, 0);
      GDALRasterIO(hBlueBandPan, GF_Write, 0, Yix, nXSize, 1, pafSLBlue, nXSize, 1,
                   SHARPENING_LIB_TYPE, 0, 0);

      if (ProgressCB != NULL) {
         ProgressCB(((10.0f * static_cast<float>(Yix)) / static_cast<float>(nYSize)), "", pProgressCBData);
      }

      memset(pafSLRed, 0, sizeof(SHARPENING_NATIVE_TYPE) * nXSize);
      memset(pafSLGreen, 0, sizeof(SHARPENING_NATIVE_TYPE) * nXSize);
      memset(pafSLBlue, 0, sizeof(SHARPENING_NATIVE_TYPE) * nXSize);
   }

   CPLFree(pafSLRed);
   CPLFree(pafSLGreen);
   CPLFree(pafSLBlue);

   GDALClose(pands);
   GDALClose(srcds);
   GDALClose(destds);

   CPLCleanupTLS();
}

/**
 * Crea una instancia de HSVSharpening.
 */
Sharpening* HSVSharpening::Create(suri::ParameterCollection& Params) {
   std::string SrcFilename;
   Params.GetValue<std::string>("SrcFilename", SrcFilename);

   std::string PanFilename;
   Params.GetValue<std::string>("PanFilename", PanFilename);

   std::string DestFilename;
   Params.GetValue<std::string>("DestFilename", DestFilename);

   std::vector<int> SrcBands;
   Params.GetValue<std::vector<int> >("SrcBands", SrcBands);

   int PanBand = 0;
   Params.GetValue<int>("PanBand", PanBand);

   return new HSVSharpening(SrcFilename, PanFilename, DestFilename, SrcBands, PanBand);
}

/**
 * Copia informacion de proyeccion, georreferenciacion, metadatos, etc
 * desde el dataset de origen al de destino.
 */
void HSVSharpening::CopyStandardData(GDALDatasetH SrcDs, GDALDatasetH DestDs) {
   const char* pszProjection = GDALGetProjectionRef(SrcDs);
   if (pszProjection != NULL && strlen(pszProjection) > 0)
      GDALSetProjection(DestDs, pszProjection);

   double adfGeoTransform[6] = { 0 };
   GDALGetGeoTransform(SrcDs, adfGeoTransform);
   GDALSetGeoTransform(DestDs, adfGeoTransform);

   char** ppmetadata = CSLDuplicate(((GDALDataset*) SrcDs)->GetMetadata());
   ((GDALDataset*) DestDs)->SetMetadata(ppmetadata);
   CSLDestroy(ppmetadata);

   const char* pinterleave = GDALGetMetadataItem(SrcDs, "INTERLEAVE",
                                                 "IMAGE_STRUCTURE");
   if (pinterleave)
      ((GDALDataset*)DestDs)->SetMetadataItem("INTERLEAVE", pinterleave, "IMAGE_STRUCTURE");

   // Mantengo datos espaciales.
   char** ppmd = ((GDALDataset*) SrcDs)->GetMetadata("RPC");
   if (ppmd != NULL) ((GDALDataset*)DestDs)->SetMetadata(ppmd, "RPC");

   ppmd = ((GDALDataset*) SrcDs)->GetMetadata("GEOLOCATION");
   if (ppmd != NULL) ((GDALDataset*)DestDs)->SetMetadata(ppmd, "GEOLOCATION");
}

}  // namespace sharpening
}  // namespace raster
}  // namespace suri
