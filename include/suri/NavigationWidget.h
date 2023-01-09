/*! \file NavigationWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
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
