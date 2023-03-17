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

// Includes standard
#include <sstream>
#include <math.h>
#include <algorithm>
#include <utility>
#include <map>
#include <string>

// Includes suri
#include "VectorRenderer.h"
#include "suri/messages.h"
#include "suri/VectorElement.h"
#include "suri/DataTypes.h"
#include "suri/Canvas.h"
#include "suri/World.h"
#include "logmacros.h"
#include "suri/xmlnames.h"
#include "suri/Dimension.h"
#include "suri/Extent.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/ExactCoordinatesTransformation.h"
#include "suri/FileVectorCanvas.h"
#include <suri/XmlFunctions.h>
#include "FiltredVectorRenderer.h"

// Includes wx
#include "wx/wx.h"
#include "wx/font.h"

// Includes otros
#include "ogrsf_frmts.h"

// Define
/** cantidad de geometrias en cache */
#define _VECTORRENDERER_GEOMETRIES_CACHE_ 100
/** Cantidad de puntos a tener en cuenta en una transformacion de coordenadas */
#define _VECTORRENDERER_EXTENT_POINTS_ 100
/** Mascara para anotacion */
#define ANNOTATION_PREVIEW_MASK_RED 1
/** Mascara para anotacion */
#define ANNOTATION_PREVIEW_MASK_GREEN 0
/** Mascara para anotacion */
#define ANNOTATION_PREVIEW_MASK_BLUE 0

/** Enumera los vertices de las polilineas */
#define __ENUMERATE_VERTEX__

/** Macro para registrar Renderers en forma automatica */
AUTO_REGISTER_RENDERER(suri::VectorRenderer);

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @return instancia de la clase VectorRenderer
 */
VectorRenderer::VectorRenderer() : pVector_(NULL) {
}

/**
 * Destructor
 */
VectorRenderer::~VectorRenderer() {
   Vector::Close(pVector_);
}

// ------------------- METODOS ESTATICOS DE CONVERSION ----------------------
/** genera un nodo XML a partir de los parametros de las capas */
/**
 * @param[in] RenderParameters parametros de VectorRenderer con los que se
 * genera nodo
 * @return nodo generado con datos en parametros
 * \attention el wxXmlNode retornado(y su eliminacion) es responsabilidad
 * del invocante
 */
wxXmlNode *VectorRenderer::GetXmlNode(
      const VectorRenderer::Parameters &RenderParameters) {
   wxXmlNode *pnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, RENDERIZATION_NODE);
   wxXmlNode *playersnode = new wxXmlNode(pnode, wxXML_ELEMENT_NODE, LAYERS_NODE);

   for (std::map<int, std::string>::const_iterator it =
         RenderParameters.layerstyle_.begin(); it != RenderParameters.layerstyle_.end();
         it++) {
      wxString wkt = (*it).second;
      std::stringstream ss;
      ss << (*it).first;
      wxXmlNode *playernode = new wxXmlNode(
            playersnode, wxXML_ELEMENT_NODE, LAYER_NODE, wxT(""),
            new wxXmlProperty(LAYER_INDEX_PROPERTY, ss.str()));
      wxXmlNode *pstylenode = new wxXmlNode(
            playernode, wxXML_ELEMENT_NODE, STYLE_NODE);
      new wxXmlNode(pstylenode, wxXML_TEXT_NODE, wxT(""), wkt);
      ss.str("");
   }
   return pnode;
}

/** Obtiene el nodo de interes a partir de un elemento */
/**
 * @param[in] pElement elemento del que quiero el nodo renderizacion
 * @return nodo obtenido
 * \attention el nodo retornado es responsabilidad de esta clase (no eliminar
 * externamente) propago la advertencia de XmlElement::GetNode
 */
wxXmlNode *VectorRenderer::GetXmlNode(const Element *pElement) {
   return pElement->GetNode(RENDERIZATION_NODE);
}

/**
 * Reemplaza el nodo renderizado por el nuevo
 * @param[out] pElement elemento que quiero configurar
 * @param[in] pNode nodo de renderizacion que quiero agregar al elemento
 * @return true si pudo reemplazar el nodo.
 * @return false si no existe el nodo o si no es valido.
 */
