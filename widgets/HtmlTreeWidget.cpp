/** \file HtmlTreeWidget.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <algorithm>
#include <list>
#include <string>
#include <vector>

#include "ContextGroupNodeFilter.h"
#include "HtmlTreeEventHandler.h"
#include "ItemId.h"
#include "TreeNodeAuxiliaryFunctions.h"
#include "suri/Configuration.h"
#include "suri/GuiTreeModel.h"
#include "suri/HtmlTreeNodeHandlerInterface.h"
#include "suri/HtmlTreeWidget.h"
#include "suri/LayerTreeModel.h"

#include "wxDecoratedHtmlListCtrl.h"
#include "wxmacros.h"
#include "wx/xrc/xmlres.h"

#define SUBLEVEL \
"<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">" \
"      <tr>" \
"         <td>" \
"            <table width=\"16px\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\"></table>" \
"         </td>" \
"         <td width=\"100%\">__SUBNODE__" \
"         </td>" \
"      </tr>" \
"</table>"

#define SUBNODE_FLAG    "__SUBNODE__"

#define ROOTNODECOUNT      1
#define LEAFNODECOUNT      1
#define LEVELOFFSET        16

#define LEVEL_SOPI_ADJUSTMENT    24
#define LEVEL_SIMUR_ADJUSTMENT   32

namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(HtmlTreeWidgetEvent, HtmlTreeWidget)
   IMPLEMENT_EVENT_CALLBACK(OnLink, OnLink(event), wxHtmlLinkEvent)
   IMPLEMENT_EVENT_CALLBACK(OnResize, OnResize(event), wxSizeEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRightUp, OnRightUp(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLeftDown, OnLeftDown(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnDoubleClick, OnDoubleClick(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnUiUpdate, OnUiUpdate(event), wxUpdateUIEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Ctor */
HtmlTreeWidget::HtmlTreeWidget(wxWindow* pParent, GuiTreeModel* pGuiTreeModel,
                        HtmlTreeNodeHandlerInterface* pHtmlTreeNodeHandler,
                        Subject* pChangesNotifier,
                        long ConfigurationFlags) :
                        suri::Widget(pParent),
                        NEW_EVENT_OBJECT(HtmlTreeWidgetEvent), viewcontextId_(
				SuriObject::NullUuid) {
   pHtmlTreeNodeHandler_ = pHtmlTreeNodeHandler;
   pGuiTreeModel_ = pGuiTreeModel;
   pChangesNotifier_ = pChangesNotifier;
   if (pChangesNotifier_)
      observerId_ = pChangesNotifier_->Attach(this);
   configurationFlags_ = ConfigurationFlags;
   pWxTree_ = NULL;
   reloadHtml_ = false;
}

/** Dtor */
HtmlTreeWidget::~HtmlTreeWidget() {
   if (pChangesNotifier_)
      pChangesNotifier_->Detach(observerId_);
   delete pGuiTreeModel_;
   delete pHtmlTreeNodeHandler_;
   DELETE_EVENT_OBJECT;
}

/** Se crea la ventana de herramienta en las clases derivadas */
bool HtmlTreeWidget::CreateToolWindow() {
   int htmllistflag = (configurationFlags_ & DraggableItems) != 0 ?
                                              wxDraggableItems : wxNoBehaviour;
   int wxcontrolflag = (configurationFlags_ & MultipleSelection) != 0 ? wxLB_MULTIPLE : 0;
   pWxTree_ = new wxDecoratedHtmlListCtrl(pParentWindow_,
                                          wxcontrolflag, htmllistflag);
   pWxTree_->SetEventHandler(new HtmlTreeEventHandler(pGuiTreeModel_));
   pToolWindow_ = pWxTree_;

   pToolWindow_->Connect(wxEVT_COMMAND_HTML_LINK_CLICKED,
           wxHtmlLinkEventHandler(HtmlTreeWidgetEvent::OnLink), NULL,
           pEventHandler_);

   pToolWindow_->Connect(wxEVT_SIZE, wxSizeEventHandler(HtmlTreeWidgetEvent::OnResize), NULL,
           pEventHandler_);

   pToolWindow_->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(HtmlTreeWidgetEvent::OnLeftDown),
           NULL, pEventHandler_);

   pToolWindow_->Connect(wxEVT_LEFT_DCLICK,
           wxMouseEventHandler(HtmlTreeWidgetEvent::OnDoubleClick), NULL,
           pEventHandler_);

   pToolWindow_->Connect(wxEVT_RIGHT_UP,
           wxMouseEventHandler(HtmlTreeWidgetEvent::OnRightUp),
           NULL, pEventHandler_);

   pToolWindow_->Connect(wxEVT_UPDATE_UI,
           wxUpdateUIEventHandler(HtmlTreeWidgetEvent::OnUiUpdate),
           NULL, pEventHandler_);

   return true;
}

