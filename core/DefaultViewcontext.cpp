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
#include <list>
#include <string>

// Includes Suri
#include "DefaultViewcontext.h"
#include "suri/LayerList.h"
#include "suri/LayerInterface.h"
#include "suri/LayerFactory.h"
#include "TerrainLayer.h"
#include "VectorLayer.h"
#include "suri/TerrainElement.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/RasterElement.h"
#include "HtmlElement.h"
#include "MultimediaElement.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
DefaultViewcontext::DefaultViewcontext(bool Support3dLayers,
      bool Support2dLayers, suri::ViewcontextInterface::ViewcontextType Type,
      const suri::SuriObject::UuidType& ViewcontextId) :
      ViewcontextInterface(Type),
      pLayerList_(NULL), support3dLayers_(Support3dLayers),
      support2dLayers_(Support2dLayers), type_(Type), viewcontextId_(ViewcontextId),
      pViewcontextManager_(NULL) {
   pLayerList_ = new LayerList;
}

/**
 * Destructor
 */
DefaultViewcontext::~DefaultViewcontext() {
}

/**
 * Obtiene LayerList
 * @return Devuelve el puntero al LayerList usado
 * \deprecated
 */
LayerList* DefaultViewcontext::GetLayerList() {
   return pLayerList_;
}


/** Configura la layerlist asociada \deprecated **/
void DefaultViewcontext::SetLayerList(LayerList* pLayerList) {
   if (pLayerList_)
      delete pLayerList_;
   pLayerList_ = pLayerList;

}
/**
 * Obtiene el grupo al cual pertenece un elemento
 * Compatibilidad Con element
 * @param[in] pLayer layer que posee el elemento a obtener su grupo
 * \deprecated
 */
int DefaultViewcontext::GetElementGroup(LayerInterface* pLayer) {
   const char *pelementtypes[] = { "RasterElement", "WxsElement", "TerrainElement",
                                   "AnotationElement", "HotLinkVectorElement", "VectorElement" };
   int elementposition[] = { 6, 6, 6, 2, 4, 4 };
   int group = 6;
   std::string layertype = pLayer->GetElement()->GetClassId();
   for (size_t i = 0; i < 6; ++i) {
      if (layertype == pelementtypes[i]) {
         group = elementposition[i];
         break;
      }
   }
   return group;
}

/**
 * Agrega una capa
 * @param[in] pLayer puntero a la capa
 * @param[in] Update boolean que indica si se tiene que que actualizar la LayerList
 * @return true
 * \deprecated
 */
bool DefaultViewcontext::AddLayer(LayerInterface* pLayer, bool Update) {
   if (Exists(pLayer) || !IsCompatibleLayerType(pLayer))
      return false;
   if (GetLayer(pLayer->GetId()) == NULL) {
      layerInterfaceList_.push_back(pLayer);
      int group = GetElementGroup(pLayer);
      pLayerList_->AddElement(pLayer->GetElement(), group, Update);
      pLayer->SetViewcontext(this);
   } else {
      return false;
   }
   return true;
}

/**
 * Agrega una capa
 * @param[in] pLayer puntero a la capa
 * @return Devuelve true si pudo agregar el elemento correctamente, sino devuelve false
 */
bool DefaultViewcontext::AddLayer(LayerInterface* pLayer) {
   if (!Exists(pLayer) && IsCompatibleLayerType(pLayer)) {
      layerInterfaceList_.push_back(pLayer);
      int group = GetElementGroup(pLayer);
      return pLayerList_->AddElement(pLayer->GetElement(), group);
   }
   return false;
}

/** Verifica si existe la capa en la lista del viewcontext**/
bool DefaultViewcontext::Exists(LayerInterface* pLayer) const {
   LayerInterfaceList::const_iterator it = layerInterfaceList_.begin();
   bool exists = false;
   for (; !exists && it != layerInterfaceList_.end(); ++it) {
      exists = pLayer->GetId().compare((*it)->GetId()) == 0;
   }
   return exists;
}
/**
 * Elimina Layer dado un Id
 * @param[in] LayerId Id de la capa
 * @return true si la capa fue eliminada, false en otro caso;
 */