bool VectorRenderer::SetXmlNode(Element *pElement, wxXmlNode *pNode) {
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

/**
 *
 */
void VectorRenderer::LoadRenderizationNodeIntoParameters(
      const wxXmlNode *pnode, Parameters *pParams) {
   wxXmlNode *prnode = pnode->GetChildren();
   while (prnode) {
      if (prnode->GetName() == LAYERS_NODE) {
         wxXmlNode *plnode = prnode->GetChildren();
         LoadLayersStyle(plnode, &(pParams->layerstyle_));
      }
      prnode = prnode->GetNext();
   }
}

/**
 *
 */
void VectorRenderer::LoadGeorreferenceNodeIntoParameters(
      const wxXmlNode *pnode, Parameters *pParams) {
   wxXmlNode *pgeonode = pnode->GetChildren();
   while (pgeonode != NULL) {
      if (pgeonode->GetName() == SPATIAL_REFERENCE_NODE) {
         wxXmlNode *psrnode = pgeonode->GetChildren();
         while (psrnode) {
            if (psrnode->GetName() == LAYERS_NODE) {
               wxXmlNode *plnode = psrnode->GetChildren();
               LoadLayersSrs(plnode, &(pParams->layersSR_));
            }
            psrnode = psrnode->GetNext();
         }
      }
      pgeonode = pgeonode->GetNext();
   }
}


/**
 * carga en un map de capas las capas correspondientes a los nodos del xml
 * @param[in] plnode nodo xml que posee las capas
 * @param[in] layers mapa a cargar
 */
void VectorRenderer::LoadLayersSrs(const wxXmlNode *plnode,
                                std::map<int, std::string> *layers) {
   while (plnode) {
      if (plnode->GetName() == LAYER_NODE
            && plnode->GetPropVal(LAYER_INDEX_PROPERTY, "-1") != "-1") {
         std::stringstream ss(plnode->GetPropVal(LAYER_INDEX_PROPERTY, "-1").c_str());
         int index;
         ss >> index;
         (*layers)[index] = plnode->GetNodeContent().c_str();
      }
      plnode = plnode->GetNext();
   }
}


/**
 * carga en un map de capas las capas correspondientes a los nodos del xml
 * @param[in] plnode nodo xml que posee las capas
 * @param[in] layers mapa a cargar
 */
void VectorRenderer::LoadLayersStyle(const wxXmlNode *plnode,
                                std::map<int, std::string> *layers) {
   while (plnode) {
      if (plnode->GetName() == LAYER_NODE
            && plnode->GetPropVal(LAYER_INDEX_PROPERTY, "-1") != "-1") {
         std::stringstream ss(plnode->GetPropVal(LAYER_INDEX_PROPERTY, "-1").c_str());
         int index;
         ss >> index;

         // Busco nodo estilo
         wxXmlNode *psnode = plnode->GetChildren();
         while (psnode) {
            if (psnode->GetName() == STYLE_NODE) {
               (*layers)[index] = psnode->GetNodeContent().c_str();
               break;
            }
            psnode = psnode->GetNext();
         }
      }
      plnode = plnode->GetNext();
   }
}

/** genera los parametros a partir de un nodo */
/**
 * @param[in] pNode nodo del elemento
 * @return clase Paramters con datos obtenidos del nodo
 */
VectorRenderer::Parameters VectorRenderer::GetParameters(const wxXmlNode *pNode) {
   if (!pNode) {
      REPORT_ERROR("D:VectorRenderer, Nodo NULO.");
      return Parameters();
   }
   wxXmlNode *pnode = pNode->GetChildren();
   if (!pnode) {
      REPORT_ERROR("D:VectorRenderer, Nodo INCORRECTO.");
      return Parameters();
   }

   Parameters params;
   // Busca url, georref, capas y estilos
   while (pnode) {
      if (pnode->GetName() == RENDERIZATION_NODE) {
         LoadRenderizationNodeIntoParameters(pnode, &params);
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
      if (pnode->GetName() == ATTRIBUTE_FILTER_NODE) {
         params.attributeFilter_ = pnode->GetNodeContent().c_str();
      }
      pnode = pnode->GetNext();
   }   
   return params;
}

// -------------------------- METODO DE CREACION ----------------------------
/** creador + inicializador */
/**
 * Crea vector renderer, le carga los parametros del elemento y controla que
 * el vector sea valido(caso contrario falla).
 * @param[in] pElement elemento del vector a renderizar
 * @param[in] pLastRenderer renderizador que esta antes en pipeline de
 * renderizacion
 * \attention el VectorRenderer retornado(y su eliminacion) son responsabilidad
 * del invocante
 * @return puntero a VectorRenderer
 * @return NULL si no existe el elemento o no hay capas para renderizar o no se
 * pudo abrir el archivo.
 */
VectorRenderer *VectorRenderer::Create(Element *pElement,
                                       Renderer *pLastRenderer) const {
   if (!pElement) {
      REPORT_DEBUG("D:Elemento nulo.");
      return NULL;
   }
   
   VectorRenderer *prenderer = new VectorRenderer;
   
   prenderer->parameters_ = GetParameters(pElement->GetNode(wxT("")));
   Vector* pvector = prenderer->OpenVector();
   if (!pvector) {
      delete prenderer;
      REPORT_AND_FAIL_VALUE("D:No se pudo abrir el archivo vectorial.", NULL);
   }
   Vector::Close(pvector);
   if (prenderer->parameters_.layerstyle_.empty()
         || prenderer->parameters_.layerstyle_.size()
               != prenderer->parameters_.layersSR_.size()) {
      delete prenderer;
      REPORT_AND_FAIL_VALUE("D:No hay capas para renderizar.", NULL);
   }
   return prenderer;
}

/**
 * Nombre del renderizador == al nombre del nodo
 * @return string con el nombre del nodo de capas
 */
std::string VectorRenderer::CreatedNode() const {
   return LAYERS_NODE;
}

// ----------------------- METODOS DE RENDERIZACION -------------------------
/**
 * Renderiza el elemento en un Canvas para un World dado.
 * @param[in] pWorldWindow: puntero a la ventana del mundo.
 * @param[in] pCanvas: puntero al Canvas.
 * @param[in] pMask: puntero a la mascara raster binaria.
 * @return true si pudo renderizar las capas
 * @return false si no pudo renderizar las capas
 */
bool VectorRenderer::Render(const World *pWorldWindow, Canvas* pCanvas, Mask* pMask) {
   bool retval = true;
   // Renderizacion de las capas
   std::map<int, std::string>::iterator mapit = parameters_.layerstyle_.find(
         parameters_.activelayer_);

   FiltredVectorRenderer* prenderer =
                     dynamic_cast<FiltredVectorRenderer*>(pPreviousRenderer_);
   if (prenderer != NULL) {
      // TODO(Gabriel - TCK #4040): Fix para que se cree genere el vector filtrado
      pVector_ = prenderer->OpenVector();
   }
   if (mapit != parameters_.layerstyle_.end()) {
      bool retvalaux = RenderLayer(mapit->first, pWorldWindow, pCanvas, pMask);
      if (retval && !retvalaux) retval = retvalaux;
   } else {
      std::map<int, std::string>::iterator mapit = parameters_.layerstyle_.begin();
      for (; mapit != parameters_.layerstyle_.end(); mapit++) {
         bool retvalaux = RenderLayer(mapit->first, pWorldWindow, pCanvas, pMask);
         if (retval && !retvalaux) { // para que la asignacion se haga 1 sola vez.
            retval = retvalaux;
         }
      }
   }

   return retval;
}

/**
 * Obtiene el "bounding box" del elemento renderizado
 * @param[in]  pWorld: puntero a la ventana del mundo.
 * @param[in]  Ulx: uper-left x
 * @param[in]  Uly: uper-left y
 * @param[in]  Lrx: lower-right x
 * @param[in]  Lry: lower-right y
 */
void VectorRenderer::GetBoundingBox(const World *pWorld, double &Ulx, double &Uly,
                                    double &Lrx, double &Lry) {
   if (parameters_.layerstyle_.empty()) {
      REPORT_DEBUG("D:Vector invalido.");
      return;
   }
   // TODO calcular para distintos extents en distintas capas
   // OGRLayer *player = pVector_->GetLayer(parameters_.layerstyle_.begin()->first);

   // Busca el estilo de la capa activa
   std::map<int, std::string>::iterator it = parameters_.layerstyle_.find(
         parameters_.activelayer_);
   OGRLayer *player = NULL;
   if (it != parameters_.layerstyle_.end())
      player = pVector_->GetLayer(it->first);
   else
      player = pVector_->GetLayer(parameters_.layerstyle_.begin()->first);

   if (!player) {
      REPORT_DEBUG("D:Vector invalido.");
      return;
   }

   /* CoordinatesTransform *pct = CoordinatesTransform::Create(
    parameters_.layersSR_.begin()->second, pWorld->GetSpatialReference()); */

   // busca las coordenadas de la capa activa
   std::map<int, std::string>::iterator srit = parameters_.layersSR_.find(
         parameters_.activelayer_);
   ParameterCollection params;
   if (srit != parameters_.layersSR_.end()) {
      params.AddValue<std::string>(TransformationFactory::kParamWktIn, srit->second);
   } else {
      params.AddValue<std::string>(TransformationFactory::kParamWktIn,  parameters_.layersSR_.begin()->second);
   }
   params.AddValue<std::string>(TransformationFactory::kParamWktOut, pWorld->GetSpatialReference());
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                     params);
   TransformationFactoryBuilder::Release(pfactory);
   if (pct == NULL) {
      REPORT_DEBUG("D:Bounding box imposible de calcular en coordenadas de mundo.");
      return;
   }
   if (!pVector_) {
      pVector_ = OpenVector();
   }
   if (pVector_) {
      // Para preservar el filtro espacial actual
      OGRGeometry * pfilter = player->GetSpatialFilter();
      if (pfilter != NULL) {
         pfilter = pfilter->clone();
      }
      player->SetSpatialFilter(NULL);
      OGREnvelope envelope;
      if (player->GetExtent(&envelope, TRUE) == OGRERR_FAILURE) {
         player->SetSpatialFilter(pfilter);
         REPORT_DEBUG("D:Vector con extension desconocida.");
      } else {
         player->SetSpatialFilter(pfilter);
         OGRGeometryFactory::destroyGeometry(pfilter);

         // Calcula el extent
         Subset wextent(envelope.MinX, envelope.MinY, envelope.MaxX, envelope.MaxY);
         pct->Transform(wextent, _VECTORRENDERER_EXTENT_POINTS_);

         Ulx = wextent.ul_.x_;
         Uly = wextent.ul_.y_;
         Lrx = wextent.lr_.x_;
         Lry = wextent.lr_.y_;
      }
   }
   delete pct;
   Vector::Close(pVector_);
}

/**
 * Obtiene los parametros de entrada del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void VectorRenderer::GetInputParameters(int &SizeX, int &SizeY, int &BandCount,
                                        std::string &DataType) const {
   SizeX = 0;
   SizeY = 0;
   BandCount = 3;
   DataType = DataInfo<unsigned char>::Name;
}

/**
 * Obtiene los parametros de salida del renderer
 * @param[out] SizeX Tamanio en X requerido por el renderizador.
 * @param[out] SizeY Tamanio en Y requerido por el renderizador.
 * @param[out] BandCount Bandas requeridas.
 * @param[out] DataType Tipo de dato.
 */
void VectorRenderer::GetOutputParameters(int &SizeX, int &SizeY, int &BandCount,
                                         std::string &DataType) const {
   BandCount = 3;
   DataType = DataInfo<unsigned char>::Name;
}

bool VectorRenderer::ApplySpatialFilter(OGRLayer* pLayer,
                                        CoordinatesTransformation *pCoordsTransform,
                                        const World* pWorldWindow) {
   Subset wworld, wwindow;
   pWorldWindow->GetWindow(wwindow);
   pWorldWindow->GetWorld(wworld);
   Dimension windim(wwindow);
   Subset wextent = Intersect(
         wworld, wwindow + std::max(windim.GetHeight(), windim.GetWidth()));
   if (pCoordsTransform->Transform(wextent, _VECTORRENDERER_EXTENT_POINTS_) < 2) {
      REPORT_DEBUG("D:Error al transformar coordenadas del mundo a vectoriales.");
      return false;
   }
   Extent extent(wextent);
   pLayer->SetSpatialFilterRect(extent.min_.x_, extent.min_.y_, extent.max_.x_,
                                extent.max_.y_);
   REPORT_DEBUG("D:Filtro espacial min(%.2f ; %.2f), max(%.2f ; %.2f)", extent.min_.x_,
                extent.min_.y_, extent.max_.x_, extent.max_.y_);
   return true;
}

/** Metodo que encargado de la renderizacion segun el tipo de geometrias de
 *  la capa**/
bool VectorRenderer::RenderGeometries(int LayerIndex,
                                      std::vector<OGRGeometry *>& Geometries,
                                      std::vector < std::string >& Annotations,
                                      VectorStyle* pVectorStyle,
                                      const World *pWorldWindow, wxDC * pDC,
                                      Mask* &pMask) {
   bool renderizationresult = true;
   Vector::VectorType type = pVector_->GetLayerType(LayerIndex);
   switch (type) {
      case Vector::Point: {
         renderizationresult = RenderPoints(Geometries, pVectorStyle, pWorldWindow, pDC,
                                            pMask, Annotations);
         break;
      }
      case Vector::Line: {
         renderizationresult = RenderLines(Geometries, pVectorStyle, pWorldWindow, pDC,
                                            pMask, Annotations);
         break;
      }
      case Vector::Polygon: {
         renderizationresult = RenderPolygons(Geometries, pVectorStyle, pWorldWindow,
                                              pDC, pMask, Annotations);
         break;
      }
      case Vector::GeometryCollection:
         renderizationresult = RenderPoints(Geometries, pVectorStyle, pWorldWindow,
                                              pDC, pMask, Annotations);
         renderizationresult |= RenderLines(Geometries, pVectorStyle, pWorldWindow,
                                              pDC, pMask, Annotations);
         renderizationresult |= RenderPolygons(Geometries, pVectorStyle, pWorldWindow,
                                              pDC, pMask, Annotations);
         break;
      default: {
         renderizationresult = false;
         break;
      }
   }
   return renderizationresult;
}

/** Renderiza una capa vectorial */
/**
 * Verifica que tiene lo elementos para renderizar el vector
 * Defino la ventana como filtro para el vector
 * Para cada feature en la capa cargo geometrias y/o multigeometrias,
 *    - Si es multigeometria la parsea y carga cada geometria en el geomvec
 *    - Si es geometria la carga en el geomvec
 *    - Renderizo segun el tipo
 * @param[in] LayerIndex indice de la capa que quiero renderizar
 * @param[in] pWorldWindow: puntero a la ventana del mundo.
 * @param[in] pCanvas: puntero al Canvas.
 * @param[in] pMask: puntero a la mascara raster binaria.
 * @return true si pudo renderizar la capa
 * @return false si no pudo renderizar la capa
 */
bool VectorRenderer::RenderLayer(int LayerIndex, const World *pWorldWindow,
                                 Canvas* pCanvas, Mask* pMask) {
   if (pCanvas == NULL) {
      REPORT_DEBUG("D:Canvas invalido.");
      return false;
   }

   if (pWorldWindow == NULL) {
      REPORT_DEBUG("D:WorldWindow invalido.");
      return false;
   }

   // Verifico que el vector exista
   if (!pVector_) {
      pVector_ = OpenVector();
   }
   if (pVector_ == NULL) {
      REPORT_DEBUG("D:Error en vector.");
      return false;
   }

   // Capa a renderizar
   OGRLayer *player = pVector_->GetLayer(LayerIndex);
   if (player == NULL) {
      REPORT_DEBUG("D:Capa vectorial invalida.");
      Vector::Close(pVector_);
      return false;
   }
   // Estilo
   VectorStyle * pvstyle = VectorStyle::Create(parameters_.layerstyle_[LayerIndex]);
   if (pvstyle == NULL) {
      REPORT_DEBUG("D:Estilo no definido.");
      Vector::Close(pVector_);
      return false;
   }
   bool success = DoRenderLayer(player, pvstyle, LayerIndex, pWorldWindow, pCanvas, pMask);
   VectorStyle::Destroy(pvstyle);
   Vector::Close(pVector_);
   return success;
}


bool VectorRenderer::DoRenderLayer(OGRLayer* pLayer, VectorStyle * pVstyle,
                                    int LayerIndex, const World *pWorldWindow,
                                    Canvas* pCanvas, Mask* pMask) {
   // Georreferencia
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                pWorldWindow->GetSpatialReference());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                parameters_.layersSR_[LayerIndex]);
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                     params);
   TransformationFactoryBuilder::Release(pfactory);
   if (pct == NULL) {
      REPORT_DEBUG("D:Imposible reproyectar capa vectorial en coordenadas de mundo.");
      return false;
   }
   // Configuracion de Labels
   int labelfieldindex = 0;
   std::string expression;
   GetExpressionFromStyle(pLayer, pVstyle, expression, labelfieldindex);

   // Filtrado de atributos
   pLayer->SetAttributeFilter(parameters_.attributeFilter_.c_str());

   // Filtrado espacial
   if (!ApplySpatialFilter(pLayer, pct, pWorldWindow)) {
      delete pct;
      return false;
   }

   // Ciclo principal de renderizado
   pLayer->ResetReading();
   std::vector<OGRFeature *> featurecache; // Cache de los features
   OGRFeature *pfeature = pLayer->GetNextFeature(); // Apunta al primer feature
   bool renderizationresult = true;
   while (pfeature) {
      featurecache.push_back(pfeature);
      pfeature = pLayer->GetNextFeature();
      size_t totalcachefeatures = featurecache.size();
      // Verifico si se lleno el cache
      if (totalcachefeatures >= _VECTORRENDERER_GEOMETRIES_CACHE_ || pfeature == NULL) {
         for (size_t nfeat = 0; nfeat < totalcachefeatures; nfeat++) {
            // Cargo geometrias y/o multigeometrias
            std::vector<OGRGeometry *> geomvec;
            std::vector < std::string > annotvec;

            OGRGeometryCollection * pmultigeom =
                  dynamic_cast<OGRGeometryCollection *>(featurecache[nfeat]->GetGeometryRef());

            // Si es multigeometria la parsea y carga cada geometria en el geomvec
            if (pmultigeom != NULL) {
               int totalgeoms = pmultigeom->getNumGeometries();
               for (int ngeom = 0; ngeom < totalgeoms; ngeom++) {
                  OGRGeometry * pgeom =
                        dynamic_cast<OGRGeometry *>(pmultigeom->getGeometryRef(ngeom));
                  if (pgeom != NULL) {
                     // Reproyecta la geometria al sistema del mundo
                     // TODO(Gabriel - TCK #2324): Ver porque se necesita este metodo puntual
                     ExactCoordinatesTransformation* pctcoord =
                           dynamic_cast<ExactCoordinatesTransformation*>(pct);
                     if (pctcoord != NULL && !pctcoord->IsIdentity()
                           && pctcoord->GetOGRCT(true) != NULL) {
                        pgeom->transform(pctcoord->GetOGRCT(true));
                     }
                     geomvec.push_back(pgeom);
                  }
                  // Labels
                  annotvec.push_back(GetAnnotationFromFeature(pfeature, labelfieldindex, expression));
               }
            }
            // Si es geometria la carga en el geomvec
            else {
               OGRGeometry * pgeom =
                     dynamic_cast<OGRGeometry *>(featurecache[nfeat]->GetGeometryRef());
               if (pgeom != NULL) {
                  // Reproyecta la geometria al sistema del mundo
                  // TODO(Gabriel - TCK #2324): Ver porque se necesita este metodo puntual
                  ExactCoordinatesTransformation* pctcoord =
                                             dynamic_cast<ExactCoordinatesTransformation*>(pct);
                  if (pct != NULL && !pct->IsIdentity()
                        && pctcoord->GetOGRCT(true) != NULL) {
                     pgeom->transform(pctcoord->GetOGRCT(true));
                  }
                  geomvec.push_back(pgeom);
                  // Labels
                  if (!expression.empty()) {
                     if (labelfieldindex != -1) {
                        std::string value = std::string(
                              featurecache[nfeat]->GetFieldAsString(
                                    labelfieldindex));
                        // Convierto Multibyte a Byte.
                        std::string auxtemp =
                              wxString(wxConvUTF8.cMB2WC(value.c_str()),
                                    wxCSConv(
                                          suri::XmlElement::xmlEncoding_.c_str())).c_str();

                        if (!auxtemp.empty()) value = auxtemp;
                        annotvec.push_back(value);
                     } else {
                        // Convierto Multibyte a Byte.
                        std::string auxtemp =
                              wxString(wxConvUTF8.cMB2WC(expression.c_str()),
                                    wxCSConv(
                                          suri::XmlElement::xmlEncoding_.c_str())).c_str();
                        if (!auxtemp.empty()) expression = auxtemp;
                        annotvec.push_back(expression);
                     }
                  }
               }
            }

            wxDC* pDC = pCanvas->GetDC();
            // Renderizo segun el tipo
            renderizationresult = RenderGeometries(LayerIndex, geomvec, annotvec,
                                                   pVstyle, pWorldWindow,
                                                   pDC, pMask);
         }

         // Limpia el cache
         for (size_t nfeat = 0; nfeat < totalcachefeatures; nfeat++)
            OGRFeature::DestroyFeature(featurecache[nfeat]);
         featurecache.clear();
      }
   } // Ciclo principal

   if(pCanvas->GetDC() && renderizationresult && pVstyle->IsInverted()) {
      wxDC *pDC = pCanvas->GetDC();
      int oldfunc = pDC->GetLogicalFunction();
      pDC->SetLogicalFunction(wxINVERT);
      wxSize size = pDC->GetSize();
      pDC->SetBrush(*wxBLACK_BRUSH);
      pDC->DrawRectangle(wxPoint(0, 0), size);
      pDC->SetLogicalFunction(oldfunc);
   }

   pCanvas->Flush(pMask);

