/*! \file Viewer2D.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWER2D_H_
#define VIEWER2D_H_

// Includes suri
#include "ViewerWidget.h"
#include "View.h"
#include "suri/Subset.h"
#include "suri/DragEvent.h"

/** Forwards wx */
class wxScrolledWindow;
class wxScrollWinEvent;
class wxMouseEvent;
class wxTimer;
class wxCommandEvent;
class wxCloseEvent;
class wxToggleBitmapButton;

/** namespace suri */
namespace suri {
/** Declara la clase Viewer2DEvent */
DECLARE_EVENT_OBJECT(Viewer2DEvent);

class LayerList;
class RenderizationManager;
class ScreenCanvas;
class Mask;
class World;
class AspectPreservingWorld;
class Subset;
class WorldExtentManager;
class CoordinatesTransformation;
template<class CallbackClass>
class WorldView;

template<class CallbackClass>
class ListViewer;

class ActivationLogic;

/** ViewerWidget que provee un visualizador 2D. */
/**
 *  Herramienta que provee un visualizador de imagenes 2D.
 *
 *  Crea su propio World y RenderizationManager, de manera que si existen
 * otras instancias del visualizador con la misma lista, se vera la misma
 * renderizacion (combinacion de bandas, realce, filtros, etc), pero con mundos
 * distintos (ZoomRenderer, subset).
 *  Modifica los parametros del Mundo asociado para disparar las
 * renderizaciones que terminan sobre un Canvas asociado a una ventana.
 *
 */
class Viewer2D : public ViewerWidget {
private:
   /** Ctor. de Copia. */
   Viewer2D(const Viewer2D &Viewer2D);

public:
   /** Declara friend la clase de evento Viewer2DEvent */
   FRIEND_EVENT_OBJECT(Viewer2DEvent);
   /** ctor */
   Viewer2D(wxWindow *pParent = NULL, DataViewManager* pDataViewManager = NULL);
   /** dtor */
   virtual ~Viewer2D();
   /** Creacion de la ventana */
   virtual bool CreateToolWindow();
   /** Resetea el View */
   virtual void ResetViewer();

// ------------------------ MODIFICACION DE MUNDO -------------------------
   /** Enumerado con los tipos de modos posibles */
   typedef enum {
      Gis, Raster
   } Mode;
   /** Setea el modo */
   virtual void SetMode(Viewer2D::Mode mode);
   /** Getea el modo */
   virtual Viewer2D::Mode GetMode() const;

// ------------------------ MODIFICACION DE MUNDO -------------------------
   /** Hace ZoomIn */
   void ZoomIn(int X, int Y, int Factor = 2, int Z = 0);
   /** Hace ZoomOut */
   void ZoomOut(int X, int Y, int Factor = 2, int Z = 0);
   /** Cambia la ventana del mundo */
   void ChangeWindow(const Subset &NewWindow, bool PreventUpdate = false);
   /** Setea el WorldExtentManager */
   void SetWorldExtentManager(WorldExtentManager* pExtentManager);
   /** Retorna el WorldExtentManager */
   WorldExtentManager* GetWorldExtentManager() {
      return pWorldExtentManager_;
   }
   /** Metodo de actualizacion de la lista, el mundo y/o los elementos. */
   void UpdateViewers(Model *pModel);
   /** Verifica el estado de la renderizacion. */
   void CheckRenderizationStatus();
   /** Obtiene la transformacion asociada **/
   virtual CoordinatesTransformation* GetTransformation();
   wxScrolledWindow * GetScrolledWindow() {return pScrolledWin_;}
   Element* actualElement_;
protected:
   /** Permite obtener la ventana con las herramientas del viewer */
   virtual wxWindow *DoGetViewerTools(wxWindow *pParent);
   /** Despliega un elemento (puede ser funcion de RenderizationControl) */
   virtual void DisplayElement(Element *pElement);
   /** Oculta un elemento (puede ser funcion de RenderizationControl) */
   virtual void HideElement(Element *pElement);
   /** El elemento ha sido modificado */
   virtual void UpdateElement(Element *pElement);
   /** Se le setea la lista de elementos */
   virtual void DoSetElementList(LayerList *pList);
// --------------------------------- EVENTOS --------------------------------
   /** evento OnSize */
   void OnSize();
   /** evento OnIdle */
   void OnIdle();
   /** evento OnTimer (para la modificacion del World frente a un resize) */
   void OnTimer();
   /** evento OnScrollTimer (para la modificacion del World frente a un scroll) */
   void OnScrollTimer();
   /** evento de scroll */
   void OnScrollRelease(wxScrollWinEvent &Event);
   /** evento de scroll */
   void OnScrollPageUp(wxScrollWinEvent &Event);
   /** evento de scroll */
   void OnScrollPageDown(wxScrollWinEvent &Event);
   /** evento de scroll */
   void OnScrollLineUp(wxScrollWinEvent &Event);
   /** evento de scroll */
   void OnScrollLineDown(wxScrollWinEvent &Event);
   /** evento de scroll */
   void OnScrollTop(wxScrollWinEvent &Event);
   /** evento de scroll */
   void OnScrollBottom(wxScrollWinEvent &Event);
   /** Ajusta las barras de scroll */
   void AdjustScrollbars(int Orientation, int Increment);
   /** evento de cierre de ventana que contiene a la herramienta */
   void OnClose(wxCloseEvent &Event);
   void OnWheelRotate(wxMouseEvent &Event);
   void OnMiddleDown(wxMouseEvent &Event);
   void OnMiddleUp(wxMouseEvent & OnMiddleUp);
   void OnMouseMove(wxMouseEvent &Event);
// ------------------------ MODIFICACION DE MUNDO -------------------------
   /** Cambia el tamano del viewport */
   void ChangeViewport(bool PreventUpdate = false);
   /** Ajusta los tamanios virtuales */
   void AdjustVirtualSize();

private:
   EVENT_OBJECT_PROPERTY(Viewer2DEvent); /*! Objeto para conectar eventos */
   RenderizationManager *pRenderer_; /*! Renderizador */
   RenderizationManager *pMaskRenderer_; /*! Renderizador */
   ScreenCanvas *pScreenCanvas_; /*! Canvas de pantalla (para pasar al renderizador) */
   Mask *pMask_;  /*! Mascara de renderizacion */
   wxScrolledWindow *pScrolledWin_; /*! ventana de tipo scroll = pToolWindow_ */
   double windowUlx_; /*! UL visualizado upper left x */
   double windowUly_; /*! UL visualizado upper left y */
   WorldView<Viewer2D> *pWorldViewer_; /*! View del mundo */
   ListView<Viewer2D> *pListViewer_; /*! View de la lista */
   CallbackView<Viewer2D> *pElementViewer_; /*! View de elementos */
   int lastX_; /*! Utilizados para calcular el scroll x */
   int lastY_; /*! Utilizados para calcular el scroll y */
   wxTimer *pResizeTimer_; /*! Timer utilizado para el evento OnSize */
   wxTimer *pScrollTimer_; /*! Timer utilizado para los eventos Scroll */
   Subset newScrollWindow_; /*! Ventana nueva disparada por un evento scroll */
   WorldExtentManager* pWorldExtentManager_; /*! Determina extent de mundo al renderizar */
   Mode mode_; /*! Modo */
   /** Transformacion utilizada por el visualizador **/
   CoordinatesTransformation* pTransform_;
   DragEvent* pDragEvent_;
};

}

#endif /*VIEWER2D_H_*/
