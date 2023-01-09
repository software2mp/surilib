/*! \file PrincipalComponentAnalysisRenderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <PrincipalComponentAnalysisRenderer.h>

#include "MemoryCanvas.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Canvas.h"
#include "suri/DataTypes.h"
#include "suri/Element.h"
#include "suri/ParameterCollection.h"
#include "suri/xmlnames.h"

AUTO_REGISTER_RENDERER(suri::raster::PrincipalComponentAnalysisRenderer);

namespace suri {
namespace raster {

typedef void (*PTRANSFORM)(const std::vector<void*>& InData, std::vector<void*>& OutData,
      const std::vector<double*>& AutoVec, int InDataLen);

/**
 * Traduce el valor de la imagen de entrada a valor correspondiente
 * segun el analisis de componentes principales realizado.
 */
template<typename T>
void TranformData(const std::vector<void*>& InData, std::vector<void*>& OutData,
                  const std::vector<double*>& AutoVec, int InDataLen) {
   for (int dnix = 0, lendnix = InDataLen; dnix < lendnix; ++dnix) {
      for (int outix = 0, lenoutix = OutData.size(); outix < lenoutix; ++outix) {
         double* poutband = static_cast<double*>(OutData[outix]);
         double* pautovec = AutoVec[outix];

         double accum = 0.0;
         for (int inx = 0, leninx = InData.size(); inx < leninx; ++inx) {
            accum += pautovec[inx] * (static_cast<T*>(InData[inx]))[dnix];
         }

         poutband[dnix] = accum;
      }
   }
}

/**
 * Ctor.
 */
PrincipalComponentAnalysisRenderer::PrincipalComponentAnalysisRenderer() {

}

/**
 * Dtor.
 */
PrincipalComponentAnalysisRenderer::~PrincipalComponentAnalysisRenderer() {

}

/**
 * Crea un renderizador a partir de un pElement.
 */
Renderer* PrincipalComponentAnalysisRenderer::Create(Element* pElement, Renderer* pPreviousRenderer) const {
   if (pPreviousRenderer == NULL) {
      return NULL;
   }

   ParameterCollection params;

   // Verifico que tengo la informacion basica.
   wxString outputbandsnp = wxString(wxT(RENDERIZATION_NODE)) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(PCANALYSIS_NODE) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(wxT(PCANALYSIS_COMPONENTS_NODE));

   wxXmlNode* poutputnode = pElement->GetNode(outputbandsnp);
   if (!poutputnode) {
      REPORT_ERROR("D:No se pudo encontrar el nodo XML en el PrincipalComponentAnalysisRenderer\"%s\"",
                   pElement->GetName().c_str());
      return NULL;
   }

   // Reestablezco los autovectores que ademas me dan las bandas de salida.
   std::vector<double*> autovec;

   wxXmlNode* pcomp = poutputnode->GetChildren();
   while (pcomp != NULL) {
      wxXmlNode* pav = pcomp->GetChildren();
      if (pav != NULL) {
         std::string content = pav->GetNodeContent().c_str();
         std::vector<std::string> vals = SplitEx(content, " ");

         double* pvals = new double[vals.size()];
         for (int ix = 0, lenix = vals.size(); ix < lenix; ++ix)
            pvals[ix] = StringToNumber<double>(vals[ix]);

         autovec.push_back(pvals);
      }
      pcomp = pcomp->GetNext();
   }


   PrincipalComponentAnalysisRenderer* prenderer = new PrincipalComponentAnalysisRenderer();
   prenderer->SetAutoVec(autovec);

   return prenderer;
}

/**
 * Nombre del renderizador == al nombre del nodo.
 */
std::string PrincipalComponentAnalysisRenderer::CreatedNode() const {
   return PCANALYSIS_NODE;
}

/**
 * Renderiza el elemento dado un World en un Canvas.
 */
