/*! \file CategorizedVectorRenderer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "CategorizedVectorRenderer.h"
#include "suri/xmlnames.h"
#include "logmacros.h"
#include "suri/Element.h"
#include "FiltredVectorRenderer.h"
#include "suri/ParameterCollection.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/ExactCoordinatesTransformation.h"
#include "suri/World.h"
#include "suri/VectorStyle.h"
#include "suri/Vector.h"
#include "suri/Canvas.h"

// Includes Wx
#include "wx/wx.h"

// Includes gdal
#include "ogrsf_frmts.h"

// Defines
// forwards

/** Macro para registrar Renderers en forma automatica */
AUTO_REGISTER_RENDERER(suri::vector::render::CategorizedVectorRenderer);

namespace suri {
namespace vector {
namespace render {

CategorizedVectorRenderer::CategorizedVectorRenderer() {
}

CategorizedVectorRenderer::~CategorizedVectorRenderer() {
}

bool  CategorizedVectorRenderer::RenderLayer(int LayerIndex, const World *pWorldWindow,
                                             Canvas* pCanvas,
                                             Mask* pMask) {
   // validacion
   if (pCanvas == NULL) {
      REPORT_DEBUG("D:Canvas invalido.");
      return false;
   }

   if (pWorldWindow == NULL) {
      REPORT_DEBUG("D:WorldWindow invalido.");
      return false;
   }

   /**
    * Es necesario reabrir el vector para volver a renderizar. Esto evita
    * que se tengan los datos en cache y no se actualice la renderizacion si
    * cambia algun valor en la tabla
    */
   if (pVector_) {
      Vector::Close(pVector_);
   }
   pVector_ = OpenVector();

   if (pVector_ == NULL) {
      REPORT_DEBUG("D:Error en vector.");
      return false;
   }

   // Capa a renderizar
   OGRLayer *player = pVector_->GetLayer(params_.activelayer_);
   if (player == NULL) {
      REPORT_DEBUG("D:Capa vectorial invalida.");
      Vector::Close(pVector_);
      return false;
   }
   std::vector< std::pair<std::string, std::string> > categorizedstyles =
                        params_.categorizedlayerstyle_[parameters_.activelayer_];
   std::vector< std::pair<std::string, std::string> >::const_iterator it = categorizedstyles.begin();
   bool success = true;
   for (; success && it != categorizedstyles.end(); ++it) {
       player->SetAttributeFilter(it->first.c_str());
       VectorStyle * pvstyle = VectorStyle::Create(it->second);
       success = DoRenderLayer(player, pvstyle, LayerIndex, pWorldWindow, pCanvas, pMask);
       VectorStyle::Destroy(pvstyle);
   }
   Vector::Close(pVector_);
   return success;
}



/**
 * Actualizacion de parametros para representar un elemento
 * @param[in] pElement elemento del vector con el que quiero actualizar
 * el renderizador
 */
void CategorizedVectorRenderer::Update(Element *pElement) {
   CategorizedParameters params = GetParameters(pElement->GetNode(wxT("")));
   if (params_.categorizedlayerstyle_.empty()
         || params.categorizedlayerstyle_.size() != params.layersSR_.size()) {
      REPORT_AND_FAIL("D:VectorRenderer, no hay capas para renderizar.");
   }
   // abro antes de cerrar por si se trata de un vector en memoria
   Vector *ptemp = Vector::Open(params_.vectorUrl_);
   if (!ptemp) {
      REPORT_AND_FAIL("D:VectorRenderer, no se pudo abrir el archivo vectorial.");
   }
   Vector::Close(ptemp);
   parameters_ = params;
   params_ = params;
   CategorizedParameters::LayerStyleMap::iterator it = params_.categorizedlayerstyle_.begin();
   for (int i = 0; it != params_.categorizedlayerstyle_.end(); ++it, ++i)
      parameters_.layerstyle_.insert(std::pair<int, std::string>(i, (*it)[i].second));
}

/** creador + inicializador */
VectorRenderer *CategorizedVectorRenderer::Create(Element *pElement,
                                                  Renderer *pLastRenderer) const {
   if (!pElement) {
      REPORT_DEBUG("D:Elemento nulo.");
      return NULL;
   }
   CategorizedVectorRenderer* prender = new CategorizedVectorRenderer;
   prender->params_ = GetParameters(pElement->GetNode(wxT("")));
   prender->parameters_ = prender->params_;
   CategorizedParameters::LayerStyleMap::iterator it = prender->params_.categorizedlayerstyle_.begin();
   for (int i = 0; it != prender->params_.categorizedlayerstyle_.end(); ++it, ++i) {
      prender->parameters_.layerstyle_.insert(std::pair<int, std::string>(i, (*it)[i].second));
   }

   Vector* pvector = prender->OpenVector();
   if (!pvector) {
      delete prender;
      REPORT_AND_FAIL_VALUE("D:No se pudo abrir el archivo vectorial.", NULL);
   }
   return prender;
}

/** Nombre del renderizador == al nombre del nodo */
std::string CategorizedVectorRenderer::CreatedNode() const {
   return CATEGORIZED_LAYER_NODE;
}

/** genera un nodo XML a partir de los parametros */
wxXmlNode *CategorizedVectorRenderer::GetXmlNode(
      const CategorizedParameters &RenderParameters) {
   // Se fija la capa activa
   if (RenderParameters.activelayer_ >= (int) RenderParameters.categorizedlayerstyle_.size())
      return NULL;

   wxXmlNode *pnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, RENDERIZATION_NODE);
   wxXmlNode *pcatstylesnode = new wxXmlNode(pnode, wxXML_ELEMENT_NODE,
   CATEGORIZED_LAYER_NODE);
   /** si tienen la misma cantidad entonces se encuentra bien configurado
    *  las etiquetas para los estilos
    */
   bool includelabels = (RenderParameters.categorizedStyleNames_.size()
         == RenderParameters.categorizedlayerstyle_[RenderParameters.activelayer_].size());
   std::vector< std::pair< std::string, std::string > >::const_iterator lit = 
				RenderParameters.categorizedlayerstyle_[RenderParameters.activelayer_].begin();
   std::vector<std::string>::const_iterator cnamesit =
         RenderParameters.categorizedStyleNames_.begin();
	wxXmlNode* plastclassnode = NULL;
   for (; lit != RenderParameters.categorizedlayerstyle_[RenderParameters.activelayer_].end(); ++lit, ++cnamesit) {
      wxXmlNode* pclassnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
      CATEGORY_CLASS_NODE);

