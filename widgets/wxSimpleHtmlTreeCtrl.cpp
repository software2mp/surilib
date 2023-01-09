/*! \file wxSimpleHtmlTreeCtrl.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "wxSimpleHtmlTreeCtrl.h"

// Includes standard

// Includes Suri
#include "wxNoModelTreeEventHandler.h"
#include "TreeInnerNode.h"
#include "TreeLeaf.h"
#include "logmacros.h"

// Includes Wx

// Includes App

// Defines

namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(wxSimpleHtmlTreeCtrlEvent, wxSimpleHtmlTreeCtrl)IMPLEMENT_EVENT_CALLBACK(OnLink, OnLink(event), wxHtmlLinkEvent)
   IMPLEMENT_EVENT_CALLBACK(OnResize, OnResize(event), wxSizeEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRightUp, OnRightUp(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLeftDown, OnLeftDown(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnDoubleClick, OnDoubleClick(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnContextMenu, OnContextMenu(event), wxMenuEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Ctor
 * @param[in] pParent ventana que contendra el arbol html
 * @param[in] Style estilo que se usa para configurar wxHtmlListBox
 * @param[in] BehaviorFlags Configura que comportamiento tendra la lista
 */
wxSimpleHtmlTreeCtrl::wxSimpleHtmlTreeCtrl(wxWindow* pParent, long Style,
                                           long BehaviorFlags) :
      wxDecoratedHtmlListCtrl(pParent, Style, BehaviorFlags),
      NEW_EVENT_OBJECT(wxSimpleHtmlTreeCtrlEvent)
{
   batchCount_ = 0;
   validDoubleClick_ = true;
   dirty_ = false;
   root_ = new TreeInnerNode(NULL, true);
   root_->SetNodeId(TreeNodeId("root"));
   pTreeEventHandler_ = new wxNoModelTreeEventHandler(this);

   Connect(wxEVT_COMMAND_HTML_LINK_CLICKED,
           wxHtmlLinkEventHandler(wxSimpleHtmlTreeCtrlEvent::OnLink), NULL,
           pEventHandler_);

   Connect(wxEVT_SIZE, wxSizeEventHandler(wxSimpleHtmlTreeCtrlEvent::OnResize), NULL,
           pEventHandler_);

   Connect(wxEVT_UPDATE_UI,
           wxUpdateUIEventHandler(wxSimpleHtmlTreeCtrlEvent::OnUIUpdate), NULL,
           pEventHandler_);

   Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(wxSimpleHtmlTreeCtrlEvent::OnLeftDown),
           NULL, pEventHandler_);

   if ((behaviorFlags_ & wxCheckableItems) != 0) {
      Connect(wxEVT_LEFT_DCLICK,
              wxMouseEventHandler(wxSimpleHtmlTreeCtrlEvent::OnDoubleClick), NULL,
              pEventHandler_);
   }

   if ((behaviorFlags_ & wxContextMenu) != 0) {
      Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(wxSimpleHtmlTreeCtrlEvent::OnRightUp),
              NULL, pEventHandler_);
   }
}

/** Dtor */
wxSimpleHtmlTreeCtrl::~wxSimpleHtmlTreeCtrl() {

   Disconnect(wxEVT_COMMAND_HTML_LINK_CLICKED,
              wxHtmlLinkEventHandler(wxSimpleHtmlTreeCtrlEvent::OnLink), NULL,
              pEventHandler_);

   Disconnect(wxEVT_SIZE, wxSizeEventHandler(wxSimpleHtmlTreeCtrlEvent::OnResize), NULL,
              pEventHandler_);

   Disconnect(wxEVT_UPDATE_UI,
              wxUpdateUIEventHandler(wxSimpleHtmlTreeCtrlEvent::OnUIUpdate), NULL,
              pEventHandler_);

   Disconnect(wxEVT_LEFT_DOWN,
              wxMouseEventHandler(wxSimpleHtmlTreeCtrlEvent::OnLeftDown), NULL,
              pEventHandler_);

   Disconnect(wxEVT_LEFT_DCLICK,
              wxMouseEventHandler(wxSimpleHtmlTreeCtrlEvent::OnDoubleClick), NULL,
              pEventHandler_);

   Disconnect(wxEVT_RIGHT_UP, wxMouseEventHandler(wxSimpleHtmlTreeCtrlEvent::OnRightUp),
              NULL, pEventHandler_);

   DELETE_EVENT_OBJECT;

   delete pTreeEventHandler_;
   delete root_;
}

