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
#include "GdalWarperTool.h"
#include "suri/messages.h"
#include "logmacros.h"

// GDAL
#include "gdal.h"
#include "gdalwarper.h"
#include "cpl_vsi.h"
#include "cpl_conv.h"
#include "cpl_string.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include "vrtdataset.h"
// wx
#include "wx/progdlg.h"
/**
 * \todo (danieltesta-#4287): Mover el codigo en este namespace anonimo a un lugar
 * comun junto con el codigo que se usa en GeoreferenceProcess
 */
namespace suri {
   
const std::string GdalWarperTool::kDefaultDriver("GTiff");

int CPL_DLL CPL_STDCALL DummyProgreeProc(double Complete, const char* pMessage,
                                         void* pProgressArg) {
   if (pProgressArg != NULL) {
      wxProgressDialog* pprogress = static_cast<wxProgressDialog*>(pProgressArg);
      pprogress->Update(static_cast<int>(Complete * 10));
   }
   return TRUE;
}

/**
 * Constructor
 */
GdalWarperTool::GdalWarperTool(const std::string& SrcFilename,
                       const std::string& DestFilename,
                       const std::vector<int>& selectedBands):
                       srcFilename(SrcFilename),
                       destFilename(DestFilename),
                       selectedBands(selectedBands) {}
/**
 * Constructor
 */
GdalWarperTool::GdalWarperTool(const std::string& SrcFilename,
                       const std::string& DestFilename):
                       srcFilename(SrcFilename),
                       destFilename(DestFilename) {}


/**
 * Crea el archivo de salida.
 */
GDALDatasetH GDALWarpCreateOutput(GDALDatasetH hSrcDS,
                                  const char *pszFilename,
                                  const char *pszFormat,
                                  const char *pszSourceSRS,
                                  const char *pszTargetSRS,
                                  int nOrder,
                                  char **papszCreateOptions,
                                  bool UseGCPs) {
   GDALDriverH driver;
   GDALDatasetH destds;
   void* transformarg;
   double adfdstgeotransform[6] = { 0.0 };
   double adfExtent[4] = { 0.0 };
   int pixels = 0, lines = 0;
   GDALColorTableH colortable;
   double dfWrkMinX = 0, dfWrkMaxX = 0, dfWrkMinY = 0, dfWrkMaxY = 0;
   double dfWrkResX = 0, dfWrkResY = 0;

   // Driver de salida.
   driver = GDALGetDriverByName(pszFormat);
   if (driver == NULL || GDALGetMetadataItem(driver, GDAL_DCAP_CREATE, NULL) == NULL) {
      SHOW_ERROR(message_FORMAT_NOT_SUPPORTED, pszFormat);
      return NULL;
   }

   // Transformacion de origen a destino.
   transformarg = GDALCreateGenImgProjTransformer(hSrcDS, pszSourceSRS, NULL,
                                                  pszTargetSRS, UseGCPs, 0.0, nOrder);

   if (transformarg == NULL) return NULL;

   GDALTransformerInfo* psInfo = (GDALTransformerInfo*) transformarg;
   if (GDALSuggestedWarpOutput2(hSrcDS, psInfo->pfnTransform, transformarg,
                                adfdstgeotransform, &pixels, &lines, adfExtent, 0)
         != CE_None) return NULL;

   if (CPLGetConfigOption("CHECK_WITH_INVERT_PROJ", NULL) == NULL) {
      double MinX = adfExtent[0];
      double MaxX = adfExtent[2];
      double MaxY = adfExtent[3];
      double MinY = adfExtent[1];
      int bSuccess = TRUE;

      /* Check that the the edges of the target image are in the validity area */
      /* of the target projection */
#define N_STEPS 20
      int i, j;
      for (i = 0; i <= N_STEPS && bSuccess; i++) {
         for (j = 0; j <= N_STEPS && bSuccess; j++) {
            double dfRatioI = i * 1.0 / N_STEPS;
            double dfRatioJ = j * 1.0 / N_STEPS;
            double expected_x = (1 - dfRatioI) * MinX + dfRatioI * MaxX;
            double expected_y = (1 - dfRatioJ) * MinY + dfRatioJ * MaxY;
            double x = expected_x;
            double y = expected_y;
            double z = 0;
            /* Target SRS coordinates to source image pixel coordinates */
            if (!psInfo->pfnTransform(transformarg, TRUE, 1, &x, &y, &z, &bSuccess)
                  || !bSuccess) bSuccess = FALSE;
            /* Source image pixel coordinates to target SRS coordinates */
            if (!psInfo->pfnTransform(transformarg, FALSE, 1, &x, &y, &z, &bSuccess)
                  || !bSuccess) bSuccess = FALSE;
            if (fabs(x - expected_x) > (MaxX - MinX) / pixels
                  || fabs(y - expected_y) > (MaxY - MinY) / lines) bSuccess = FALSE;
         }
      }

      /* If not, retry with CHECK_WITH_INVERT_PROJ=TRUE that forces ogrct.cpp */
      /* to check the consistency of each requested projection result with the */
      /* invert projection */
      if (!bSuccess) {
         CPLSetConfigOption("CHECK_WITH_INVERT_PROJ", "TRUE");
         CPLDebug("WARP", "Recompute out extent with CHECK_WITH_INVERT_PROJ=TRUE");

         if (GDALSuggestedWarpOutput2(hSrcDS, psInfo->pfnTransform, transformarg,
                                      adfdstgeotransform, &pixels, &lines, adfExtent, 0)
               != CE_None) {
            return NULL;
         }
      }
   }

   if (dfWrkMaxX == 0.0 && dfWrkMinX == 0.0) {
      dfWrkMinX = adfExtent[0];
      dfWrkMaxX = adfExtent[2];
      dfWrkMaxY = adfExtent[3];
      dfWrkMinY = adfExtent[1];
      dfWrkResX = adfdstgeotransform[1];
      dfWrkResY = ABS(adfdstgeotransform[5]);
   } else {
      dfWrkMinX = MIN(dfWrkMinX, adfExtent[0]);
      dfWrkMaxX = MAX(dfWrkMaxX, adfExtent[2]);
      dfWrkMaxY = MAX(dfWrkMaxY, adfExtent[3]);
      dfWrkMinY = MIN(dfWrkMinY, adfExtent[1]);
      dfWrkResX = MIN(dfWrkResX, adfdstgeotransform[1]);
      dfWrkResY = MIN(dfWrkResY, ABS(adfdstgeotransform[5]));
   }

   GDALDestroyGenImgProjTransformer(transformarg);

   adfdstgeotransform[0] = dfWrkMinX;
   adfdstgeotransform[1] = dfWrkResX;
   adfdstgeotransform[2] = 0.0;
   adfdstgeotransform[3] = dfWrkMaxY;
   adfdstgeotransform[4] = 0.0;
   adfdstgeotransform[5] = -1 * dfWrkResY;

   pixels = (int) ((dfWrkMaxX - dfWrkMinX) / dfWrkResX + 0.5);
   lines = (int) ((dfWrkMaxY - dfWrkMinY) / dfWrkResY + 0.5);

   // Archivo de salida.
   destds = GDALCreate(driver, pszFilename, pixels, lines, GDALGetRasterCount(hSrcDS),
                       GDALGetRasterDataType(GDALGetRasterBand(hSrcDS, 1)),
                       papszCreateOptions);

   if (destds == NULL) return NULL;

   // Definicion de la proyeccion.
   GDALSetProjection(destds, pszTargetSRS);
   GDALSetGeoTransform(destds, adfdstgeotransform);

   // Copio la tabla de colores.
   colortable = GDALGetRasterColorTable(GDALGetRasterBand(hSrcDS, 1));
   if (colortable != NULL)
      GDALSetRasterColorTable(GDALGetRasterBand(destds, 1), colortable);

   return destds;
}

bool GdalWarperTool::Warp(const std::string& InWkt, bool UseGCPs, int Order) {
   GDALDatasetH srcds, destds;
   char* sourcesrs = NULL;
   void* ptransformarg = NULL;
   void* pgenimgprojarg = NULL;
   void* papproxarg = NULL;
   double errorthreshold = 0.125;
   GDALTransformerFunc ptransformerproc = NULL;
   char** ppcreateoptions = NULL;

   GDALAllRegister();

   char* psrcfilename = const_cast<char*>(this->srcFilename.c_str());
   char* pdestfilename = const_cast<char*>(this->destFilename.c_str());

   srcds = GDALOpen(psrcfilename, GA_ReadOnly);

   if (srcds == NULL) return false;

   if (GDALGetRasterCount(srcds) == 0) {
      SHOW_ERROR(message_NO_RASTER_BANDS, psrcfilename);
      return false;
   }

   if (sourcesrs == NULL) {
      if (GDALGetProjectionRef(srcds) != NULL
            && strlen(GDALGetProjectionRef(srcds)) > 0)
         sourcesrs = CPLStrdup(GDALGetProjectionRef(srcds));

      else if (GDALGetGCPProjection(srcds) != NULL
            && strlen(GDALGetGCPProjection(srcds)) > 0 && GDALGetGCPCount(srcds) > 1)
         sourcesrs = CPLStrdup(GDALGetGCPProjection(srcds));
      else
         sourcesrs = CPLStrdup("");
   }

   char* ptargetsrs = const_cast<char*>(InWkt.c_str());
   if (InWkt.empty()) ptargetsrs = CPLStrdup(sourcesrs);

   CPLPushErrorHandler (CPLQuietErrorHandler);
   destds = GDALOpen(pdestfilename, GA_Update);
   CPLPopErrorHandler();

   if (destds == NULL) {
      destds = GDALWarpCreateOutput(srcds, pdestfilename, kDefaultDriver.c_str(), sourcesrs,
                                    ptargetsrs, Order, ppcreateoptions, UseGCPs);
      CSLDestroy(ppcreateoptions);
      ppcreateoptions = NULL;
   }

   if (destds == NULL) return false;

   ptransformarg = pgenimgprojarg = GDALCreateGenImgProjTransformer(srcds, sourcesrs,
                                                                    destds, ptargetsrs,
                                                                    UseGCPs, 0.0,
                                                                    Order);

   if (ptransformarg == NULL) return false;

   ptransformerproc = GDALGenImgProjTransform;

   // Verificar si esto es necesario o no ( ! )
   if (errorthreshold != 0.0) {
      ptransformarg = papproxarg = GDALCreateApproxTransformer(GDALGenImgProjTransform,
                                                               pgenimgprojarg,
                                                               errorthreshold);
      ptransformerproc = GDALApproxTransform;
   }

   // Configuro las opciones de transformacion
   GDALWarpOptions *psWarpOptions = GDALCreateWarpOptions();

   psWarpOptions->hSrcDS = srcds;
   psWarpOptions->hDstDS = destds;
   psWarpOptions->eResampleAlg = GRA_NearestNeighbour;

   psWarpOptions->nBandCount = GDALGetRasterCount(srcds);
   psWarpOptions->panSrcBands = (int*) CPLMalloc(
         sizeof(int) * psWarpOptions->nBandCount);
   psWarpOptions->panDstBands = (int *) CPLMalloc(
         sizeof(int) * psWarpOptions->nBandCount);

   for (int ix = 0; ix < psWarpOptions->nBandCount; ++ix) {
      psWarpOptions->panSrcBands[ix] = ix + 1;
      psWarpOptions->panDstBands[ix] = ix + 1;
   }

   wxProgressDialog progress("Procesando...", "", 10);
   progress.SetSize(wxSize(400, -1));
   progress.Centre(wxCENTRE_ON_SCREEN);

   psWarpOptions->pfnProgress = DummyProgreeProc;
   psWarpOptions->pProgressArg = &progress;

   // reproyeccion
   psWarpOptions->pTransformerArg = ptransformarg;
   psWarpOptions->pfnTransformer = ptransformerproc;

   GDALWarpOperation oOperation;

   oOperation.Initialize(psWarpOptions);
   oOperation.ChunkAndWarpImage(0, 0, GDALGetRasterXSize(destds),
                                GDALGetRasterYSize(destds));

   GDALDestroyWarpOptions(psWarpOptions);

   if (papproxarg != NULL) GDALDestroyApproxTransformer(papproxarg);

   if (pgenimgprojarg != NULL) GDALDestroyGenImgProjTransformer(pgenimgprojarg);

   GDALFlushCache(destds);
   GDALClose(destds);
   GDALClose(srcds);
   return true;
}

}  // namespace suri (codigo copiado de GeoreferenceProcess)