bool DefaultViewcontext::RemoveLayer(const SuriObject::UuidType &LayerId) {
   bool removed = false;
   LayerInterfaceList::iterator it = layerInterfaceList_.begin();
   while (!removed && it != layerInterfaceList_.end()) {
      if (((*it)->GetId() == LayerId)) {
         if ((*it)->IsBlocked())
            break;
         removed = true;
         if (pLayerList_->DelElement((*it)->GetElement())) {
            layerInterfaceList_.erase(it);
         }
      }
      ++it;
   }
   return removed;
}

/**
 * Obtiene Layer dado un id
 * @param[in] LayerId Id de la capa
 * @return Devuelve el Layer si lo encuentra, sino NULL
 */
LayerInterface* DefaultViewcontext::GetLayer(const SuriObject::UuidType &LayerId) {
   bool found = false;
   LayerInterfaceList::iterator it = layerInterfaceList_.begin();
   LayerInterface *player = NULL;
   while (!found && it != layerInterfaceList_.end()) {
      if (((*it)->GetId() == LayerId)) {
         found = true;
         player = *it;
      }
      ++it;
   }
   return player;
}

/**
 * Obtiene la layer asociada al datasource indicado. Trazabilidad entre datasource y layer
 * @param[in] DatasourceId Id del datasource asociada a la layer
 * @return bool true en caso de encontar la layer asociada, false en otro caso
 */
LayerInterface* DefaultViewcontext::GetAssociatedLayer(const SuriObject::UuidType &DatasourceId) {
   bool found = false;
   LayerInterfaceList::iterator it = layerInterfaceList_.begin();
   LayerInterface* player = NULL;
   while (!found && it != layerInterfaceList_.end()) {
      if (((*it)->GetAssociatedDatasourceId() == DatasourceId)) {
         found = true;
         player = *it;
      }
      ++it;
   }
   return player;
}

/**
 * Elimina una capa de la lista de seleccionados
 * @param[in] LayerId Id del layer al cual se desea deseleccionar
 */
void DefaultViewcontext::UnselectLayer(const SuriObject::UuidType &LayerId) {
   LayerInterface* player = GetLayer(LayerId);
   if (player == NULL) {
      return;
   }
   std::list<SuriObject::UuidType>::iterator it = selectedLayersList_.begin();
   while (it != selectedLayersList_.end()) {
      if ((*it) == LayerId)
         selectedLayersList_.erase(it++);
      else
         ++it;
   }
}

/**
 * Agrega una capa de la lista de seleccionados
 * @param[in] LayerId Id del layer al cual se desea seleccionar
 */
void DefaultViewcontext::DoSelectLayer(const SuriObject::UuidType &LayerId) {
   if (GetLayer(LayerId) == NULL)
      return;

   std::list<SuriObject::UuidType>::iterator it = selectedLayersList_.begin();
   for (; it != selectedLayersList_.end() && *it != LayerId; ++it);

   if (it == selectedLayersList_.end())
      selectedLayersList_.push_back(LayerId);
}

/** Deselecciona todas las capas */
void DefaultViewcontext::ResetSelection() {
   selectedLayersList_.clear();
}

/**
 * Cambia el estado de seleccion de una capa
 * @param[in] LayerId Id del layer al cual se desea seleccionar/deseleccionar
 * @param[in] Status estado de la seleccion que se desea
 */
void DefaultViewcontext::SelectLayer(const SuriObject::UuidType &LayerId, bool Status) {
   if (Status == true) {
      DoSelectLayer(LayerId);
   } else {
      UnselectLayer(LayerId);
   }
}

/**
 * Elimina una capa de la lista de seleccionados a partir del Id del elemento asociado
 * @param[in] ElementId Id del elemento asociado
 * \deprecated
 */
void DefaultViewcontext::UnselectLayerByElementId(const SuriObject::UuidType &ElementId) {
   LayerInterface* player = GetLayerByElementId(ElementId);
   if (player == NULL)
      return;
   SuriObject::UuidType layerid = player->GetId();
   UnselectLayer(layerid);
}

