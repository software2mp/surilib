/** \file SREnhancementRenderer.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <climits>
#include <cmath>

#include "SREnhancementRenderer.h"

#include "LutRenderer.h"
#include "suri/xmlnames.h"
#include "suri/DataTypes.h"
#include "suri/Element.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Canvas.h"
#include "suri/XmlFunctions.h"
#include "SREEnhancementUtils.h"

namespace suri {
namespace render {

typedef void (*FTRANSLATE)(int* pLut, void* pInData, void* pOutData, int DataSize,
                           int NumBins, double Min, double Max, double NoDataValue,
                           bool NoDataValueAvailable);

/**
 * Traduce el valor de la imagen de entrada a valor correspondiente
 * segun la LUT provista.
 */
template<typename T>
void TranslateData(int* pLut, void* pInData, void* pOutData, int DataSize, int NumBins,
                   double Min, double Max, double NoDataValue, bool NoDataValueAvailable) {
   T* pindata = static_cast<T*>(pInData);
   T* poutdata = static_cast<T*>(pOutData);

   double scale = ((double) Max - (double) Min) / (double) (NumBins - 1);

   for (int DSix = 0; DSix < DataSize; ++DSix) {
      if (pindata[DSix] != INFINITY && pindata[DSix] != -INFINITY
            && !(NoDataValueAvailable && pindata[DSix] == NoDataValue)
            && Min <= pindata[DSix]) {
         int binvalue = static_cast<int>(floor((pindata[DSix] - Min) / scale));
         /**
          * TODO(Gabriel): Si es mayor que Numbins se va fuera del array.
          * Esto ocurre cuando se modifican los maximos y minimos
          * del histograma
          *
          */
         if (binvalue >= 0)
            poutdata[DSix] = (binvalue < NumBins) ? pLut[binvalue] : pLut[NumBins - 1];
         else
            poutdata[DSix] = pLut[0];
      } else {
         poutdata[DSix] = pindata[DSix];
      }
   }
}

/**
 * Caso especial para realce 0-255. Traduce el valor de la imagen de entrada
 * a valor correspondiente segun la LUT provista siempre y cuando el valor
 * de entrada se encuentre entre 0 y 255. Menor que 0 devuelve 0 y mayor
 * que 255 devuelve 255.
 */
template<typename T>
void TranslateRawData(int* pLut, void* pInData, void* pOutData, int DataSize,
                      int NumBins, double Min, double Max, double NoDataValue,
                      bool NoDataValueAvailable) {
   T* pindata = static_cast<T*>(pInData);
   T* poutdata = static_cast<T*>(pOutData);

   for (int DSix = 0; DSix < DataSize; ++DSix) {
      if (!(NoDataValueAvailable && pindata[DSix] == NoDataValue)) {
         if (pindata[DSix] >= 0.0 && pindata[DSix] <= 255.0) {
            poutdata[DSix] = pindata[DSix];
         } else if (pindata[DSix] < 0.0) {
            poutdata[DSix] = 0;
         } else if (pindata[DSix] > 255.0) {
            poutdata[DSix] = 255;
         }
      }
   }
}

/**
 * Caso especial para realce 0-255. Traduce el valor de la imagen de entrada
 * a valor correspondiente segun la LUT provista siempre y cuando el valor
 * de entrada se encuentre entre 0 y 255. Menor que 0 devuelve 0 y mayor
 * que 255 devuelve 255.
 */
template<>
void TranslateRawData<unsigned char>(int* pLut, void* pInData, void* pOutData,
                                     int DataSize, int NumBins, double Min, double Max,
                                     double NoDataValue, bool NoDataValueAvailable) {
   unsigned char* pindata = static_cast<unsigned char*>(pInData);
   unsigned char* poutdata = static_cast<unsigned char*>(pOutData);

   for (int DSix = 0; DSix < DataSize; ++DSix) {
      if (!(NoDataValueAvailable && pindata[DSix] == NoDataValue)) {
         poutdata[DSix] = pindata[DSix];
      }
   }
}

AUTO_REGISTER_RENDERER(suri::render::EnhancementRenderer);

/**
 * Ctor.
 */
EnhancementRenderer::EnhancementRenderer() {

}

/**
 * Ctor.
 */
EnhancementRenderer::EnhancementRenderer(int BandCount, int* pNumBins,
                                         double* pLowValue, double* pHighValue,
                                         int** pBins, bool Active, std::string& Name) :
      bandCount_(BandCount), pNumBins_(pNumBins), pLowValue_(pLowValue),
      pHighValue_(pHighValue), pBins_(pBins), active_(Active), name_(Name) {

}

/**
 * Dtor.
 */
EnhancementRenderer::~EnhancementRenderer() {

}

/**
 * Crea un renderizador a partir de un pElement.
 */