/**
 * Retorna raiz del arbol
 * @return ItemId id del nodo raiz
 */
ItemId wxSimpleHtmlTreeCtrl::GetRoot() {
   return root_->GetNodeId();
}

/**
 * Busca el nodo con id 'NodeId' y retorna el id del primer hijo.
 * @param[in] NodeId id de nodo existente en arbol
 * @return ItemId id del primer hijo del nodo de entrada
 * @return INVALIDID si es nodo hoja o no encontro NodeId en arbol
 */
ItemId wxSimpleHtmlTreeCtrl::GetFirstChild(const ItemId &NodeId) {
   return root_->GetFirstChild(NodeId);
}

/**
 * Busca el nodo con id 'NodeId' y retorna el siguiente hijo del mismo padre.
 * @param[in] NodeId id de nodo existente en arbol
 * @return ItemId id del proximo nodo del mismo padre
 * @return INVALIDID si no tiene proximo hermano o no encontro !NodeId en arbol
 */
ItemId wxSimpleHtmlTreeCtrl::GetNextSibling(const ItemId &NodeId) {
   return root_->GetNextSibling(NodeId);
}

/**
 * Busca el nodo con id 'NodeId' y retorna el padre.
 * @param[in] NodeId id de nodo existente en arbol
 * @return ItemId id del padre
 * @return INVALIDID si NodeId es root.
 */
ItemId wxSimpleHtmlTreeCtrl::GetParentNode(const ItemId &NodeId) {
   return root_->GetParent(NodeId);
}

/**
 * Retorna HtmlConfigurationData de nodo con identificador NodeId.
 * @param[in] Id id de nodo
 * @return HtmlConfigurationData* del nodo si encuentra Id
 * @return NULL si no encuentra NodeId
 */
HtmlConfigurationData* wxSimpleHtmlTreeCtrl::GetConfigurationData(const ItemId &Id) {
   if (Id.Compare(INVALIDTREEID) == 0) return NULL;
   return root_->GetHtmlConfigurationData(Id);
}

/**
 * Busca el nodo con id 'NodeId' y agrega Node al mismo nivel, despues del
 * nodo encontrado.
 * @param[in] NodeId id de nodo existente en arbol
 * @param[in] pHtmlNode pHtmlNode nodo que quiero agregar al arbol
 * \attention pHtmlNode pasa a ser responsabilidad de wxSimpleHtmlTreeCtrl. No
 * eliminar fuera de la clase
 * @return ItemId id del nodo que se agrego
 * @return INVALIDTREEID si no encontro NodeId en el arbol.
 */
ItemId wxSimpleHtmlTreeCtrl::AddNodeAfter(const ItemId &NodeId,
                                          HtmlConfigurationData* pHtmlNode) {
   TreeNode* ptreenode = CreateTreeNode(pHtmlNode);
   ItemId newnodeid = root_->AddNodeAfter(NodeId, ptreenode);
   if (newnodeid.Compare(INVALIDTREEID) == 0) {
      delete ptreenode;
   }

   dirty_ = true;
   return newnodeid;
}

/**
 * Busca el nodo con id 'NodeId' y agrega 'Node' al mismo nivel, antes del
 * nodo encontrado.
 * @param[in] NodeId id de nodo existente en arbol
 * @param[in] HtmlTreeNode pHtmlNode nodo que quiero agregar al arbol
 * \attention pHtmlNode pasa a ser responsabilidad de wxSimpleHtmlTreeCtrl. No
 * eliminar fuera de la clase
 * @return ItemId id del nodo que se agrego
 * @return INVALIDTREEID si no encontro NodeId en el arbol.
 */
ItemId wxSimpleHtmlTreeCtrl::AddNodeBefore(const ItemId &NodeId,
                                           HtmlConfigurationData* pHtmlNode) {
   TreeNode* ptreenode = CreateTreeNode(pHtmlNode);
   ItemId newnodeid = root_->AddNodeBefore(NodeId, ptreenode);
   if (newnodeid.Compare(INVALIDTREEID) == 0) {
      delete ptreenode;
   }

   dirty_ = true;
   return newnodeid;
}

