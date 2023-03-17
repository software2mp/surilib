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
#include <string>
#include <list>
#include <set>

// Includes Suri
#include "LayerTreeEventHandler.h"
#include "ElementHtmlConfigurationData.h"
#include "suri/LayerTreeWidget.h"
#include "suri/LayerList.h"
#include "suri/ElementManager.h"
#include "suri/ElementPropertiesWidget.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/ViewcontextInterface.h"
#include "suri/LayerInterface.h"
#include "suri/DatasourceInterface.h"
#include "suri/xmlnames.h"
#include "suri/LayerFactory.h"
#include "suri/DataViewManager.h"

// Includes Wx

// Includes App

// Defines
#ifdef __DEBUG__
/** Define que indica si se desea mostrar las propiedades durante la edicion
 *  de un elemento vectorial que no se ha salvado a disco
 */
#   define __SHOW_PROPERTIES_DURING_EDITION__
#endif
// Forwards

namespace suri {

/** Ctor
 *  @param[in] pDatasourceManager referencia al administrador de las fuentes de datos
 *  @param[in] pViewcontext viewcontext que se esta asociado al widget del handler
 *
 */
LayerTreeEventHandler::LayerTreeEventHandler(DataViewManager *pDataViewManager,
                                             ViewcontextInterface *pViewcontext):
                         lastMenuItemId_(INVALIDTREEID), pDataViewManager_(pDataViewManager),
                         pViewcontext_(pViewcontext) {
}

/** Dtor */
LayerTreeEventHandler::~LayerTreeEventHandler() {
}

/**
 * Configura el widget que despliega la LayerList
 * @param[in] pTreeWidget widget que despliega la LayerList
 */
void LayerTreeEventHandler::SetTreeWidget(LayerTreeWidget* pTreeWidget) {
   pTreeWidget_ = pTreeWidget;
}

/**
 * Retorna el widget donde se despliega la LayerList
 * @retorna puntero a wxSimpleHtmlTreeCtrl
 */
wxSimpleHtmlTreeCtrl* LayerTreeEventHandler::GetHtmlTreeCtrl() {
   return (pTreeWidget_ != NULL) ? pTreeWidget_->GetWxTreeCtrl() : NULL;
}

/** Se ejecuta cuando se modifican los elementos seleccionados */
void LayerTreeEventHandler::DoOnSelectionChanged() {
   std::set<Element*> selectedelements;

   std::list<ItemId> selecteditems = GetHtmlTreeCtrl()->GetSelectedItemsIds();
   std::list<ItemId>::iterator it = selecteditems.begin();
   for (; it != selecteditems.end(); it++)
      selectedelements.insert(pTreeWidget_->GetElement(*it));

   int elementcount = pViewcontext_->GetLayerList()->GetElementCount();
   for (int i = 0; i < elementcount; i++) {
      bool isactive = (selectedelements.count(pViewcontext_->GetLayerList()->GetElement(i)) > 0);
      pViewcontext_->GetLayerList()->SelectElement(i, isactive);
   }

   ShowLayerListContent();
}

/**
 * Se encarga de modificar el estado de un item y el de sus hijos
 * @param[in] Id del item que quiero modificar
 * @param[in] NewState nuevo estado del item que quiero modificar
 */
void LayerTreeEventHandler::DoOnStateChanged(const ItemId &Id,
                                             HtmlConfigurationData::State NewState) {
   LayerInterface* player =
         pViewcontext_->GetLayerByElementId(pTreeWidget_->GetElement(Id)->GetUid());
   if (player != NULL && !player->IsHidden()) {
      pViewcontext_->GetLayerList()->ActivateElement(pTreeWidget_->GetElement(Id),
                                                     NewState == HtmlConfigurationData::ON);
   }
   ShowLayerListContent();
}

/**
 * Valida si se puede mover nodo Id debajo de DestinationId.
 * Se puede redefinir para hacer mas restrictivo el movimiento de nodos dentro
 * del arbol.
 * @param[in] Id id del nodo que quiero mover
 * @param[in] ParentId id del nuevo nodo padre
 * @return resultado de la validacion
 */
bool LayerTreeEventHandler::ValidateNodeNewParent(const ItemId &Id,
                                                  const ItemId &ParentId) {
   if (GetHtmlTreeCtrl()->GetRoot().Compare(ParentId) != 0) {
      // Solo permito un nivel de anidamiento.
      if (!GetHtmlTreeCtrl()->GetConfigurationData(Id)->IsLeaf()) return false;

      // No permito que se agrupen(en arbol) elementos de distintos
      // grupos(en LayerList)
      LayerList::ElementData* movedelementdata = pViewcontext_->GetLayerList()->GetElementData(
            pTreeWidget_->GetElement(Id));
      LayerList::ElementData* firstgroupchilddata = pViewcontext_->GetLayerList()->GetElementData(
            pTreeWidget_->GetElement(GetHtmlTreeCtrl()->GetFirstChild(ParentId)));
      if (firstgroupchilddata != NULL
            && (movedelementdata == NULL
                  || firstgroupchilddata->group_ != movedelementdata->group_))
         return false;
   }
   return true;
}

/**
 * Mueve un nodo despues de otro.
 * @param[in] Id id del nodo que quiero mover
 * @param[in] DestinationId id del destino
 */
void LayerTreeEventHandler::DoOnMoveNodeAfter(const ItemId &Id,
                                              const ItemId &DestinationId) {
   pViewcontext_->GetLayerList()->BlockViewerUpdate();

   if (!GetHtmlTreeCtrl()->GetConfigurationData(Id)->IsLeaf()) {
      HtmlConfigurationData* phtmlconf = GetHtmlTreeCtrl()->GetConfigurationData(Id);
      ItemId newgroup = GetHtmlTreeCtrl()->AddNodeAfter(DestinationId, phtmlconf);
      if (MoveGroupContent(Id, newgroup))
         GetHtmlTreeCtrl()->RemoveNode(Id);
      else
         GetHtmlTreeCtrl()->RemoveNode(newgroup);
      ShowLayerListContent();
   } else {
      // Muevo el elemento en LayerList
      Element* pelem = pTreeWidget_->GetElement(Id);
      Element* pdestelem = pTreeWidget_->GetElement(
            pTreeWidget_->GetNextTreeLeaf(DestinationId));
      if (pelem == pdestelem || pViewcontext_->GetLayerList()->MoveElement(pelem, pdestelem)) {
         // Actualizo el arbol
         HtmlConfigurationData* phtmlconf = GetHtmlTreeCtrl()->RemoveNode(Id);
         GetHtmlTreeCtrl()->AddNodeAfter(DestinationId, phtmlconf);
         DoOnSelectionChanged();
      }
   }
   pViewcontext_->GetLayerList()->UnblockViewerUpdate();
   pViewcontext_->GetLayerList()->SendViewerUpdate();
}

/**
 * Mueve un nodo antes de otro.
 * @param[in] Id id del nodo que quiero mover
 * @param[in] DestinationId id del destino
 */
void LayerTreeEventHandler::DoOnMoveNodeBefore(const ItemId &Id,
                                               const ItemId &DestinationId) {
   pViewcontext_->GetLayerList()->BlockViewerUpdate();

   if (!GetHtmlTreeCtrl()->GetConfigurationData(Id)->IsLeaf()) {
      HtmlConfigurationData* phtmlconf = GetHtmlTreeCtrl()->GetConfigurationData(Id);
      ItemId newgroup = GetHtmlTreeCtrl()->AddNodeBefore(DestinationId, phtmlconf);
      if (MoveGroupContent(Id, newgroup))
         GetHtmlTreeCtrl()->RemoveNode(Id);
      else
         GetHtmlTreeCtrl()->RemoveNode(newgroup);
      ShowLayerListContent();
   } else {
      // Muevo el elemento en LayerList
      Element* pelem = pTreeWidget_->GetElement(Id);
      Element* pdestelem = NULL;
      if (GetHtmlTreeCtrl()->GetConfigurationData(DestinationId)->IsLeaf())
         pdestelem = pTreeWidget_->GetElement(DestinationId);
      else
         pdestelem = pTreeWidget_->GetElement(
               pTreeWidget_->GetNextTreeLeaf(DestinationId));
      if (pelem == pdestelem || pViewcontext_->GetLayerList()->MoveElement(pelem, pdestelem)) {
         // Actualizo el arbol
         HtmlConfigurationData* phtmlconf = GetHtmlTreeCtrl()->RemoveNode(Id);
         GetHtmlTreeCtrl()->AddNodeBefore(DestinationId, phtmlconf);
         DoOnSelectionChanged();
      }
   }
   pViewcontext_->GetLayerList()->UnblockViewerUpdate();
   pViewcontext_->GetLayerList()->SendViewerUpdate();
}

/**
 * Mueve un nodo debajo de otro.
 * @param[in] Id id del nodo que quiero mover
 * @param[in] DestinationId id del destino
 */
void LayerTreeEventHandler::DoOnMoveNodeUnder(const ItemId &Id,
                                              const ItemId &DestinationId) {
   pViewcontext_->GetLayerList()->BlockViewerUpdate();

   if (!GetHtmlTreeCtrl()->GetConfigurationData(Id)->IsLeaf()) {
      HtmlConfigurationData* phtmlconf = GetHtmlTreeCtrl()->GetConfigurationData(Id);
      ItemId newgroup = GetHtmlTreeCtrl()->AppendNode(DestinationId, phtmlconf);
      if (MoveGroupContent(Id, newgroup))
         GetHtmlTreeCtrl()->RemoveNode(Id);
      else
         GetHtmlTreeCtrl()->RemoveNode(newgroup);
      ShowLayerListContent();
   } else {
      // Busco el elemento del primer nodo tipo hoja posterior a grupo 'DestinationId'
      Element* pdestelem = NULL;
      ItemId nextsibling = GetHtmlTreeCtrl()->GetNextSibling(DestinationId);
      HtmlConfigurationData* pdestconfdata = GetHtmlTreeCtrl()->GetConfigurationData(
            nextsibling);
      if (pdestconfdata != NULL && !pdestconfdata->IsLeaf())
         nextsibling = pTreeWidget_->GetNextTreeLeaf(nextsibling);
      pdestelem = pTreeWidget_->GetElement(nextsibling);

      // Muevo el elemento en LayerList
      Element* pelem = pTreeWidget_->GetElement(Id);
      if (pelem == pdestelem || pViewcontext_->GetLayerList()->MoveElement(pelem, pdestelem)) {
         // Actualizo el arbol
         HtmlConfigurationData* phtmlconf = GetHtmlTreeCtrl()->RemoveNode(Id);
         GetHtmlTreeCtrl()->AppendNode(DestinationId, phtmlconf);
         DoOnSelectionChanged();
      }
   }
   pViewcontext_->GetLayerList()->UnblockViewerUpdate();
   pViewcontext_->GetLayerList()->SendViewerUpdate();
}

/**
 * Genera menu que se debera mostrar sobre el arbol.
 * @param[in] ItemId id del item sobre el que actuara el menu
 * @return menu creado
 * \attention el menu debe ser eliminado por el arbol
 * \attention: El menu que se levanta carga al mostrar, depende de los recursos
 *  los recursos de la aplicacion sobreescriben los de la biblioteca, por lo que
 *  puede ser distinto segun el .xrc de la aplicacion. Ver TCK
 */
wxMenu* LayerTreeEventHandler::CreateContextMenu(const ItemId &Id) {
   Element* pelem = pTreeWidget_->GetElement(Id);
   if (!pelem)
      return NULL;

   lastMenuItemId_ = Id;
   wxMenu *pmenu = wxXmlResource::Get()->LoadMenu(wxT("ID_ELEMENT_MENU"));
   if (pelem->IsActive()) {
      // No se traduce
      wxMenuItem *pitemdisplay = pmenu->FindItem(pmenu->FindItem(_(menu_DISPLAY)));
      pmenu->Remove(pitemdisplay);
   } else {
      // No se traduce
      wxMenuItem *pitemhide = pmenu->FindItem(pmenu->FindItem(_(menu_HIDE)));
      pmenu->Remove(pitemhide);
   }
   return pmenu;
}

/**
 * Obtiene el elemento de la lista de fuente de datos asociado al del widget
 * @param[in] pElement referencia al elemento que se le aplica alguna accion dentro del widget
 * @return Element * elemento de la lista de fuente de datos que corresponde al elemento del widget
 * @return NULL en caso de que no se encuentra la fuente de datos asociada
 */
DatasourceInterface* LayerTreeEventHandler::GetAssociatedDatasource(Element *pElement) {
   LayerInterface *player = pViewcontext_->GetLayerByElementId(pElement->GetUid());
   DatasourceInterface *pdatasource = NULL;
   if (player) {
      SuriObject::UuidType datasourceId = player->GetAssociatedDatasourceId();
      pdatasource = pDataViewManager_->GetDatasourceManager()->GetDatasource(datasourceId);
   }
   return pdatasource;
}

/**
 * Se llama cuando se selecciona item del menu contextual
 * @param[in] Event evento de menu
 */
void LayerTreeEventHandler::DoOnContextMenu(wxMenuEvent &Event) {
   Element* pelemlayer = pTreeWidget_->GetElement(lastMenuItemId_);
   DatasourceInterface *pdatasource = GetAssociatedDatasource(pelemlayer);
   LayerInterface *player = pViewcontext_->GetLayerByElementId(pelemlayer->GetUid());
#ifdef __SHOW_PROPERTIES_DURING_EDITION__
   bool tempdatasourcecreated = false;
   if (!pdatasource || !player) {
      tempdatasourcecreated = true;
      const char *pdatasourcetypes[] = { "RasterDatasource", "VectorDatasource" };
      for (int i = 0; i < 2 && pdatasource == NULL; ++i)
         pdatasource = DatasourceInterface::Create(pdatasourcetypes[i], pelemlayer);
      if (pdatasource == NULL)
         return;
      player = LayerFactory::CreateLayer(pdatasource);
   }
#endif
   if (!pdatasource)
      return;
   // Menu de propiedades
   if (Event.GetId() == XRCID("ID_ELEMENT_PROPERTIES")) {
      ElementPropertiesWidget *ppropertieswidget =
            new ElementPropertiesWidget(pDataViewManager_, pdatasource, player);
      ppropertieswidget->ShowModal();
      pelemlayer->SetChanged();
      pelemlayer->SendViewerUpdate();
   }  else if (Event.GetId() == XRCID("ID_ELEMENT_CLOSE")) {
      ElementManager::Instance().DeleteElement(pelemlayer->GetUid());
   } else if (Event.GetId() == XRCID("ID_ELEMENT_DISPLAY")) {
      if (player != NULL && !player->IsHidden()) {
         pViewcontext_->GetLayerList()->ActivateElement(pelemlayer, true);
      }
   } else if (Event.GetId() == XRCID("ID_ELEMENT_HIDE")) {
      if (player != NULL && !player->IsHidden()) {
         pViewcontext_->GetLayerList()->ActivateElement(pelemlayer, false);
      }
   }
#ifdef __SHOW_PROPERTIES_DURING_EDITION__
   if (tempdatasourcecreated) {
        DatasourceInterface::Destroy(pdatasource);
        delete player;
   }
#endif
}

/**
 * Mueve el contenido de un grupo dentro de otro grupo. El cambio
 * se realiza en la interfaz grafica y en la LayerList
 * @param[in] GroupId Id del grupo con elementos a mover
 * @param[in] NewGroupId Id del grupo donde se enviaran los elementos.
 * @return bool que indica si tuvo exito
 */
bool LayerTreeEventHandler::MoveGroupContent(const ItemId &GroupId,
                                             const ItemId &NewGroupId) {
   // Encuentro nodo siguiente a donde quiero mover el grupo
   Element* pdestelem = pTreeWidget_->GetElement(
         pTreeWidget_->GetNextTreeLeaf(NewGroupId));

   // Muevo los elementos
   bool moveelementinlayerlist = true;
   ItemId firstchild = GetHtmlTreeCtrl()->GetFirstChild(GroupId);
   while (firstchild.Compare(INVALIDTREEID) != 0) {
      Element* pelem = pTreeWidget_->GetElement(firstchild);
      if (pelem == pdestelem) moveelementinlayerlist = false;

      if (moveelementinlayerlist && !pViewcontext_->GetLayerList()->MoveElement(pelem, pdestelem))
         return false;

      HtmlConfigurationData* phtmlconf = GetHtmlTreeCtrl()->RemoveNode(firstchild);
      GetHtmlTreeCtrl()->AppendNode(NewGroupId, phtmlconf);

      firstchild = GetHtmlTreeCtrl()->GetFirstChild(GroupId);
   }

   if (GetHtmlTreeCtrl()->IsExpanded(GroupId))
      GetHtmlTreeCtrl()->Expand(NewGroupId, true);
   return true;
}

/** Muestra el nombre de los elementos en la layer list ordenados */
void LayerTreeEventHandler::ShowLayerListContent() {
   int count = pViewcontext_->GetLayerList()->GetElementCount();
   for (int i = 0; i < count; i++) {
      std::string selected = "Selected";
      if (pViewcontext_->GetLayerList()->GetElementData(i)->selected_ == false) selected = "";
      REPORT_DEBUG("LayerList[%d] -> %s (%s) ", i, pViewcontext_->GetLayerList()->
                   GetElement(i)->GetName().c_str(), selected.c_str());
   }
}
} /** Namespace suri */