/** Actualiza control grafico con datos en modelo */
void HtmlTreeWidget::UpgradeControlContent() {
   reloadHtml_ = true;
}

/** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
void HtmlTreeWidget::Update(NotificationInterface* pNotification) {
   pNotification->ApplyNotification(this);
}

/**
 * Retorna el html de un nodo segun sea su tipo
 * @param[in] Path path del nodo
 * @param[in] WindowWidth Cantidad de pixeles de ancho disponibles
 * @return html del nodo que corresponde a Path.
 */
std::string HtmlTreeWidget::GetNodeHtml(const NodePath &Path, int WindowWidth) {
   std::string nodehtml = "";
   // Calcula y centa el nombre de los recursos mostrados en la lista.
   int width = WindowWidth; // CalculateNodeAvailableWith(Path, WindowWidth); Comentado ticket 4535.

   if (pGuiTreeModel_->IsVisible(Path)) {
      if (Path.GetPathCount() - ROOTNODECOUNT - LEAFNODECOUNT > 0) {
         bool simur = !(Configuration::GetParameter("app_short_name", "") == wxT("SoPI"));
         int adjustment = simur ? LEVEL_SIMUR_ADJUSTMENT : LEVEL_SOPI_ADJUSTMENT;
         width -= (Path.GetPathCount() - ROOTNODECOUNT - LEAFNODECOUNT) * adjustment;
      }
      nodehtml = pHtmlTreeNodeHandler_->GetHtml(Path, width);
   }

   return !nodehtml.empty() ? AddIdentantion(Path, nodehtml) : "";
}

/**
 * Encapsula html que recibe por parametro para que parezca dentro de un
 * arbol
 * @param[in] Node nodo
 * @param[in] ItemHtml html correspondiente a nodo sin identacion
 * @return html con ItemHtml encapsulado
 */
std::string HtmlTreeWidget::AddIdentantion(const NodePath &Node,
                                             const std::string &ItemHtml) {
   // Agrego decoracion a los nodos cargados
   std::string decoratedhtml = ItemHtml;
   for (int index = 0; index < Node.GetPathCount() - ROOTNODECOUNT - LEAFNODECOUNT; index++) {
      std::string temphtml = decoratedhtml;
      decoratedhtml = SUBLEVEL;
      size_t position = decoratedhtml.find(SUBNODE_FLAG);
      decoratedhtml.replace(position, sizeof(SUBNODE_FLAG), temphtml);
   }
   return decoratedhtml;
}


/**
 * Calcula el ancho que ocupa la identacion
 * @param[in] Node nodo
 * @param[in] WindowWidth Cantidad de pixeles de ancho disponibles
 * @return ancho de la indentacion
 */
int HtmlTreeWidget::CalculateNodeAvailableWith(const NodePath &Node, int WindowWidth) {
   // Agrego decoracion a los nodos cargados
   int identationwidth = LEVELOFFSET * (Node.GetPathCount() - ROOTNODECOUNT - LEAFNODECOUNT);
   int width = std::max(WindowWidth - identationwidth , 0);
   return width;
}

/** 
 * Actualiza vector html con los elementos visibles 
 * @param[in] pWorkGroup Grupo raiz del arbol que quiero convertir a html.
 * @param[in] WindowWidth Cantidad de pixeles de ancho disponibles
 * @param[out] HtmlVector Vector que quiero actualizar
 */
