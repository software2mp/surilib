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

#include "wxDecoratedHtmlListCtrl.h"

// Includes standard
#include <list>

// Includes Suri
#include "logmacros.h"
#if defined(__LINUX__)
#include "suri/XmlElement.h"
#endif

// Includes Wx

// Includes App

// Defines
/** Intervalo de tiempo en milisegundos */
#define TIMER_INTERVAL 300
/** ID para el objeto wxtimer */
#define ID_TIMER 10020

namespace suri {
// Defines
/** cte para ajustar tamanio del HTML */
#define MAX_WIDTH_SUBSTRACTION 64
#if defined(WX_VLISTBOX_HAS_GETITEMRECT)
/** Macro que utiliza el GetItemRect de HtmlSimpleList del patch wxsuri */
#  define GETITEMRECT(Index) GetItemRect(Index)
#else
/** Clase workaround para las versiones de wx sin el patch wxsuri */
class _dummy_Item_Rect_
{
public:
   /** ctor */
   _dummy_Item_Rect_() {
   }
   /** retorna un ancho prefijado */
   int GetWidth() {
      return 128;
   }
   /** retorna alto prefijado */
   int GetHeight() {
      return 64;
   }
};
/** Macro que utiliza la clase dummy para poder compilar sin el patch wxsuri */
#  define GETITEMRECT(Index) _dummy_Item_Rect_()
#endif

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(wxDecoratedHtmlListCtrlEvent, wxDecoratedHtmlListCtrl)IMPLEMENT_EVENT_CALLBACK(OnLeftDown, OnLeftDown(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLeftUp, OnLeftUp(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnDrag, OnDrag(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
   IMPLEMENT_EVENT_CALLBACK(OnTimer, OnTimer(event), wxTimerEvent)
   IMPLEMENT_EVENT_CALLBACK(OnSelectionChanged, OnSelectionChanged(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Ctor
 * @param[in] pParent ventana padre donde se creara esta ventana
 * @param[in] Style estilo de la ventana. Se usa para permitir o no
 * seleccion multiple
 * @param[in] BehaviorFlags Configura que comportamiento tendra la lista
 * opciones: wxDraggableItems o wxNoBehaviour.
 */
wxDecoratedHtmlListCtrl::wxDecoratedHtmlListCtrl(wxWindow* pParent, long Style,
                                                 long BehaviorFlags) :
      wxHtmlListBox(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, Style),
      NEW_EVENT_OBJECT(wxDecoratedHtmlListCtrlEvent)
{
   pSuriEventHandler_ = NULL;
   behaviorFlags_ = BehaviorFlags;

   indexHtmlItemFrom_ = wxNOT_FOUND;
   indexHtmlItemTo_ = wxNOT_FOUND;
   moveAfter_ = false;
   isDragging_ = false;
   coordLastLine_ = -1;
   dragMouseX_ = -1;
   dragMouseY_ = -1;
   scrollDirection_ = 0;

   pTimer_ = new wxTimer(this, ID_TIMER);

   Connect(wxEVT_LEFT_DOWN,
           wxMouseEventHandler(wxDecoratedHtmlListCtrlEvent::OnLeftDown), NULL,
           pEventHandler_);
   Connect(wxEVT_LEFT_UP, wxMouseEventHandler(wxDecoratedHtmlListCtrlEvent::OnLeftUp),
           NULL, pEventHandler_);
   Connect(wxEVT_MOTION, wxMouseEventHandler(wxDecoratedHtmlListCtrlEvent::OnDrag),
           NULL, pEventHandler_);
   Connect(wxEVT_UPDATE_UI,
           wxUpdateUIEventHandler(wxDecoratedHtmlListCtrlEvent::OnUIUpdate), NULL,
           pEventHandler_);
   Connect(wxEVT_TIMER, wxTimerEventHandler(wxDecoratedHtmlListCtrlEvent::OnTimer),
           NULL, pEventHandler_);
   Connect(wxEVT_COMMAND_LISTBOX_SELECTED,
           wxCommandEventHandler(wxDecoratedHtmlListCtrlEvent::OnSelectionChanged),
           NULL, pEventHandler_);
}

/** Dtor */
wxDecoratedHtmlListCtrl::~wxDecoratedHtmlListCtrl() {
   Disconnect(wxEVT_LEFT_DOWN,
              wxMouseEventHandler(wxDecoratedHtmlListCtrlEvent::OnLeftDown), NULL,
              pEventHandler_);
   Disconnect(wxEVT_LEFT_UP,
              wxMouseEventHandler(wxDecoratedHtmlListCtrlEvent::OnLeftUp), NULL,
              pEventHandler_);
   Disconnect(wxEVT_MOTION, wxMouseEventHandler(wxDecoratedHtmlListCtrlEvent::OnDrag),
              NULL, pEventHandler_);
   Disconnect(wxEVT_UPDATE_UI,
              wxUpdateUIEventHandler(wxDecoratedHtmlListCtrlEvent::OnUIUpdate), NULL,
              pEventHandler_);
   Disconnect(wxEVT_TIMER, wxTimerEventHandler(wxDecoratedHtmlListCtrlEvent::OnTimer),
              NULL, pEventHandler_);
   Disconnect(wxEVT_COMMAND_LISTBOX_SELECTED,
              wxCommandEventHandler(wxDecoratedHtmlListCtrlEvent::OnSelectionChanged),
              NULL, pEventHandler_);

   delete pSuriEventHandler_;
   DELETE_EVENT_OBJECT
   ;
}

/** 
 * Configura un EventHandler 
 * @param[in] pSuriEventHandler Nuevo event handler. Pasa a ser responsabilidad de esta
 * clase, no eliminar fuera de ella.
 */
void wxDecoratedHtmlListCtrl::SetEventHandler(HtmlListEventHandlerInterface* pSuriEventHandler) {
   delete pSuriEventHandler_;
   pSuriEventHandler_ = pSuriEventHandler;
}

/**
 * Retorna lista con el id de los elementos seleccionados
 * @return std::list<ItemId> Ids de elementos seleccionados
 */
std::list<ItemId> wxDecoratedHtmlListCtrl::GetSelectedItemsIds() const {
   std::list<ItemId> itemidlist;
   int htmlitemcount = htmlItemList_.size();

   // wxHtmlListBox brinda interfaces diferentes dependiendo si wxLB_MULTIPLE
   // esta seteado o no.
   if (!HasMultipleSelection()) {
      int item = GetSelection();
      if (item != wxNOT_FOUND && item < htmlitemcount)
         itemidlist.push_back(htmlItemList_[item].GetItemId());
   } else {
      // Recorro items seleccionados en control
      unsigned long cookie;
      int item = this->GetFirstSelected(cookie);
      while (item != wxNOT_FOUND) {
         // Obtengo ItemId asociado a item y lo agregitemId_o a itemidlist
         if (item >= htmlitemcount) {
            REPORT_DEBUG("item en lista fuera de rango");
         } else {
            itemidlist.push_back(htmlItemList_[item].GetItemId());
         }

         item = this->GetNextSelected(cookie);
      }
   }

   return itemidlist;
}

/**
 * Funcion de debug para ver html por log
 */
void wxDecoratedHtmlListCtrl::PrintHtml() {
   int size = htmlItemList_.size();
   for (int i = 0; i < size; i++)
      REPORT_DEBUG(htmlItemList_[i].GetHtml().c_str());
}

/**
 * Funcion que permite seleccionar un elemento usando ItemId
 * @param[in] Id ItemId del elemento que quiero seleccionar
 * @return bool que indica si encontro el elemento y lo pudo seleccionar
 */
bool wxDecoratedHtmlListCtrl::SelectItem(const ItemId &Id, bool Selected) {
   int size = htmlItemList_.size();
   int i = 0;
   while (i < size && htmlItemList_[i].GetItemId().Compare(Id) != 0)
      i++;

   if (i != size) {
      if (HasMultipleSelection()) {
         Select(i, Selected);
      } else {
         if (Selected) {
            SetSelection(i);
         } else {
            if (GetSelection() == i) SetSelection(-1);
         }
      }
   }

   return i != size;
}

/** 
 * Retorna referencia a vector interno con el html de los items desplegados 
 * @return referencia a vector interno con el html de los items desplegados.
 */
std::vector<DecoratedHtmlItem>& wxDecoratedHtmlListCtrl::GetHtmlItemList() {
   return htmlItemList_;
}

/**
 * Metodo que ejecuta wxHtmlListBox para obtener el html de un item de la lista.
 * @param[in] n posicion del item en el control grafico
 * return wxString con html del item
 */
wxString wxDecoratedHtmlListCtrl::OnGetItem(size_t n) const {
   if (n >= htmlItemList_.size()) {
      return "";
   }

   std::string string = htmlItemList_[n].GetHtml();

#if defined(__LINUX__)
   // Correccion a codificacion en Linux
   string = wxString(wxConvUTF8.cMB2WC(string.c_str()),
                     wxCSConv(XmlElement::xmlEncoding_.c_str()));
   if (string.empty()) string = htmlItemList_[n].GetHtml();
#endif

   return string;
}

/**
 * Si se esta drageando actualiza atributos que determinan
 * la nueva posicion de un item
 */
void wxDecoratedHtmlListCtrl::UpdateDragDestination() {
   if (!isDragging_) {
      return;
   }

   moveAfter_ = false;
   indexHtmlItemTo_ = HitTest(dragMouseX_, dragMouseY_);

   if (indexHtmlItemTo_ == wxNOT_FOUND) {
      return;
   }

   // Si el mouse esta sobre un item veo si esta en parte inferior o superior
   int afterindex = HitTest(
         dragMouseX_, dragMouseY_ + GETITEMRECT(indexHtmlItemTo_).GetHeight() / 2);
   if (afterindex != indexHtmlItemTo_) {
      moveAfter_ = true;
   }
}

/**
 * Si se esta drageando analiza la posicion del item destino para saber si debe
 * activar scroll.
 */
void wxDecoratedHtmlListCtrl::ConfigureTimer() {
   if (!isDragging_) {
      pTimer_->Stop();
      return;
   }

   // Esto es para activar el Timer y poder scrollear.
   scrollDirection_ = 0;
   if ((indexHtmlItemTo_ == wxNOT_FOUND || indexHtmlItemTo_ == (int) GetVisibleEnd() - 1)
         && GetVisibleEnd() != GetItemCount()) {
      scrollDirection_ = 1;
   }
   if (indexHtmlItemTo_ == (int) GetVisibleBegin() && indexHtmlItemTo_ != 0) {
      scrollDirection_ = -1;
   }

   if (scrollDirection_ != 0 && !pTimer_->IsRunning()) {
      pTimer_->Start(TIMER_INTERVAL, wxTIMER_CONTINUOUS);
   }
   if (scrollDirection_ == 0 && pTimer_->IsRunning()) {
      pTimer_->Stop();
   }
}

/**
 * Termina el drag si hay uno realizandose.
 */
void wxDecoratedHtmlListCtrl::EndDrag() {
   isDragging_ = false;
   indexHtmlItemFrom_ = indexHtmlItemTo_ = wxNOT_FOUND;
   pTimer_->Stop();
}

/**
 * Este metodo se ejecuta cuando la lista recibe el evento de soltado del boton
 * izq. del mouse.
 * Si se estaba arrastrando un elemento finaliza el drag
 * y llama a metodo encargado de mover el item.
 * @param[in]	Event: evento.
 */
void wxDecoratedHtmlListCtrl::OnLeftUp(wxMouseEvent &Event) {
   // Evita que al hacer click sobre lista y luego sobre otra cosa, la lista
   // atrape el segundo evento
   if (HasCapture()) {
      ReleaseMouse();
   }

   if (Event.LeftUp() && isDragging_) {
      int fromindex = indexHtmlItemFrom_;
      int toindex = indexHtmlItemTo_;
      bool moveafter = moveAfter_;

      EndDrag();

      if (fromindex != wxNOT_FOUND && fromindex != toindex) {
         if (toindex == wxNOT_FOUND) {
            DoOnMoveEnd(htmlItemList_[fromindex].GetItemId());
         } else if (moveafter) {
            DoOnMoveAfter(htmlItemList_[fromindex].GetItemId(),
                          htmlItemList_[toindex].GetItemId());
         } else {
            DoOnMoveBefore(htmlItemList_[fromindex].GetItemId(),
                           htmlItemList_[toindex].GetItemId());
         }
      }

      Refresh();
      Update();
   }

   Event.Skip();
}

/**
 * Este metodo se ejecuta cuando la lista recibe el evento de click izq.(sin
 * soltar el boton).
 * Obtiene el indice del elemento sobre el cual se hizo click para luego
 * permitir el arrastre de dicho elemento dentro de la lista.
 * @param[in]	Event: evento.
 */
void wxDecoratedHtmlListCtrl::OnLeftDown(wxMouseEvent &Event) {
   indexHtmlItemFrom_ = wxNOT_FOUND;
   if (!Event.ControlDown()) indexHtmlItemFrom_ = HitTest(Event.m_x, Event.m_y);
   Event.Skip();
}

/**
 * Este metodo se ejecuta cuando se produce un evento de drag sobre la lista.
 * Si no se activo el flag de drag no se continua con los eventos.
 * Setea isDragging_ para indicar que se esta arrastrando un elemento,
 * actualiza el destino del drag y configura el timer para scroll.
 * Fuerza que se dispare el evento UIUpdate para que se dibuje sobre la lista
 * una linea que indica la siguiente posicion del elemento arrastrado.
 * @param[in]	Event: evento.
 */
void wxDecoratedHtmlListCtrl::OnDrag(wxMouseEvent &Event) {
   if ((behaviorFlags_ & wxDraggableItems) == 0) {
      return;
   }

   if (Event.Dragging() && !(Event.RightIsDown() || Event.RightDown()) &&
   indexHtmlItemFrom_ != wxNOT_FOUND) {
      if (!isDragging_) {
         isDragging_ = true;
      }

      if (!HasCapture()) {
         CaptureMouse();
      }

      dragMouseX_ = Event.m_x;
      dragMouseY_ = Event.m_y;

      UpdateDragDestination();

      ConfigureTimer();

      UpdateWindowUI(wxUPDATE_UI_FROMIDLE);
   } else {
      EndDrag();
   }

   Event.Skip();
}

/**
 * Este metodo responde al evento UIUpdate que se ejecuta cuando no hay eventos
 * de usuario en la cola de eventos de wx.
 * Si se esta llevando a cabo una operacion de drag pinta una linea marcando
 * la nueva posicion del elemento que se esta draggeando.
 * @param[in]	Event: evento.
 */
void wxDecoratedHtmlListCtrl::OnUIUpdate(wxUpdateUIEvent &Event) {
   if (isDragging_) {
      /*Sumo las alturas de los items para pintar la linea al final del item.
       * Nota: no se xq no necesito hacer el for inverso cuando voy de abajo
       * hacia arriba*/
      int index = indexHtmlItemTo_;
      if (index == wxNOT_FOUND) {  // despues del ultimo elemento de la lista.
         index = GetVisibleEnd();
      } else if (moveAfter_) {
         index++;
      }

      int coordy = 0;
      for (int i = GetVisibleBegin(); i < index; i++)
         coordy += GETITEMRECT(i).GetHeight();

      if (coordLastLine_ != coordy) {
         Refresh();
         coordLastLine_ = coordy;
      }

      wxClientDC *pclientdc = new wxClientDC(this);
      pclientdc->SetBrush(wxBrush(wxColour(0, 0, 0)));
      pclientdc->SetPen(wxPen(wxColour(0, 0, 0), 2));
      pclientdc->DrawLine(0, coordy, GetSize().GetX(), coordy);
      delete pclientdc;
   }

   Event.Skip();
}

/**
 * Este metodo responde a los eventos de Timer sobre la lista.
 * El objetivo de este metodo es poder realizar un scroll vertical , de forma
 * automatica, sobre la lista de elementos.
 * @param[in]	Event: evento.
 */
void wxDecoratedHtmlListCtrl::OnTimer(wxTimerEvent &Event) {
   ScrollLines(scrollDirection_);
   Refresh();

   UpdateDragDestination();

   ConfigureTimer();

   Event.Skip();
}

/**
 * Atrapa evento disparado cuando se modifica la seleccion
 * @param[in] Event: evento.
 */
void wxDecoratedHtmlListCtrl::OnSelectionChanged(wxCommandEvent &Event) {
   DoOnSelectionChanged();
   Event.Skip();
}

/** Se ejecuta cuando se mueve un item despues de otro */
bool wxDecoratedHtmlListCtrl::DoOnMoveAfter(const ItemId &Id,
                                             const ItemId &DestinationId) {
   if (pSuriEventHandler_)
      return pSuriEventHandler_->DoOnMoveAfter(Id, DestinationId);
   return false;
}

/** Se ejecuta cuando se mueve un item antes que otro */
bool wxDecoratedHtmlListCtrl::DoOnMoveBefore(const ItemId &Id,
                                             const ItemId &DestinationId) {
   if (pSuriEventHandler_)
      return pSuriEventHandler_->DoOnMoveBefore(Id, DestinationId);
   return false;
}

/** Se ejecuta cuando se mueve un item despues del ultimo elemento */
bool wxDecoratedHtmlListCtrl::DoOnMoveEnd(const ItemId &Id) {
   if (pSuriEventHandler_)
      return pSuriEventHandler_->DoOnMoveEnd(Id);
   return false;
}

/** Se ejecuta cuando se modifica la seleccion */
void wxDecoratedHtmlListCtrl::DoOnSelectionChanged() {
   if (pSuriEventHandler_) {
      std::list<ItemId> selectedelements = GetSelectedItemsIds();
      pSuriEventHandler_->DoOnSelectionChanged(selectedelements);
   }
}
} /* namespace suri */
