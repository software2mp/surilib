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
#include <utility>
#include <map>
#include <string>
#include <map>
#include <list>

// Includes Suri
#include "suri/LayerTreeWidget.h"
#include "LayerTreeEventHandler.h"
#include "suri/LayerList.h"
#include "suri/Element.h"
#include "ElementHtmlConfigurationData.h"
#include "suri/messages.h"
#include "suri/DataViewManager.h"
#include "suri/ViewcontextInterface.h"

// Includes Wx
#include "wx/wx.h"

// Includes App

// Defines

// Forwards

namespace suri {

/** Ctor */
LayerTreeWidget::LayerTreeWidget(wxWindow* pParent, DataViewManager *pDataViewManager,
                                 ViewcontextInterface *pViewcontext) :
      suri::Widget(pParent), pViewcontext_(pViewcontext), pDataViewManager_(pDataViewManager) {
   windowTitle_ = caption_LAYERS;
}

/** Dtor */
LayerTreeWidget::~LayerTreeWidget() {
   if (Model::IsValid(pViewcontext_->GetLayerList()))
      pViewcontext_->GetLayerList()->UnregisterViewer(this);
   // Si pToolWindow_ == NULL quiere decir que hay grandes
   // chances que pEventHandler_ sea invalido (referencias
   // circulares de Model/View)
   if (pToolWindow_ && pEventHandler_) pEventHandler_->SetTreeWidget(NULL);
}

/**
 * Crea un panel y agrega un wxSimpleHtmlTreeCtrl dentro del mismo
 * @return true Creacion exitosa.
 * @return false No se pudo crear la lista
 */
bool LayerTreeWidget::CreateToolWindow() {
   // Configuro wxSimpleHtmlTreeCtrl que muestra los elementos

   pHtmlTreeCtrl_ = new wxSimpleHtmlTreeCtrl(
         pParentWindow_, wxLB_MULTIPLE,
         wxDraggableItems | wxContextMenu | wxCheckableItems);

   pEventHandler_ = new LayerTreeEventHandler(pDataViewManager_, pViewcontext_);
   pEventHandler_->SetTreeWidget(this);
   pHtmlTreeCtrl_->SetTreeEventHandler(pEventHandler_);

   pToolWindow_ = pHtmlTreeCtrl_;
   pViewcontext_->GetLayerList()->RegisterViewer(this);

   return true;
}

/**
 * Crea un grupo con los elementos seleccionados y lo
 * agrega a la interfaz grafica
 * @return bool que indica si se pudo crear el grupo.
 */
bool LayerTreeWidget::CreateGroupWithSelectedElements() {
   std::list<ItemId> selecteditems = pHtmlTreeCtrl_->GetSelectedItemsIds();
   if (selecteditems.size() == 0) return false;

   Update();

   std::string newgroupname = "";
   if (!GetNewGroupName(newgroupname)) return false;

   // Si es primer elemento seleccionado esta dentro de otro grupo 'X',
   // agrego el nuevo grupo antes de 'X' en la raiz
   ItemId firstitem = *selecteditems.begin();
   ItemId firstitemparent = pHtmlTreeCtrl_->GetParentNode(firstitem);
   if (firstitemparent.Compare(pHtmlTreeCtrl_->GetRoot()) != 0) firstitem =
         firstitemparent;

   // Creo el grupo
   HtmlConfigurationData* pgrouphtmlconfig = new HtmlConfigurationData(newgroupname,
                                                                       false);
   ItemId groupid = pHtmlTreeCtrl_->AddNodeBefore(firstitem, pgrouphtmlconfig);

   // Muevo lo elementos al grupo.
   std::list<ItemId>::iterator it = selecteditems.begin();
   while (it != selecteditems.end()) {
      if (pEventHandler_->ValidateNodeNewParent(*it, groupid))
         pEventHandler_->DoOnMoveNodeUnder(*it, groupid);
      it++;
   }

   pHtmlTreeCtrl_->Expand(groupid, true);

   // Limpio la seleccion de la lista
   int elementcount = pViewcontext_->GetLayerList()->GetElementCount();
   for (int i = 0; i < elementcount; i++)
      pViewcontext_->GetLayerList()->SelectElement(i, false);

   return true;
}

/**
 * Si hay uno o mas grupos seleccionados se eliminan. Los elementos
 * contenidos en el grupo tambien se eliminan
 * @return bool que indica si se pudo eliminar el grupo
 */
bool LayerTreeWidget::DeleteSelectedGroups() {
   std::list<ItemId> selecteditems = pHtmlTreeCtrl_->GetSelectedItemsIds();
   if (selecteditems.size() == 0) return false;

   Update();

   pViewcontext_->GetLayerList()->BlockViewerUpdate();

   // Elimino los elementos del grupo
   std::list<ItemId>::iterator it = selecteditems.begin();
   while (it != selecteditems.end()) {
      HtmlConfigurationData* pconfigdata = pHtmlTreeCtrl_->GetConfigurationData(*it);
      if (pconfigdata != NULL && !pconfigdata->IsLeaf()) {
         ItemId child = pHtmlTreeCtrl_->GetFirstChild(*it);
         while (child.Compare(INVALIDTREEID) != 0) {
            pViewcontext_->GetLayerList()->DelElement(GetElement(child));
            child = pHtmlTreeCtrl_->GetNextSibling(child);
         }
         Update();
         pHtmlTreeCtrl_->RemoveNode(*it);
      }
      it++;
   }

   pViewcontext_->GetLayerList()->UnblockViewerUpdate();

   // Elimino el grupo
   return true;
}

/**
 * Analiza los cambios en la iterfaz grafica y acutaliza la lista
 * @param[in] pModel modelo que quiero actualizar
 */
void LayerTreeWidget::Update(Model *pModel) {
   Update();
}

/**
 * Recibe el aviso de delete de LayerList
 * @param[in] pModel modelo que quiero actualizar
 */
void LayerTreeWidget::OnModelDelete(Model *pModel) {
   // pLayerList_ = NULL;
}

/**
 * Sincroniza la interfaz grafica con la LayerList
 */
void LayerTreeWidget::Update() {
   // guarda para evitar actualizar luego de destruccion
   if (!pToolWindow_) return;
   pHtmlTreeCtrl_->BeginBatch();

   // Elimino del arbol los nodos que no estan en la LayerList
   ItemId currentnode = GetNextTreeLeaf(pHtmlTreeCtrl_->GetRoot());
   ItemId previousnode = pHtmlTreeCtrl_->GetRoot();
   while (currentnode.Compare(INVALIDTREEID) != 0) {
      Element* currentelement = GetElement(currentnode);
      if (pViewcontext_->GetLayerList()->GetElement(currentelement) == NULL)
         delete pHtmlTreeCtrl_->RemoveNode(currentnode);
      else
         previousnode = currentnode;
      currentnode = GetNextTreeLeaf(previousnode);
   }

   // Elimino de elementsConfigData_ los items que ya no estan en la LayerList
   std::map<Element*, ElementHtmlConfigurationData*>::iterator it =
         elementsConfigData_.begin();
   while (it != elementsConfigData_.end()) {
      if (pViewcontext_->GetLayerList()->GetElement(it->first) == NULL)
         elementsConfigData_.erase(it++);
      else
         ++it;
   }

   // Agrego elementos nuevos de LayerList a arbol y actualizo los nodos existentes
   currentnode = pHtmlTreeCtrl_->GetRoot();
   int elementcount = pViewcontext_->GetLayerList()->GetElementCount();
   for (int i = 0; i < elementcount; i++) {
      Element* pelement = pViewcontext_->GetLayerList()->GetElement(i);

      if (elementsConfigData_.count(pelement) == 0) {
         currentnode = AddNodeAfterLeaf(currentnode, pelement);
      } else {
         // El atributo elementsConfigData_ y esta linea son necesarios porque
         // el ActivationLogic de la LayerList bloquea la actualizacion de
         // los viewer de los elementos antes de desactivarlos.
         // Por esta razon es necesario que en Update de LayerTreeWidget
         // tambien se actualicen todos los viewers de los elementos.
         elementsConfigData_[pelement]->Update();

         currentnode = GetNextTreeLeaf(currentnode);
      }
   }

   // CODIGO TEMPORAL PARA QUE EN SIMUR NO QUEDEN GRUPOS VACIOS AL CERRAR UN PAT
   currentnode = pHtmlTreeCtrl_->GetFirstChild(pHtmlTreeCtrl_->GetRoot());
   while (currentnode.Compare(INVALIDTREEID) != 0) {
      ItemId nextnode = pHtmlTreeCtrl_->GetNextSibling(currentnode);
      if (!pHtmlTreeCtrl_->GetConfigurationData(currentnode)->IsLeaf()
            && pHtmlTreeCtrl_->GetFirstChild(currentnode).Compare(INVALIDTREEID) == 0)
         pHtmlTreeCtrl_->RemoveNode(currentnode);
      currentnode = nextnode;
   }

   pHtmlTreeCtrl_->EndBatch();
}

/**
 * Agrega elemento al arbol despues de la hoja que recibe por parametro
 * @param[in] LastLeaf ItemId de la hoja anterior a donde quiero agregar el
 * elemento. Si es la raiz del arbol agrego el elemento al comienzo del mismo
 * @param[in] pElement elemento con el que voy a configurar la hoja.
 * @return ItemId del nodo agregado. INVALIDTREEID si ocurrio un error.
 */
ItemId LayerTreeWidget::AddNodeAfterLeaf(const ItemId &LastLeaf, Element* pElement) {
   ItemId newitemid = INVALIDTREEID;
   ElementHtmlConfigurationData* pconfdata = new ElementHtmlConfigurationData(
         pElement, pHtmlTreeCtrl_);

   ItemId root = pHtmlTreeCtrl_->GetRoot();
   if (LastLeaf.Compare(root) == 0) {
      ItemId firstchild = pHtmlTreeCtrl_->GetFirstChild(root);
      if (firstchild.Compare(INVALIDTREEID) == 0)
         newitemid = pHtmlTreeCtrl_->AppendNode(root, pconfdata);
      else
         newitemid = pHtmlTreeCtrl_->AddNodeBefore(firstchild, pconfdata);
   } else {
      // Agrego nodo en raiz si LastLeaf es ultimo elemento de grupo
      ItemId lastleafparent = pHtmlTreeCtrl_->GetParentNode(LastLeaf);
      if (lastleafparent.Compare(pHtmlTreeCtrl_->GetRoot()) != 0
            && pHtmlTreeCtrl_->GetNextSibling(LastLeaf).Compare(INVALIDTREEID) == 0)
         newitemid = pHtmlTreeCtrl_->AddNodeAfter(lastleafparent, pconfdata);
      else
         newitemid = pHtmlTreeCtrl_->AddNodeAfter(LastLeaf, pconfdata);
   }

   if (newitemid.Compare(INVALIDTREEID) == 0) {
      delete pconfdata;
      return INVALIDTREEID;
   }

   elementsConfigData_.insert(
         std::pair<Element*, ElementHtmlConfigurationData*>(pElement, pconfdata));

   return newitemid;
}

/**
 * Busca la siguiente hoja en el arbol
 * @param[in] CurrentLeaf ItemId de la hoja actual. Si es la raiz del arbol
 * busca la primer hoja
 * @return ItemId de la siguiente hoja. INVALIDID si no hay siguiente hoja.
 */
ItemId LayerTreeWidget::GetNextTreeLeaf(const ItemId &CurrentLeaf) {
   // Este ciclo itera sobre el arbol en profundidad buscando la siguiente hoja
   bool isleaf = false;
   ItemId nextnode = CurrentLeaf;
   ItemId root = pHtmlTreeCtrl_->GetRoot();
   int direction = 1;  // 1: entrando nodo, 0: siguiente hijo, -1: saliendo nodo
   while (!isleaf && nextnode.Compare(INVALIDTREEID) != 0) {
      if (direction == 1) {
         if (pHtmlTreeCtrl_->GetFirstChild(nextnode).Compare(INVALIDTREEID) != 0) {
            nextnode = pHtmlTreeCtrl_->GetFirstChild(nextnode);
         } else {
            direction = 0;
         }
      }

      if (direction == 0) {
         if (pHtmlTreeCtrl_->GetNextSibling(nextnode).Compare(INVALIDTREEID) != 0) {
            nextnode = pHtmlTreeCtrl_->GetNextSibling(nextnode);
            direction = 1;
         } else {
            direction = -1;
         }
      }

      if (direction == -1) {
         if (pHtmlTreeCtrl_->GetParentNode(nextnode).Compare(INVALIDTREEID) != 0) {
            nextnode = pHtmlTreeCtrl_->GetParentNode(nextnode);
            direction = 0;
         } else {
            nextnode = INVALIDTREEID;
         }
      }

      if (nextnode.Compare(INVALIDTREEID) != 0 && nextnode.Compare(root) != 0) isleaf =
            pHtmlTreeCtrl_->GetConfigurationData(nextnode)->IsLeaf();
   }

   return nextnode;
}

/**
 * Obtiene el elemento a partir del id del nodo
 * @param[in] Id id del nodo que corresponde al elemento
 * @return Element* del elemento encontrado. NULL si es un nodo interno o
 * el Id es invalido
 */
Element* LayerTreeWidget::GetElement(const ItemId &Id) {
   if (pHtmlTreeCtrl_ == NULL || Id.Compare(INVALIDTREEID) == 0) return NULL;

   HtmlConfigurationData* pconfigdata = pHtmlTreeCtrl_->GetConfigurationData(Id);
   ElementHtmlConfigurationData* ptreeconfigdata =
         dynamic_cast<ElementHtmlConfigurationData*>(pconfigdata);

   if (!ptreeconfigdata) return NULL;

   return ptreeconfigdata->GetElement();
}

/**
 * Retorna el control wx que despliega la LayerList
 * @return wxSimpleHtmlTreeCtrl que se usa para graficar el contenido
 * de LayerList
 */
wxSimpleHtmlTreeCtrl* LayerTreeWidget::GetWxTreeCtrl() {
   return pHtmlTreeCtrl_;
}

/**
 * Obtiene del usuario el nombre del nuevo grupo
 * @param[out] NewGroupName el nombre del nuevo grupo
 * @return bool que indica si el nombre retornado es valido
 */
bool LayerTreeWidget::GetNewGroupName(std::string &NewGroupName) {
   // creo un dialogo para ingresar un txt
   wxDialog dialog(GetFrame(), wxID_ANY, _(caption_NEW_GROUP_INPUT), wxDefaultPosition,
                   wxDefaultSize, wxCAPTION | wxSYSTEM_MENU);
   dialog.SetSizer(new wxBoxSizer(wxVERTICAL));
   wxTextCtrl *ptxtctrl = new wxTextCtrl(&dialog, wxID_ANY,
                                         _(content_DEFAULT_GROUP_NAME),
                                         wxDefaultPosition, wxSize(250, -1));
   dialog.GetSizer()->Add(ptxtctrl, 0, wxEXPAND | wxALL, 5);
   dialog.GetSizer()->Add(dialog.CreateButtonSizer(wxOK), 0, wxEXPAND | wxALIGN_BOTTOM);
   dialog.Fit();

   // Valor por default
   wxString newgroupname = _(content_DEFAULT_GROUP_NAME);
   ptxtctrl->SetValidator(wxTextValidator(wxFILTER_NONE, &newgroupname));
   ptxtctrl->SelectAll();
   ptxtctrl->SetFocusFromKbd();

   // muestro el dialogo
   if (dialog.ShowModal() != wxID_OK) return false;

   NewGroupName = newgroupname.c_str();
   return true;
}

/** Obtiene la layerlist \deprecated*/
LayerList *LayerTreeWidget::GetLayerList() {
   return pViewcontext_->GetLayerList();
}

}  // namespace

