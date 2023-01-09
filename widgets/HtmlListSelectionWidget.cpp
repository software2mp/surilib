/*! \file HtmlListSelectionWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <list>
#include <set>

// Includes Suri
#include "HtmlListSelectionWidget.h"
#include "wxDecoratedHtmlListCtrl.h"
#include "TreeNodeId.h"
#include "ElementHtmlConfigurationData.h"
#include "suri/Element.h"
#include "GuiHtmlEditor.h"
#include "resources.h"
#include "HtmlListItemStateChangeNotification.h"

// Includes Wx
#include "wx/htmllbox.h"

// Defines
// forwards

namespace suri {

namespace ui {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(HtmlItemSelectionWidgetEvent, HtmlListSelectionWidget)
   IMPLEMENT_EVENT_CALLBACK(OnLink, OnLink(event), wxHtmlLinkEvent)
   IMPLEMENT_EVENT_CALLBACK(OnDoubleClick, OnDoubleClick(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnSelectionChanged, OnSelectionChanged(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */


/** ctor
 *  @param[in] Items lista de items iniciales
 *  @param[in] Flags configuracion de layout de la lista
 **/
HtmlListSelectionWidget::HtmlListSelectionWidget(
      const std::list<HtmlListItemInterface*>& Items, long Flags) :
      pEventHandler_(new HtmlItemSelectionWidgetEvent(this)), pTreeCtrl_(NULL),
      items_(Items), layoutFlags_(Flags) {
}

/** Constructor sin items iniciales
 *  @param[in] Flags configuracion de layout de la lista
 **/
HtmlListSelectionWidget::HtmlListSelectionWidget(long Flags) :
      pEventHandler_(new HtmlItemSelectionWidgetEvent(this)), pTreeCtrl_(NULL),
      layoutFlags_(Flags) {
}

/** dtor **/
HtmlListSelectionWidget::~HtmlListSelectionWidget()  {
   delete pEventHandler_;
   pEventHandler_ = NULL;
   for (std::list<HtmlListItemInterface*>::iterator it = items_.begin();
            it != items_.end(); ++it) {
      delete *it;
   }
   items_.clear();
}

/** Se crea la ventana de herramienta en las clases derivadas */
bool HtmlListSelectionWidget::CreateToolWindow() {
   pTreeCtrl_ = new wxSimpleHtmlListBox(pParentWindow_, -1);
   /** configura la lista para permitir multiple seleccion **/
   if (layoutFlags_ & MultipleSelection)
      pTreeCtrl_->SetWindowStyle(wxHLB_DEFAULT_STYLE | wxHLB_MULTIPLE);
   pToolWindow_ = pTreeCtrl_;
   pToolWindow_->Connect(wxEVT_COMMAND_HTML_LINK_CLICKED,
   wxHtmlLinkEventHandler(HtmlItemSelectionWidgetEvent::OnLink),
                         NULL, pEventHandler_);

   pToolWindow_->Connect(wxEVT_LEFT_DCLICK,
   wxMouseEventHandler(HtmlItemSelectionWidgetEvent::OnDoubleClick),
                         NULL, pEventHandler_);

   pToolWindow_->Connect(wxEVT_COMMAND_LISTBOX_SELECTED,
           wxCommandEventHandler(HtmlItemSelectionWidgetEvent::OnSelectionChanged),
           NULL, pEventHandler_);
   for (std::list<HtmlListItemInterface*>::iterator it = items_.begin();
         it != items_.end(); ++it) {
      long layoutflag =
            (layoutFlags_ & Checkeable) ? HtmlListItemStringConverterInterface::Checkeable :
                                          !HtmlListItemStringConverterInterface::Checkeable;
      pTreeCtrl_->Append(
            GetCompatibleEncodingItemString((*it)->GetItemAsString(layoutflag)));
   }
   return true;
}

/** Se ejecuta cuando se hace click sobre link de un nodo */
void HtmlListSelectionWidget::OnLink(wxHtmlLinkEvent &Event) {
   // Elimino seleccion. Al contraer/expandir la seleccion se vuelve invalida
   wxMouseEvent eventmouse = *(Event.GetLinkInfo().GetEvent());
   int item = pTreeCtrl_->HitTest(eventmouse.m_x, eventmouse.m_y);
   InvertItemState(item);
}

/** Se ejecuta cuando se hace doble click sobre un item de la lista */
void HtmlListSelectionWidget::OnDoubleClick(wxMouseEvent &Event) {
   // Elimino seleccion. Al contraer/expandir la seleccion se vuelve invalida
   int item = pTreeCtrl_->HitTest(Event.m_x, Event.m_y);
   InvertItemState(item);
}

