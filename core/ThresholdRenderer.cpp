/*! \file ThresholdRenderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <limits>
#include <string>
#include <vector>
// Includes Suri
#include "suri/Element.h"
#include "Mask.h"
#include "ThresholdRenderer.h"
#include "SRDHistogram.h"
// Includes Wx
// Defines
// forwards

AUTO_REGISTER_RENDERER(suri::ThresholdRenderer);

namespace suri {

const int ThresholdRenderer::kHistogramBins = 100;


ThresholdRenderer::~ThresholdRenderer() {}

ThresholdRenderer::Parameters::Parameters() :
    thresholdHigh(Mask::Valid), thresholdLow(Mask::Invalid),
    thresholdPercent(90){}

ThresholdRenderer::Parameters ThresholdRenderer::GetParameters(const wxXmlNode* pNode) {
   wxString path = wxT(RENDERIZATION_NODE NODE_SEPARATION_TOKEN THRESHOLD_NODE);
   const wxXmlNode *pParamNode = GetNodeByName(path, pNode);
   ThresholdRenderer::Parameters params;
   if (pParamNode){
      std::string str = Xml2String(pParamNode);
      str.c_str();
      wxString value;
      if (pParamNode->GetPropVal(wxT(THRESHOLD_HIGH_PROPERTY), &value)) {
         params.thresholdHigh = StringToNumber<int>(value.mb_str());
      }
      if (pParamNode->GetPropVal(wxT(THRESHOLD_LOW_PROPERTY), &value)) {
         params.thresholdLow = StringToNumber<int>(value.mb_str());
      }
      if (pParamNode->GetPropVal(wxT(THRESHOLD_PERCENT_PROPERTY), &value)) {
         params.thresholdPercent = StringToNumber<int>(value.mb_str());
      }
   }
   
   return params;
}


Renderer *ThresholdRenderer::Create(Element *pElement,
                                    Renderer *pPreviousRenderer) const {
   if (!pPreviousRenderer) {
      return NULL;
   }
   Parameters params;
   params = GetParameters(pElement->GetNode(wxT("")));
   ThresholdRenderer *pRenderer = new ThresholdRenderer;
   pRenderer->thresholdHigh_ = params.thresholdHigh;
   pRenderer->thresholdLow_ = params.thresholdLow;
   pRenderer->thresholdPercent_ = params.thresholdPercent;
   pRenderer->adjustmentLoops_ = 1;
   return pRenderer;
}

/**
 * Nombre del renderizador == al nombre del nodo
 * @return string con el nombre del renderer
 */
std::string ThresholdRenderer::CreatedNode() const {
   return wxT(THRESHOLD_NODE);
}

/**
 * Obtiene el "bounding box" del elemento renderizado
 * @param[in]  pWorld: puntero a la ventana del mundo.
 * @param[in]  Ulx: uper-left x
 * @param[in]  Uly: uper-left y
 * @param[in]  Lrx: lower-right x
 * @param[in]  Lry: lower-right y
 */
void ThresholdRenderer::GetBoundingBox(const World *pWorld, double &Ulx,
                                               double &Uly, double &Lrx, double &Lry) {
}

/**
 * Obtiene los parametros de entrada del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void ThresholdRenderer::GetInputParameters(int &SizeX, int &SizeY,
                                                   int &BandCount,
                                                   std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
}

/**
 * Obtiene los parametros de salida del renderer
 * @param[out] SizeX Tamanio de salida en X por el renderizador.
 * @param[out] SizeY Tamanio de salida en Y por el renderizador.
 * @param[out] BandCount Bandas generadas.
 * @param[out] DataType Tipo de dato.
 */
void ThresholdRenderer::GetOutputParameters(int &SizeX, int &SizeY,
                                                    int &BandCount,
                                                    std::string &DataType) const {
   GetInputParameters(SizeX, SizeY, BandCount, DataType);
   DataType = DataInfo<unsigned char>::Name;
}


// ----------------------- METODOS DE ACTUALIZACION -------------------------
/**
 * Actualizacion de parametros para representar un elemento
 * @param[in]  pElement: puntero al elemento.
 */
void ThresholdRenderer::Update(Element *pElement) {
   // wxXmlNode *pNode = pElement->GetNode(wxT(""));
}

/**
 * Calcula la registrabilidad de las baldosas segun la cantidad entropia que
 * poseen en cada subregion.
 * @param[in] pWorldWindow mundo con informacion del subset a renderizar
 * @param[in] pCanvas datos sobre los que se aplicara el filtro
 * @param[out] pCanvas datos obtenidos de aplicar el filtro
 * @param[in] pMask mascara
 */
