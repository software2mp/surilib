/*! \file TasseledCapRenderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <string>

#include "TasseledCapRenderer.h"

#include "MemoryCanvas.h"
#include "TasseledCapFactory.h"
#include "TasseledCapTransformation.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Canvas.h"
#include "suri/DataTypes.h"
#include "suri/Element.h"
#include "suri/xmlnames.h"

AUTO_REGISTER_RENDERER(suri::raster::TasseledCapRenderer);

namespace suri {
namespace raster {

/**
 * Ctor.
 */
TasseledCapRenderer::TasseledCapRenderer() :
      pTcTranf_(NULL) {

}

/**
 * Dtor.
 */
TasseledCapRenderer::~TasseledCapRenderer() {

}

/**
 * Crea un renderizador a partir de un pElement.
 */
Renderer* TasseledCapRenderer::Create(Element* pElement, Renderer* pPreviousRenderer) const {
   namespace SRT = suri::raster::transformation;

   if (pPreviousRenderer == NULL) {
      return NULL;
   }

   ParameterCollection params;

   // Nombre del sensor.
   wxString sensornp = wxString(wxT(RENDERIZATION_NODE)) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(TASSELEDCAP_NODE) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(wxT(TASSELEDCAP_SENSOR_NODE));

   wxXmlNode* psensornode = pElement->GetNode(sensornp);
   if (!psensornode) {
      REPORT_ERROR("D:No se pudo encontrar el nodo XML en el TasseledCapRenderer\"%s\"",
                   pElement->GetName().c_str());
      return NULL;
   }

   std::string sensor = psensornode->GetNodeContent().c_str();

   // Bandas de salida.
   wxString outputbandsnp = wxString(wxT(RENDERIZATION_NODE)) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(TASSELEDCAP_NODE) + wxT(NODE_SEPARATION_TOKEN)
         + wxString(wxT(TASSELEDCAP_OUTPUT_NODE));

   wxXmlNode* poutputnode = pElement->GetNode(outputbandsnp);
   if (!poutputnode) {
      REPORT_ERROR("D:No se pudo encontrar el nodo XML en el TasseledCapRenderer\"%s\"",
                   pElement->GetName().c_str());
      return NULL;
   }

   std::vector<int> outputbands;

   std::vector<std::string> tempob = SplitEx(poutputnode->GetNodeContent().c_str(), " ");
   for (int ix = 0, lenix = tempob.size(); ix < lenix; ++ix)
      outputbands.push_back(suri::StringToNumber<int>(tempob[ix]));

   params.AddValue<std::vector<int> >("OUTPUTBANDS", outputbands);

   // Creo el transformador.
   SRT::TasseledCapTransformation* ptranf =
         SRT::TasseledCapFactory::GetInstance()->Create(sensor, params);

   TasseledCapRenderer* prenderer = new TasseledCapRenderer();
   prenderer->SetTransformation(ptranf);

   return prenderer;
}

/**
 * Nombre del renderizador == al nombre del nodo.
 */
std::string TasseledCapRenderer::CreatedNode() const {
   return TASSELEDCAP_NODE;
}

/**
 * Renderiza el elemento dado un World en un Canvas.
 */
bool TasseledCapRenderer::Render(const World* pWorldWindow, Canvas* pCanvas,
                                 Mask* pMask) {
   bool prevrenderizationstatus = true;

   // Cambio tipo de dato en canvas y llamo a Render
   Canvas* ptempcanvas = pCanvas;
   int canvassizex = 0, canvassizey = 0, originalbandcount = 0, outputbandcount = 0;
   std::string datatype;
   if (pPreviousRenderer_ && pCanvas) {
      pPreviousRenderer_->GetOutputParameters(canvassizex, canvassizey,
                                              originalbandcount, datatype);

      if (pTcTranf_->GetRequiredInputBands() > originalbandcount)
         REPORT_AND_FAIL_VALUE("El elemento seleccionado no tiene la cantidad de bandas requeridas por el sensor elegido.", false);

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
      pCanvas->SetDataType(DataInfo<TASSELED_CAP_OUTPUT_DATA_TYPE>::Name);
   }
   outputbandcount = pTcTranf_->GetOutputBandCount();
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
   pTcTranf_->Transform(indatatype, originalbands, canvassizex * canvassizey, outdata);

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
void TasseledCapRenderer::GetBoundingBox(const World* pWorld, double& Ulx, double& Uly,
                                         double& Lrx, double& Lry) {
   // NO SE IMPLEMENTA
}

/**
 * Obtiene los parametros de entrada del renderer.
 */
void TasseledCapRenderer::GetInputParameters(int& SizeX, int& SizeY, int& BandCount,
                                             std::string& DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
}

/**
 * Obtiene los parametros de salida del renderer.
 */
void TasseledCapRenderer::GetOutputParameters(int& SizeX, int& SizeY, int& BandCount,
                                              std::string& DataType) const {
   if (pPreviousRenderer_) {
      pPreviousRenderer_->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
   }
   DataType = DataInfo<TASSELED_CAP_OUTPUT_DATA_TYPE>::Name;
   BandCount = TASSELED_CAP_OUTPUT_BAND_COUNT;
}

/**
 * Actualizacion de parametros para representar un elemento.
 */
void TasseledCapRenderer::Update(Element* pElement) {
   // NO SE IMPLEMENTA
}

/**
 * Establece la transformacion a ser utilizada.
 */
void TasseledCapRenderer::SetTransformation(
      suri::raster::transformation::TasseledCapTransformation* pTcTranf) {
   pTcTranf_ = pTcTranf;
}

}  // namespace raster
}  // namespace suri