/**
 * Busca el nodo con id 'NodeId'. Agrega 'pNode' como ultimo nodo debajo
 * de nodo encontrado.
 * @param[in] NodeId id de nodo interno existente en arbol
 * @param[in] pHtmlNode pHtmlNode nodo que quiero agregar al arbol
 * \attention pHtmlNode pasa a ser responsabilidad de wxSimpleHtmlTreeCtrl. No
 * eliminar fuera de la clase.
 * @return ItemId id del nodo que se agrego
 * @return INVALIDTREEID si no encontro NodeId en el arbol.
 */
ItemId wxSimpleHtmlTreeCtrl::AppendNode(const ItemId &NodeId,
                                        HtmlConfigurationData* pHtmlNode) {
   TreeNode* ptreenode = CreateTreeNode(pHtmlNode);
   ItemId newnodeid = root_->AppendNode(NodeId, ptreenode);
   if (newnodeid.Compare(INVALIDTREEID) == 0) {
      delete ptreenode;
   }

   dirty_ = true;
   return newnodeid;
}

/**
 * Busca el nodo con id 'NodeId' y lo saca del arbol. Si es un nodo
 * interno, solo se puede remover si esta vacio.
 * @param[in] NodeId id de nodo existente en arbol
 * @return HtmlTreeNode nodo que se saco del arbol
 * \attention el nodo retornado deja de ser responsabilidad de wxSimpleHtmlTreeCtrl.
 * Eliminar puntero en codigo cliente. La instancia es la misma que se
 * agrego con AddNodeBefore, AddNodeAfter o AppendNode.
 * @return NULL si no encontro NodeId en el arbol.
 */
HtmlConfigurationData* wxSimpleHtmlTreeCtrl::RemoveNode(const ItemId &NodeId) {
   TreeNode* ptreenode = root_->RemoveNode(NodeId);
   HtmlConfigurationData* phtmlconfiguration = ptreenode->GetHtmlConfigurationData();
   delete ptreenode;

   dirty_ = true;
   return phtmlconfiguration;
}

/**
 * Busca el nodo con id 'NodeId', si es nodo interno lo expande/contrae
 * deacuerdo a contenido de 'Expand'.
 * @param[in] NodeId id de nodo interno existente en arbol
 * @param[in] Expand indica si el nodo deberia estar expandido o no
 */
void wxSimpleHtmlTreeCtrl::Expand(const ItemId &NodeId, bool Expand) {
   root_->Expand(NodeId, Expand);
   dirty_ = true;
}

/**
 * Busca el nodo con id 'NodeId', informa si esta expandido
 * @param[in] NodeId id de nodo interno existente en arbol
 * @return bool que indica si el nodo esta expandido
 */
bool wxSimpleHtmlTreeCtrl::IsExpanded(const ItemId &NodeId) {
   return root_->IsExpanded(NodeId);
}

/**
 * Informa si el nodo es visible en la lista o esta oculto dentro de
 * un nodo colapsado.
 * @param[in] NodeId id de nodo existente en arbol
 */
bool wxSimpleHtmlTreeCtrl::IsVisible(const ItemId &NodeId) {
   return root_->IsVisible(NodeId);
}

/**
 * Valida si un ItemId sigue siendo valido en la estructura del arbol.
 * @param[in] NodeId id del nodo que quiero validar
 * @return bool que indica si encontro el nodo en el arbol.
 */
bool wxSimpleHtmlTreeCtrl::ValidateItemId(const ItemId &NodeId) {
   return (root_->GetHtmlConfigurationData(NodeId) != NULL);
}

/**
 * Clase que se usa para definir comportamiento ante eventos de usuario
 * @param[in] pTreeEventHandler puntero a wxHtmlTreeEventHandler. No puede ser NULL
 * \attention pTreeEventHandler pasa a ser resposabilidad de esta clase
 * No eliminar fuera de la misma.
 */
void wxSimpleHtmlTreeCtrl::SetTreeEventHandler(
      wxHtmlTreeEventHandler* pTreeEventHandler) {
   if (pTreeEventHandler == NULL) return;
   delete pTreeEventHandler_;
   pTreeEventHandler_ = pTreeEventHandler;
}

/**
 * Incrementa en 1 el contador que bloquea la actualizacion del arbol
 */
void wxSimpleHtmlTreeCtrl::BeginBatch() {
   batchCount_++;
}

/**
 * Decrementa en 1 el contador que bloquea la actualizacion del arbol
 */