/** Atrapa evento disparado cuando se modifica la seleccion */
void HtmlListSelectionWidget::OnSelectionChanged(wxCommandEvent &Event) {
}

/** Agrega un item al final de la lista
 *  @param[in] pItem item a insertar. Se guarda la referencia, no se hace una copia.
 */
void HtmlListSelectionWidget::AppendItem(HtmlListItemInterface* pItem) {
   if (pTreeCtrl_) {
      long layoutflag =
            (layoutFlags_ & Checkeable) ? HtmlListItemStringConverterInterface::Checkeable :
                                          !HtmlListItemStringConverterInterface::Checkeable;
      pTreeCtrl_->Append(GetCompatibleEncodingItemString(pItem->GetItemAsString(layoutflag)));
   }
   items_.push_back(pItem);
}

/** Inserta un item en la posocion deseada
 *  @param[in] pItem item a insertar. Se guarda la referencia, no se hace una copia.
 *  @param[in] Position posicion en donde se desea insertar el item
 **/
void HtmlListSelectionWidget::InsertItem(HtmlListItemInterface* pItem, int position) {
   if (pTreeCtrl_) {
      long layoutflag =
            (layoutFlags_ & Checkeable) ? HtmlListItemStringConverterInterface::Checkeable :
                                          !HtmlListItemStringConverterInterface::Checkeable;
      pTreeCtrl_->Insert(
            GetCompatibleEncodingItemString(pItem->GetItemAsString(layoutflag)),
            position);
   }
   std::list<HtmlListItemInterface*>::iterator it = items_.begin();
   std::advance(it, position);
   items_.insert(it, pItem);
}

/** Elimina el item que coincide con el id que se pasa por parametro. Se elimina
 * tambien la memoria reserva del item.
 *  @param[in] ItemId id del item a eliminar */
bool HtmlListSelectionWidget::DeleteItem(const SuriObject::UuidType& ItemId) {
   std::list<HtmlListItemInterface*>::iterator it = items_.begin();
   bool deleted = false;
   for (int i = 0; it != items_.end(); ++it, ++i) {
      if ((*it)->GetId().compare(ItemId) == 0) {
         HtmlListItemInterface* pitem = *it;
         items_.erase(it);
         delete pitem;
         deleted = true;
         if (pTreeCtrl_)
            pTreeCtrl_->Delete(i);
         break;
      }
   }
   /** Elimino el item de la seleccion **/
   if (deleted)
      selectedItems_.erase(ItemId);
   return deleted;
}

/** Devuelve los items seleccionados por el usuario del widget **/
std::set<SuriObject::UuidType> HtmlListSelectionWidget::GetSelectedItems() {
   if (!(layoutFlags_ & MultipleSelection) && !(layoutFlags_ & Checkeable)) {
      int itemselected = pTreeCtrl_->GetSelection();
      if (itemselected < 0 )
         return selectedItems_;
      std::list<HtmlListItemInterface*>::const_iterator it = items_.begin();
      std::advance(it, itemselected);
      if (selectedItems_.find((*it)->GetId()) == selectedItems_.end()) {
         selectedItems_.clear();
         selectedItems_.insert((*it)->GetId());
         InvertItemState(itemselected);
      }
   }
   return selectedItems_;
}
/** Selecciona todos los items de la lista **/
void HtmlListSelectionWidget::SelectAll() {
   pTreeCtrl_->Freeze();
   size_t firstline = pTreeCtrl_->GetVisibleBegin();
   selectedItems_.clear();
   std::list<HtmlListItemInterface*>::iterator it = items_.begin();
   for (size_t i = 0; i < items_.size() && it != items_.end(); ++it, ++i) {
      HtmlListItemInterface* pitem = *it;
      /** Para solo actualizar los que se encuentran activados **/
      if (!pitem->IsActive()) {
         pTreeCtrl_->Delete(i);
         pitem->Active();
         long layoutflag =
               (layoutFlags_ & Checkeable) ? HtmlListItemStringConverterInterface::Checkeable :
                                             !HtmlListItemStringConverterInterface::Checkeable;
         pTreeCtrl_->Insert(
               GetCompatibleEncodingItemString(pitem->GetItemAsString(layoutflag)), i);
      }
      selectedItems_.insert((*it)->GetId());
   }
   if (pTreeCtrl_->GetVisibleBegin() != firstline)
      pTreeCtrl_->ScrollToLine(firstline);
   pTreeCtrl_->Thaw();
}