#ifdef __UNUSED_CODE__
   /**
    * No tiene sentido dejar los if con dynamic_cast ya que no es una solucion extensible
    * Se creo el TCK #7818 para implementar una solucion a los defectos causados por falta de Flush
    */
   // Se diferencia entre FileVectorCanvas y un Canvas comun ya que de otra
   // forma no funciona la generacion de mascara para la exportacion.
   FileVectorCanvas* pfilevectorcanvas = dynamic_cast<FileVectorCanvas*>(pCanvas);
   MemoryCanvas* pfilecanvas = dynamic_cast<MemoryCanvas*>(pCanvas);
   if (pfilevectorcanvas) {
      pfilevectorcanvas->Flush(pMask);
      // Si es un FileCanvas no es necesario el wxDC por lo tanto se fuerza el valor true
      renderizationresult = true;
   } else if (pfilecanvas) {
      pfilecanvas->Flush(pMask);
      renderizationresult = true;
   }
#endif
   // Si no limipio el filtro, otras clases que editen la capa lo usaran sin saber
   pLayer->SetSpatialFilter(NULL);
   delete pct;
   return renderizationresult;
}   


/** Renderiza una vista previa de un estilo */
/**
 * Setea el DC
 * Llena el fondo con el color de la mascara para que el fondo sea transparente
 * Setea el mundo
 * Renderiza segun el tipo de vector
 * Setea la mascara para transparencia en 1,0,0 para que el negro (0,0,0) lo
 * dibuje bien
 * @param[in] pVStyle estilo con que quiero renderizar el vector
 * @param[out] Bitmap lugar donde renderizo el preview
 * @param[in] Xsize ancho del icono
 * @param[in] Ysize altura del icono
 */