Renderer* EnhancementRenderer::Create(Element *pElement, Renderer *pPreviousRenderer) const {
   namespace SREU = suri::raster::enhancement::utils;

   wxXmlNode* pchildren = NULL;
   wxXmlNode* prendnode = pElement->GetNode(wxT(""))->GetChildren();

   // Busco el nodo de renderizacion.
   while (prendnode != NULL && pchildren == NULL) {
      if (prendnode->GetName() == wxT(RENDERIZATION_NODE)) {
         pchildren = prendnode->GetChildren();
      } else {
         prendnode = prendnode->GetNext();
      }
   }

   // Datos para crear el renderer.
   int bandcount;
   int* pnumbins;
   double* plowvalue;
   double* phighvalue;
   int** pbins;
   bool active;
   std::string name;

   // Busco el nodo con informacion para realce y
   while (pchildren != NULL) {
      if (pchildren->GetName() == wxT(ENHANCEMENT_NODE)) {
         SREU::LutFromXml(pchildren, bandcount, pnumbins, plowvalue, phighvalue, pbins, active, name);
         break;
      } else {
         pchildren = pchildren->GetNext();
      }
   }

   // Momentaneamente el valor no valido se saca aca y solo para el
   // renderizador de realces.
   bool available = false;
   double nodatavalue = 0.0;
   wxXmlNode* pdatanotvalidvalue = pElement->GetNode(
   PATH_NOT_VALID_DATA_VALUE_NODE_PIPES);
   if (pdatanotvalidvalue) {
      pdatanotvalidvalue = pdatanotvalidvalue->GetChildren();
      if (pdatanotvalidvalue->GetType() == wxXML_TEXT_NODE) {
         wxString ctt = pdatanotvalidvalue->GetContent();
         available = true;
         nodatavalue = StringToNumber<double>(ctt.mb_str());
      }
   }

   EnhancementRenderer* prenderer = new EnhancementRenderer(bandcount, pnumbins,
                                                           plowvalue, phighvalue, pbins,
                                                           active, name);
   prenderer->SetNoDataValueAvailable(available);
   prenderer->SetNoDataValue(nodatavalue);

   return prenderer;
}

/**
 * Nombre del renderizador == al nombre del nodo.
 */
std::string EnhancementRenderer::CreatedNode() const {
   return ENHANCEMENT_NODE;
}

/**
 * Renderiza el elemento dado un World en un Canvas.
 */
bool EnhancementRenderer::Render(const World *pWorldWindow, Canvas* pCanvas,
                                 Mask* pMask) {
   bool prevrenderizationstatus = true;

   // Verifico que datos de entrada sean correctos
   if (pPreviousRenderer_) {
      prevrenderizationstatus = pPreviousRenderer_->Render(pWorldWindow, pCanvas,
                                                           pMask);
   }
   if (!pCanvas) {
      REPORT_AND_FAIL_VALUE("D:Canvas no puede ser nulo al aplicar LUT.", false);
   }

   int sizex, sizey, bandcount = 0;
   std::string dt;
   GetInputParameters(sizex, sizey, bandcount, dt);
   pCanvas->GetSize(sizex, sizey);

   if (pCanvas->GetDataType() != dt) {
      std::string message = "D:No se puede renderizar, inconsistencia entre el ";
      message.append("tipo de datos del canvas y los de salida del renderer");
      REPORT_AND_FAIL_VALUE(message.c_str(), false);
   }

   int size = sizex * sizey;

   // Leo los datos en el canvas
   std::vector<int> bands(bandcount);
   std::vector<void*> indata(bandcount);
   for (int bandix = 0; bandix < bandcount; ++bandix) {
      bands[bandix] = bandix;
      indata[bandix] = new unsigned char[sizex * sizey * pCanvas->GetDataSize()];
   }
   pCanvas->Read(bands, indata);

   // Inicializo el canvas con la nueva dimension
   pCanvas->SetBandCount(bandcount);
   pCanvas->SetSize(sizex, sizey);

   // Creo vector donde se guardan datos de salida temporales
   std::vector<void*> outdata(bandcount);
   for (int bandix = 0; bandix < bandcount; bandix++)
      outdata[bandix] = new unsigned char[sizex * sizey * pCanvas->GetDataSize()];

   // Aplico LUTs a las bandas
   std::vector<int> newbands(bandcount);
   for (int bandix = 0; bandix < bandcount; ++bandix) {
      Translate(pCanvas->GetDataType(), pBins_[bandix], indata[bandix], outdata[bandix],
                size, pNumBins_[bandix], pLowValue_[bandix], pHighValue_[bandix],
                GetNoDataValue(), IsNoDataValueAvailable());

      newbands[bandix] = bandix;
   }
   pCanvas->Write(newbands, outdata);

   // Elimino datos de entrada temporales y los datos de salida temporales
   for (int bandix = 0; bandix < bandcount; bandix++) {
      delete[] static_cast<unsigned char*>(indata[bandix]);
      delete[] static_cast<unsigned char*>(outdata[bandix]);
   }

   return prevrenderizationstatus;
}