/** Metodo que elimina la seleccion actual**/
void HtmlListSelectionWidget::ClearSelection() {
   pTreeCtrl_->Freeze();
   selectedItems_.clear();
   std::list<HtmlListItemInterface*>::iterator it = items_.begin();
   size_t firstline = pTreeCtrl_->GetVisibleBegin();
   for (size_t i = 0; i < items_.size() && it != items_.end(); ++it, ++i) {
      HtmlListItemInterface* pitem = *it;
      /** Para solo actualizar los que se encuentran activados **/
      if (pitem->IsActive()) {
         pTreeCtrl_->Delete(i);
         pitem->Deactive();
         long layoutflag =
               (layoutFlags_ & Checkeable) ? HtmlListItemStringConverterInterface::Checkeable :
                                             !HtmlListItemStringConverterInterface::Checkeable;
         pTreeCtrl_->Insert(
               GetCompatibleEncodingItemString(pitem->GetItemAsString(layoutflag)), i);
      }
   }
   if (pTreeCtrl_->GetVisibleBegin() != firstline)
      pTreeCtrl_->ScrollToLine(firstline);
   pTreeCtrl_->Thaw();
}

/** Metodo auxiliar que actualiza el estado de un item de la lista.
 *  Para esto es necesario frenar el refresco de la interfaz grafica (si no
 *  se genera un blinkeo molesto) y cambiar el estado del item. En caso de
 *  que sea checkeable es necesario actualizar el item para cambiar
 *  la imagen del check (es por esto que se elimina y se vuelve a inertar).
 *  En el caso de que no se soporte multiple seleccion antes de invertir
 *  el estado se debe limpiar la seleccion de la lista (seleccion excluyente).
 *  @param[in] Position posicion del item dentro de la lista html
 *  **/
void HtmlListSelectionWidget::InvertItemState(int Position) {
   if (Position < 0)
      return;
   pTreeCtrl_->Freeze();
   size_t firstline = pTreeCtrl_->GetVisibleBegin();
   std::list<HtmlListItemInterface*>::iterator it = items_.begin();
   std::advance(it, Position);
   HtmlListItemInterface* pitem = *it;
   /** Obtengo antes el item para poder saber el estado anterior */
   if (!(layoutFlags_ & MultipleSelection)) {
      bool isactive = pitem->IsActive();
      ClearSelection();
      isactive ? pitem->Deactive() : pitem->Active();
   } else {
      pitem->IsActive() ? pitem->Deactive() : pitem->Active();
   }
   /** En el caso de checkeable es necesario cambiar la imagen del check
    *  por eso es necesario reinsertar el elemento con el reemplazo mencionado
    */
   if (layoutFlags_ & Checkeable) {
      pTreeCtrl_->Delete(Position);
      long checkeableflag = HtmlListItemStringConverterInterface::Checkeable;
      pTreeCtrl_->Insert(
            GetCompatibleEncodingItemString(pitem->GetItemAsString(checkeableflag)),
            Position);
   }
   if (pTreeCtrl_->GetVisibleBegin() != firstline)
      pTreeCtrl_->ScrollToLine(firstline);

   pTreeCtrl_->Thaw();
   /** Actualizo los items seleccionados **/
   if (pitem->IsActive()) {
      selectedItems_.insert(pitem->GetId());
   } else {
      selectedItems_.erase(pitem->GetId());
   }
   HtmlListItemStateChangeNotification notif(pitem->GetId(), pitem->IsActive());
   Notify(&notif);
}

/** Metodo auxiliar que convierte el string pasado por parametro en un
 *  enconding valido. Esto es importante para los acentos.
 *  En linux modifica el string correspondiente al HTML para convertirlo
 *  a UTF8 y asi mostrar los caracteres no-ASCII 127 en forma correcta.
 *  @param[in] ItemString item sobre el cual se desea convertir a un encode
 *  que permita acentos
 */
std::string HtmlListSelectionWidget::GetCompatibleEncodingItemString(
      const std::string& ItemString) {
   wxString itemstr = ItemString;
#ifdef __LINUX__
   itemstr = wxString(wxConvUTF8.cMB2WC(itemstr), wxCSConv(XML_FILE_ENCODING));
#endif
   return itemstr.c_str();
}

/** Obtiene el item asociado al id **/
HtmlListItemInterface* HtmlListSelectionWidget::GetItem(const SuriObject::UuidType& ItemId) {
   std::list<HtmlListItemInterface*>::iterator item = items_.begin();
   HtmlListItemInterface* pitem = NULL;
   for (; !pitem && item != items_.end(); ++item)
      pitem = ItemId.compare((*item)->GetId()) == 0 ? *item : NULL;
   return pitem;
}
} /** ui */
} /** namespace suri */