void VectorRenderer::PreviewStyle(VectorStyle * pVStyle, wxBitmap &Bitmap, size_t Xsize,
                                  size_t Ysize) {
   if (pVStyle == NULL) {
      REPORT_AND_FAIL("D:VectorRenderer, estilo no definido para preview.");
   }

   wxImage img((int) Xsize, (int) Ysize, true);
   Bitmap = wxBitmap(img);
   if (!Bitmap.IsOk()) {
      REPORT_AND_FAIL("D:VectorRenderer, imposible crear BitMap para preview.");
   }

   // Setea el DC
   wxMemoryDC tempdc(Bitmap);
   if (!tempdc.IsOk()) {
      REPORT_AND_FAIL("D:VectorRenderer, imposible crear DC para preview.");
   }REPORT_DEBUG("D:Valor de mascara %d", Bitmap.GetMask()!=NULL);

   // Llena el fondo con el color de la mascara para que el fondo sea transparente
   tempdc.SetBrush(
         wxBrush(
               wxColour(ANNOTATION_PREVIEW_MASK_RED, ANNOTATION_PREVIEW_MASK_GREEN,
                        ANNOTATION_PREVIEW_MASK_BLUE),
               wxSOLID));
   tempdc.FloodFill(1, 1, *wxBLACK);

   // Setea el mundo
   World worldwindow;
   worldwindow.SetSpatialReference(RASTER_SPATIAL_REFERENCE);
   worldwindow.SetWorld(0.0, 0.0, 1.0, 1.0);
   worldwindow.SetWindow(0.0, 0.0, 1.0, 1.0);
   worldwindow.SetViewport((int) Xsize, (int) Ysize);

   // Renderizo segun el tipo de vector
   std::vector<OGRGeometry *> geometryvector;
   std::vector < std::string > labelvector;

   Mask *pmask = NULL;
   switch (pVStyle->GetVectorType()) {
      case Vector::Polygon: {
         OGRPolygon poly;
         OGRLinearRing linearring;
         linearring.addPoint(0.10, 0.10, 0.00);
         linearring.addPoint(0.90, 0.10, 0.00);
         linearring.addPoint(0.90, 0.90, 0.00);
         linearring.addPoint(0.10, 0.90, 0.00);
         poly.addRing(&linearring);
         geometryvector.push_back(&poly);
         labelvector.push_back(_(content_TEXT));
         RenderPolygons(geometryvector, pVStyle, &worldwindow, &tempdc, pmask,
                        labelvector);
         break;
      }
      case Vector::Line: {
         OGRLineString line;
         line.addPoint(0.10, 0.50, 0.00);
         line.addPoint(0.30, 0.10, 0.00);
         line.addPoint(0.70, 0.90, 0.00);
         line.addPoint(0.90, 0.50, 0.00);
         geometryvector.push_back(&line);
         labelvector.push_back(_(content_TEXT));
         RenderLines(geometryvector, pVStyle, &worldwindow, &tempdc, pmask,
                     labelvector);
         break;
      }
      case Vector::Point: {
         OGRPoint point;
         point.setX(0.50);
         point.setY(0.50);
         point.setZ(0.00);
         geometryvector.push_back(&point);
         labelvector.push_back(_(content_TEXT));
         RenderPoints(geometryvector, pVStyle, &worldwindow, &tempdc, pmask,
                      labelvector);
         break;
      }
      default: {
         REPORT_AND_FAIL("D:VectorRenderer, geometria desconocida para preview.");
         break;
      }
   }

   tempdc.SelectObject(wxNullBitmap);
   // Setea la mascara para transparencia en 1,0,0 para que el negro (0,0,0) lo dibuje bien
   wxMask *pmaskbmp = new wxMask(
         Bitmap,
         wxColour(ANNOTATION_PREVIEW_MASK_RED, ANNOTATION_PREVIEW_MASK_GREEN,
                  ANNOTATION_PREVIEW_MASK_BLUE));
   Bitmap.SetMask(pmaskbmp);
}

// ----------------------- METODOS DE ACTUALIZACION -------------------------

/**
 * Actualizacion de parametros para representar un elemento
 * @param[in] pElement elemento del vector con el que quiero actualizar
 * el renderizador
 */
void VectorRenderer::Update(Element *pElement) {
   Parameters param = GetParameters(pElement->GetNode(wxT("")));
   if (param.layerstyle_.empty()
         || param.layerstyle_.size() != param.layersSR_.size()) {
      REPORT_AND_FAIL("D:VectorRenderer, no hay capas para renderizar.");
   }
   // abro antes de cerrar por si se trata de un vector en memoria
   Vector *ptemp = Vector::Open(param.vectorUrl_);
   if (!ptemp) {
      REPORT_AND_FAIL("D:VectorRenderer, no se pudo abrir el archivo vectorial.");
   }
   Vector::Close(ptemp);
   parameters_ = param;
}

// --------------------------- METODOS PROTEGIDOS -------------------------------


/**
 * Retorna vector que se esta renderizando
 * @return vector en edicion
 */
Vector* VectorRenderer::OpenVector() {
   return Vector::Open(parameters_.vectorUrl_);
}

/** Renderiza una capa de poligonos */
/**
 *  TODO Soporte de poligonos con huecos
 * Verifica que tiene los elementos para renderizar el poligono
 * Obtiene estructura de renderizado de poligonos
 * Renderiza
 * Limpia la estructura de renderizado
 * @param[in] GeometriesVector vector con las geometrias a renderizar
 * @param[in] pVStyle estilo que deben tener las geometrias
 * @param[in] pWorldWindow ventana que quiero renderizar
 * @param[out] pDC herramienta que uso para renderizar
 * @param[in] pMask puntero a la mascara raster binaria.
 * @param[in] LabelsVector vector con los labels de las geometria
 * @return informa si pudo renderizar el vector
 */
