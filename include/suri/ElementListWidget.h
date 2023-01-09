/*! \file ElementListWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ELEMENTLISTWIDGET_H_
#define ELEMENTLISTWIDGET_H_

// Includes standard
#include <list>
#include <map>

// Includes Suri
#include "suri/Widget.h"
#include "suri/Element.h"
#include "CallbackView.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/htmllbox.h"
#include <wx/timer.h>

// Includes App

// Defines
/** ID para el item del menu contextual (check) para elementos ocultos */
#define ID_MENU_HIDDEN_ITEMS 10030

// forwards
class wxCommandEvent;

/** namespace suri */
namespace suri {

// forwards
class LayerList;
class LayerListViewer;
class DataViewManager;

/** Macro para declarar el objeto de los eventos */
DECLARE_EVENT_OBJECT(LayerEventHandler);

/** Clase/Herramienta que muestra un LayerList. */
/**
 * Esta clase muestra en un control el listado de elementos abiertos en un
 * contexto.
 * Maneja los eventos que se generan sobre la lista (seleccionar, activar y
 * mover elementos de la lista, desplegar y manejo del menu contextual, etc.).
 * Informa el estado de los elementos de la lista (activo, seleccionado, etc.).
 */
class ElementListWidget : public suri::Widget, public View {
private:
   /** Ctor. de Copia. */
   ElementListWidget(const ElementListWidget &ElementListWidget);

public:
   /** clase amiga para el procesamiento de eventos */
   friend class LayerEventHandler;
   /** Constructor */
   ElementListWidget(wxWindow *pParent, suri::LayerList *pLayerList,
                     DataViewManager *DataViewManager);
   /** Destructor */
   virtual ~ElementListWidget();
   /** Crea la ventana de herramienta */
   virtual bool CreateToolWindow();
   /** Retorna el titulo/nombre de la herramienta */
   virtual const wxString GetWindowTitle() const;
   /** Cambia estado checkbox a true */
   virtual void ActivateElement(int Index);
   /** Checkea item del menu "Mostrar elementos ocultos". */
   void CheckMenuItem();
   /** Informa si el elemento esta activo */
   virtual bool IsElementActive(Element* pElement);
   /** Des/Activa el elemento */
   virtual void ActivateElement(Element* pElement, bool Value = true);
   /** Informa si el elemento esta seleccionado */
   virtual bool IsElementSelected(Element* pElement);
   /** Des/Selecciona el elemento */
   virtual void SelectElement(Element* pElement, bool Value = true);

protected:
   /** Recibe el update */
   virtual void Update(Model *pModel);
   /** Recibe el aviso de delete */
   virtual void OnModelDelete(Model *pModel);
   /** Recibe el aviso de delete */
   virtual void ElementDelete(Model *pModel);
   /** Evento doble-click en un elemento de la lista */
   void OnDoubleClick(wxMouseEvent &Event);
   /** Evento OnClick para un elemento */
   void OnMouseClick(wxMouseEvent &Event);
   /** Evento LeftDown en una celda HTML */
   void OnLeftDown(wxMouseEvent &Event);
   /** Evento LeftUp en una celda HTML */
   void OnLeftUp(wxMouseEvent &Event);
   /** Evento Menu de propiedades */
   void OnContextMenu(wxMenuEvent &Event);
   /** Evento para la seleccion */
   void OnSelectionChanged(wxCommandEvent &Event);
   /** Lanzado cuando cambia el tamanio de la ventana */
   virtual void OnResize(wxSizeEvent &Event);
   /** Evento para checkear Activo */
   void OnCheck(wxHtmlLinkEvent &Event);
   /** Evento de Drag&Drop */
   void OnDrag(wxMouseEvent &Event);
   /** Evento para dibujar la posicion del elemento en el Drag&Drop */
   void OnUIUpdate(wxUpdateUIEvent &Event);
   /** Evento para poder utilizar ESC en Drag&Drop */
   void OnKeyPress(wxKeyEvent &Event);
   /** Evento disparado por el Timer */
   void OnTimer(wxTimerEvent &Event);
   /** Selecciona el proximo elemento para el Drag&Drop */
   bool SelectNextElement(long MouseX, long MouseY);
   /** Este metodo se ejecuta cuando la lista libera el mouse */
   void OnCaptureLost(wxMouseCaptureLostEvent &Event);
   /** Determina si hay que mostrar el elemento */
   virtual bool CanShowElement(Element* pElement);
   /** Retorna el dibujo que hay que mostrar junto al elemento */
   virtual std::string GetElementIcon(Element* pElement);
   /** Retorna la descripcion del elemento */
   virtual std::string GetElementDescription(Element* pElement);
   /** Muestra un menu sobre la lista */
   virtual wxMenu* CreateContextMenu(wxMouseEvent &Event, int Item);
   /** Se llama cuando se activa boton del menu contextual */
   virtual void DoOnContextMenu(wxMenuEvent &Event) {}
   /** Select=false para todos los elementos */
   virtual void ClearSelectedElements();

   /** Devuelve el ancho maximo de los elementos visibles de la lista */
   int GetMaxItemWidth();
   LayerEventHandler *pEventHandler_; /*! Manejador de eventos */
   suri::LayerList *pLayerList_; /*! Lista de elementos */
   CallbackView<ElementListWidget> *pElementViewer_; /*! View de los elementos */
   suri::Element *pElementFrom_; /*! Elemento Dde utilizado en el drag&drop */
   wxTimer *pTimer_; /*! Timer */
   int indexHtmlItemFrom_; /*! indice del elemento Dde para el drag&drop */
   int indexHtmlItemTo_; /*! indice del elemento hacia para el drag&drop */
   bool isDragging_; /*! flag que indica si se esta draggeando un elemento */
   bool resized_; /*! flag que indica si se modifico el tamanio de la ventana. */
// int maxWidthRect_; /*! rectangulo de mayor longitud de la lista. */
   int coordLastLine_; /*! Posicion de la linea que se pinta al mover items */
   int scrollDirection_; /*! direccion en la que se scrollea(1:abajo;-1:arriba) */
   long mouseX_; /*! pos. X del mouse. Utilizada para simular un evento de drag */
   long mouseY_; /*! pos. Y del mouse. Utilizada para simular un evento de drag */
   bool isMenuChecked_; /*! guarda el estado del item del menu "Mostrar */
   /* elementos ocultos". */
   std::set<Element*> activeElements_; /*! Lista de elementos activos */
   std::set<Element*> selectedElements_; /*! Lista de elementos seleccionados */
   std::map<int, Element*> displayedElements_; /*! Lista de elementos desplegados */
   bool dragActivated_; /*! flag que indica si esta activado el drag en la lista */
   DataViewManager *pDataViewManager_; /*! Puntero al DataView Manager */
};

}

#endif /*ELEMENTLISTWIDGET_H_*/