/**
 * Agrega una capa de la lista de seleccionados a partir del Id del elemento asociado
 * @param[in] ElementId Id del elemento asociado
 * \deprecated
 */
void DefaultViewcontext::DoSelectLayerByElementId(const SuriObject::UuidType &ElementId) {
   LayerInterface* player = GetLayerByElementId(ElementId);
   if (player == NULL)
      return;
   SuriObject::UuidType layerid = player->GetId();
   DoSelectLayer(layerid);
}

/**
 * Cambia el estado de seleccion de una capa
 * @param[in] ElementId Id del elemento asociado
 * @param[in] Status estado de la seleccion que se desea
 * \deprecated
 */
void DefaultViewcontext::SelectLayerByElementId(const SuriObject::UuidType &ElementId,
                                                bool Status) {
   if (Status == true) {
      SelectLayerByElementId(ElementId);
   } else {
      UnselectLayerByElementId(ElementId);
   }
}

/**
 * Obtiene las capas seleccionadas
 * @return Devuelve la lista de las layers seleccionadas
 */
std::list<SuriObject::UuidType> DefaultViewcontext::GetSelectedLayers() {
   RemoveInvalidSelectedLayers();
   return selectedLayersList_;
}

/**
 * Elimina las capas seleccionadas
 */
void DefaultViewcontext::RemoveSelectedLayers() {
   std::list<SuriObject::UuidType>::iterator it = selectedLayersList_.begin();
   while (it != selectedLayersList_.end()) {
      SuriObject::UuidType layerid = *it;
      ++it;
      RemoveLayer(layerid);
   }

   selectedLayersList_.clear();
}

/**
 * Update que implementa para actuar como Observer
 * @param[in] Notification Notificacion a ser aplicada
 */
void DefaultViewcontext::Update(NotificationInterface* Notification) {
   Notification->ApplyNotification(this);
}

/**
 * Cambiar una layer de orden dentro la lista
 * @param[in] pLayer Layer a mover
 * @param[in] pLayerDestination Layer la cual se encuentra en la posicion a insertar
 * @return bool true si pudo cambiar la layer de posicion, false en caso contrario
 */
bool DefaultViewcontext::OrderChangeLayer(LayerInterface* pLayer,
                                          LayerInterface* pLayerDestination) {
   if (pLayer == pLayerDestination)
      return true;

   LayerInterface *playeraux = NULL;
   LayerInterfaceList::iterator it = layerInterfaceList_.begin();
   bool found = false;
   for (; !found && it != layerInterfaceList_.end(); ++it) {
      if ((*it)->GetId() == pLayer->GetId()) {
         playeraux = *it;
         layerInterfaceList_.erase(it);
         found = true;
      }
   }
   if (found) {
      LayerInterfaceList::iterator itTo = layerInterfaceList_.begin();
      bool moved = false;
      for (; !moved && itTo != layerInterfaceList_.end(); ++itTo) {
         if ((*itTo)->GetId() == pLayerDestination->GetId()) {
            layerInterfaceList_.insert(itTo, playeraux);
            moved = true;
         }
      }
      return moved
            && pLayerList_->MoveElement(pLayer->GetElement(), pLayerDestination->GetElement());
   }
   return false;
}

/**
 * Mueve una layer al final de la lista
 * @param[in] pLayer Layer a mover
 * @return bool true si pudo mover la layer al final de la lista, false en caso contrario
 */
bool DefaultViewcontext::MoveLayerToEnd(LayerInterface* pLayer) {
   bool found = false;
   LayerInterface* playeraux = NULL;
   LayerInterfaceList::iterator it = layerInterfaceList_.begin();
   for (; !found && it != layerInterfaceList_.end(); ++it) {
      if ((*it)->GetId() == pLayer->GetId()) {
         playeraux = *it;
         layerInterfaceList_.erase(it);
         found = true;
      }
   }
   if (found)
      layerInterfaceList_.push_back(playeraux);
   return found && pLayerList_->MoveElement(pLayer->GetElement(), NULL);
}

