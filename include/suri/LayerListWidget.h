/*! \file LayerListWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LAYERLISTWIDGET_H_
#define LAYERLISTWIDGET_H_

// Includes standard
#include <list>

// Includes Suri
#include "suri/Widget.h"
#include "suri/Element.h"
#include "CallbackView.h"
#include "suri/ElementListWidget.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/htmllbox.h"
#include <wx/timer.h>

// Includes App

// Defines

// Forwards

/** namespace suri */
namespace suri {

// Forwards
class DataViewManager;
class ViewcontextInterface;
/** Clase/Herramienta que muestra un LayerList. */
/**
 * Esta clase muestra en un control el listado de elementos abiertos en un
 * contexto.
 *  Tambien genera eventos que modificaran la lista o mostraran las propiedades
 * de los elementos a partir de otras herramientas.
 */
class LayerListWidget : public ElementListWidget {
private:
   /** Ctor. de Copia. */
   LayerListWidget(const LayerListWidget &LayerListWidget);
public:
   /** Constructor */
   LayerListWidget(wxWindow *pParent, ViewcontextInterface* pViewcontext,
		   DataViewManager* pDataViewManager);

   /** Constructor \deprecated */
   LayerListWidget(wxWindow *pParent, ViewcontextInterface* pViewcontext, LayerList* pLayerList, DataViewManager* pDataViewManager);

   /** Constructor para compatibilidad de Simur **/
   LayerListWidget(wxWindow *pParent, LayerList* pLayerList, DataViewManager* pDataViewManager);
   /** Destructor */
   virtual ~LayerListWidget();
   /** Crea la ventana de herramienta */
   virtual bool CreateToolWindow();
   /** Muestra las propiedades del (ultimo) elemento seleccionado */
   virtual void ShowElementProperties();
   /** Cierra los elementos seleccionados */
   virtual void CloseSelectedElements();
   /** Despliega los elementos seleccionados */
   virtual void DisplaySelectedElements(bool Display);
   /** Activate Element */
   virtual void ActivateElement(int Index);
protected:
   /** Informa si el elemento esta activo */
   virtual bool IsElementActive(Element* pElement);
   /** Des/Activa el elemento */
   virtual void ActivateElement(Element* pElement, bool Value = true);
   /** Informa si el elemento esta seleccionado */
   virtual bool IsElementSelected(Element* pElement);
   /** Des/Selecciona el elemento */
   virtual void SelectElement(Element* pElement, bool Value = true);
   /** Muestra un menu sobre la lista */
   virtual wxMenu* CreateContextMenu(wxMouseEvent &Event, int Item);
   /** Se llama cuando se activa boton del menu contextual */
   virtual void DoOnContextMenu(wxMenuEvent &Event);

private:
   /** \deprecated */
   ViewcontextInterface* pViewcontext_; /** Puntero a visualizador de contexto */
   LayerList* pLayerList_; /** Puntero a capa */
   DataViewManager* pDataViewManager_; /** Puntero a visualizador de datos */
};

}  // namespace suri

#endif /*LAYERLISTWIDGET_H_*/
