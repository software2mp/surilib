/*! \file StackingRenderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "StackingRenderer.h"
#include "suri/xmlnames.h"
#include "suri/Element.h"
#include "suri/AuxiliaryFunctions.h"
#include "Mask.h"
#include "suri/DataTypes.h"

// Includes Wx
// Defines
/** Macro para registrar el renderer */
AUTO_REGISTER_RENDERER(suri::StackingRenderer);

// forwards

namespace suri {

/** constructor **/
StackingRenderer::StackingRenderer() :
      bandCount_(0) {
}

/** destructor **/
StackingRenderer::~StackingRenderer() {
   delete_second_all(stackingParameters_);
}

// ----------------------- METODOS DE RENDERIZACION -------------------------
/** Renderiza el elemento dado un World en un Canvas */
bool StackingRenderer::Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask) {
   pCanvas->SetBandCount(bandCount_);
   if (bandCount_ != pCanvas->GetBandCount())
      return false;
   StackingMap::iterator it = stackingParameters_.begin();
   int bands = 0;
   for (int i = 0; it != stackingParameters_.end(); ++it, ++i) {
      RasterRenderer* pcurrent = pRenderers_.at(i);
      pcurrent->parameters_.imageUrl_ = it->second->imageUrl_;
      pcurrent->parameters_.bandCombination_ = it->second->bandCombination_;
      pcurrent->parameters_.readFunction_ = generalParameters_.readFunction_;
      pcurrent->parameters_.noDataValue_ = generalParameters_.noDataValue_;
      pcurrent->parameters_.spatialModel_ = generalParameters_.spatialModel_;
      pcurrent->parameters_.rasterModel_ = generalParameters_.rasterModel_;
      pcurrent->parameters_.generateMask_ = generalParameters_.generateMask_;
      MemoryCanvas* ptempcanvas = new MemoryCanvas();
      Mask* ptempmask = new Mask();
      if (pcurrent->Render(pWorldWindow, ptempcanvas, ptempmask)) {
         std::vector<int> bandindex;
         for (size_t i = 0; i < pcurrent->parameters_.bandCombination_.size(); ++i)
            bandindex.push_back(i);
         std::vector<void*> outputdata;
         ptempcanvas->GetInternalData(bandindex, outputdata);
         std::vector<int>::iterator bit = bandindex.begin();
         for (; bit != bandindex.end(); ++bit) {
            // muevo los indices de las bandas
            *bit += bands;
         }
         // pMask->Merge(ptempmask);
         dynamic_cast<Canvas*>(pMask)->Write(ptempmask, NULL);
         if (pCanvas->GetDataType().compare(ptempcanvas->GetDataType()) != 0)
            pCanvas->SetDataType(ptempcanvas->GetDataType());
         pCanvas->Write(bandindex, outputdata, pMask);
      } else {
         delete ptempcanvas;
         delete ptempmask;
         return false;
      }
      delete ptempcanvas;
      delete ptempmask;
      bands +=  pcurrent->parameters_.bandCombination_.size();
   }
   return true;
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------
/** Actualizacion de parametros para representar un elemento */
void StackingRenderer::Update(Element *pElement) {
   LoadRendererParameters(pElement);
}

/**
 *  Retorna el nombre del nodo que maneja el renderizador.
 * @return Nombre del nodo manejado por la clase derivada.
 */
std::string StackingRenderer::CreatedNode() const {
   return BAND_STACKING_NODE;
}

/** Crea un renderizador a partir de un pElement */
/**
 *  Metodo  abstracto puro utilizado para crear con el metodo
 * estatico Create() instancias de clases derivadas registradas con el
 * metodo RegisterViewer().
 * @param[in] pElement Elemento a renderizar.
 * @param[in] pPreviousRenderer Renderizador anterior.
 */
StackingRenderer *StackingRenderer::Create(Element *pElement,
                                           Renderer *pPreviousRenderer) const {
   StackingRenderer* prender = new StackingRenderer();
   prender->LoadRendererParameters(pElement);
   return prender;
}

/** Metodo auxiliar que se encarga de cargar los parametros de renderizacion
 *  a partir del xml que contiene el element que se pasa por parametro
 * @param pElement
 */
void StackingRenderer::LoadRendererParameters(Element *pElement) {
   bandCount_ = 0;
   delete_second_all(stackingParameters_);
   stackingParameters_.clear();
   delete_all(pRenderers_);
   pRenderers_.clear();
   wxXmlNode* prootnode = pElement->GetNode(wxT(""));
   if (!prootnode) {
      REPORT_ERROR("D:Nodo NULO.");
   }
   wxXmlNode *pnode = prootnode->GetChildren();
   RasterRenderer::Parameters generalparams;
   while (pnode != NULL) {
      if (pnode->GetName() == RENDERIZATION_NODE) {
         wxXmlNode *pstacknode = pnode->GetChildren();
         while (pstacknode != NULL) {
            if (pstacknode->GetName() == BAND_STACKING_NODE) {
               LoadCombinationParameters(pstacknode);
               break;
            }
            pstacknode = pstacknode->GetNext();
         }
      } else if (pnode->GetName() == URL_NODE) {
         wxString propval;
         std::string stackingid = "0";
         if (pnode->GetPropVal(BAND_STACKING_ID, &propval)) {
            stackingid = propval.c_str();
         }

         StackingMap::iterator findit = stackingParameters_.find(stackingid);
         RasterRenderer::Parameters* pparams = NULL;
         if (findit != stackingParameters_.end()) {
            pparams = findit->second;
         } else {
            pparams = new RasterRenderer::Parameters();
            stackingParameters_.insert(std::make_pair(stackingid, pparams));
         }
         pparams->imageUrl_ = pnode->GetNodeContent().c_str();

      } else if (pnode->GetName() == GEORREFERENCE_NODE) {
         RasterRenderer::GetReferenceNode(generalparams, pnode);
      } else if (pnode->GetName() == FILE_NODE) {
         RasterRenderer::GetNoValidDnValue(generalparams, pnode);
      }
      pnode = pnode->GetNext();
   }
   generalParameters_ = generalparams;
}

void StackingRenderer::LoadCombinationParameters(wxXmlNode* pStackingNode) {
   wxXmlNode *pcnode = pStackingNode->GetChildren();
   while (pcnode != NULL) {
      if (pcnode->GetName() == BAND_COMBINATION_NODE) {
         wxString propval;
         std::string stackingid = "0";
         if (pcnode->GetPropVal(BAND_STACKING_ID, &propval)) {
            stackingid = propval.c_str();
         }
         StackingMap::iterator findit = stackingParameters_.find(stackingid);
         RasterRenderer::Parameters* pparams = NULL;
         if (findit != stackingParameters_.end())
            pparams = findit->second;
         else {
            pparams = new RasterRenderer::Parameters();
            stackingParameters_.insert(std::make_pair(stackingid, pparams));
         }
         std::string bandcombination;
         bandcombination = pcnode->GetNodeContent().c_str();
         pparams->bandCombination_ = split<int>(bandcombination);
         // Siempre usa decimacion (NN) para la lectura
         pparams->readFunction_ = RasterRenderer::Parameters::Decimate;
         bandCount_ += pparams->bandCombination_.size();
         pRenderers_.push_back(new RasterRenderer());
      }
      pcnode = pcnode->GetNext();
   }
}

/** Obtiene los parametros de entrada del renderer */
void StackingRenderer::GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                          std::string &DataType) const {
   DataType = DataInfo<void>::Name;
   SizeX = SizeY = -1;
   BandCount = 0;
}

