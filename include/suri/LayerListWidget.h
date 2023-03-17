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
