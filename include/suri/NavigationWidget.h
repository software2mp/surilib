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

#ifndef NAVIGATIONWIDGET_H_
#define NAVIGATIONWIDGET_H_


// Includes Estandard
#include <string>

// Includes suri
#include "suri/ToolGroupManager.h"
#include "suri/Widget.h"
#include "suri/Coordinates.h"
#include "suri/ListView.h"

// Includes wx
#include "wx/event.h"

/** namespace suri */
namespace suri {
class ViewerWidget;
class NavigationInputWidget;
class LibraryManager;
class DataViewManager;
class CoordinatesNavigationInputInterface;


/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(NavigationWidgetEvent);

/** Permite ingresar coordenadas y contiene botones que realizan operaciones con ellas. */
/**
 * Permite ingresar coordenadas. Antes de mostrar las coordenadas
 * verifica que todos los campos hallan sido cargados con datos validos.
 * Cuando se agregan botones a la herramienta se hace visible y luego
 * de mostrar el ultimo se oculta.
 * \todo cambiar separador de decimales segun region
 */
class NavigationWidget : public ToolGroupManager, public suri::Widget {
   /** Ctor. Copia */
   NavigationWidget(const NavigationWidget &);

public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(NavigationWidgetEvent);
   /** ctor */
   NavigationWidget(wxWindow *pParent, ViewerWidget* pViewer,
                    CoordinatesNavigationInputInterface* pCoordinateInput,
                    DataViewManager* pDataViewManager = NULL);
   /** dtor */
   virtual ~NavigationWidget();

   /** Creacion de la ventana */
   virtual bool CreateToolWindow();
   /** Titulo de la ventana (en caso de pParent!=NULL */
   virtual const wxString GetWindowTitle() const;
   /** Retorna la ventana asociada a la herramienta */
   virtual wxWindow *GetToolWindow() const {
      return pToolWindow_;
   }
   /** retorna la coordenada que ingresaron, informa si es valida */
   virtual bool GetCoordinate(Coordinates &LatLongCoord);
   /** Retorna la coordenada que ingresaron e informa si es valida en el sistema de
    * referencia que se pasa por parametro.*/
   virtual bool GetCoordinate(Coordinates &LatLongCoordinate,
                              const std::string& OutputSpatialReference);
   /** cambia la herramienta para ingresar coordenadas */
   virtual bool SetInputWidget(CoordinatesNavigationInputInterface* pNavigationWidget);
   /** cambia la herramienta para ingresar coordenadas */
   virtual CoordinatesNavigationInputInterface* GetInputWidget();

   /** Des/habilita el control wx ante cambios en la lista */
   void ListUpdate(Model *pModel);

   /** Metodo temporal. Devuelve el admin de la aplicacion que posee el widget**/
   DataViewManager* GetDataViewManager();

protected:
   /** Cuando se cierra la ventana no se elimina el window sino que se esconde */
   void OnClose(wxCloseEvent &Event);
   /** Obtiene el icono del navegador */
   virtual void GetWindowIcon(wxBitmap& ToolBitmap, int X = 0, int Y = 0) const;
   /** metodo virtual que se llama cuando se agrega boton a tool */
   virtual void DoOnAddButton(Button *pTool);
   /** metodo virtual que se llama cuando se elimina boton a tool */
   virtual void DoOnRemoveButton(Button *pTool);

private:
   ViewerWidget* pViewer_; /*! puntero al Viewer asociado */
   CoordinatesNavigationInputInterface* pCoordinateInputPart_;
   /* coordenadas */
   ListView<NavigationWidget> *pListViewer_; /*! View de la lista */
   /*! Objeto que conecta los  eventos */
   EVENT_OBJECT_PROPERTY(NavigationWidgetEvent);
   DataViewManager* pDataViewManager_;
};
}

#endif /*NAVIGATIONWIDGET_H_*/