/**
 * Obtiene el elemento asociado a una capa
 * @param[in] LayerId Id de la layer que se desea obtener el elemento
 * @return Element* Elemento asociado a la layer
 * \deprecated
 */
Element* DefaultViewcontext::GetAssociatedElement(const SuriObject::UuidType &LayerId) {
   LayerInterface* player = GetLayer(LayerId);
   LayerList::ElementListType::iterator it = pLayerList_->GetElementIterator(true);
   Element *pelement = NULL;
   bool found = false;
   for (; !found && it != pLayerList_->GetElementIterator(false); ++it) {
      REPORT_DEBUG("Layer: %s; ElementIt: %s; pLayer: %s",
                   LayerId.c_str(), (*it)->GetUid().c_str(), player->GetElement()->GetUid().c_str());
      if ((*it)->GetUid() == player->GetElement()->GetUid()) {
         found = true;
         pelement = *it;
      }
   }
   return pelement;
}

/**
 * Devuelve un layer a partir de su element id
 * @param[in] ElementId Id del element de interes
 * @return El layer asociado al id del elemento dado
 * \deprecated
 */
LayerInterface* DefaultViewcontext::GetLayerByElementId(const SuriObject::UuidType &ElementId) {
   LayerInterface* player = NULL;
   LayerInterfaceList::iterator it = layerInterfaceList_.begin();
   for (; it != layerInterfaceList_.end(); ++it) {
      if ((*it)->GetElement()->GetUid() == ElementId) {
         player = *it;
         break;
      }
   }
   return player;
}

/**
 * Activa/Desactiva una capa a partir de un Id
 * @param[in] LayerId Id de la layer a activar
 * @param[in] Status estado de activacion
 */
bool DefaultViewcontext::ActivateLayer(const SuriObject::UuidType &LayerId, bool Status) {
   // Si se quiere activar un raster y hay uno bloqueado
   // entonces sale
   LayerInterface* player = GetLayer(LayerId);
   if (player->IsBlocked())
      return false;
   if (player != NULL && !player->IsHidden() && CanDisplayLayer(player)) {
      Element *pelement = GetAssociatedElement(LayerId);
      pLayerList_->ActivateElement(pelement, Status);
   }
   return true;
}

/** Verifica que no exista una capa de tipo raster desplegada y bloqueada **/
bool DefaultViewcontext::CanDisplayLayer(LayerInterface* player) {
   // TODO(Gabriel - TCK #3881): Se tendria que permitir desplegar raster
   // que posean sistema de referencia (el vector se tendria que desplegar
   // sobre el mismo de forma correcta
   DatasourceInterface * pDatasource = GetAssociatedDatasource(player->GetId());
   if (pDatasource->IsBlocked()) return false;
   if (player->GetElement()->GetClassId().compare("RasterElement") != 0)
      return true;
   bool candisplay = true;
   LayerInterfaceList::iterator it = layerInterfaceList_.begin();
   for (; it != layerInterfaceList_.end() && candisplay; ++it) {
      if ((*it)->IsBlocked() && (*it)->GetElement()->GetClassId().compare("RasterElement") == 0) {
         candisplay = false;
      }
   }
   return candisplay;
}
/**
 * Reemplaza el xml del elemento menos los nodos correspondientes a las capas
 * @param[in] pElementFrom elemento al que se quiere reemplazar el contenido del xml
 * @param[out] pElementDest elemento que posee el xml para reemplazar
 * @return true si pudo reemplazar el nodo, false en caso contrario
 */
bool DefaultViewcontext::ReplaceElementNodes(Element* pElementFrom, Element *pElementDest) {
   wxXmlNode* pnodefrom = new wxXmlNode(*pElementFrom->GetNode());
   // Guardo el nodo render del destino ya que esta informacion no es parte de las fuentes de datos
   wxXmlNode *prendernode = new wxXmlNode(*pElementDest->GetNode(RENDERIZATION_NODE));
   pElementDest->AddNode(NULL, pnodefrom);
   // Una vez reemplazado el xml del elemento piso el de renderizacion para que tenga
   // la informacion correspondientes de las capas
   wxXmlNode* prenderaux = pElementDest->GetNode(RENDERIZATION_NODE);
   bool result = pElementDest->ReplaceNode(prenderaux->GetParent(), prendernode, prenderaux);
   return result;
}