void HtmlTreeWidget::UpdateHtmlVectorWithGroupNodes(int WindowWidth,
                                 std::vector<DecoratedHtmlItem>& HtmlVector) {
   HtmlVector.clear();
   TreeIterator it = pGuiTreeModel_->GetIterator();
   if (it.GetPath().GetPathCount() == 1) {
      // Ignoro la raiz del arbol
      it.NextNode();
   }
   for (; it.IsValid(); it.NextNode()) {
      NodePath nodepath = it.GetPath();
      if (pGuiTreeModel_->IsVisible(nodepath)) {
         ItemId nodeid(*nodepath.GetLastPathNode());
         std::string indentedhtml = GetNodeHtml(nodepath, WindowWidth);
         if (!indentedhtml.empty()) {
            DecoratedHtmlItem decoratedhtml(nodeid, indentedhtml);
            HtmlVector.push_back(decoratedhtml);
         }
      }
   }
   pWxTree_->SetItemCount(HtmlVector.size());
}


/**
 * Se ejecuta cuando se hace click sobre link de un nodo
 * @param[in] Event evento tipo wxHtmlLinkEvent
 */
void HtmlTreeWidget::OnLink(wxHtmlLinkEvent &Event) {
   wxHtmlLinkInfo stlinkinfo = Event.GetLinkInfo();

   wxMouseEvent eventmouse = *(stlinkinfo.GetEvent());

   // Elimino seleccion. Al contraer/expandir la seleccion se vuelve invalida
   int item = pWxTree_->HitTest(eventmouse.m_x, eventmouse.m_y);
   pWxTree_->SetSelection(item);
   // Termino drag porque en ocasiones se mueve elemento al colapsar innernode
   pWxTree_->EndDrag();

   NodePath path(NULL, NULL);
   if (!GetListItemNodePath(item, path))
      return;

   if (!path.AllowsChildren()) {
      pHtmlTreeNodeHandler_->ExecuteHtmlLink(path, stlinkinfo.GetHref().c_str());
   } else {
      // Eventos de grupo
      if (stlinkinfo.GetHref() == "expand") {  // No se traduce, elemento en define
         bool expand = !pGuiTreeModel_->IsExpanded(path);
         pGuiTreeModel_->ExpandGroup(path, expand);
         UpgradeControlContent();
      }
   }

   validDoubleClick_ = false;
   UpgradeControlContent();
   Event.Skip();
}

/**
 * Se ejecuta con evento de click izq.(sin soltar el boton).
 * Setea validDoubleClick_=true.
 * @param[in] Event evento tipo wxMouseEvent
 */