void wxSimpleHtmlTreeCtrl::EndBatch() {
   if (batchCount_ > 0) batchCount_--;
   dirty_ = true;
}

/**
 * Crea TreeNode configurado con pHtmlNode
 * @param[in] pHtmlNode Datos de visualizacion del TreeNode que quiro crear.
 * \attention el nodo creado guarda referencia a pHtmlNode que recibe por
 * parametro. No eliminar antes de elimianar instancia pHtmlNode antes de
 * eliminar el nodo generado.
 * @return TreeNode* puntero a nodo creado
 * \attention es responsabilidad del invocante eliminar el nodo creado
 */
TreeNode* wxSimpleHtmlTreeCtrl::CreateTreeNode(HtmlConfigurationData* pHtmlNode) {
   if (!pHtmlNode) {
      REPORT_DEBUG("Error al crear el nodo");
      return NULL;
   }

   long nodeflags = SimpleNode;
   if ((behaviorFlags_ & wxCheckableItems) != 0) {
      nodeflags = CheckableNode;
   }

   TreeNode* pnewnode = NULL;
   if (pHtmlNode->IsLeaf()) {
      pnewnode = new TreeLeaf(pHtmlNode, nodeflags);
   } else {
      pnewnode = new TreeInnerNode(pHtmlNode, false, nodeflags);
   }
   return pnewnode;
}

/**
 * Actualiza wxDecoratedHtmlListCtrl con el nuevo contenido del arbol.
 */
void wxSimpleHtmlTreeCtrl::UpdateDecoratedHtmlList() {
   if (batchCount_ > 0) return;

   //Guardo estado antes del update
   std::list<ItemId> selecteditems = GetSelectedItemsIds();
   int visiblebegin = GetVisibleBegin();

   //Si wxNoAutoState no esta seteado
   //actualizo el estado de los nodos internos
   if ((behaviorFlags_ & wxNoAutoState) == 0) {
      root_->UpdateStateUsingLeafs();
   }

   // Evita que se actualice la pantalla mientras se actualiza la lista
   Freeze();

   // Actualizo el html de los elementos
   int position = 0;
   int windowsize = std::max(GetClientSize().GetWidth(), 100);
   root_->UpdateHtmlVectorWithVisibleItems(windowsize, position, htmlItemList_);
   if (position < (int) htmlItemList_.size()) {
      htmlItemList_.erase(htmlItemList_.begin() + position, htmlItemList_.end());
   }
   SetItemCount(htmlItemList_.size());

   // Si se agregaron scrollbars recalculo html
   int newwindowsize = std::max(GetClientSize().GetWidth(), 100);
   position = 0;
   if (windowsize != newwindowsize)
      root_->UpdateHtmlVectorWithVisibleItems(newwindowsize, position, htmlItemList_);
   SetItemCount(htmlItemList_.size());

   // Trato de mantener la posicion en la lista.
   ScrollToLine(visiblebegin);

   // Trato de mantener los elementos seleccionados
   SetSelection(-1);
   std::list<ItemId>::iterator it = selecteditems.begin();
   while (it != selecteditems.end()) {
      SelectItem(*it);
      it++;
   }

   Thaw();
}

/**
 * Se ejecuta cuando se invierte estado de check de un nodo desde la interfaz
 * @param[in] Id id del nodo
 */
void wxSimpleHtmlTreeCtrl::InvertItemCheckFromGui(ItemId Id) {
   if ((behaviorFlags_ & wxCheckableItems) == 0) {
      return;
   }

   HtmlConfigurationData* pconfdata = root_->GetHtmlConfigurationData(Id);

   if (pconfdata->GetExecuteOnState() != NULL) {
      pconfdata->GetExecuteOnState()->Execute();
   } else {
      // Modifico estado de control
      HtmlConfigurationData::State newstate = HtmlConfigurationData::ON;
      if (pconfdata->GetState() == HtmlConfigurationData::ON) {
         newstate = HtmlConfigurationData::OFF;
      }

      pTreeEventHandler_->DoOnStateChanged(Id, newstate);
   }

   dirty_ = true;
}

/**
 * Se ejecuta cuando se desea mover un nodo(Id) a continueacion de
 * otro(DestinationId)
 * @param[in] Id id del nodo que quiero mover
 * @param[in] DestinationId id del destino
 * @return bool que indica si se puede mover el nodo
 */