/**
 * Actualiza los datos que son partes de la fuente de datos
 * @param[in] pDatasource Puntero a la fuente de datos a actualizar
 * @return true si tuvo exito, false en caso contrario
 * \deprecated
 */
bool DefaultViewcontext::DatasourcePropertiesChanged(DatasourceInterface* pDatasource) {
   LayerInterface* player = GetAssociatedLayer(pDatasource->GetId());
   bool result = ReplaceElementNodes(pDatasource->GetElement(), player->GetElement());
   player->GetElement()->SetChanged();
   player->GetElement()->SendViewerUpdate();
   return result;
}

/**
 * Activa/Desactiva las capas 3d
 * @param[in] Enable indica si se desea activar/desactivar las capas 3d
 */
void DefaultViewcontext::Enable3d(bool Enable) {
   // Recorre las capas y oculta si son 2d y muestra 3d
   LayerInterfaceList::iterator it = layerInterfaceList_.begin();
   for (; it != layerInterfaceList_.end(); ++it) {
      LayerInterface* player = dynamic_cast<TerrainLayer*>(*it);
      if (!player)
         player = dynamic_cast<VectorLayer*>(*it);
      (*it)->Hide(player != NULL && !Enable);
   }
}

/**
 * Activa/Desactiva las capas 2d
 * @param[in] Enable indica si se desea activar/desactivar las capas 2d
 */
void DefaultViewcontext::Enable2d(bool Enable) {
   // recorre las capas y oculta si son 3d y muestra 2d
   LayerInterfaceList::iterator it = layerInterfaceList_.begin();
   for (; it != layerInterfaceList_.end(); ++it) {
      LayerInterface* player = dynamic_cast<TerrainLayer*>(*it);
      (*it)->Hide(player == NULL && !Enable);
   }
}

/**
 * Indica si el viewcontext soporta capas 3d
 * @return true si soporta capas 3d, false en caso contrario
 */
bool DefaultViewcontext::Is3dLayersSupported() const {
   return support3dLayers_;
}

/**
 * Indica si el viewcontext soporta capas 3d
 * @return true si soporta capas 3d, false en caso contrario
 */
bool DefaultViewcontext::Is2dLayersSupported() const {
   return support2dLayers_;
}

/** Viewcontextmgr al cual pertenece. Esquema contenedor-contenido **/
void DefaultViewcontext::SetViewcontextManager(
      ViewcontextManagerInterface* pViewcontextManager) {
   pViewcontextManager_ = pViewcontextManager;
}

/**
 * Obtiene la instancia fuente de datos asociada a una capa
 * @param LayerId id de la capa sobre la cual se quiere obtener la instancia
 * de la fuente de datos
 * @return DatasourceInterface* referencia a la instancia de la fuente de datos
 * asociada a la capa
 * @return NULL en caso de que no exista la fuente de datos
 */
DatasourceInterface* DefaultViewcontext::GetAssociatedDatasource(
      const SuriObject::UuidType &LayerId) const {
   DatasourceInterface* pdatasource =
         pViewcontextManager_->GetAssociatedDatasource(LayerId);
   return pdatasource;
}

/** Elimina de selectedLayersList_ las capas invalidas. */
void DefaultViewcontext::RemoveInvalidSelectedLayers() {
   std::list<SuriObject::UuidType>::iterator it = selectedLayersList_.begin();
   while (it != selectedLayersList_.end()) {
      if (GetLayer(*it) == NULL)
         selectedLayersList_.erase(it++);
      else
         ++it;
   }
}

/** Bloquea la capa asociada a la fuente de datos con id que se pasa por parametro **/
bool DefaultViewcontext::BlockLayerByDatasourceId(
      const SuriObject::UuidType& DatasourceId) {
   LayerInterface* player = GetAssociatedLayer(DatasourceId);
   if (!player || player->IsBlocked()) {
      return false;
   }
   player->Block();
   return true;
}