/**
 * Obtiene el "bounding box" del elemento renderizado.
 */
void EnhancementRenderer::GetBoundingBox(const World *pWorld, double &Ulx, double &Uly, double &Lrx,
                            double &Lry) {

}

/**
 * Obtiene los parametros de entrada del renderer.
 */
void EnhancementRenderer::GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }

   BandCount = bandCount_;
}

/**
 * Obtiene los parametros de salida del renderer.
 */
void EnhancementRenderer::GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                 std::string &DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }

   BandCount = bandCount_;
}

/**
 * Actualizacion de parametros para representar un elemento.
 */
void EnhancementRenderer::Update(Element *pElement) {
   // Se actualizan los parametros del render en funcion del elemento de entrada
   namespace SREU = suri::raster::enhancement::utils;

   wxXmlNode* pchildren = NULL;
   wxXmlNode* prendnode = pElement->GetNode(wxT(""))->GetChildren();

   // Busco el nodo de renderizacion.
   while (prendnode != NULL && pchildren == NULL) {
      if (prendnode->GetName() == wxT(RENDERIZATION_NODE)) {
         pchildren = prendnode->GetChildren();
      } else {
         prendnode = prendnode->GetNext();
      }
   }

   // Datos para crear el renderer.
   int bandcount;
   int* pnumbins;
   double* plowvalue;
   double* phighvalue;
   int** pbins;
   bool active;
   std::string name;

   // Busco el nodo con informacion para realce y
   while (pchildren != NULL) {
      if (pchildren->GetName() == wxT(ENHANCEMENT_NODE)) {
         SREU::LutFromXml(pchildren, bandcount, pnumbins, plowvalue, phighvalue, pbins, active, name);
         break;
      } else {
         pchildren = pchildren->GetNext();
      }
   }
   bandCount_ = bandcount;
   pNumBins_ = pnumbins;
   pLowValue_ = plowvalue;
   pHighValue_ = phighvalue;
   pBins_ = pbins;
   active_ = active;
   name_ = name;
}

/**
 * Ejecuta la verdadera funcion de transformacion teniendo en cuenta
 * el realce seleccionado y el tipo de dato. Ver documentacion de
 * TranslateData y TranslateRawData.
 */
void EnhancementRenderer::Translate(const std::string& DataType, int* pLut,
                                    void* pInData, void* pOutData, int DataSize,
                                    int NumBins, double Min, double Max,
                                    double NoDataValue, bool NoDataValueAvailable) {
   FTRANSLATE ftranslate = NULL;
   if (name_.compare("Linear255Enhancement") != 0) {
      if (DataType.compare(suri::DataInfo<unsigned char>::Name) == 0) {
         ftranslate = &TranslateData<unsigned char>;
      } else if (DataType.compare(suri::DataInfo<unsigned short>::Name) == 0) {
         ftranslate = &TranslateData<unsigned short>;
      } else if (DataType.compare(suri::DataInfo<short>::Name) == 0) {
         ftranslate = &TranslateData<short>;
      } else if (DataType.compare(suri::DataInfo<unsigned int>::Name) == 0) {
         ftranslate = &TranslateData<unsigned int>;
      } else if (DataType.compare(suri::DataInfo<int>::Name) == 0) {
         ftranslate = &TranslateData<int>;
      } else if (DataType.compare(suri::DataInfo<float>::Name) == 0) {
         ftranslate = &TranslateData<float>;
      } else if (DataType.compare(suri::DataInfo<double>::Name) == 0) {
         ftranslate = &TranslateData<double>;
      }
   } else {
      if (DataType.compare(suri::DataInfo<unsigned char>::Name) == 0) {
         ftranslate = &TranslateRawData<unsigned char>;
      } else if (DataType.compare(suri::DataInfo<unsigned short>::Name) == 0) {
         ftranslate = &TranslateRawData<unsigned short>;
      } else if (DataType.compare(suri::DataInfo<short>::Name) == 0) {
         ftranslate = &TranslateRawData<short>;
      } else if (DataType.compare(suri::DataInfo<unsigned int>::Name) == 0) {
         ftranslate = &TranslateRawData<unsigned int>;
      } else if (DataType.compare(suri::DataInfo<int>::Name) == 0) {
         ftranslate = &TranslateRawData<int>;
      } else if (DataType.compare(suri::DataInfo<float>::Name) == 0) {
         ftranslate = &TranslateRawData<float>;
      } else if (DataType.compare(suri::DataInfo<double>::Name) == 0) {
         ftranslate = &TranslateRawData<double>;
      }
   }

   if (ftranslate != NULL) {
      ftranslate(pLut, pInData, pOutData, DataSize, NumBins, Min, Max,
                 NoDataValue, NoDataValueAvailable);
   }
}

}  // namespace render
}  // namespace suri