void HtmlTreeWidget::OnLeftDown(wxMouseEvent &Event) {
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
void HtmlTreeWidget::OnDoubleClick(wxMouseEvent &Event) {
   if (!validDoubleClick_) return;

   // Elimino seleccion. Al contraer/expandir la seleccion se vuelve invalida
   pWxTree_->SetSelection(-1);
   // Termino drag porque en ocasiones se mueve elemento al colapsar innernode
   pWxTree_->EndDrag();

   NodePath path(NULL, NULL);
   int item = pWxTree_->HitTest(Event.m_x, Event.m_y);
   if (!GetListItemNodePath(item, path))
      return;

   if (!path.AllowsChildren()) {
      pHtmlTreeNodeHandler_->ExecuteDoubleClick(path);
      UpgradeControlContent();
   }

   Event.Skip();
}

/**
 * Este metodo se ejecuta cuando la lista recibe los eventos de resize para
 * poder recalcular la longitud de los textos.
 * @param[in]  Event: evento
 */
void HtmlTreeWidget::OnResize(wxSizeEvent &Event) {
   UpgradeControlContent();
   Event.Skip();
}

/**
 * Se ejecuta cuando se suelta click derecho sobre lista.
 * Despliega menu contextual sobre el elemento.
 * @param[in] Event evento.
 */
void HtmlTreeWidget::OnRightUp(wxMouseEvent &Event) {
   NodePath path(NULL, NULL);
   int item = pWxTree_->HitTest(Event.m_x, Event.m_y);
   if (!GetListItemNodePath(item, path))
      return;

   pHtmlTreeNodeHandler_->CreateContextMenu(path, this, Event.m_x, Event.m_y);

   Event.Skip();
}

/** Evento para dibujar la posicion del elemento en el Drag&Drop */
void HtmlTreeWidget::OnUiUpdate(wxUpdateUIEvent &Event) {
   if (!reloadHtml_)
      return;
   reloadHtml_ = false;

   // Evita que se actualice la pantalla mientras se actualiza la lista
   pWxTree_->Freeze();
   int visiblebegin = pWxTree_->GetVisibleBegin();
   std::list<NodePath> selecteditems =
                  pGuiTreeModel_->GetSelectedNodes();

   std::vector<DecoratedHtmlItem>& htmlvector = pWxTree_->GetHtmlItemList();
   htmlvector.clear();

   /** Renderiza vectores. */

   unsigned int a = pWxTree_->GetClientSize().GetWidth();

   // Ajusto el minimo
   SetMaxItemSize(a);
   // Ajusto el maximko
   SetMinItemSize(0);

   int windowsize = GetManualWindowsSize();
   UpdateHtmlVectorWithGroupNodes(windowsize, htmlvector);
   int newwindowsize = GetManualWindowsSize();
   if (windowsize != newwindowsize)
      // En caso que se agregue barra vertical.
      UpdateHtmlVectorWithGroupNodes(newwindowsize, htmlvector);

   // Trato de mantener los elementos seleccionados
   pWxTree_->SetSelection(-1);
   std::list<NodePath>::iterator it = selecteditems.begin();
   for (; it != selecteditems.end(); ++it) {
      ItemId id(*(it->GetLastPathNode()));
      pWxTree_->SelectItem(id, true);
   }

   pWxTree_->Thaw();


   pWxTree_->ScrollToLine(visiblebegin);
}

/** Fuerza la actualizacion del widget **/
void HtmlTreeWidget::ForceRefresh() {
   if (!reloadHtml_)
       return;
    // Evita que se actualice la pantalla mientras se actualiza la lista
    pWxTree_->Freeze();
    int visiblebegin = pWxTree_->GetVisibleBegin();
    std::list<NodePath> selecteditems =
                   pGuiTreeModel_->GetSelectedNodes();

    std::vector<DecoratedHtmlItem>& htmlvector = pWxTree_->GetHtmlItemList();
    htmlvector.clear();

    /** Renderiza vectores. */
    int windowsize = GetManualWindowsSize();
    UpdateHtmlVectorWithGroupNodes(windowsize, htmlvector);
    int newwindowsize = GetManualWindowsSize();
    if (windowsize != newwindowsize)
       // En caso que se agregue barra vertical.
       UpdateHtmlVectorWithGroupNodes(newwindowsize, htmlvector);

    // Trato de mantener los elementos seleccionados
    pWxTree_->SetSelection(-1);
    std::list<NodePath>::iterator it = selecteditems.begin();
    for (; it != selecteditems.end(); ++it) {
       ItemId id(*(it->GetLastPathNode()));
       pWxTree_->SelectItem(id, true);
    }

    pWxTree_->Thaw();

    pWxTree_->ScrollToLine(visiblebegin);
    pParentWindow_->Refresh();
}

/**
 * Retorna el path al nodo asociado al elemento en 'Position' de la lista
 * @param[in] Position posicion del elemento en la lista
 * @param[out] Path path del nodo que corresponde al elemento
 * @return bool que indica que lo encontro
 */
bool HtmlTreeWidget::GetListItemNodePath(int Position, NodePath &Path) {
   if (Position < 0 || Position >=
                  static_cast<int>(pWxTree_->GetHtmlItemList().size()))
      return false;
   ItemId id = pWxTree_->GetHtmlItemList()[Position].GetItemId();
   NodePath path = pGuiTreeModel_->SearchNode(id.GetId());
   if (path.IsValid())
      Path = path;
   return Path.IsValid();
}

void  HtmlTreeWidget::SetMinItemSize( int Min )
{
   if ( Min < 0 )
      minSize_ = 100;
   else
      minSize_ = Min;
}

void  HtmlTreeWidget::SetMaxItemSize( int Max )
{
   if ( Max > 2000 )
      maxSize_ = 2000;
   else
      maxSize_ = Max;
}

int HtmlTreeWidget::GetManualWindowsSize() {
   return std::max(minSize_, maxSize_);
}

int HtmlTreeWidget::GetFitToWindowsSize() {
   return std::max(pWxTree_->GetClientSize().GetWidth(), 100);
}

void HtmlTreeWidget::SetViewcontextId(const SuriObject::UuidType& ViewcontextId) {
	viewcontextId_ = ViewcontextId;
	LayerTreeModel* ptreemodel = dynamic_cast<LayerTreeModel*>(pGuiTreeModel_);
	if (ptreemodel) {
		ContextGroupNodeFilter* pfilter = new ContextGroupNodeFilter(viewcontextId_);
		ptreemodel->SetFilter(pfilter);
	}
}
} /* namespace suri */