      wxXmlNode* pcondnode = new wxXmlNode(pclassnode, wxXML_ELEMENT_NODE,
      CATEGORY_CONDITION_NODE);
      new wxXmlNode(pcondnode, wxXML_TEXT_NODE, wxEmptyString, lit->first);

      wxXmlNode* pcatnode = new wxXmlNode(pclassnode, wxXML_ELEMENT_NODE,
      CATEGORY_STYLE_NODE);
      new wxXmlNode(pcatnode, wxXML_TEXT_NODE, wxEmptyString, lit->second);

      if (includelabels) {
         wxXmlNode* pcatnode = new wxXmlNode(pclassnode, wxXML_ELEMENT_NODE,
         CATEGORY_LABEL_NODE);
         new wxXmlNode(pcatnode, wxXML_TEXT_NODE, wxEmptyString, *cnamesit);
      }
		pcatstylesnode->InsertChildAfter(pclassnode, plastclassnode);
		plastclassnode = pclassnode;
   }
   return pnode;
}

/** Obtiene el nodo de interes a partir de un elemento */
wxXmlNode *CategorizedVectorRenderer::GetXmlNode(const Element *pElement) {
   return pElement->GetNode(RENDERIZATION_NODE);
}

/** Reemplaza el nodo renderizado por el nuevo */
bool CategorizedVectorRenderer::SetXmlNode(Element *pElement, wxXmlNode *pNode) {
   if (!pNode) {
      REPORT_AND_FAIL_VALUE("D:No se encontro el nodo.", false);
   }

   if (pNode->GetName().Cmp(wxT(RENDERIZATION_NODE)) != 0) {
      REPORT_AND_RETURN_VALUE("D:Nodo %s invalido.", false, pNode->GetName().c_str());
   }

   wxXmlNode *pnode = GetXmlNode(pElement);
   if (!pnode) {
      REPORT_AND_FAIL_VALUE("D:No se encontro el nodo.", false);
   }
   pElement->AddNode(pnode->GetParent(), pNode, true);
   return true;
}

/** genera los parametros a partir de un nodo */
CategorizedVectorRenderer::CategorizedParameters CategorizedVectorRenderer::GetParameters(
      const wxXmlNode *pNode) {
   if (!pNode) {
      REPORT_ERROR("D:CategorizedVectorRenderer, Nodo NULO.");
      return CategorizedParameters();
   }
   wxXmlNode *pnode = pNode->GetChildren();
   if (!pnode) {
      REPORT_ERROR("D:CategorizedVectorRenderer, Nodo INCORRECTO.");
      return CategorizedParameters();
   }
   CategorizedParameters params;
   // Busca url, georref, capas y estilos
   while (pnode) {
      if (pnode->GetName() == RENDERIZATION_NODE) {
         LoadRenderizationParameters(pnode, &params);
      }
      if (pnode->GetName() == URL_NODE) {
         params.vectorUrl_ = pnode->GetNodeContent().c_str();
      }
      if (pnode->GetName() == GEORREFERENCE_NODE) {
         LoadGeorreferenceNodeIntoParameters(pnode, &params);
      }
      // Busco cual es la capa activa
      if (pnode->GetName() == ACTIVELAYER_NODE) {
         params.activelayer_ = wxAtoi(pnode->GetNodeContent());
      }
      pnode = pnode->GetNext();
   }
   return params;
}

/** Metodo auxiliar para cargar parametros de renderizacion **/
void CategorizedVectorRenderer::LoadRenderizationParameters(
      const wxXmlNode *pnode, CategorizedParameters *pParams) {
   wxXmlNode *prnode = pnode->GetChildren();
   while (prnode) {
      if (prnode->GetName() == CATEGORIZED_LAYER_NODE) {
         wxXmlNode *pclassnode = prnode->GetChildren();
         while (pclassnode) {
            // carga el estilo por cada clase
            LoadClassStyle(pclassnode, pParams);
            pclassnode = pclassnode->GetNext();
         }
      }
      prnode = prnode->GetNext();
   }
}

/** Metodo auxiliar encargado de cargar el mapa de estilos clasificados **/
void CategorizedVectorRenderer::LoadClassStyle(const wxXmlNode *pClassNode,
                                               CategorizedParameters *pParams) {
   if (pClassNode->GetName().compare(CATEGORY_CLASS_NODE) || !pParams)
      return;
   wxXmlNode *pcnode = pClassNode->GetChildren();
   std::string condition, style, label;
   while (pcnode) {
      if (pcnode->GetName().compare(CATEGORY_CONDITION_NODE) == 0) {
         condition = pcnode->GetNodeContent().c_str();
      } else if (pcnode->GetName().compare(CATEGORY_STYLE_NODE) == 0) {
         style = pcnode->GetNodeContent().c_str();
      } else if (pcnode->GetName().compare(CATEGORY_LABEL_NODE) == 0) {
         label = pcnode->GetNodeContent().c_str();
      }
      pcnode = pcnode->GetNext();
   }
   if (!condition.empty() && !style.empty()) {
      pParams->categorizedStyleNames_.push_back(label);
      if (pParams->activelayer_ < (int)pParams->categorizedlayerstyle_.size()) {
         pParams->categorizedlayerstyle_[pParams->activelayer_].push_back(std::make_pair(condition, style));
      } else {
			std::vector< std::pair< std::string, std::string > > styles(1);
			styles[0] = std::make_pair(condition, style); 
			pParams->categorizedlayerstyle_.push_back(styles);
      }
   }

}

} /** namespace renderer */
} /** namespace vector */
} /** namespace suri */