bool VectorRenderer::RenderPolygons(const std::vector<OGRGeometry *> &GeometriesVector,
                                    VectorStyle * pVStyle, const World *pWorldWindow,
                                    wxDC * pDC, Mask* &pMask,
                                    std::vector<std::string> LabelsVector) {
   if (pWorldWindow == NULL) {
      REPORT_DEBUG("D:WorldWindow invalido.");
      return false;
   }
   if (pVStyle == NULL) {
      REPORT_DEBUG("D:Estilo no definido.");
      return false;
   }
   if (GeometriesVector.empty()) {
      REPORT_DEBUG("D:Nada para renderizar.");
      return true;
   }

   // Estructura de renderizado de poligonos
   std::vector < std::pair<wxPoint *, int> > brushpaint;
   std::vector < std::pair<wxPoint *, int> > pentrace;
   std::vector<wxPoint *> symbolmark;
   std::vector<wxPoint *> labelposition;

   int totalgeoms = GeometriesVector.size();
   int numberofpolygons = totalgeoms;
   int* pcount = NULL;
   for (int ngeom = 0; ngeom < totalgeoms; ngeom++) {
      OGRPolygon * ppoly = dynamic_cast<OGRPolygon *>(GeometriesVector[ngeom]);
      OGRLinearRing *plinearring = NULL;
      if (ppoly != NULL) {
         plinearring = ppoly->getExteriorRing();
      }
      if (plinearring != NULL) {
         plinearring = dynamic_cast<OGRLinearRing *>(plinearring->clone());
         plinearring->closeRings(); // fuerza anillo cerrado
         int totalringpoints = plinearring->getNumPoints();
         int interiorringscount = ppoly->getNumInteriorRings();
         numberofpolygons += interiorringscount;
         pcount = new int[numberofpolygons];
         pcount[0] = totalringpoints;
         int totalinteriorringpoints = 0;
         std::vector<OGRLinearRing*> interiorrings;
         for (int i = 0; i < interiorringscount; ++i) {
            interiorrings.push_back(
                  dynamic_cast<OGRLinearRing *>(ppoly->getInteriorRing(i)->clone()));
            interiorrings[i]->closeRings(); // fuerza anillo cerrado
            // i+1 porque la posicion 0 la tiene la geometria exterior
            pcount[i+1] = interiorrings[i]->getNumPoints();
            totalinteriorringpoints += interiorrings[i]->getNumPoints();
         }

         // Poligonos externos
         wxPoint *pwxpoly = new wxPoint[totalringpoints+totalinteriorringpoints];
         wxPoint *pwxline = new wxPoint[totalringpoints];
         int xtemp = 0, ytemp = 0;
         int polygonspointcount = 0;
         int exteriorlinepointcount = 0;
         for (int i = 0; i < totalringpoints; i++) {
            int x, y;
            pWorldWindow->W2VTransform(plinearring->getX(i), plinearring->getY(i), x, y);
            if (xtemp != x || ytemp != y || i == 0) {
               pwxpoly[polygonspointcount] = wxPoint(x, y);
               pwxline[exteriorlinepointcount] = wxPoint(x, y);
               xtemp = x;
               ytemp = y;
               polygonspointcount++;
               exteriorlinepointcount++;
               symbolmark.push_back(new wxPoint(x, y));
            }
         }

         // Poligonos internos
         for (size_t i = 0; i < interiorrings.size(); ++i) {
            int innerpointcount = 0;
            int totalinteriorringpoints = interiorrings[i]->getNumPoints();
            wxPoint *pwxinnerline = new wxPoint[totalinteriorringpoints];
            for (int ix = 0; ix < totalinteriorringpoints; ++ix) {
               int x, y;
               pWorldWindow->W2VTransform(interiorrings[i]->getX(ix), interiorrings[i]->getY(ix), x, y);
               if (xtemp != x || ytemp != y || i == 0) {
                  pwxpoly[polygonspointcount] = wxPoint(x, y);
                  pwxinnerline[innerpointcount] = wxPoint(x, y);
                  xtemp = x;
                  ytemp = y;
                  polygonspointcount++;
                  innerpointcount++;
                  symbolmark.push_back(new wxPoint(x, y));
               }
            }
            pentrace.push_back(std::pair<wxPoint *, int>(pwxinnerline, totalinteriorringpoints));
            OGRGeometryFactory::destroyGeometry(interiorrings[i]);
         }

         brushpaint.push_back(std::pair<wxPoint *, int>(pwxpoly, polygonspointcount));
         pentrace.push_back(std::pair<wxPoint *, int>(pwxline, exteriorlinepointcount));
         // Obtengo el centroide
         OGRPoint polygoncentroid;
         if (ppoly->Centroid(&polygoncentroid) == OGRERR_NONE) {
            int x, y;
            pWorldWindow->W2VTransform(polygoncentroid.getX(), polygoncentroid.getY(), x, y);
            labelposition.push_back(new wxPoint(x, y));
         }
         OGRGeometryFactory::destroyGeometry(plinearring);
      }
   }

   // Renderiza
   if (!Paint(brushpaint, pVStyle, pDC, pMask, numberofpolygons, pcount)
         || !Trace(pentrace, pVStyle, pDC, pMask) || !Mark(symbolmark, pVStyle, pDC, pMask)) {
      return false;
   }

   Annotate(labelposition, LabelsVector, pVStyle, pDC, pMask);

   // Limpia la estructura de renderizado
   for (size_t i = 0; i < brushpaint.size(); i++)
      delete[] brushpaint[i].first;
   for (size_t i = 0; i < pentrace.size(); i++)
      delete[] pentrace[i].first;
   for (size_t i = 0; i < symbolmark.size(); i++)
      delete symbolmark[i];
   for (size_t i = 0; i < labelposition.size(); i++)
      delete labelposition[i];

   delete [] pcount;

   return true;
}

/** Renderiza una capa de lineas */
/**
 * Verifica que tiene los elementos para renderizar la linea
 * Obtiene estructura de renderizado de lineas
 * Renderiza
 * Limpia la estructura de renderizado
 * @param[in] GeometriesVector vector con las geometrias a renderizar
 * @param[in] pVStyle estilo que deben tener las geometrias
 * @param[in] pWorldWindow ventana que quiero renderizar
 * @param[out] pDC herramienta que uso para renderizar
 * @param[in] pMask puntero a la mascara raster binaria.
 * @param[in] LabelsVector vector con los labels de las geometria
 * @return informa si pudo renderizar el vector
 */
bool VectorRenderer::RenderLines(const std::vector<OGRGeometry *> &GeometriesVector,
                                 VectorStyle * pVStyle, const World *pWorldWindow,
                                 wxDC * pDC, Mask* &pMask,
                                 std::vector<std::string> LabelsVector) {
   if (pWorldWindow == NULL) {
      REPORT_DEBUG("D:WorldWindow invalido.");
      return false;
   }
   if (pVStyle == NULL) {
      REPORT_DEBUG("D:Estilo no definido.");
      return false;
   }
   if (GeometriesVector.empty()) {
      REPORT_DEBUG("D:Nada para renderizar.");
      return true;
   }

   // Estructura de renderizado de lineas
   std::vector < std::pair<wxPoint *, int> > pentrace;
   std::vector<wxPoint *> symbolmark;
   std::vector<wxPoint *> labelposition;

   int totalgeoms = GeometriesVector.size();
   for (int ngeom = 0; ngeom < totalgeoms; ngeom++) {
      OGRLineString * pline = dynamic_cast<OGRLineString *>(GeometriesVector[ngeom]);
      if (pline != NULL) {
         int totallinepoints = pline->getNumPoints();
         wxPoint *pwxline = new wxPoint[totallinepoints];
         int xtemp = 0, ytemp = 0;
         int pointcount = 0;
         for (int i = 0; i < totallinepoints; i++) {
            int x, y;
            pWorldWindow->W2VTransform(pline->getX(i), pline->getY(i), x, y);
            if (xtemp != x || ytemp != y || i == 0) {
               pwxline[pointcount] = wxPoint(x, y);
               xtemp = x;
               ytemp = y;
               pointcount++;
               symbolmark.push_back(new wxPoint(x, y));
            }
         }
         // Obtengo el centroide
         OGRPoint linecentroid;
         OGRCurve *pcurve = dynamic_cast<OGRCurve *>(pline);
         if (pcurve) {
            pcurve->Value(pcurve->get_Length() / 2.0, &linecentroid);
            int x, y;
            pWorldWindow->W2VTransform(linecentroid.getX(), linecentroid.getY(), x, y);
            labelposition.push_back(new wxPoint(x, y));
         }
         pentrace.push_back(std::pair<wxPoint *, int>(pwxline, pointcount));
      }
   }

   // Renderiza
   if (!Trace(pentrace, pVStyle, pDC, pMask)
         || !Mark(symbolmark, pVStyle, pDC, pMask)) {
      return false;
   }

   Annotate(labelposition, LabelsVector, pVStyle, pDC, pMask);

   // Limpia la estructura de renderizado
   for (size_t i = 0; i < pentrace.size(); i++)
      delete[] pentrace[i].first;
   for (size_t i = 0; i < symbolmark.size(); i++)
      delete symbolmark[i];
   for (size_t i = 0; i < labelposition.size(); i++)
      delete labelposition[i];

   return true;
}

// ------------------------------------------------------------------------------
/** Renderiza una capa de puntos con su respectivo label para cada uno */
/**
 * Verifica que tiene los elementos para renderizar el punto
 * Obtiene estructura de renderizado de lineas
 * Renderiza
 * Limpia la estructura de renderizado
 * @param[in] GeometriesVector vector con las geometrias a renderizar
 * @param[in] pVStyle estilo que deben tener las geometrias
 * @param[in] pWorldWindow ventana que quiero renderizar
 * @param[out] pDC herramienta que uso para renderizar
 * @param[in] pMask puntero a la mascara raster binaria.
 * @param[in] LabelsVector vector con los labels de las geometria
 * @return informa si pudo renderizar el vector
 */