bool wxSimpleHtmlTreeCtrl::DoOnMoveAfter(const ItemId &Id,
                                         const ItemId &DestinationId) {
   // Valido que se pueda realizar el movimiento
   TreeNodeId nodeid(Id);
   HtmlConfigurationData* pdestinationdata = root_->GetHtmlConfigurationData(
         DestinationId);
   ItemId newparent =
         (pdestinationdata->IsLeaf()) ? root_->GetParent(DestinationId) : DestinationId;
   if ((nodeid.Compare(DestinationId) >= 0)
         || !pTreeEventHandler_->ValidateNodeNewParent(Id, newparent)) {
      return false;
   }

   // Agrego el nodo en la posicion deseada
   ItemId newid = INVALIDTREEID;

   if (pdestinationdata->IsLeaf())
      pTreeEventHandler_->DoOnMoveNodeAfter(Id, DestinationId);
   else {
      ItemId firstchildid = GetFirstChild(DestinationId);
      if (root_->IsExpanded(DestinationId)
            && firstchildid.Compare(INVALIDTREEID) != 0) {
         // Si el nodo ya es el primer nodo del padre retorno true.
         if (firstchildid.Compare(Id) == 0) return true;

         pTreeEventHandler_->DoOnMoveNodeBefore(Id, firstchildid);
      } else
         pTreeEventHandler_->DoOnMoveNodeUnder(Id, DestinationId);
   }
   return true;
}

/**
 * Se ejecuta cuando se desea mover un nodo(Id) a antes de
 * otro(DestinationId)
 * @param[in] Id id del nodo que quiero mover
 * @param[in] DestinationId id del nodo destino
 * @return bool que indica si se pudo mover el nodo
 */
bool wxSimpleHtmlTreeCtrl::DoOnMoveBefore(const ItemId &Id,
                                          const ItemId &DestinationId) {
   TreeNodeId nodeid(Id);
   if ((nodeid.Compare(DestinationId) >= 0)
         || !pTreeEventHandler_->ValidateNodeNewParent(Id,
                                                       root_->GetParent(DestinationId))) {
      return false;
   }
   pTreeEventHandler_->DoOnMoveNodeBefore(Id, DestinationId);
   return true;
}

/**
 * Se ejecuta cuando se mueve un item despues del ultimo elemento
 * @param[in] Id id del nodo que quiero mover
 * @return bool que indica si se pudo mover el nodo
 */
bool wxSimpleHtmlTreeCtrl::DoOnMoveEnd(const ItemId &Id) {
   if (!pTreeEventHandler_->ValidateNodeNewParent(Id, root_->GetNodeId())) return false;

   pTreeEventHandler_->DoOnMoveNodeUnder(Id, root_->GetNodeId());
   return true;
}

/**
 * Se ejecuta cuando se modifica la seleccion
 */
void wxSimpleHtmlTreeCtrl::DoOnSelectionChanged() {
   pTreeEventHandler_->DoOnSelectionChanged();
}

/**
 * Se ejecuta cuando se hace click sobre link de un nodo
 * @param[in] Event evento tipo wxHtmlLinkEvent
 */
void wxSimpleHtmlTreeCtrl::OnLink(wxHtmlLinkEvent &Event) {
   wxHtmlLinkInfo stlinkinfo = Event.GetLinkInfo();

   wxMouseEvent eventmouse = *(stlinkinfo.GetEvent());

   // Elimino seleccion. Al contraer/expandir la seleccion se vuelve invalida
   int item = HitTest(eventmouse.m_x, eventmouse.m_y);
   SetSelection(item);
   // Termino drag porque en ocasiones se mueve elemento al colapsar innernode
   EndDrag();

   ItemId id = htmlItemList_[item].GetItemId();

   if (stlinkinfo.GetHref() == "expand") {  // No se traduce, elemento en define
      Expand(id, !root_->IsExpanded(id));
   }

   if (stlinkinfo.GetHref() == "checkbox") {  // No se traduce, elemento en define
      InvertItemCheckFromGui(id);
   }
   dirty_ = true;
   validDoubleClick_ = false;
   Event.Skip();
}

/**
 * Se ejecuta con evento de click izq.(sin soltar el boton).
 * Setea validDoubleClick_=true.
 * @param[in] Event evento tipo wxMouseEvent
 */
