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

#include "suri/SharpeningProcess.h"

#include "BandInfo.h"
#include "SpatialReference.h"
#include "SRDHistogram.h"
#include "SRDStatistics.h"
#include "SRELinear2PercentEnhancement.h"
#include "SRSSharpening.h"
#include "SRSSharpeningFactory.h"
#include "suri/Element.h"
#include "suri/ParameterCollection.h"
#include "suri/ProcessAdaptLayer.h"
#include "suri/ProcessInterface.h"
#include "suri/RasterElement.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/XmlFunctions.h"

#include "cpl_conv.h"
#include "cpl_multiproc.h"
#include "cpl_string.h"
#include "cpl_vsi.h"
#include "gdal_priv.h"
#include "gdal.h"
#include "gdalwarper.h"
#include "ogr_spatialref.h"
#include "vrtdataset.h"

#include "wx/filename.h"
#include "wx/progdlg.h"
#include "wx/sstream.h"

namespace {

/**
 * Crea el objeto para el calculo de estadisticas a partir
 * del tipo de dato provisto.
 */
suri::raster::data::StatisticsBase* CreateStatisticsFromGdalType(GDALDataType Type,
                                                                 int BandCount) {
   suri::raster::data::StatisticsBase* pret = NULL;

   if (Type == GDT_Byte) {
      pret = new suri::raster::data::Statistics<unsigned char>(BandCount);
   } else if (Type == GDT_UInt16) {
      pret = new suri::raster::data::Statistics<unsigned short>(BandCount);
   } else if (Type == GDT_Int16) {
      pret = new suri::raster::data::Statistics<short>(BandCount);
   } else if (Type == GDT_UInt32) {
      pret = new suri::raster::data::Statistics<unsigned int>(BandCount);
   } else if (Type == GDT_Int32) {
      pret = new suri::raster::data::Statistics<int>(BandCount);
   } else if (Type == GDT_Float32) {
      pret = new suri::raster::data::Statistics<float>(BandCount);
   } else if (Type == GDT_Float64) {
      pret = new suri::raster::data::Statistics<double>(BandCount);
   }

   return pret;
}

/**
 * Crea el objeto para calculo de histograma de intensidad a
 * partir del tipo de dato provisto.
 */
suri::raster::data::HistogramBase* CreateHistogramFromGdalType(GDALDataType Type,
                                                               int BandCount,
                                                               int* pNumBins,
                                                               double* pMinValue,
                                                               double* pMaxValue) {
   namespace SRD = suri::raster::data;

   suri::raster::data::HistogramBase* pret = NULL;

   if (Type == GDT_Byte) {
      pret = new SRD::Histogram<unsigned char>(BandCount, pNumBins, pMinValue, pMaxValue);
   } else if (Type == GDT_UInt16) {
      pret = new SRD::Histogram<unsigned short>(BandCount, pNumBins, pMinValue, pMaxValue);
   } else if (Type == GDT_Int16) {
      pret = new SRD::Histogram<short>(BandCount, pNumBins, pMinValue, pMaxValue);
   } else if (Type == GDT_UInt32) {
      pret = new SRD::Histogram<unsigned int>(BandCount, pNumBins, pMinValue, pMaxValue);
   } else if (Type == GDT_Int32) {
      pret = new SRD::Histogram<int>(BandCount, pNumBins, pMinValue, pMaxValue);
   } else if (Type == GDT_Float32) {
      pret = new SRD::Histogram<float>(BandCount, pNumBins, pMinValue, pMaxValue);
   } else if (Type == GDT_Float64) {
      pret = new SRD::Histogram<double>(BandCount, pNumBins, pMinValue, pMaxValue);
   }

   return pret;
}

/**
 * Crea un buffer para lectura/escritura de datos segun el tipo de dato provisto.
 */
void* CreateBufferFromGdalType(GDALDataType Type, int Size) {
   if (Type == GDT_Byte)
      return CPLMalloc(sizeof(unsigned char) * Size);
   else if (Type == GDT_UInt16)
      return CPLMalloc(sizeof(unsigned short) * Size);
   else if (Type == GDT_Int16)
      return CPLMalloc(sizeof(short) * Size);
   else if (Type == GDT_UInt32)
      return CPLMalloc(sizeof(unsigned int) * Size);
   else if (Type == GDT_Int32)
      return CPLMalloc(sizeof(int) * Size);
   else if (Type == GDT_Float32)
      return CPLMalloc(sizeof(float) * Size);
   else if (Type == GDT_Float64)
      return CPLMalloc(sizeof(double) * Size);

   return NULL;
}

/**
 * Callback para traduccion de valores (realce)
 */
typedef void (*FTRANSLATE)(int* pLut, void* pInData, unsigned char* pOutData, int DataSize,
                           int NumBins, double Min, double Max, double NoDataValue,
                           bool NoDataValueAvailable);

/**
 * Traduce el valor de la imagen de entrada a valor correspondiente
 * segun la LUT provista.
 */
template<typename T>
void TranslateData(int* pLut, void* pInData, unsigned char* pOutData, int DataSize,
                   int NumBins, double Min, double Max, double NoDataValue,
                   bool NoDataValueAvailable) {
   T* pindata = static_cast<T*>(pInData);

   for (int DSix = 0; DSix < DataSize; ++DSix) {
      if (pindata[DSix] != INFINITY && pindata[DSix] != -INFINITY
            && !(NoDataValueAvailable && pindata[DSix] == NoDataValue)) {
         double scale = (static_cast<double>(Max) - static_cast<double>(Min))
               / static_cast<double>(NumBins - 1);
         int binvalue = static_cast<int>(floor((pindata[DSix] - Min) / scale));
         pOutData[DSix] = pLut[binvalue];
      } else {
         pOutData[DSix] = pindata[DSix];
      }
   }
}

/**
 * Ejecuta la verdadera funcion de transformacion teniendo en cuenta
 * el tipo de dato.
 */
void Translate(GDALDataType Type, int* pLut, void* pInData, unsigned char* pOutData,
               int DataSize, int NumBins, double Min, double Max, double NoDataValue,
               bool NoDataValueAvailable) {
   FTRANSLATE ftranslate = NULL;

   if (Type == GDT_Byte) {
      ftranslate = &TranslateData<unsigned char>;
   } else if (Type == GDT_UInt16) {
      ftranslate = &TranslateData<unsigned short>;
   } else if (Type == GDT_Int16) {
      ftranslate = &TranslateData<short>;
   } else if (Type == GDT_UInt32) {
      ftranslate = &TranslateData<unsigned int>;
   } else if (Type == GDT_Int32) {
      ftranslate = &TranslateData<int>;
   } else if (Type == GDT_Float32) {
      ftranslate = &TranslateData<float>;
   } else if (Type == GDT_Float64) {
      ftranslate = &TranslateData<double>;
   }

   if (ftranslate != NULL) {
      ftranslate(pLut, pInData, pOutData, DataSize, NumBins, Min, Max, NoDataValue,
                 NoDataValueAvailable);
   }
}


/**
 * Funcion callbaack para mostrar avance del proceso.
 */
int CPL_DLL CPL_STDCALL ProgressProc(double Complete, const char* pMessage, void* pProgressArg) {
   if (pProgressArg != NULL) {
      wxProgressDialog* pprogress = static_cast<wxProgressDialog*>(pProgressArg);
      pprogress->Update(static_cast<int>(Complete * 10));
   }
   return TRUE;
}

/**
 * Obtiene, si esta disponible, el valor no valido para el elemento.
 */
void GetNoDataValue(suri::Element* pElement, bool& Available, double& NoDataValue) {
   Available = false;
   NoDataValue = 0.0;
   wxXmlNode* pdatanotvalidvalue = pElement->GetNode(
   PATH_NOT_VALID_DATA_VALUE_NODE_PIPES);
   if (pdatanotvalidvalue) {
      pdatanotvalidvalue = pdatanotvalidvalue->GetChildren();
      if (pdatanotvalidvalue->GetType() == wxXML_TEXT_NODE) {
         wxString ctt = pdatanotvalidvalue->GetContent();
         Available = true;
         NoDataValue = suri::StringToNumber<double>(ctt.mb_str());
      }
   }
}

VRTDataset* CreateBasicVRTDataset(const std::string& Filename, int Width, int Height) {
   GDALDatasetH srcds;
   VRTDataset* pvirtds = NULL;

   //
   srcds = GDALOpen(Filename.c_str(), GA_ReadOnly);

   //
   pvirtds = (VRTDataset*)VRTCreate(Width, Height);

   const char* pszProjection = GDALGetProjectionRef(srcds);
   if (pszProjection != NULL && strlen(pszProjection) > 0)
      pvirtds->SetProjection(pszProjection);

   double adfGeoTransform[6] = { 0 };
   GDALGetGeoTransform(srcds, adfGeoTransform);
   pvirtds->SetGeoTransform(adfGeoTransform);

   char** ppmetadata = CSLDuplicate(((GDALDataset*) srcds)->GetMetadata());
   pvirtds->SetMetadata(ppmetadata);
   CSLDestroy(ppmetadata);

   const char* pinterleave = GDALGetMetadataItem(srcds, "INTERLEAVE",
                                                 "IMAGE_STRUCTURE");
   if (pinterleave)
      pvirtds->SetMetadataItem("INTERLEAVE", pinterleave, "IMAGE_STRUCTURE");

   // Mantengo datos espaciales.
   char** ppmd = ((GDALDataset*) srcds)->GetMetadata("RPC");
   if (ppmd != NULL) pvirtds->SetMetadata(ppmd, "RPC");

   ppmd = ((GDALDataset*) srcds)->GetMetadata("GEOLOCATION");
   if (ppmd != NULL) pvirtds->SetMetadata(ppmd, "GEOLOCATION");

   GDALClose(srcds);

   return pvirtds;
}

}  // namespace (anonymous)