bool VectorRenderer::RenderPoints(const std::vector<OGRGeometry *> &GeometriesVector,
                                  VectorStyle * pVStyle, const World *pWorldWindow,
                                  wxDC * pDC, Mask* &pMask,
                                  std::vector<std::string> LabelsVector) {
   if (pWorldWindow == NULL) {
      REPORT_DEBUG("D:WorldWindow invalido.");
      return false;
   }
   if (pVStyle == NULL) {
      REPORT_DEBUG("D:Estilo no definido.");
      return false;
   }
   if (GeometriesVector.empty()) {
      REPORT_DEBUG("D:Nada para renderizar.");
      return true;
   }

   // Estructura de renderizado de puntos
   std::vector<wxPoint *> symbolmark;

   int totalgeoms = GeometriesVector.size();
   int xtemp = 0, ytemp = 0;
   for (int ngeom = 0; ngeom < totalgeoms; ngeom++) {
      OGRPoint * ppoint = dynamic_cast<OGRPoint *>(GeometriesVector[ngeom]);
      if (ppoint) {
         int x, y;
         pWorldWindow->W2VTransform(ppoint->getX(), ppoint->getY(), x, y);
         if (xtemp != x || ytemp != y || ngeom == 0) {
            symbolmark.push_back(new wxPoint(x, y));
            xtemp = x;
            ytemp = y;
         }
      }
   }

   // Renderiza
   if (!Mark(symbolmark, pVStyle, pDC, pMask)
         || !Annotate(symbolmark, LabelsVector, pVStyle, pDC, pMask)) {
      return false;
   }

   // Limpia la estructura de renderizado
   for (size_t i = 0; i < symbolmark.size(); i++)
      delete symbolmark[i];

   return true;
}

// ------------------------------------------------------------------------------
/** Pinta */
/**
 * Verifica que tiene los elementos para realizar el Paint
 * Selecciona el tipo de brush usando pVStyle->GetBrush()->id_
 * Realiza el brush
 * @param[in] Polygons vector con poligonos que se quiere rellenar
 * @param[in] pVStyle estilo que deben tener el relleno
 * @param[out] pDC herramienta que uso para renderizar
 * @param[in] pMask puntero a la mascara raster binaria.
 * @return informa si pudo rellenar
 */
bool VectorRenderer::Paint(std::vector<std::pair<wxPoint *, int> > Polygons,
                           VectorStyle * pVStyle, wxDC* pDC, Mask* &pMask,
                           int NumberOfPolygons, int* Count) {
   if (pDC == NULL) {
      REPORT_DEBUG("D:DC invalido.");
      return false;
   }

   if (pVStyle == NULL || pVStyle->GetBrush() == NULL) {
      REPORT_DEBUG("D:Estilo de brush no encontrado.");
      return false;
   }

   if (pVStyle->GetBrush()->id_ == VectorStyle::Brush::Transparent)
      return true;

   // Selecciona el tipo de brush
   ConfigureDC(pVStyle->GetBrush(), pDC);

   for (size_t i = 0; i < Polygons.size(); i++)
      pDC->DrawPolyPolygon(NumberOfPolygons, Count, Polygons[i].first);
      //pDC->DrawPolygon(Polygons[i].second, Polygons[i].first);
   return true;
}

/** Traza */
/**
 * Verifica que tiene los elementos para realizar el Trace
 * Selecciona el tipo de pen usando pVStyle->GetPen()->id_
 * Realiza el pen
 * @param[in] Lines vector con lineas que se quieren dibujar
 * @param[in] pVStyle estilo que deben tener las lineas
 * @param[out] pDC herramienta que uso para renderizar
 * @param[in] pMask puntero a la mascara raster binaria.
 * @return informa si pudo dibujar
 */
bool VectorRenderer::Trace(std::vector<std::pair<wxPoint *, int> > Lines,
                            VectorStyle* pVStyle, wxDC* pDC, Mask* &pMask) {
   if (pDC == NULL) {
      REPORT_DEBUG("D:DC invalido.");
      return false;
   }

   if (pVStyle == NULL || pVStyle->GetPen() == NULL) {
      REPORT_DEBUG("D:Estilo de pen no encontrado.");
      return false;
   }

   // Si es transparente o de tamanio invalido , vuelve sin hacer nada
   if (pVStyle->GetPen()->id_ == VectorStyle::Pen::Transparent ||
                                          pVStyle->GetPen()->width_ <= 0) {
      return true;
   }

   ConfigureDC(pVStyle->GetPen(), pDC);

   for (size_t i = 0; i < Lines.size(); i++)
      pDC->DrawLines(Lines[i].second, Lines[i].first);

   return true;
}

// ------------------------------------------------------------------------------
/** Pone marcas */
/**
 * \todo Generar bitmap para los simbolos
 * \todo Mejorar dibujo de los simbolos
 * Verifica que tiene los elementos para poner marcas
 * Setea brush y pen dependiendo si esta relleno o no
 * Dibuja el simbolo segun pVStyle->GetSymbol()->id_
 * @param[in] Points vector con puntos que se quieren dibujar
 * @param[in] pVStyle estilo que deben tener los puntos
 * @param[out] pDC herramienta que uso para renderizar
 * @param[in] pMask puntero a la mascara raster binaria.
 * @return informa si pudo dibujar
 */
bool VectorRenderer::Mark(std::vector<wxPoint *> Points,
                  VectorStyle * pVStyle, wxDC* pDC, Mask* &pMask) {
   if (pDC == NULL) {
      REPORT_DEBUG("D:DC invalido.");
      return false;
   }

   if (pVStyle == NULL || pVStyle->GetSymbol() == NULL) {
      REPORT_DEBUG("D:Estilo de simbolo no encontrado.");
      return false;
   }
   // Si no tiene simbolo, regresa
   if (pVStyle->GetSymbol()->id_ == VectorStyle::Symbol::NoSymbol
         || pVStyle->GetSymbol()->size_ <= 0) {
      return true;
   }

   // Tamanio de simbolo
   int pointcount = 0;
   wxPoint *pwxpoints = NULL;
   ConfigureDC(pVStyle->GetSymbol(), pDC);
   GetSymbolPoints(pVStyle->GetSymbol(), pwxpoints, pointcount);

   for (size_t i = 0; i < Points.size(); i++) {
      if (pVStyle->GetSymbol()->id_ == VectorStyle::Symbol::Circle || pVStyle->GetSymbol()->id_ ==
            VectorStyle::Symbol::EmptyCircle)
         pDC->DrawCircle(Points[i]->x, Points[i]->y, pVStyle->GetSymbol()->size_);
      else
         pDC->DrawPolygon(pointcount, pwxpoints, Points[i]->x, Points[i]->y);
   }
   delete[] pwxpoints;

#if defined(__DEBUG__) && defined(__ENUMERATE_VERTEX__)
   for (size_t i = 0; (i + 1) < Points.size(); i++) {
      pDC->DrawText(wxString::Format(PERCENT_U, i), *(Points[i]));
   }
#endif

   return true;
}

/**
 * Metodo auxiliar que obtiene la etiqueta configurada de un estilo
 */
void VectorRenderer::GetExpressionFromStyle(OGRLayer* pLayer,
                                            VectorStyle* pStyle,
                                            std::string& Expression, int& ExpIndex) {
   if (pStyle->GetLabel()) {
      Expression = pStyle->GetLabel()->expression_;
      if (Expression.size() > 2 && Expression[0] == '{'
            && Expression[Expression.size() - 1] == '}') {
         ExpIndex = pLayer->GetLayerDefn()->GetFieldIndex(
               Expression.substr(1, Expression.size() - 2).c_str());
      } else {
         ExpIndex = -1;
      }
   }
}

/** Metodo que obtiene la anotacion o etiqueta correspondiente a un feature **/
std::string VectorRenderer::GetAnnotationFromFeature(OGRFeature* pfeature,
                                                     int LabelIndex,
                                                     const std::string& Expression) {
   if (!Expression.empty() && LabelIndex != -1) {
         return pfeature->GetFieldAsString(LabelIndex);
   }
   return Expression;
}

// ------------------------------------------------------------------------------
/**
 * Verifica que tiene los elementos para poner etiqueta
 * Configura los colores, offset, anchor, angulo,font
 * Pone las etiquetas
 * @param[in] Points vector con puntos que se quieren anotar
 * @param[in] LabelsVector lista de labels que se deben usar en cada punto
 * @param[in] pVStyle estilo que deben tener las anotaciones
 * @param[out] pDC herramienta que uso para renderizar
 * @param[in] pMask puntero a la mascara raster binaria.
 * @return true si pudo poner las etiquetas
 * @return false si no pudo poner las etiquetas
 */
