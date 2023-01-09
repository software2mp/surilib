/*! \file ViewerWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWERWIDGET_H_
#define VIEWERWIDGET_H_

// Includes standard
#include <stack>

// Includes suri
#include "suri/Widget.h"
#include "suri/LayerList.h"
#include "suri/Button.h"
#include "suri/World.h"
#include "suri/ToolGroupManager.h"
#include "suri/Navigator.h"
#include "suri/ViewportInterface.h"
#include "ListView.h"
#include "ViewerBlockedNotification.h"
#include "ViewerUnblockedNotification.h"

// forwards wx
class wxUpdateUIEvent;
class wxCommandEvent;
class wxMouseEvent;
class wxBitmap;

/** namespace suri */
namespace suri {
// forwards
class CoordinatesTransformation;
class MeassureWidget;
class DataViewManager;
class WorldExtentManager;

/** Interfaz para los Manager de los Viewer */
/*!
 * Clase abstracta (interfaz) para que los Manager de los viewer implementen
 * el metodo que devuelve el viewer del contexto activo.
 */
class ActiveViewerManager {
public:
   /** Ctor. */
   ActiveViewerManager() {}
   /** Dtor. */
   virtual ~ActiveViewerManager() {}
   /** Devuelve el viewer del contexto activo */
   virtual ViewerWidget *GetActiveViewer()=0;
};

/** Declara la clase ViewerWidgetEvent */
DECLARE_EVENT_OBJECT(ViewerWidgetEvent);

/** Widget base para los visualizadores de Element. */
/*!
 *  Provee la funcionalidad e interfas basica para los visualizadores.
 *  Recibe eventos de la lista.
 *  Provee funcionalidad para Button.
 * \TODO(Javier - Sin TCK): Migrar la funcionalidad de administracion de Button a una clase separada
 */
class ViewerWidget : public Widget, public ToolGroupManager, public ViewportInterface,
                     public Subject {
private:
   /** Ctor. de Copia. */
   ViewerWidget(const ViewerWidget &ViewerWidget);

public:
   /** Declara friend la clase de evento ViewerWidgetEvent */
   FRIEND_EVENT_OBJECT(ViewerWidgetEvent);

   /** Constructor */
   ViewerWidget(wxWindow *pParent = NULL, DataViewManager* pDataViewManager = NULL);
   /** Destructor */
   virtual ~ViewerWidget();

   /** Creacion de la ventana */
   virtual bool CreateToolWindow();
   /** Obtiene el mundo */
   virtual World *GetWorld() const {
      return pWorld_;
   }

   /** Permite obtener la ventana con las herramientas del viewer */
   wxWindow *GetViewerTools(wxWindow *pParent);
   /** Se le setea la lista de elementos */
   void SetElementList(LayerList *pList);
   /** Se le setea la lista de elementos (vacio porque no es necesario */
   virtual void DoSetElementList(LayerList *pList) {
   }
   /** Resetea el estado del visualizador */
   virtual void ResetViewer()=0;
   /** Elimina un boton de herramienta */
   bool DeleteButtonTool(Button *pTool);
   /** Retorna instacia de Navigator */
   Navigator* GetNavigator();
   /** Obtiene la lista */
   virtual LayerList* GetList() const;
   /** Obtiene el ViewerActivo */
   static ViewerWidget *GetActiveViewer();
   /** Carga un objeto que devuelve el Viewer activo */
   static bool SetActiveViewerManager(ActiveViewerManager *pViewerManager);

   virtual SuriObject::UuidType GetId() const;
   virtual SuriObject::UuidIntType GetIntId() const;

   virtual SuriObject::UuidType GetViewcontextUuid() const;
   virtual bool SetViewcontext(ViewcontextInterface *pViewcontext);
   /** Obtiene la transformacion asociada **/
   virtual CoordinatesTransformation* GetTransformation() {
      return NULL;
   }

   /** Obtiene el widget de medicion */
   MeassureWidget* GetMeasureWidget();

   /** Establece el widget de medicion */
   void SetMeasureWidget(MeassureWidget* pMeasure);

   void Block() {
      block_ = true;
      ViewerBlockedNotification notif;
      Notify(&notif);
   }

   void UnBlock() {
      block_ = false;
      ViewerUnblockedNotification notif;
      Notify(&notif);
   }

   bool IsBlock() {
      return block_;
   }

   /** Despliega un elemento (puede ser funcion de RenderizationControl) */
   virtual void DisplayElement(Element *pElement)=0;
   /** Oculta un elemento (puede ser funcion de RenderizationControl) */
   virtual void HideElement(Element *pElement)=0;
   /** El elemento ha sido modificado */
   virtual void UpdateElement(Element *pElement)=0;
   /** ------- Modificacion del mundo del viewer -------------------------------**/
   /** Hace ZoomIn */
   virtual void ZoomIn(int X, int Y, int Factor = 2, int Z = 0) {
   }
   /** Hace ZoomOut */
   virtual void ZoomOut(int X, int Y, int Factor = 2, int Z = 0) {
   }
   /** Cambia la ventana del mundo */
   virtual void ChangeWindow(const Subset &NewWindow, bool PreventUpdate = false) {
   }
   /** Setea el WorldExtentManager */
   virtual void SetWorldExtentManager(WorldExtentManager* pExtentManager) {
   }
   /** Retorna el WorldExtentManager */
   virtual WorldExtentManager* GetWorldExtentManager() {
      return NULL;
   }
   /** Metodo de actualizacion de la lista, el mundo y/o los elementos. */
   virtual void UpdateViewers(Model *pModel) { }
   /** Verifica el estado de la renderizacion. */
   virtual void CheckRenderizationStatus() {
   }

   /** Realiza la creacion de la barra de herramientas en las clases derivadas */
   /*!
    *  Metodo que retorna la ventana de controles de las herramientas del
    * visualizador (navegacion, salvar, imprimir, etc.)
    * @param[in] pParent window donde agregan los botones.
    * @return Ventana de la herramienta.
    */
   virtual wxWindow *DoGetViewerTools(wxWindow *pParent)=0;

protected:
   /** Actualiza la barra de herramientas */
   virtual void OnViewerToolsUiUpdate(wxUpdateUIEvent &Event);
   /** Evento de elemento modificado */
   void ElementChanged(Model *pModel);
   /** Evento lista agregada */
   void ListChanged(Model *pModel);
   /** Evento lista eliminada */
   void ListDeleted(Model *pModel);
   World *pWorld_; /*! Mundo. */
   DataViewManager* pDataViewManager_;
   
   /** \todo (ramirogonzalez - #4196): PARCHE, En el futuro mover esta
    * funcionalidad a GuiStateChangeMediator */
   std::set<Button*> pViewerToolButtons_; /*! Botones de ViewerWidget */
   ListView<ViewerWidget> *pListViewer_; /*! View de la lista */

private:
   EVENT_OBJECT_PROPERTY(ViewerWidgetEvent); /*! Objeto para conectar eventos */
   CallbackView<ViewerWidget> *pElementViewer_; /*! View de elementos */
   Navigator *pNavigator_; /*! permite recorrer el mundo usando puntos */
   static ActiveViewerManager *pViewerManager_; /*! Objeto que retorna el */
   /* viewer activo */
   SuriObject idObject_;
   SuriObject viewcontextIdObject_;
   MeassureWidget* pmeasure_;
   bool block_;
};

}

#endif /*VIEWERWIDGET_H_*/