namespace suri {

/** Nombre del proceso **/
const std::string SharpeningProcess::kProcessName = "SharpeningProcess";

/**
 * Ctor.
 */
SharpeningProcess::SharpeningProcess(Element*& pOutput, DataViewManager* pDataViewManager) :
      pOutputElement_(pOutput), pDataViewManager_(pDataViewManager) {
   SetProcessName(kProcessName);
}

/**
 * Dtor.
 */
SharpeningProcess::~SharpeningProcess() {

}

/**
 * Corre el proceso y genera la salida usando los metodos de configuracion.
 */
bool SharpeningProcess::RunProcess() {

   std::vector<std::string> statusrgb;
   statusrgb.push_back("Analizando imagen para componer los valores RVA...");
   statusrgb.push_back("Preparando imagen para componer los valores RVA...");
   statusrgb.push_back("Procesando imagen para componer los valores RVA...");

   std::vector<std::string> statuspan;
   statuspan.push_back("Analizando imagen con banda de alta resolucion...");
   statuspan.push_back("Preparando imagen con banda de alta resolucion...");
   statuspan.push_back("Procesando imagen con banda de alta resolucion...");

   suri::ParameterCollection Params;
   ProcessAdaptLayer* ppal = GetProcessAdaptLayer();

   std::vector<BandInfo> rgbelements;
   ppal->GetAttribute<std::vector<BandInfo> >(ProcessAdaptLayer::OutputRGBBands,
                                               rgbelements);

   if (rgbelements.size() < 3)
      return false;

   BandInfo panElement;
   bool fetched = ppal->GetAttribute<BandInfo>(
         ProcessAdaptLayer::OutputHighResBand, panElement);

   if (!fetched)
      return false;

   int width = 0, height = 0;
   panElement.GetElement()->GetRasterSize(width, height);

   std::string stacktemp = wxFileName::CreateTempFileName(_("")).mb_str();
   std::string resizetemp = wxFileName::CreateTempFileName(_("")).mb_str();
   std::string enhancetemp = wxFileName::CreateTempFileName(_("")).mb_str();
   std::string pantemp = wxFileName::CreateTempFileName(_("")).mb_str();;

   bool srcndavailable = false, panndavailable = false;
   double srcndvalue = 0.0, panndvalue = 0.0;

   ppal->GetAttribute<bool>(ProcessAdaptLayer::OutputRGBNVDAvailable, srcndavailable);
   ppal->GetAttribute<double>(ProcessAdaptLayer::OutputRGBNVD, srcndvalue);
   GetNoDataValue(panElement.GetElement(), panndavailable, panndvalue);

   Stack(stacktemp, rgbelements);

   Enhance(stacktemp, enhancetemp, statusrgb, srcndavailable, srcndvalue);
   Resize(enhancetemp, resizetemp, width, height);

   Enhance(panElement.GetElement()->GetUrl().mb_str(), pantemp, statuspan,
           panndavailable, panndvalue);

   std::string srcfilename = resizetemp;
   std::string panfilename = pantemp;

   Params.AddValue<std::string>("SrcFilename", srcfilename);
   Params.AddValue<std::string>("PanFilename", panfilename);

   std::string DestFilename;
   ppal->GetAttribute<std::string>(ProcessAdaptLayer::OutputFileNameKeyAttr,
                                   DestFilename);
   Params.AddValue<std::string>("DestFilename", DestFilename);

   std::vector<int> SrcBands;
   SrcBands.push_back(0);
   SrcBands.push_back(1);
   SrcBands.push_back(2);
   Params.AddValue<std::vector<int> >("SrcBands", SrcBands);

   int PanBand = 0;
   Params.AddValue<int>("PanBand", PanBand);

   suri::raster::sharpening::Sharpening* psharpening =
         suri::raster::sharpening::SharpeningFactory::GetInstance()->Create(
               "HSVSharpening", Params);

   wxProgressDialog* pprogress = new wxProgressDialog(
            _("Realizando fusion de bandas..."), "", 100);
   pprogress->SetSize(wxSize(400, -1));
   pprogress->Centre(wxCENTRE_ON_SCREEN);

   psharpening->Transform(&ProgressProc, pprogress);

   delete pprogress;

   wxRemoveFile(stacktemp);
   wxRemoveFile(resizetemp);
   wxRemoveFile(enhancetemp);
   wxRemoveFile(pantemp);

   ConfigureOutput();

   return true;
}

/**
 * Permite configurar la salida del proceso antes de correrlo.
 */
bool SharpeningProcess::ConfigureProcess() {
   return true;
}

/**
 * Una vez ejecutado el proceso hace visible la salida.
 */
bool SharpeningProcess::ConfigureOutput() {
   std::string destfilepath;
   GetProcessAdaptLayer()->GetAttribute<std::string>(
         ProcessAdaptLayer::OutputFileNameKeyAttr, destfilepath);

   // Crea el elemento con el url del objeto salvado
   pOutputElement_ = Element::Create("RasterElement", destfilepath);
   wxFileName file(destfilepath);
   if (pOutputElement_) {
      pOutputElement_->SetName(file.GetName());      
      std::string parameters = GetParametersAsXmlString();
      if (!parameters.empty()) {
#ifdef __WINDOWS__
         parameters =
               wxString(parameters.c_str(), wxCSConv(suri::XmlElement::xmlEncoding_.c_str()));
#endif
         REPORT_DEBUG("Parametros del proceso %s", parameters.c_str());
         wxStringInputStream ss(parameters);
         wxXmlDocument doc(ss, suri::XmlElement::xmlEncoding_);
         if (pOutputElement_->GetNode(PROCESS_HISTORY_NODE) == NULL)
            pOutputElement_->AddNode(pOutputElement_->GetNode(), PROCESS_HISTORY_NODE);
         pOutputElement_->AddNode(pOutputElement_->GetNode(PROCESS_HISTORY_NODE), doc.DetachRoot());
      }
   }
   return pOutputElement_ != NULL;
}

/**
 * Copia informacion de una banda.
 */
void SharpeningProcess::CopyBandInfo(GDALRasterBand* SrcBand, GDALRasterBand* DstBand,
                                     int CanCopyStatsMetadata, int CopyScale,
                                     int CopyNoData) {
   if (CanCopyStatsMetadata) {
      DstBand->SetMetadata(SrcBand->GetMetadata());
   } else {
      char** metadata = SrcBand->GetMetadata();
      char** metadatanew = NULL;
      for (int i = 0; metadata != NULL && metadata[i] != NULL; i++) {
         if (strncmp(metadata[i], "STATISTICS_", 11) != 0)
            metadatanew = CSLAddString(metadatanew, metadata[i]);
      }
      DstBand->SetMetadata(metadatanew);
      CSLDestroy(metadatanew);
   }

   DstBand->SetColorTable(SrcBand->GetColorTable());
   DstBand->SetColorInterpretation(SrcBand->GetColorInterpretation());
   if (strlen(SrcBand->GetDescription()) > 0)
      DstBand->SetDescription(SrcBand->GetDescription());

   if (CopyNoData) {
      int success = FALSE;
      double nodata = SrcBand->GetNoDataValue(&success);
      if (success) DstBand->SetNoDataValue(nodata);
   }

   if (CopyScale) {
      DstBand->SetOffset(SrcBand->GetOffset());
      DstBand->SetScale(SrcBand->GetScale());
   }

   DstBand->SetCategoryNames(SrcBand->GetCategoryNames());
   if (!EQUAL(SrcBand->GetUnitType(), "")) DstBand->SetUnitType(SrcBand->GetUnitType());
}

/**
 * Escala el raster especificado.
 */
void SharpeningProcess::Resize(const std::string& SrcFilename,
                               const std::string& DestFilename,
                               int OutXSize, int OutYSize) {
   GDALDatasetH srcds, destds;
   VRTDataset* pvirtds = NULL;

   //
   srcds = GDALOpen(SrcFilename.c_str(), GA_ReadOnly);

   GDALDriverH hDriver = GDALGetDriverByName("GTiff");

   //
   pvirtds = (VRTDataset *) VRTCreate(OutXSize, OutYSize);

   const char* pszProjection = GDALGetProjectionRef(srcds);
   if (pszProjection != NULL && strlen(pszProjection) > 0)
      pvirtds->SetProjection(pszProjection);

   double adfGeoTransform[6] = { 0 };
   GDALGetGeoTransform(srcds, adfGeoTransform);
   pvirtds->SetGeoTransform(adfGeoTransform);

   char** ppmetadata = CSLDuplicate(((GDALDataset*) srcds)->GetMetadata());
   pvirtds->SetMetadata(ppmetadata);
   CSLDestroy(ppmetadata);

   const char* pinterleave = GDALGetMetadataItem(srcds, "INTERLEAVE",
                                                 "IMAGE_STRUCTURE");
   if (pinterleave)
      pvirtds->SetMetadataItem("INTERLEAVE", pinterleave, "IMAGE_STRUCTURE");

   // Mantengo datos espaciales.
   char** ppmd = ((GDALDataset*) srcds)->GetMetadata("RPC");
   if (ppmd != NULL) pvirtds->SetMetadata(ppmd, "RPC");

   ppmd = ((GDALDataset*) srcds)->GetMetadata("GEOLOCATION");
   if (ppmd != NULL) pvirtds->SetMetadata(ppmd, "GEOLOCATION");

   // Copio las bandas.
   for (int ix = 0; ix < GDALGetRasterCount(srcds); ++ix) {
      VRTSourcedRasterBand* pvirtband;
      GDALRasterBand* psrcband;

      int nSrcBand = ix + 1;

      psrcband = ((GDALDataset *) srcds)->GetRasterBand(ABS(nSrcBand));

      pvirtds->AddBand(psrcband->GetRasterDataType(), NULL);
      pvirtband = (VRTSourcedRasterBand *) pvirtds->GetRasterBand(ix + 1);
      if (nSrcBand < 0) {
         pvirtband->AddMaskBandSource(psrcband);
         continue;
      }

      pvirtband->AddSimpleSource(psrcband, 0, 0, GDALGetRasterXSize(srcds),
                                 GDALGetRasterYSize(srcds), 0, 0, OutXSize, OutYSize);

      CopyBandInfo(psrcband, pvirtband, FALSE, TRUE, FALSE);
   }

   // Mando al disco.
   wxProgressDialog* pprogress = new wxProgressDialog(
         _("Ajustando imagen para componer los valores RVA..."), "", 10);
   pprogress->SetSize(wxSize(400, -1));
   pprogress->Centre(wxCENTRE_ON_SCREEN);

   destds = GDALCreateCopy(hDriver, DestFilename.c_str(), (GDALDatasetH) pvirtds, FALSE, NULL,
                           ProgressProc, pprogress);

   delete pprogress;

   if (destds != NULL) {
      CPLErrorReset();
      GDALFlushCache(destds);
      GDALClose(destds);
   }

   GDALClose((GDALDatasetH) pvirtds);
   GDALClose(srcds);

   CPLCleanupTLS();
}

/**
 * Aplica un realce a la imagen de origen (lineal 2%)
 */
void SharpeningProcess::Enhance(const std::string& SrcFilename,
                                const std::string& DestFilename,
                                const std::vector<std::string>& Status,
                                bool NDAvailable, double NDValue) {
   GDALDatasetH hSrcDS, hDstDS;
   hSrcDS = GDALOpen(SrcFilename.c_str(), GA_ReadOnly);

   int bandcount = GDALGetRasterCount(hSrcDS);

   GDALDriverH hDriver = GDALGetDriverByName("GTiff");
   hDstDS = GDALCreate(hDriver, DestFilename.c_str(), GDALGetRasterXSize(hSrcDS),
                       GDALGetRasterYSize(hSrcDS), bandcount, GDT_Byte,
                       NULL);

   double adfGeoTransform[6] = { 0 };
   GDALGetGeoTransform(hSrcDS, adfGeoTransform);
   GDALSetGeoTransform(hDstDS, adfGeoTransform);

   const char *pszSRS_WKT = GDALGetProjectionRef(hSrcDS);
   GDALSetProjection(hDstDS, pszSRS_WKT);

   GDALRasterBandH hSrcBand = GDALGetRasterBand(hSrcDS, 1);
   GDALDataType type = GDALGetRasterDataType(hSrcBand);
   int nXSize = GDALGetRasterBandXSize(hSrcBand);
   int nYSize = GDALGetRasterBandYSize(hSrcBand);

   suri::raster::data::StatisticsBase* pstats = CreateStatisticsFromGdalType(type, bandcount);
   pstats->SetNoDataValueAvailable(NDAvailable);
   pstats->SetNoDataValue(NDValue);

   // *** Saco estadisticas ***
   int progresstotal = bandcount * 100;
   wxProgressDialog* pprogress = new wxProgressDialog(_(Status[0].c_str()), "", progresstotal);
   pprogress->SetSize(wxSize(400, -1));
   pprogress->Centre(wxCENTRE_ON_SCREEN);

   for (int bandix = 0; bandix < bandcount; ++bandix) {
      hSrcBand = GDALGetRasterBand(hSrcDS, bandix + 1);

      void* pafScanline = CreateBufferFromGdalType(type, nXSize);

      for (int Yix = 0; Yix < nYSize; ++Yix) {
         GDALRasterIO(hSrcBand, GF_Read, 0, Yix, nXSize, 1, pafScanline, nXSize, 1,
                      GDALGetRasterDataType(hSrcBand), 0, 0);

         pstats->Process(bandix, nXSize, pafScanline);

         pprogress->Update(
               (bandix * 100)
                     + ((100.0f * static_cast<float>(Yix)) / static_cast<float>(nYSize)));

         memset(pafScanline, 0, pstats->GetTypeSize() * nXSize);
      }

      CPLFree(pafScanline);
   }

   delete pprogress;

   // *** calculo histograma de intensidad ***
   pprogress = new wxProgressDialog(_(Status[1].c_str()), "", progresstotal);
   pprogress->SetSize(wxSize(400, -1));
   pprogress->Centre(wxCENTRE_ON_SCREEN);

   int* pbins = new int[bandcount];
   double* pmin = new double[bandcount];
   double* pmax = new double[bandcount];
   for (int ix = 0; ix < bandcount; ++ix) {
      pbins[ix] = 256;
      pmin[ix] = pstats->GetMin(ix);
      pmax[ix] = pstats->GetMax(ix);
   }

   suri::raster::data::HistogramBase* phist = CreateHistogramFromGdalType(type,
                                                                          bandcount,
                                                                          pbins, pmin,
                                                                          pmax);
   phist->SetNoDataValueAvailable(NDAvailable);
   phist->SetNoDataValue(NDValue);

   for (int bandix = 0; bandix < bandcount; ++bandix) {
      hSrcBand = GDALGetRasterBand(hSrcDS, bandix + 1);

      void* pafScanline = CreateBufferFromGdalType(type, nXSize);

      for (int Yix = 0; Yix < nYSize; ++Yix) {
         GDALRasterIO(hSrcBand, GF_Read, 0, Yix, nXSize, 1, pafScanline, nXSize, 1,
                      type, 0, 0);

         phist->CountPixels(bandix, nXSize, pafScanline);

         pprogress->Update(
               (bandix * 100)
                     + ((100.0f * static_cast<float>(Yix)) / static_cast<float>(nYSize)));

         memset(pafScanline, 0, pstats->GetTypeSize() * nXSize);
      }

      CPLFree(pafScanline);
   }

   delete pprogress;

   // *** aplico el realce ***
   suri::raster::enhancement::Linear2PercentEnhancement linear2perc(bandcount, pbins,
          phist->GetBinAccumFrequencyPercentage());
   int** ppLut = linear2perc.CreateLUT(phist->GetBins());

   pprogress = new wxProgressDialog(_(Status[2].c_str()), "", progresstotal);
   pprogress->SetSize(wxSize(400, -1));
   pprogress->Centre(wxCENTRE_ON_SCREEN);

   for (int bandix = 0; bandix < bandcount; ++bandix) {
      hSrcBand = GDALGetRasterBand(hSrcDS, bandix + 1);

      void* pafScanline = CreateBufferFromGdalType(type, nXSize);

      GDALRasterBandH hDstBand = GDALGetRasterBand(hDstDS, bandix + 1);

      for (int Yix = 0; Yix < nYSize; ++Yix) {
         GDALRasterIO(hSrcBand, GF_Read, 0, Yix, nXSize, 1, pafScanline, nXSize, 1,
                      type, 0, 0);

         unsigned char* outdata = (unsigned char*)CPLMalloc(sizeof(unsigned char) * nXSize);

         Translate(type, ppLut[bandix], pafScanline, outdata, nXSize, pbins[bandix],
                   pmin[bandix], pmax[bandix], NDValue, NDAvailable);

         GDALRasterIO(hDstBand, GF_Write, 0, Yix, nXSize, 1, outdata, nXSize, 1,
                      GDT_Byte, 0, 0);

         pprogress->Update(
               (bandix * 100)
                     + ((100.0f * static_cast<float>(Yix)) / static_cast<float>(nYSize)));

         CPLFree(outdata);

         memset(pafScanline, 0, pstats->GetTypeSize() * nXSize);
      }

      CPLFree(pafScanline);
   }

   delete pprogress;

   GDALClose(hDstDS);
   GDALClose(hSrcDS);

   delete pstats;
   delete phist;

   delete[] pbins;
   delete[] pmin;
   delete[] pmax;

   for (int ix = 0; ix < bandcount; ++ix)
      delete[] ppLut[ix];
   delete[] ppLut;

   CPLCleanupTLS();
}

/**
 * Apila las bandas seleccionadas para RVA en una imagen.
 */
void SharpeningProcess::Stack(const std::string& DestFilename,
                              const std::vector<BandInfo>& Bands) {
   GDALDatasetH srcds[3], destds;
   VRTDataset* pvirtds = NULL;
   GDALDriverH hDriver;

   //
   hDriver = GDALGetDriverByName("GTiff");

   //
   pvirtds = CreateBasicVRTDataset(Bands[0].GetElement()->GetUrl().c_str(),
                                   Bands[0].GetWidth(), Bands[0].GetHeight());

   // Copio las bandas.
   for (int xx = 0, lenxx = Bands.size(); xx < lenxx; ++xx) {
      srcds[xx] = GDALOpen(Bands[xx].GetElement()->GetUrl().c_str(), GA_ReadOnly);

      VRTSourcedRasterBand* pvirtband = NULL;
      GDALRasterBand* psrcband = NULL;

      int nSrcBand = Bands[xx].GetPosition() + 1;

      psrcband = ((GDALDataset *) srcds[xx])->GetRasterBand(ABS(nSrcBand));

      pvirtds->AddBand(psrcband->GetRasterDataType(), NULL);
      pvirtband = (VRTSourcedRasterBand *) pvirtds->GetRasterBand(xx + 1);
      if (nSrcBand < 0) {
         pvirtband->AddMaskBandSource(psrcband);
         continue;
      }

      pvirtband->AddSimpleSource(psrcband, 0, 0, GDALGetRasterXSize(srcds[xx]),
                                 GDALGetRasterYSize(srcds[xx]), 0, 0, Bands[xx].GetWidth(),
                                 Bands[xx].GetHeight());

      CopyBandInfo(psrcband, pvirtband, FALSE, TRUE, FALSE);
   }

   // Mando al disco.
   wxProgressDialog* pprogress = new wxProgressDialog(
         _("Configurando imagen para componer los valores RVA..."), "", 10);
   pprogress->SetSize(wxSize(400, -1));
   pprogress->Centre(wxCENTRE_ON_SCREEN);

   destds = GDALCreateCopy(hDriver, DestFilename.c_str(), (GDALDatasetH) pvirtds, FALSE,
                           NULL, ProgressProc, pprogress);

   delete pprogress;

   if (destds != NULL) {
      CPLErrorReset();
      GDALFlushCache(destds);
      GDALClose(destds);
   }

   GDALClose((GDALDatasetH) pvirtds);

   for (int ix = 0; ix < 3; ++ix)
      GDALClose(srcds[ix]);

   CPLCleanupTLS();
}

/** 
    * Metodo que a partir de un xml que contiene los parametros del proceso configura el mismo.
    * @param[in] XmStr string en formato xml que contiene los parametros necesarios para la 
    * correcta ejecucion del proceso
    * @return true en caso de poder configurar correctamente el proceso en funcion de los parametros
    * @return false en caso contrario
   **/
bool SharpeningProcess::ConfigureProcessFromXmlString(const std::string& XmlStr) {
   return false;
}

/** Metodo que obtiene los parametros asociados al proceso en formato XML. 
 * Esto permite que el codigo cliente pueda hacer eso del xml resultante 
 * (ej. para la exportacion del mismo)
 * @return string en formato xml con los parametros asociados al proceso
 * @return string vacio en caso contrario. 
 */
std::string SharpeningProcess::GetParametersAsXmlString() const {   
   wxXmlNode* pparamsnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(PROCESS_NODE));
   pparamsnode->AddChild(CreateKeyValueNode(NAME_NODE, _(processName_.c_str())));