bool VectorRenderer::Annotate(std::vector<wxPoint *> Points,
                              std::vector<std::string> LabelsVector,
                              VectorStyle * pVStyle, wxDC * pDC, Mask * &pMask) {
   if (pDC == NULL) {
      REPORT_DEBUG("D:DC invalido.");
      return false;
   }

   if (pVStyle == NULL || pVStyle->GetLabel() == NULL) {
      REPORT_DEBUG("D:Estilo de etiqueta no encontrado.");
      return false;
   }

   /* \note Invierto el orden de:
    * if(pVStyle->GetLabel()->id_ == VectorStyle::Label::NoLabel || pVStyle->GetLabel()->size_ <= 0)
    *  y
    * if(Points.size() != LabelsVector.size())
    * porque sino falla la creacion de vectores de punto.
    */
   // Si no tiene etiqueta, regresa
   if (pVStyle->GetLabel()->id_ == VectorStyle::Label::NoLabel
         || pVStyle->GetLabel()->size_ <= 0) {
      return true;
   }

   if (Points.size() != LabelsVector.size()) {
      REPORT_DEBUG("D:Error en los vectores de puntos y etiquetas.");
      return false;
   }

   // Verifico sistema, si no es Suri asigno etiqueta default
   VectorStyle::Label::LabelType labelid = pVStyle->GetLabel()->id_;
   if (pVStyle->GetLabel()->system_ != "Suri-label") {
      labelid = VectorStyle::Label::Default;
   }

   // Configura los colores
   pDC->SetTextForeground(
         wxColour(pVStyle->GetLabel()->color_.red_, pVStyle->GetLabel()->color_.green_,
                  pVStyle->GetLabel()->color_.blue_,
                  pVStyle->GetLabel()->color_.alpha_));

   pDC->SetTextBackground(
         wxColour(pVStyle->GetLabel()->backColor_.red_,
                  pVStyle->GetLabel()->backColor_.green_,
                  pVStyle->GetLabel()->backColor_.blue_,
                  pVStyle->GetLabel()->backColor_.alpha_));

   // Transparencia del fondo (limite a mitad del rango del alpha channel)
   if (pVStyle->GetLabel()->backColor_.alpha_ < 128) {
      pDC->SetBackgroundMode(wxTRANSPARENT);
   } else {
      pDC->SetBackgroundMode(wxSOLID);
   }

   // Configura angulo
   double angle = (double) pVStyle->GetLabel()->angle_;

   // Configura y offsets
   int dx = pVStyle->GetLabel()->dx_;
   int dy = pVStyle->GetLabel()->dy_;

   // Configura anchor
   double ax, ay;
   switch (pVStyle->GetLabel()->anchor_) {
      case VectorStyle::Label::TopLeft: {
         ax = 0.0;
         ay = 0.0;
         break;
      }
      case VectorStyle::Label::CenterLeft: {
         ax = 0.0;
         ay = 0.5;
         break;
      }
      case VectorStyle::Label::BottomLeft: {
         ax = 0.0;
         ay = 1.0;
         break;
      }
      case VectorStyle::Label::TopCenter: {
         ax = 0.5;
         ay = 0.0;
         break;
      }
      case VectorStyle::Label::Center: {
         ax = 0.5;
         ay = 0.5;
         break;
      }
      case VectorStyle::Label::BottomCenter: {
         ax = 0.5;
         ay = 1.0;
         break;
      }
      case VectorStyle::Label::TopRight: {
         ax = 1.0;
         ay = 0.0;
         break;
      }
      case VectorStyle::Label::CenterRight: {
         ax = 1.0;
         ay = 0.5;
         break;
      }
      case VectorStyle::Label::BottomRight: {
         ax = 1.0;
         ay = 1.0;
         break;
      }
      default: {
         ax = 0.5;
         ay = 0.5;
         break;
      }
   }
   double awx = ax * cos(M_PI * angle / 180.0);
   double awy = -ax * sin(M_PI * angle / 180.0);
   double ahx = ay * sin(M_PI * angle / 180.0);
   double ahy = ay * cos(M_PI * angle / 180.0);

   // Configura el font
   int pointsize = pVStyle->GetLabel()->size_;
   wxFontFamily family;
   switch (labelid) {
      case VectorStyle::Label::Default: {
         family = wxFONTFAMILY_DEFAULT;
         break;
      }
      case VectorStyle::Label::Decorative: {
         family = wxFONTFAMILY_DECORATIVE;
         break;
      }
      case VectorStyle::Label::Roman: {
         family = wxFONTFAMILY_ROMAN;
         break;
      }
      case VectorStyle::Label::Script: {
         family = wxFONTFAMILY_SCRIPT;
         break;
      }
      case VectorStyle::Label::Swiss: {
         family = wxFONTFAMILY_SWISS;
         break;
      }
      case VectorStyle::Label::Modern: {
         family = wxFONTFAMILY_MODERN;
         break;
      }
      case VectorStyle::Label::Teletype: {
         family = wxFONTFAMILY_TELETYPE;
         break;
      }
      default: {
         family = wxFONTFAMILY_DEFAULT;
         break;
      }
   }
   int style = pVStyle->GetLabel()->italic_ ? wxFONTSTYLE_ITALIC : wxFONTSTYLE_NORMAL;
   wxFontWeight weight =
         pVStyle->GetLabel()->bold_ ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL;
   bool underlined = pVStyle->GetLabel()->underlined_;
   wxFont font(pointsize, family, style, weight, underlined);
   pDC->SetFont(font);

   // Pone las etiquetas
   int textheight, textwidth;
   for (size_t i = 0; i < LabelsVector.size(); i++) {
      if (!LabelsVector[i].empty()) {
         // REPORT_DEBUG("D:Dibujando Label.");
         std::string label = LabelsVector[i];
         // Se hace un trimm para sacar los espacios
         // iniciales que deforman la etiqueta
         label = trim(label, " ");

         wxString wxtext(label.c_str());
         pDC->GetTextExtent(wxtext, &textwidth, &textheight);
         pDC->DrawRotatedText(
               wxtext, Points[i]->x - (int) (awx * textwidth + ahx * textheight) + dx,
               Points[i]->y - (int) (awy * textwidth + ahy * textheight) + dy, angle);
      }
   }

   return true;
}

/** 
 * Configura el pDC con formato correspondiente a pPen
 * @param[in] pPen pen con datos a configurar
 * @param[out] pDC wxDC a configurar.
 * @return bool que indica si se pudo configurar pDC
 */
bool ConfigureDC(const VectorStyle::Pen* pPen, wxDC* pDC) {
   if (!pPen)
      return false;

   // Verifico sistema, si no es Suri asigno pen default
   VectorStyle::Pen::PenType penid = pPen->id_;
   if (pPen->system_ != "Suri-pen")
      penid = VectorStyle::Pen::Solid;

   // Selecciona el tipo de pen
   int pentype = wxSOLID;
   switch (penid) {
      case VectorStyle::Pen::Transparent: {
         pentype = wxTRANSPARENT;
         break;
      }
      case VectorStyle::Pen::Solid: {
         pentype = wxSOLID;
         break;
      }
      case VectorStyle::Pen::Dot: {
         pentype = wxDOT;
         break;
      }
      case VectorStyle::Pen::LongDash: {
         pentype = wxLONG_DASH;
         break;
      }
      case VectorStyle::Pen::ShortDash: {
         pentype = wxSHORT_DASH;
         break;
      }
      case VectorStyle::Pen::DotDash: {
         pentype = wxDOT_DASH;
         break;
      }
      case VectorStyle::Pen::BackDiagonal: {
        pentype = wxBDIAGONAL_HATCH;
         break;
       }
      case VectorStyle::Pen::CrossDiagonal: {
         pentype = wxCROSSDIAG_HATCH;
         break;
      }
      case VectorStyle::Pen::ForDiagonal: {
         pentype = wxFDIAGONAL_HATCH;
         break;
      }
      case VectorStyle::Pen::Cross: {
         pentype = wxCROSS_HATCH;
         break;
      }
      case VectorStyle::Pen::Horizontal: {
         pentype = wxHORIZONTAL_HATCH;
         break;
      }
      case VectorStyle::Pen::Vertical: {
         pentype = wxVERTICAL_HATCH;
         break;
      }
         // case VectorStyle::Pen::Stipple:
         // case VectorStyle::Pen::UserDash:
      default:
         break;
   }

   wxColour pencolour(pPen->color_.red_, pPen->color_.green_,
                         pPen->color_.blue_, pPen->color_.alpha_);

   // Si es transparente o de tamanio invalido , vuelve sin hacer nada
   if (pencolour == pDC->GetPen().GetColour() &&
                           pPen->width_ == pDC->GetPen().GetWidth() &&
                           pentype == pDC->GetPen().GetStyle())
      return true;

   // Pen
   wxPen * ppen = wxThePenList->FindOrCreatePen(pencolour,
                                    pPen->width_, pentype);

   pDC->SetPen(*ppen);
   return true;
}