bool PrincipalComponentAnalysisRenderer::Render(const World* pWorldWindow, Canvas* pCanvas,
                                 Mask* pMask) {
   bool prevrenderizationstatus = true;

   // Cambio tipo de dato en canvas y llamo a Render
   Canvas* ptempcanvas = pCanvas;
   int canvassizex = 0, canvassizey = 0, originalbandcount = 0, outputbandcount = 0;
   std::string datatype;
   if (pPreviousRenderer_ && pCanvas) {
      pPreviousRenderer_->GetOutputParameters(canvassizex, canvassizey,
                                              originalbandcount, datatype);

      pCanvas->SetDataType(datatype);
      // no puede cambiar el tipo de canvas
      if (pCanvas->GetDataType() != datatype) {
         ptempcanvas = new MemoryCanvas;
         REPORT_DEBUG("D:Se creo un canvas intermedio");
      }
      prevrenderizationstatus = pPreviousRenderer_->Render(pWorldWindow, ptempcanvas,
                                                           pMask);
   } else {
      REPORT_AND_FAIL_VALUE("D:Canvas no puede ser nulo al aplicar la transformacion.", false);
   }

   // Leo el tamanio del canvas retornado
   pCanvas->GetSize(canvassizex, canvassizey);
   if (canvassizex < 1 || canvassizey < 1) {
      REPORT_AND_FAIL_VALUE("D:Tamano de canvas erroneo", false);
   }

   // leo el canvas retornado
   int indatalength = canvassizex * canvassizey * pCanvas->GetDataSize();
   std::string indatatype = pCanvas->GetDataType();
   std::vector<int> bands(originalbandcount);
   std::vector<void*> data(originalbandcount);
   for (int b = 0; b < originalbandcount; b++) {
      bands[b] = b;
      data[b] = new unsigned char[indatalength];
   }
   pCanvas->Read(bands, data);

   // Si el canvas es el original cambio el tipo de dato al de la salida y el numero de bandas
   if (ptempcanvas == pCanvas) {
      pCanvas->SetDataType(DataInfo<PCANALYSIS_OUTPUT_DATA_TYPE>::Name);
   }
   outputbandcount = PCANALYSIS_OUTPUT_BAND_COUNT;
   pCanvas->SetBandCount(outputbandcount);
   pCanvas->SetSize(canvassizex, canvassizey);

   // Creo vector donde se guardan datos de salida temporales
   std::vector<void*> outdata(outputbandcount);
   for (int b = 0; b < outputbandcount; b++)
      outdata[b] =
            new unsigned char[canvassizex * canvassizey * pCanvas->GetDataSize()];

   // Agrego todas las bandas a un std::vector.
   std::vector<void*> originalbands;
   for (int b = 0; b < originalbandcount; b++)
      originalbands.push_back(data[b]);

   // Aplico la transformacion.
   Transform(originalbands, outdata, indatatype, autoVec_, canvassizex * canvassizey);

   // Guardo cambios
   pCanvas->Write(bands, outdata);

   // Limpio el vector con bandas originales
   originalbands.clear();

   // libero el temporario
   for (int b = 0; b < originalbandcount; b++)
      delete[] static_cast<unsigned char*>(data[b]);

   for (int b = 0; b < outputbandcount; b++)
      delete[] static_cast<unsigned char*>(outdata[b]);

   return true && prevrenderizationstatus;
}

/**
 * Obtiene el "bounding box" del elemento renderizado.
 */
void PrincipalComponentAnalysisRenderer::GetBoundingBox(const World* pWorld,
                                                        double& Ulx, double& Uly,
                                                        double& Lrx, double& Lry) {
   // NO SE IMPLEMENTA
}

/**
 * Obtiene los parametros de entrada del renderer.
 */
void PrincipalComponentAnalysisRenderer::GetInputParameters(
      int& SizeX, int& SizeY, int& BandCount, std::string& DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
}

/**
 * Obtiene los parametros de salida del renderer.
 */
void PrincipalComponentAnalysisRenderer::GetOutputParameters(
      int& SizeX, int& SizeY, int& BandCount, std::string& DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
   DataType = DataInfo<PCANALYSIS_OUTPUT_DATA_TYPE>::Name;
   BandCount = PCANALYSIS_OUTPUT_BAND_COUNT;
}

/**
 * Actualizacion de parametros para representar un elemento.
 */
void PrincipalComponentAnalysisRenderer::Update(Element* pElement) {
   // NO SE IMPLEMENTA
}

/**
 * Estable los autovectores a utilizar.
 */
void PrincipalComponentAnalysisRenderer::SetAutoVec(const std::vector<double*>& AutoVec) {
   autoVec_ = AutoVec;
}

/**
 * Transforma los valores de entrada.
 */
void PrincipalComponentAnalysisRenderer::Transform(const std::vector<void*>& InData,
                                                   std::vector<void*>& OutData,
                                                   const std::string& DataType,
                                                   const std::vector<double*>& AutoVec,
                                                   int InDataLen) {
   PTRANSFORM ptransform = NULL;

   if (DataType.compare(suri::DataInfo<unsigned char>::Name) == 0) {
      ptransform = &TranformData<unsigned char>;
   } else if (DataType.compare(suri::DataInfo<unsigned short>::Name) == 0) {
      ptransform = &TranformData<unsigned short>;
   } else if (DataType.compare(suri::DataInfo<short>::Name) == 0) {
      ptransform = &TranformData<short>;
   } else if (DataType.compare(suri::DataInfo<unsigned int>::Name) == 0) {
      ptransform = &TranformData<unsigned int>;
   } else if (DataType.compare(suri::DataInfo<int>::Name) == 0) {
      ptransform = &TranformData<int>;
   } else if (DataType.compare(suri::DataInfo<float>::Name) == 0) {
      ptransform = &TranformData<float>;
   } else if (DataType.compare(suri::DataInfo<double>::Name) == 0) {
      ptransform = &TranformData<double>;
   }

   if (ptransform != NULL) {
      ptransform(InData, OutData, AutoVec, InDataLen);
   }
}

}  // namespace raster
}  // namespace suri