/** Obtiene los parametros de salida del renderer */
void StackingRenderer::GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                           std::string &DataType) const {
   //TODO(Gabriel - TCK #2981): Implementar
   std::string datatype = DataInfo<void>::Name;
   RendererVector::const_iterator it = pRenderers_.begin();
   for (; it != pRenderers_.end(); ++it) {
      std::string renderdatatype;
      (*it)->GetOutputParameters(SizeX, SizeY, BandCount, renderdatatype);
      // si alguna imagen tiene distinto tipo de dato para las bandas
      // debe fallar
      if ((datatype.compare(DataInfo<void>::Name) != 0)
            && renderdatatype.compare(datatype) != 0) {
         datatype =  DataInfo<void>::Name;
         break;
      } else if (datatype.compare(DataInfo<void>::Name) == 0) {
         datatype = renderdatatype;
      }
   }
   /*if (pRenderers_.size() > 0)
      pRenderers_.at(0)->GetOutputParameters(SizeX, SizeY, BandCount, DataType);
      */
   BandCount = bandCount_;
   DataType = datatype;
}

/** Obtiene el "bounding box" del elemento renderizado */
/**
 * \pre el subset debe ser seteado antes de llamar a esta funcion con un
 *      valor invalido.
 * \post Se refleja el subset de mundo que renderiza.
 * \post No se modifican los parametros en caso de falla o subset nulo
 * @param[in] pWorld Mundo sobre el cual se debe devolver el BB
 * @param[out] Ulx ULx de mundo renderizado.
 * @param[out] Uly ULy de mundo renderizado.
 * @param[out] Lrx LRx de mundo renderizado.
 * @param[out] Lry LRy de mundo renderizado.
 */
void StackingRenderer::GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                                      double &Lrx, double &Lry) {
   //TODO(Gabriel - TCK #2981): Implementar
   if (pRenderers_.size() > 0)
      pRenderers_.at(0)->GetBoundingBox(pWorld, Ulx, Uly, Lrx, Lry);
}
} /** namespace suri */