/**
 * Configura el pDC con formato correspondiente a pBrush
 * @param[in] pBrush brush con datos a configurar
 * @param[out] pDC dc donde quiero configurar el brush
 * @return bool que indica si se pudo configurar pDC
 */
bool ConfigureDC(VectorStyle::Brush* pBrush, wxDC* pDC) {
   if (!pBrush)
      return false;

   // Verifico sistema, si no es Suri asigno brush default
   VectorStyle::Brush::BrushType brushid = pBrush->id_;
   if (pBrush->system_ != "Suri-brush")
      brushid = VectorStyle::Brush::BackDiagonal;

   // Selecciona el tipo de brush
   int brushtype = wxTRANSPARENT;
   switch (brushid) {
      case VectorStyle::Brush::Transparent: {
         brushtype = wxTRANSPARENT;
         break;
      }
      case VectorStyle::Brush::Solid: {
         brushtype = wxSOLID;
         break;
      }
      case VectorStyle::Brush::BackDiagonal: {
         brushtype = wxBDIAGONAL_HATCH;
         break;
      }
      case VectorStyle::Brush::CrossDiagonal: {
         brushtype = wxCROSSDIAG_HATCH;
         break;
      }
      case VectorStyle::Brush::ForDiagonal: {
         brushtype = wxFDIAGONAL_HATCH;
         break;
      }
      case VectorStyle::Brush::Cross: {
         brushtype = wxCROSS_HATCH;
         break;
      }
      case VectorStyle::Brush::Horizontal: {
         brushtype = wxHORIZONTAL_HATCH;
         break;
      }
      case VectorStyle::Brush::Vertical: {
         brushtype = wxVERTICAL_HATCH;
         break;
      }
         // case VectorStyle::Brush::Stipple:
      default:
         break;
   }

   wxColour brushcolour(pBrush->color_.red_, pBrush->color_.green_,
                          pBrush->color_.blue_, pBrush->color_.alpha_);

   // Si es transparente, vuelve sin hacer nada
#ifdef __UNUSED_CODE__
   if (brushtype == pDC->GetBrush().GetStyle() &&
                 brushcolour == pDC->GetPen().GetColour())
      return true;
#endif
   // Brush
   wxBrush * pbrush = wxTheBrushList->FindOrCreateBrush(brushcolour, brushtype);
   pDC->SetBrush(*pbrush);
   // Pen
   pDC->SetPen(*wxTRANSPARENT_PEN);
   return true;
}

/**
 * Configura el pDC con formato correspondiente a pSymbol
 * @param[in] pSymbol simbolo con datos a configurar
 * @param[out] pDC wxDC que quiero configurar
 * @return bool que indica si se pudo configurar pDC
 */
bool ConfigureDC(VectorStyle::Symbol* pSymbol, wxDC* pDC) {
   if (!pSymbol)
      return false;

   // Verifico sistema, si no es Suri asigno simbolo default
   VectorStyle::Symbol::SymbolType symbid = pSymbol->id_;
   if (pSymbol->system_ != "Suri-symbol")
      symbid = VectorStyle::Symbol::Circle;

   wxColour symbolcolor(pSymbol->color_.red_, pSymbol->color_.green_,
                        pSymbol->color_.blue_, pSymbol->color_.alpha_);

   const wxPen * ppen = NULL;
   const wxBrush* pbrush = NULL;

   // Verifica si es con/sin relleno para configuracion de brush y pen
   if (symbid == VectorStyle::Symbol::EmptyCircle
         || symbid == VectorStyle::Symbol::EmptySquare
         || symbid == VectorStyle::Symbol::EmptyTriangle
         || symbid == VectorStyle::Symbol::EmptyCross
         || symbid == VectorStyle::Symbol::EmptyPentagon) {
      ppen = wxThePenList->FindOrCreatePen(symbolcolor,
                                     pSymbol->size_ / 4 + 1, wxSOLID);
      pbrush = wxTRANSPARENT_BRUSH;
   } else {
      ppen = wxThePenList->FindOrCreatePen(symbolcolor, 1, wxSOLID);
      pbrush = wxTheBrushList->FindOrCreateBrush(symbolcolor, wxSOLID);
   }

   if (*ppen == pDC->GetPen() && pDC->GetTextForeground() == symbolcolor &&
                                                   *pbrush == pDC->GetBrush())
      return true;

   pDC->SetPen(*ppen);
   pDC->SetTextForeground(symbolcolor);
   pDC->SetBrush(*pbrush);

   return true;
}

/**
 * Crea los puntos de un simbolo a partir del tipo
 * @param[in] pSymbol simbolo que se quiere dibujar
 * @param[out] pWxPoints puntos del poligono a renderizar
 * @param[out] PointCount cantidad de puntos configurados en pWxPoints
 * @return bool que indica si pudo configurar los puntos
 */
bool GetSymbolPoints(VectorStyle::Symbol* pSymbol,
                                        wxPoint* &pWxPoints, int &PointCount) {
   // Verifico sistema, si no es Suri asigno simbolo default
   VectorStyle::Symbol::SymbolType symbid = pSymbol->id_;
   if (pSymbol->system_ != "Suri-symbol")
      symbid = VectorStyle::Symbol::Circle;

   int symbolsize = pSymbol->size_;
   // Dibuja el simbolo
   switch (symbid) {
      case VectorStyle::Symbol::Square:
      case VectorStyle::Symbol::EmptySquare: {
         PointCount = 4;
         pWxPoints = new wxPoint[PointCount];
         pWxPoints[0] = wxPoint(-symbolsize, -symbolsize);
         pWxPoints[1] = wxPoint(symbolsize, -symbolsize);
         pWxPoints[2] = wxPoint(symbolsize, symbolsize);
         pWxPoints[3] = wxPoint(-symbolsize, symbolsize);
         break;
      }
      case VectorStyle::Symbol::Triangle:
      case VectorStyle::Symbol::EmptyTriangle: {
         PointCount = 3;
         pWxPoints = new wxPoint[PointCount];
         pWxPoints[0] = wxPoint(-symbolsize, symbolsize);
         pWxPoints[1] = wxPoint(0, -symbolsize);
         pWxPoints[2] = wxPoint(symbolsize, symbolsize);
         break;
      }
      case VectorStyle::Symbol::Cross:
      case VectorStyle::Symbol::EmptyCross: {
         PointCount = 12;
         pWxPoints = new wxPoint[PointCount];
         int bigside = symbolsize - 1;
         int smallside = (symbolsize + 1) / 2;
         pWxPoints[0] = wxPoint(-symbolsize, -symbolsize + smallside);
         pWxPoints[1] = wxPoint(-symbolsize + smallside, -symbolsize + smallside);
         pWxPoints[2] = wxPoint(-symbolsize + smallside, -symbolsize);
         pWxPoints[3] = wxPoint(-symbolsize + smallside + bigside, -symbolsize);
         pWxPoints[4] = wxPoint(-symbolsize + smallside + bigside,
                              -symbolsize + smallside);
         pWxPoints[5] = wxPoint(-symbolsize + 2 * smallside + bigside,
                              -symbolsize + smallside);
         pWxPoints[6] = wxPoint(-symbolsize + 2 * smallside + bigside,
                              -symbolsize + smallside + bigside);
         pWxPoints[7] = wxPoint(-symbolsize + smallside + bigside,
                              -symbolsize + smallside + bigside);
         pWxPoints[8] = wxPoint(-symbolsize + smallside + bigside,
                              -symbolsize + 2 * smallside + bigside);
         pWxPoints[9] = wxPoint(-symbolsize + smallside,
                              -symbolsize + 2 * smallside + bigside);
         pWxPoints[10] = wxPoint(-symbolsize + smallside,
                               -symbolsize + smallside + bigside);
         pWxPoints[11] = wxPoint(-symbolsize, -symbolsize + smallside + bigside);
         break;
      }
      case VectorStyle::Symbol::Pentagon:
      case VectorStyle::Symbol::EmptyPentagon: {
         PointCount = 5;
         pWxPoints = new wxPoint[PointCount];
         int delta1 = (2 * symbolsize + 1) / 2;
         pWxPoints[0] = wxPoint(-symbolsize, -symbolsize + delta1);
         pWxPoints[1] = wxPoint(0, -symbolsize);
         pWxPoints[2] = wxPoint(symbolsize, -symbolsize + delta1);
         pWxPoints[3] = wxPoint(symbolsize - delta1 / 2, symbolsize);
         pWxPoints[4] = wxPoint(-symbolsize + delta1 / 2, symbolsize);
         break;
      }
      default: {
         PointCount = 1;
         pWxPoints = new wxPoint[PointCount];
         pWxPoints[0] = wxPoint(0, 0);
         break;
      }
   }
   return true;
}
}  // namespace suri