   pparamsnode->AddChild(CreateKeyValueNode("fecha", wxNow().c_str()));;
   
   /** Configuracion de mundo de entrada **/
   World* pworld = NULL;
   if (pAdaptLayer_->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr, pworld)) {
      pparamsnode->AddChild(CreateKeyValueNode("sr-entrada", 
                           SpatialReference::GetAuthorityId(pworld->GetSpatialReference())));
      Subset sin;
      pworld->GetWindow(sin);
      std::stringstream ss;
      ss << sin;
      pparamsnode->AddChild(CreateKeyValueNode("extent-entrada", ss.str()));
   }
   /** configuracion de mundo de salida */
   Subset sout;
   if (pAdaptLayer_->GetAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr,
                                               sout)) {
      std::stringstream ss;
      ss << sout;
      pparamsnode->AddChild(CreateKeyValueNode("extent-salida", ss.str()));
   }
   
   if (pworld != NULL)
      pparamsnode->AddChild(
            CreateKeyValueNode("sr-salida",
                  SpatialReference::GetAuthorityId(pworld->GetSpatialReference())));

   std::string outfilename;
   pAdaptLayer_->GetAttribute<std::string>(ProcessAdaptLayer::OutputFileNameKeyAttr, outfilename);
#ifdef __WINDOWS__
   outfilename = wxString(wxConvUTF8.cMB2WC(outfilename.c_str()),
                  wxCSConv(suri::XmlElement::xmlEncoding_.c_str())).c_str();
