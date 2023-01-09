/*! \file wxDecoratedHtmlListCtrl.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WXDECORATEDHTMLLISTCTRL_H_
#define WXDECORATEDHTMLLISTCTRL_H_

// Includes standard
#include <list>
#include <vector>

// Includes Suri
#include "ItemId.h"
#include "DecoratedHtmlItem.h"
#include "HtmlListEventHandlerInterface.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/htmllbox.h"
#include "wxmacros.h"

// Includes App

// Defines
#define wxNoBehaviour 0
#define wxDraggableItems 1

namespace suri {
/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(wxDecoratedHtmlListCtrlEvent);

/**
 * Clase que hereda de wxHtmlListBox.
 * Los items se configuran con elementos en htmlItemList_. Esta clase no se
 * deberia instaciar directamente ya que no presenta interfaz para configurar
 * los items que contiene.
 */
class wxDecoratedHtmlListCtrl : public wxHtmlListBox {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(wxDecoratedHtmlListCtrlEvent);

   /** Ctor */
   wxDecoratedHtmlListCtrl(wxWindow* pParent, long Style = 0, long BehaviorFlags =
         wxNoBehaviour);
   /** Dtor */
   virtual ~wxDecoratedHtmlListCtrl();

   /** Configura un EventHandler */
   void SetEventHandler(HtmlListEventHandlerInterface* pSuriEventHandler);

   /** Retorna lista con el id de los elementos seleccionados */
   std::list<ItemId> GetSelectedItemsIds() const;

   /** Funcion de debug para ver html por log */
   void PrintHtml();

   /** Funcion que permite seleccionar un elemento usando ItemId */
   bool SelectItem(const ItemId &Id, bool Selected = true);

   /** Retorna vector interno con html de items desplegados */
   std::vector<DecoratedHtmlItem>& GetHtmlItemList();

   /** Termina el drag si hay uno realizandose. */
   void EndDrag();

protected:
   /** Metodo que ejecuta wxHtmlListBox para obtener el html de un item. */
   virtual wxString OnGetItem(size_t n) const;

   /** Actualiza atributos que determinan la nueva posicion de un item */
   void UpdateDragDestination();
   /** Determina si se debe activar scroll. */
   void ConfigureTimer();

private:
   /** Se ejecuta con evento de soltado del boton izq. del mouse. */
   void OnLeftUp(wxMouseEvent &Event);
   /** Se ejecuta con evento de click izq.(sin soltar el boton). */
   void OnLeftDown(wxMouseEvent &Event);
   /** Se ejecuta con evento de drag sobre la lista. */
   void OnDrag(wxMouseEvent &Event);
   /** Evento para dibujar la posicion del elemento en el Drag&Drop */
   void OnUIUpdate(wxUpdateUIEvent &Event);
   /** Evento disparado por el Timer */
   void OnTimer(wxTimerEvent &Event);
   /** Atrapa evento disparado cuando se modifica la seleccion */
   void OnSelectionChanged(wxCommandEvent &Event);

protected:
   // Metodos que se deben implementar para definir comportamiento de la lista

   /** Se ejecuta cuando se mueve un item despues de otro */
   virtual bool DoOnMoveAfter(const ItemId &Id, const ItemId &DestinationId);
   /** Se ejecuta cuando se mueve un item antes que otro */
   virtual bool DoOnMoveBefore(const ItemId &Id, const ItemId &DestinationId);
   /** Se ejecuta cuando se mueve un item despues del ultimo elemento */
   virtual bool DoOnMoveEnd(const ItemId &Id);
   /** Se ejecuta cuando se modifica la seleccion */
   virtual void DoOnSelectionChanged();

   long behaviorFlags_; /*! Flags que se usan para configurar comportamiento */
   int indexHtmlItemFrom_; /*! indice del elemento Dde para el drag&drop */
   int indexHtmlItemTo_; /*! indice del elemento hacia para el drag&drop */
   bool moveAfter_; /*! Indica si se agrega antes o despues del item */
   bool isDragging_; /*! flag que indica si se esta draggeando un elemento */
   wxTimer *pTimer_; /*! Timer que se usa para scroll */
   int scrollDirection_; /*! direccion en la que se scrollea(1:abajo;-1:arriba) */
   long dragMouseX_; /*! pos. X del mouse. Utilizada para simular un evento de drag */
   long dragMouseY_; /*! pos. Y del mouse. Utilizada para simular un evento de drag */
   int coordLastLine_; /*! Posicion de la linea que se pinta al mover items */
   std::vector<DecoratedHtmlItem> htmlItemList_; /*! Listado con los HtmlItems en control. */
   /** Si esta definido, se llama cuando el usuario realiza una accion sobre la lista */
   HtmlListEventHandlerInterface* pSuriEventHandler_;
   EVENT_OBJECT_PROPERTY(wxDecoratedHtmlListCtrlEvent);
};
}  // namespace

#endif /* WXDECORATEDHTMLLISTCTRL_H_ */