/** Desbloquea la capa asociada a la fuente de datos con id que se pasa por parametro **/
bool DefaultViewcontext::UnblockLayerByDatasourceId(
      const SuriObject::UuidType& DatasourceId) {
   LayerInterface* player = GetAssociatedLayer(DatasourceId);
   if (!player || !player->IsBlocked()) {
      return false;
   }
   player->Unblock();
   return true;
}

/** Bloquea la capa que corresponde al id que se pasa por parametro **/
bool DefaultViewcontext::BlockLayer(const SuriObject::UuidType& LayerId) {
   LayerInterface* player = GetLayer(LayerId);
   if (!player || player->IsBlocked()) {
      return false;
   }
   player->Block();
   return true;
}

/** Desbloquea la capa que corresponde al id que se pasa por parametro **/
bool DefaultViewcontext::UnblockLayer(const SuriObject::UuidType& LayerId) {
   LayerInterface* player = GetLayer(LayerId);
   if (!player || !player->IsBlocked()) {
      return false;
   }
   player->Unblock();
   return true;
}

/** Bloquea todas las capas **/
void DefaultViewcontext::BlockAllLayers() {
   LayerInterfaceList::iterator it = layerInterfaceList_.begin();
   for (; it != layerInterfaceList_.end(); ++it) {
      (*it)->Block();
   }
}

/** Desbloquea todas las capas **/
void DefaultViewcontext::UnblockAllLayers() {
   LayerInterfaceList::iterator it = layerInterfaceList_.begin();
   for (; it != layerInterfaceList_.end(); ++it) {
      (*it)->Unblock();
   }
}

/** Consulta si una capa se encuentra bloqueada **/
bool DefaultViewcontext::IsLayerBlocked(const SuriObject::UuidType& LayerId) {
   LayerInterface* player = GetLayer(LayerId);
   if (!player)
      return false;
   return player->IsBlocked();
}

/** Consulta si el tipo de capa es compatible en el contexto **/
bool DefaultViewcontext::IsCompatibleLayerType(LayerInterface* pLayer) {
   bool compatible = false;
   std::string layertype = pLayer->GetElement()->GetClassId();
   switch (type_) {
      case V2D:
      case Map:
      compatible = // (layertype.compare("TerrainElement") != 0) &&
            (layertype.compare("HtmlElement") != 0) &&
            (layertype.compare("MultimediaElement") != 0) &&
            !layertype.empty();
            /** ver porque no era posible en 2D*/
            //&& (layertype.compare("HotLinkVectorElement") != 0);
         break;
      case V3D:
         compatible = (layertype.compare("HtmlElement") != 0) &&
            (layertype.compare("MultimediaElement") != 0) &&
            !layertype.empty();
         break;
      case Html:
      case Documents:
      case Exercises: {
         compatible = (dynamic_cast<HtmlElement*>(pLayer->GetElement()) != NULL) ||
                        (dynamic_cast<MultimediaElement*>(pLayer->GetElement()));
         break;
      }
      case Multimedia:
         compatible = (dynamic_cast<MultimediaElement*>(pLayer->GetElement()) != NULL);
         break;
      case DV:
         compatible = true;
         break;
      default:
         break;
   }
   return compatible;
}

/** Obtiene el id del viewcontext asociado **/
suri::SuriObject::UuidType DefaultViewcontext::GetViewcontextId() const {
   return viewcontextId_;
}

/** Configura por unica vez el id del viewcontext */
bool DefaultViewcontext::SetViewcontextId(const SuriObject::UuidType& ViewcontextId) {
   if (viewcontextId_.compare(SuriObject::NullUuid) == 0) {
      viewcontextId_ = ViewcontextId;
      return true;
   }
   return false;
}

/** Elimina todas las capas que posee el viewcontext **/
void DefaultViewcontext::RemoveAllLayers() {
   selectedLayersList_.clear();
   layerInterfaceList_.clear();
   delete_all(layerInterfaceList_);
}
} /** namespace suri */