void wxSimpleHtmlTreeCtrl::OnLeftDown(wxMouseEvent &Event) {
   // OnLeftDown siempre se ejecuta antes que OnLink, por esta razon es
   // suficiente usar un bool. Caso contrario tambien tendria que contemplar
   // que OnLink se ejecute antes que OnLeftDown.
   validDoubleClick_ = true;
   Event.Skip();
}

/**
 * Se ejecuta cuando se hace doble click sobre item en lista.
 * Invierte estado de activacion del item.
 * @param[in] Event evento tipo wxMouseEvent
 */
void wxSimpleHtmlTreeCtrl::OnDoubleClick(wxMouseEvent &Event) {
   if (!validDoubleClick_) return;

   // Elimino seleccion. Al contraer/expandir la seleccion se vuelve invalida
   SetSelection(-1);
   // Termino drag porque en ocasiones se mueve elemento al colapsar innernode
   EndDrag();

   int item = HitTest(Event.m_x, Event.m_y);
   if (item != wxNOT_FOUND) {
      ItemId id = htmlItemList_[item].GetItemId();
      InvertItemCheckFromGui(id);
   }
   Event.Skip();
}

/**
 * Este metodo se ejecuta cuando la lista recibe los eventos de resize para
 * poder recalcular la longitud de los textos.
 * @param[in]	Event: evento
 */
void wxSimpleHtmlTreeCtrl::OnResize(wxSizeEvent &Event) {
   dirty_ = true;
   Event.Skip();
}

/**
 * Se ejecuta cuando se suelta click derecho sobre lista.
 * Despliega menu contextual sobre el elemento.
 * @param[in] Event evento.
 */
void wxSimpleHtmlTreeCtrl::OnRightUp(wxMouseEvent &Event) {
   int item = HitTest(Event.m_x, Event.m_y);
   ItemId id = (item < 0) ? INVALIDTREEID : htmlItemList_[item].GetItemId();

   SetSelection(-1);
   if (id.Compare(INVALIDTREEID) != 0) SelectItem(id);
   pTreeEventHandler_->DoOnSelectionChanged();

   wxMenu *pmenu = pTreeEventHandler_->CreateContextMenu(id);
   if (pmenu) {
      pmenu->Connect(wxEVT_COMMAND_MENU_SELECTED, wxMenuEventHandler(
            wxSimpleHtmlTreeCtrlEvent::OnContextMenu),
                     NULL, pEventHandler_);
      PopupMenu(pmenu, Event.m_x, Event.m_y);
      delete pmenu;
   }
   Event.Skip();
}

/**
 * Se llama cuando se selecciona item del menu contextual
 * @param[in] Event evento del menu
 */
void wxSimpleHtmlTreeCtrl::OnContextMenu(wxMenuEvent &Event) {
   pTreeEventHandler_->DoOnContextMenu(Event);
   Event.Skip();
}

/**
 * Este metodo responde al evento UIUpdate que se ejecuta cuando no hay eventos
 * de usuario en la cola de eventos de wx.
 * Si dirty_==true recalcula contenido de la lista
 * @param[in]  Event: evento.
 */
void wxSimpleHtmlTreeCtrl::OnUIUpdate(wxUpdateUIEvent &Event) {
   // Los metodos que modifican la lista setean flag dirty_=true, luego en OnUIUpdate
   // se recalcula el contenido de la lista.
   // Antes los metodos llamaban a UpdateDecoratedHtmlList directamente.
   // Justificacion del cambio:
   // Si al colapsar nodo, se desplaza el scroll de la lista, se modificaba
   // el elemento seleccionado.
   // El orden de los eventos era:
   //  - wxSimpleHtmlTreeCtrl::OnLeftDown
   //  - wxSimpleHtmlTreeCtrl::OnLink(Modifica items en lista y modifica scroll)
   //  - wxHtmlListBox::OnLeftDown(Selecciona elemento usando posicion mouse)
   // OnLink recibe evento de mouse const, por lo que no podia evitar
   // que se seleccione el elemento.
   // La solucion fue agregar dirty_ para que en OnLink no se modifique la lista, luego
   // wxHtmlListBox::OnLeftDown selecciona elemento correcto.
   // Si dirty_==true el evento OnUIUpdate recalcula el contenido de la lista
   if (dirty_ == true) {
      UpdateDecoratedHtmlList();
      dirty_ = false;
   }
   Event.Skip();
}

}  // namespace