#endif
   pparamsnode->AddChild(CreateKeyValueNode("url-salida", outfilename));
   /** Bandas de baja resolucion **/
   std::vector<BandInfo> rgbelements;
   pAdaptLayer_->GetAttribute<std::vector<BandInfo> >(ProcessAdaptLayer::OutputRGBBands,
                                               rgbelements);
   std::string lowresband;
   for (size_t i = 0; i < rgbelements.size(); ++i) {
      lowresband += rgbelements[i].GetElement()->GetUrl().c_str();
      lowresband += ";";
      lowresband += NumberToString<int>(rgbelements[i].GetPosition());
      lowresband += ",";
   }
#ifdef __WINDOWS__
   lowresband = wxString(wxConvUTF8.cMB2WC(lowresband.c_str()),
                  wxCSConv(suri::XmlElement::xmlEncoding_.c_str())).c_str();
#endif

   pparamsnode->AddChild(
         CreateKeyValueNode("bandas-rva", lowresband.substr(0, lowresband.size() - 1)));
   /** Banda de alta resolucion, pancromatica **/
   BandInfo panElement;
   pAdaptLayer_->GetAttribute<BandInfo>(ProcessAdaptLayer::OutputHighResBand, panElement);
   std::string pancroband = panElement.GetElement()->GetUrl().c_str();
   pancroband += ";";
   pancroband += NumberToString<int>(panElement.GetPosition());

#ifdef __WINDOWS__
   pancroband = wxString(wxConvUTF8.cMB2WC(pancroband.c_str()),
                  wxCSConv(suri::XmlElement::xmlEncoding_.c_str())).c_str();
#endif

   pparamsnode->AddChild(CreateKeyValueNode("banda-pancromatica", pancroband));
   wxXmlDocument newdoc;
   newdoc.SetRoot(pparamsnode);
   wxStringOutputStream stream;
   newdoc.Save(stream);
   return stream.GetString().c_str();
}
}  // namespace suri