bool ThresholdRenderer::Render(const World *pWorldWindow, Canvas* pCanvas,
                                       Mask* pMask) {
   bool prevrenderizationstatus = true;
   if (pPreviousRenderer_ && pCanvas) {
      prevrenderizationstatus = pPreviousRenderer_->Render(pWorldWindow, pCanvas, pMask);
   } else {
      REPORT_AND_FAIL_VALUE("D:Canvas no puede ser nulo al aplicar filtro.", false);
   }
   int canvassizex, canvassizey, bandcount = 0;
   std::string datatype;
   // Leo el tamanio del canvas retornado
   pPreviousRenderer_->GetOutputParameters(canvassizex, canvassizey, bandcount, datatype);
   // Leo el tamanio del canvas retornado
   pCanvas->GetSize(canvassizex, canvassizey);
   std::string canvasInputType = pCanvas->GetDataType();
   if (canvassizex < 1 || canvassizey < 1) {
      REPORT_AND_FAIL_VALUE("D:Tamano de canvas erroneo", false);
   }
   if (datatype.compare(DataInfo<float>::Name) != 0) {
      REPORT_AND_FAIL_VALUE("D:Tipo de dato del canvas erroneo", false);
   }
   // Cargo bandas
   std::vector<int> bands(bandcount);
   std::vector<void*> data(bandcount);
   int canvasSize = canvassizex * canvassizey;
   for (int b = 0; b < bandcount; b++) {
      bands[b] = b;
      data[b] = new unsigned char[canvasSize * pCanvas->GetDataSize()];
   }
   // Cargo bandas
   pCanvas->Read(bands, data);
   // Armo histogramas
   raster::data::Histogram<float>* hist = CreateHistograms(data, canvasSize);
   double* threshold = hist->GetMinNPercent(this->thresholdPercent_);
   ApplyThreshold(data, threshold, canvasSize);
   pCanvas->Write(bands, data);
   // Libero memoria
   for (int b = 0; b < bandcount; b++) {
      delete[] static_cast<float*>(data[b]);
   }
   delete[] threshold;
   delete hist;
   return true && prevrenderizationstatus;
}

raster::data::Histogram<float>* ThresholdRenderer::CreateHistograms(
      std::vector<void*> data, int canvasSize) {
   int numBands = data.size();
   int *numBins = new int[numBands];
   // Valores minimos por banda
   double *minValues = new double[numBands];
   // Valores maximos por banda
   double *maxValues = new double[numBands];
   /** Configuro el histograma **/
   for (int i = 0; i < numBands; ++i) {
      minValues[i] = 0, maxValues[i] = 0;
      raster::data::Histogram<float>::GetLimits(data[i], canvasSize, minValues[i], maxValues[i]);
      numBins[i] = kHistogramBins;
   }
   for (int i = 0; i < this->adjustmentLoops_; ++i){
      AdjustLimits(data, canvasSize, numBins, minValues, maxValues);
   }
   raster::data::Histogram<float>* hist;
   hist = new raster::data::Histogram<float>(numBands, numBins, minValues, maxValues);
   /** Armo el histograma **/
   for (int i = 0; i < numBands; ++i) {
      hist->CountPixels(i, canvasSize, data[i]);
   }
   delete[] minValues;
   delete[] maxValues;
   delete[] numBins;
   return hist;
}

void ThresholdRenderer::AdjustLimits(std::vector<void*> data, int canvasSize,
                                     int* pNumBins, double* pMinValues,
                                     double* pMaxValues) {
   int numBands = data.size();
   raster::data::Histogram<float>* hist;
   hist = new raster::data::Histogram<float>(numBands, pNumBins,
                                             pMinValues, pMaxValues);
   for (int i = 0; i < numBands; ++i) {
      hist->CountPixels(i, canvasSize, data[i]);
   }
   for (int band = 0; band < numBands; ++band) {
      double* newMax = hist->GetMaxNPercent(2);
      double* newMin = hist->GetMinNPercent(2);
      double* binSize = hist->GetScale();
      // Actualizo limites
      pMaxValues[band] = newMax[band] + binSize[band];
      pMinValues[band] = newMin[band];
      delete[] newMax;
      delete[] newMin;
   }
   delete hist;
}


void ThresholdRenderer::ApplyThreshold(std::vector<void*>& data,
                                       double* threshold,
                                       int canvasSize) {
   for (unsigned int bandix = 0; bandix < data.size(); ++bandix) {
      float* pData = static_cast<float*>(data[bandix]);
      for (int i = 0; i < canvasSize; ++i) {
         if (pData[i] < threshold[bandix]) {
            pData[i] = thresholdLow_;
         } else {
            pData[i] = thresholdHigh_;
         }
      }
   }
}


} /** namespace suri */
