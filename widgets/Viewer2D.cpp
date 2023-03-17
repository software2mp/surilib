/*! \file Viewer2D.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <cmath>
#include <vector>
#include <algorithm>

// Includes suri
#include "suri/Viewer2D.h"
#include "RenderizationManager.h"
#include "ScreenCanvas.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Dimension.h"
#include "suri/CoordinatesTransformInterface.h"
#include "suri/VectorElement.h"
#include "resources.h"
#include "suri/messages.h"
#include "suri/HtmlViewerPart.h"
#include "suri/PartContainerWidget.h"
#include "suri/ToggleButton.h"
#include "WorldWidget.h"
#include "AspectPreservingWorld.h"
#include "WorldButton.h"
#include "ZoomFullExtentButton.h"
#include "SaveViewButton.h"
#include "ZoomButton.h"
#include "Query2DButton.h"
#include "DragButton.h"
#include "suri/Vector.h"
#include "ZoomImageScaleButton.h"
#include "suri/WorldExtentManager.h"
#include "suri/NavigationButton.h"
#include "suri/PixelLineNavigationButton.h"
#include "SingleRasterElementActivationLogic.h"
#include "suri/VectorStyleManager.h"
#include "Mask.h"
#include "suri/ActiveRasterWorldExtentManager.h"
#include "GisWorldExtentManager.h"
#include "suri/Viewer2dTransformation.h"
#include "QuickMeasureButton.h"
#include "suri/DynamicViewerWidget.h"
#include "suri/RasterElement.h"
#include "suri/ViewportEvent.h"
#include "suri/DragEvent.h"
// Includes wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"

// Includes otros

/** Define ID del Timer */
#define WXTIMER_ID wxID_HIGHEST+1
/** Define ID para eventos del Timer. */
#define WX_SCROLL_TIMER_ID WXTIMER_ID+1
/** Tiempo del timer */
#define TIMER_WAIT_TIME_MS 250
/** Define la tolerancia al drag (Cantidad de pixles para los que no hace drag */
#define DRAG_TOLERANCE 20

/** namespace suri */
namespace suri {
// ------------------------ FIN HERRAMIENTAS TIPO BOTON ------------------------

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(Viewer2DEvent, Viewer2D)
   IMPLEMENT_EVENT_CALLBACK(OnSize, OnSize(); event.Skip(), wxSizeEvent)
   IMPLEMENT_EVENT_CALLBACK(OnIdle, OnIdle(), wxIdleEvent)
   IMPLEMENT_EVENT_CALLBACK(OnTimer, OnTimer(); event.Skip(), wxTimerEvent)
   IMPLEMENT_EVENT_CALLBACK(OnScrollTimer, OnScrollTimer(); event.Skip(), wxTimerEvent)
   IMPLEMENT_EVENT_CALLBACK(OnScrollRelease, OnScrollRelease(event), wxScrollWinEvent)
   IMPLEMENT_EVENT_CALLBACK(OnScrollPageUp, OnScrollPageUp(event), wxScrollWinEvent)
   IMPLEMENT_EVENT_CALLBACK(OnScrollPageDown, OnScrollPageDown(event), wxScrollWinEvent)
   IMPLEMENT_EVENT_CALLBACK(OnScrollLineUp, OnScrollLineUp(event), wxScrollWinEvent)
   IMPLEMENT_EVENT_CALLBACK(OnScrollLineDown, OnScrollLineDown(event), wxScrollWinEvent)
   IMPLEMENT_EVENT_CALLBACK(OnScrollTop, OnScrollTop(event), wxScrollWinEvent)
   IMPLEMENT_EVENT_CALLBACK(OnScrollBottom, OnScrollBottom(event), wxScrollWinEvent)
   IMPLEMENT_EVENT_CALLBACK(OnClose, OnClose(event), wxCloseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnWheelRotate, OnWheelRotate(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnMiddleDown, OnMiddleDown(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnMiddleUp, OnMiddleUp(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnMouseMove, OnMouseMove(event), wxMouseEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor
 * @param[in] pParent window donde se agregara el Viewer2D
 * @param[in] pDataViewManager administrador de info de la aplicacion 
 * @return instancia de la clase Viewer2D
 */
Viewer2D::Viewer2D(wxWindow *pParent, DataViewManager* pDataViewManager) :
      ViewerWidget(pParent, pDataViewManager), NEW_EVENT_OBJECT(Viewer2DEvent),
      pRenderer_(NULL),
      pMaskRenderer_(NULL), pScreenCanvas_(NULL), pMask_(NULL), pScrolledWin_(NULL),
      windowUlx_(0), windowUly_(0), pWorldViewer_(NULL), pListViewer_(NULL),
      pElementViewer_(NULL), lastX_(0), lastY_(0), pResizeTimer_(NULL),
      pScrollTimer_(NULL), newScrollWindow_(Subset()), pWorldExtentManager_(NULL),
      mode_(Raster), pTransform_(NULL) {
   windowTitle_ = caption_VIEWER2D;
   pWorld_ = new AspectPreservingWorld;
   pTransform_ = new Viewer2dTransformation(pWorld_);
   actualElement_ = NULL;
}

/**
 * Setea el modo
 * @param mode
 * 
 * \todo (danieltesta-#4287): Revisar el uso del elemento asociado (GetAssociatedElement)
 * junto con el uso (o no uso) de GisWorldExtentManager
 */
void Viewer2D::SetMode(Viewer2D::Mode mode) {
   switch (mode) {
      case Raster:
         ResetViewer();
         GetList()->SetActivationLogic(
               new SingleRasterElementActivationLogic(
                     this, message_RASTER_IMAGE_MUST_BE_DISPLAYED));
         SetWorldExtentManager(
               new ActiveRasterWorldExtentManager(GetList(), GetWorld()));
         break;
      case Gis:
         ResetViewer();
         GetList()->SetActivationLogic(NULL);
         SetWorldExtentManager(new GisWorldExtentManager(GetList(), GetWorld()));
         break;
      default:
         return;
   }

   mode_ = mode;
}

/**
 * Getea el modo
 * @return
 */
Viewer2D::Mode Viewer2D::GetMode() const {
   return mode_;
}

/** dtor */
Viewer2D::~Viewer2D() {
   // borro el canvas
   if (pRenderer_) {
      pRenderer_->SetMask(NULL);
      delete pRenderer_->SetCanvas(NULL);
   }
   if (pMaskRenderer_) {
      delete pMaskRenderer_->SetCanvas(NULL);
      delete pMask_;
   }
   // Conecto el evento de cierre de ventana contenedora
   if (GetFrame()) {
      GetFrame()->Disconnect(wxEVT_CLOSE_WINDOW,
                             wxCloseEventHandler(Viewer2DEvent::OnClose), NULL,
                             pEventHandler_);
   } else {
      REPORT_DEBUG("D: No se pudo desconectar el evento OnClose()");
   }
   // desconecta los timers
   wxCloseEvent dummyevent;
   OnClose(dummyevent);
   // desconecto el evento size
   DISCONNECT_TOOL_EVENT(Viewer2DEvent, OnSize, wxEVT_SIZE, wxSizeEventHandler);
   // desconecto el evento al dejar de mover un scrollbar
   DISCONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollTop, wxEVT_SCROLLWIN_TOP,
                         wxScrollWinEventHandler);
   DISCONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollBottom, wxEVT_SCROLLWIN_BOTTOM,
                         wxScrollWinEventHandler);
   DISCONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollLineUp, wxEVT_SCROLLWIN_LINEUP,
                         wxScrollWinEventHandler);
   DISCONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollLineDown, wxEVT_SCROLLWIN_LINEDOWN,
                         wxScrollWinEventHandler);
   DISCONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollPageUp, wxEVT_SCROLLWIN_PAGEUP,
                         wxScrollWinEventHandler);
   DISCONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollPageDown, wxEVT_SCROLLWIN_PAGEDOWN,
                         wxScrollWinEventHandler);
   DISCONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollRelease, wxEVT_SCROLLWIN_THUMBRELEASE,
                         wxScrollWinEventHandler);
   DISCONNECT_TOOL_EVENT(Viewer2DEvent, OnWheelRotate, wxEVT_MOUSEWHEEL, wxMouseEventHandler);
   DISCONNECT_TOOL_EVENT(Viewer2DEvent, OnMiddleDown, wxEVT_MIDDLE_DOWN, wxMouseEventHandler);
   // elimino el helper de eventos
   DELETE_EVENT_OBJECT;
   // Me desregistro
   if (Model::IsValid(pWorld_)) {
      pWorld_->UnregisterViewer(pWorldViewer_);
   }
   delete pWorldViewer_;

   suri::LayerList *playerlist = pListViewer_ ? pListViewer_->GetList() : NULL;
   if (Model::IsValid(playerlist)) {
      suri::LayerList::ElementListType::iterator it = playerlist->GetElementIterator();
      for (; it != playerlist->GetElementIterator(false); ++it) {
         suri::Element *pelement = *it;
         pelement->UnregisterViewer(pElementViewer_);
      }
   }
   delete pElementViewer_;
   // Elimino la listviewer. Se desregistra automaticamente
   delete pListViewer_;

   // borro el renderizador
   delete pRenderer_;
   delete pMaskRenderer_;
   // borro el manejador de extent de mundo
   delete pWorldExtentManager_;
   if (pTransform_) delete pTransform_;
}

/** Creacion de la ventana */
/**
 * Crea wxScrolledWindow y conecta los eventos del mismo
 * @return informa si se pudo crear la ventana
 */
bool Viewer2D::CreateToolWindow() {
   // creo la ventana con scroll
   pScrolledWin_ = new wxScrolledWindow(pParentWindow_, wxID_ANY, wxDefaultPosition,
                                        wxDefaultSize,
                                        wxHSCROLL | wxVSCROLL | wxALWAYS_SHOW_SB);
   pScrolledWin_->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
   // seteo != 0 para que aparezcan las barras
   pScrolledWin_->SetScrollRate(1, 1);
   // se la paso al toolwin
   pToolWindow_ = pScrolledWin_;
   // creo el canvas (asociado a la ventana de scroll)
   pScreenCanvas_ = new ScreenCanvas(pScrolledWin_);
   PushPaintEvent(pScreenCanvas_);
   // Creo la mascara
   pMask_ = new Mask;
   // llamo para que arme la ventana en caso de ser autonoma
   ViewerWidget::CreateToolWindow();
   // Conecto los eventos
   // Resize de la pantalla
   CONNECT_TOOL_EVENT(Viewer2DEvent, OnSize, wxEVT_SIZE, wxSizeEventHandler);
   // Scrollbar release
   CONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollTop, wxEVT_SCROLLWIN_TOP,
                      wxScrollWinEventHandler);
   CONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollBottom, wxEVT_SCROLLWIN_BOTTOM,
                      wxScrollWinEventHandler);
   CONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollLineUp, wxEVT_SCROLLWIN_LINEUP,
                      wxScrollWinEventHandler);
   CONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollLineDown, wxEVT_SCROLLWIN_LINEDOWN,
                      wxScrollWinEventHandler);
   CONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollPageUp, wxEVT_SCROLLWIN_PAGEUP,
                      wxScrollWinEventHandler);
   CONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollPageDown, wxEVT_SCROLLWIN_PAGEDOWN,
                      wxScrollWinEventHandler);
   CONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollRelease, wxEVT_SCROLLWIN_THUMBRELEASE,
                      wxScrollWinEventHandler);
   CONNECT_TOOL_EVENT(Viewer2DEvent, OnScrollRelease, wxEVT_SCROLLWIN_THUMBRELEASE,
                         wxScrollWinEventHandler);
   CONNECT_TOOL_EVENT(Viewer2DEvent, OnIdle, wxEVT_IDLE, wxIdleEventHandler);
   CONNECT_TOOL_EVENT(Viewer2DEvent, OnWheelRotate, wxEVT_MOUSEWHEEL, wxMouseEventHandler);
   CONNECT_TOOL_EVENT(Viewer2DEvent, OnMiddleDown, wxEVT_MIDDLE_UP , wxMouseEventHandler);
   CONNECT_TOOL_EVENT(Viewer2DEvent, OnMiddleUp,  wxEVT_MIDDLE_UP, wxMouseEventHandler);
   CONNECT_TOOL_EVENT(Viewer2DEvent, OnMouseMove, wxEVT_MOTION, wxMouseEventHandler);
   /*
    * No se conecta el evento Close ya que se modifican los Frames a los que
    * pertenecen los suri::Widget, en este caso, cuando se destruye un Contexto (
    * ventana contenedora de Viewer) el viewer adquiere otro frame, pero el evento
    * sigue conectado al frame original, por lo que al cerrar esta ventana se
    * realiza la llamada. ver TCK #526.
    * Pasos para reproducir.
    *  1. Usar SuriEdu/trunk@5470
    *  2. Abrir 2 imagenes
    *  3. Desplegar una de ellas
    *  4. Cerrar contexto activo
    *  5. Cerrar aplicacion
    */
#ifdef __UNUSED_CODE__
   // Conecto el evento de cierre de ventana contenedora
   GetFrame()->Connect(wxEVT_CLOSE_WINDOW,
         wxCloseEventHandler(Viewer2DEvent::OnClose),
         NULL, pEventHandler_);
#endif
   // Forma no estandar de conectarme al evento del timer
   pResizeTimer_ = new wxTimer(pEventHandler_, WXTIMER_ID);
   pEventHandler_->Connect(WXTIMER_ID, wxEVT_TIMER,
                           wxTimerEventHandler(Viewer2DEvent::OnTimer));
   pScrollTimer_ = new wxTimer(pEventHandler_, WX_SCROLL_TIMER_ID);
   pEventHandler_->Connect(WX_SCROLL_TIMER_ID, wxEVT_TIMER,
                           wxTimerEventHandler(Viewer2DEvent::OnScrollTimer));
   pDragEvent_ = new DragEvent(pDataViewManager_->GetViewportManager(),
                                        pDataViewManager_->GetViewcontextManager());
   ResetViewer();
   return pToolWindow_ != NULL;
}

void Viewer2D::OnWheelRotate(wxMouseEvent &Event) {
   int rotation = Event.GetWheelRotation();
   wxSize size = GetWindow()->GetSize();
   if (rotation > 0){
      ZoomIn(size.GetWidth() / 2, size.GetHeight() / 2);
   }
   if (rotation < 0) {
      ZoomOut(size.GetWidth() / 2, size.GetHeight() / 2);
   }
   return ;
}
void Viewer2D::OnMiddleDown(wxMouseEvent &Event) {
   pDragEvent_->OnMiddleDown(Event);
}

void Viewer2D::OnMouseMove(wxMouseEvent &Event) {
   pDragEvent_->OnMouseMove(Event);
}
void Viewer2D::OnMiddleUp(wxMouseEvent &Event) {
   pDragEvent_->OnMiddleUp(Event);
}
/** Resetea el View */
void Viewer2D::ResetViewer() {
   if (!pToolWindow_) {
      return;
   }
   pWorld_->ResetWorld();
   // Para que el bitmap pintado sea el mismo que el escrito (en este caso
   // en blanco) tiene que haber un nro igual de llamadas Clear / Flush
   pScreenCanvas_->Clear();
   pScreenCanvas_->Flush(NULL);
   int x, y;
   pScreenCanvas_->GetScreenSize(x, y);
   if (dynamic_cast<AspectPreservingWorld*>(pWorld_)) {
      dynamic_cast<AspectPreservingWorld*>(pWorld_)->SetMaxViewport(x, y);
   }
   pScrolledWin_->SetVirtualSize(x, y);
   ChangeViewport();
   windowUlx_ = 0;
   windowUly_ = 0;
   lastX_ = 0;
   lastY_ = 0;

   SetWindowTitle(caption_VIEWER2D);
}

// ------------------------ MODIFICACION DE MUNDO -------------------------

/** Hace ZoomIn */
/**
 * Obtengo la ventana anterior y calculo ventana Factor veces mas chica.
 * Obtengo las cordenadas espaciales del punto donde hizo click.
 * Cambio las coord de la ventana centrandola en donde hizo click y con
 *  nuevo tamano.
 * @param[in] X coord x del centro de la nueva ventana
 * @param[in] Y coord y del centro de la nueva ventana
 * @param[in] Factor factor por el que se hace zoom in
 */
void Viewer2D::ZoomIn(int X, int Y, int Factor, int Z) {
   Subset window;
   pWorld_->GetWindow(window);
   Dimension dim(window);
   // REPORT_DEBUG("D:ZoomIn");
   double deltax, deltay;
   // 2*Factor porque lo saco 2 veces (ej. por derecha e izquierda)
   deltax = dim.GetWidth() / (2 * Factor);
   deltay = dim.GetHeight() / (2 * Factor);
   int sx, sy;
   dim.GetSign(sx, sy);
   Coordinates windowpoint;
   pWorld_->V2WTransform(X, Y, windowpoint.x_, windowpoint.y_);
   ChangeWindow(
         Subset(windowpoint.x_ - sx * deltax, windowpoint.y_ - sy * deltay,
                windowpoint.x_ + sx * deltax, windowpoint.y_ + sy * deltay));
}

/** Hace ZoomOut */
/**
 * Obtengo la ventana anterior y calculo ventana Factor veces mas grande.
 * Obtengo las cordenadas espaciales del punto donde hizo click.
 * Cambio las coord de la ventana centrandola en donde hizo click y con
 * nuevo tamanio.
 * @param[in] X coord x del centro de la nueva ventana
 * @param[in] Y coord y del centro de la nueva ventana
 * @param[in] Factor factor por el que se hace zoom out
 */
void Viewer2D::ZoomOut(int X, int Y, int Factor, int Z) {
   Subset window;
   pWorld_->GetWindow(window);
   Dimension dim(window);
   // REPORT_DEBUG("D:ZoomOut");
   double deltax, deltay;
   deltax = static_cast<double>((dim.GetWidth() * Factor) / 2);
   deltay = static_cast<double>((dim.GetHeight() * Factor) / 2);
   // Trato de actualizar el viewport al tamanio de la ventana
   // sin enviar update
   ChangeViewport(true);
   int sx, sy;
   dim.GetSign(sx, sy);
   Coordinates windowpoint;
   pWorld_->V2WTransform(X, Y, windowpoint.x_, windowpoint.y_);
   ChangeWindow(
         Subset(windowpoint.x_ - sx * deltax, windowpoint.y_ - sy * deltay,
                windowpoint.x_ + sx * deltax, windowpoint.y_ + sy * deltay));
}

/**
 * Cambia la ventana del mundo
 * @param[in] NewWindow nuevo subset
 * @param[in] PreventUpdate flag que indica si deben actualizarce los viewers
 * luego de la modificacion.
 */
void Viewer2D::ChangeWindow(const Subset &NewWindow, bool PreventUpdate) {
   Subset window = NewWindow;
   TRY {
      pWorld_->PreventNextViewerUpdate(PreventUpdate);
      pWorld_->World::SetWindow(window);
   } CATCH {
      // Aca se estaba volviendo al subset anterior lo que traia problemas
      // con el zoom cuando en realidad no habia ningun problema en ningun
      // lado. Si hicieron varias pruebas con imagenes con SRS geograficos
      // y proyectados, con y sin vectores, y no hubo problemas.
      // - Deshacer cambio si hace falta -
   }
   pWorld_->GetWindow(newScrollWindow_);
   AdjustVirtualSize();
}

/**
 * Guarda el WorldExtentManager y le pasa al RenderizationManager el mismo
 * WorldExtentManager
 * \pre Se ejecuto DoSetElementList
 * @param[in] pExtentManager WorldExtentManager a guardar
 * \attention El WorldExtentManager es responsabilidad de esta clase,
 * no eliminar externamente.
 */
void Viewer2D::SetWorldExtentManager(WorldExtentManager* pExtentManager) {
   delete pWorldExtentManager_;
   pWorldExtentManager_ = pExtentManager;
   if (!pRenderer_) {
      REPORT_AND_FAIL("D:Se trato de setear WorldExtentManager sin renderizador.");
   }
   pRenderer_->SetWorldExtentManager(pExtentManager);
   if (!pMaskRenderer_) {
      REPORT_AND_FAIL("D:Se trato de setear WorldExtentManager sin pMaskRenderer_.");
   }
   pMaskRenderer_->SetWorldExtentManager(pExtentManager);
}

/**
 *  Metodo Update que se ejecuta cuando pListViewer_, pWorldViewer_ o
 *  pElementViewer_ es modificado.
 *  Es el callback que se registra en pListViewer_, pWorldViewer_,
 *  pElementViewer_.
 *  Invoca a CheckRenderizationStatus para verificar el resultado de
 *  la renderizacion.
 *  @param[in] pModel modelo del objeto modificado
 */
void Viewer2D::UpdateViewers(Model *pModel) {
   /**
    * Fix temporal para no realizar acciones si el visualizador activo no es 2D
    */
   DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(
         pDataViewManager_->GetViewportManager()->GetSelectedViewport());
   if (pdynamic && !dynamic_cast<Viewer2D*>(pdynamic->GetViewer()))
      return;
   CheckRenderizationStatus();
   AdjustVirtualSize();
}

/**
 * Verifica el estado de la renderizacion y en caso de error emite un mensaje
 * con los nombres de los elementos que no pudieron ser renderizados.
 */
void Viewer2D::CheckRenderizationStatus() {
   if (!pRenderer_->GetRenderizationStatus()
         && pListViewer_->GetList()->GetActiveCount() != 0) {
      REPORT_DEBUG("D:Error en la renderizacion de los elementos.");
      std::vector<Element*> notrenderizedelements =
            pRenderer_->GetNotRenderizedElements();
      std::vector<Element*>::iterator it = notrenderizedelements.begin();
      for (; it != notrenderizedelements.end(); it++)
         if ((*it)) {
            (*it)->Activate(false);
            SHOW_ERROR(message_RENDERER_ELEMENT_ERROR_s, (*it)->GetName().c_str());
         }
   }
}

/**
 * Permite obtener la ventana con las herramientas del viewer. Si no habia
 * una configurada le agrega una nueva a pParent y le carga todos los botones
 * del Viewer
 * @param[in] pParent window donde agrega el panel con botones. Luego
 * configura toolbar con la ventana devuelta.
 * @return toolbar de la herramienta. Si ya estaba configurada no
 * la modifica, caso contrario setea la creada en pParent.
 * \todo Conectar los eventos
 */


wxWindow *Viewer2D::DoGetViewerTools(wxWindow *pParent) {
   wxWindow* ptoolbar = GetToolbar();
   Button* pbutton = NULL;
   // Si existe el toolbar como propiedad lo devuelve
   if (ptoolbar) {
      return ptoolbar;
   }

   // Sino lo arma y lo devuelve al final
   ptoolbar = new wxPanel(pParent, wxID_ANY);
   ptoolbar->SetSizer(new wxBoxSizer(wxHORIZONTAL));
   InitializeToolbar(ptoolbar, this, GetList());

   // SAVE SCREEN BUTTON-----------------------------------------------
   pbutton = new SaveViewButton(ptoolbar);
   pViewerToolButtons_.insert(pbutton);
   AddButtonTool(pbutton, ViewerWidget::First, ViewerWidget::StandAlone);

   // INSERT TOGGLE BUTTON FOR DRAG
   pbutton = new DragButton(ptoolbar, this);
   pViewerToolButtons_.insert(pbutton);
   AddButtonTool(pbutton, Last, Navigation);
   ActivateButton(pbutton);

   // INSERT TOGGLE BUTTON FOR ZOOM
   pbutton = new ZoomButton(ptoolbar, this);
   pViewerToolButtons_.insert(pbutton);
   AddButtonTool(pbutton, Last, Navigation);

   // INSERT TOGGLE BUTTON FOR QUERY
   pbutton = new Query2DButton(ptoolbar, this, pDataViewManager_);
   pViewerToolButtons_.insert(pbutton);
   AddButtonTool(pbutton, Last, Navigation);

   //NAVIGATION BUTTON-------------------------------------------
   pbutton = new NavigationButton(ptoolbar, this);
   pViewerToolButtons_.insert(pbutton);
   AddButtonTool(pbutton, ViewerWidget::Last, Navigation);
   /*AddButtonTool(new PixelLineNavigationButton(ptoolbar, this), Last,
    Navigation | Edition);*/
   // ZOOM 1:1 BUTTON-------------------------------------------
   pbutton = new ZoomImageScaleButton(ptoolbar, this);
   pViewerToolButtons_.insert(pbutton);
   AddButtonTool(pbutton, ViewerWidget::Last, ViewerWidget::StandAlone);

   // FIT TO VIEWPORT BUTTON-------------------------------------------
   pbutton = new ZoomFullExtentButton(ptoolbar, this);
   pViewerToolButtons_.insert(pbutton);
   AddButtonTool(pbutton, ViewerWidget::Last, ViewerWidget::StandAlone);

#ifdef __ENABLE_WORLDWATCHER_TOOL__
   // WORLD WATCHER BUTTON---------------------------------------------
   AddButtonTool(new WorldButton(ptoolbar, pWorld_), ViewerWidget::Last,
         ViewerWidget::StandAlone);
#endif   // __ENABLE_WORLDWATCHER_TOOL__
   pbutton = new QuickMeasureButton(ptoolbar, this);
   pViewerToolButtons_.insert(pbutton);
   AddButtonTool(pbutton, ViewerWidget::Last, Navigation);

   // DEVUELVE EL TOOLBAR
   return GetToolbar();
}


/////////////////////////

/**
 * Despliega un elemento (puede ser funcion de RenderizationControl)
 * @param[in] pElement elemento desplegado
 */
void Viewer2D::DisplayElement(Element *pElement) {
   // ("Muestro Elemento");
   if (dynamic_cast<RasterElement*>(pElement) && (pElement != actualElement_) )
      ResetViewer();
   if (pWorld_->GetSpatialReference().empty()) {
      // Se bloquea al setear spatial reference porque al cambiar el worldwidow se renderizaba por
      // 2da vez
      pWorld_->BlockViewerUpdate();
      pWorld_->SetSpatialReference(pElement->GetSpatialReference().c_str());
      pWorld_->UnblockViewerUpdate();
   }
   actualElement_ = pElement;
}

/**
 * Oculta un elemento (puede ser funcion de RenderizationControl)
 * @param[in] pElement elemento que se desea ocultar
 */
void Viewer2D::HideElement(Element *pElement) {
   // REPORT_DEBUG("D:Oculto Elemento");
   // Se resetea el viewer para que, en modo GIS, el cálculo del nuevo extent
   // del mundo deje de tomar en cuenta la capa recién ocultada
   if (dynamic_cast<RasterElement*>(pElement))
      ResetViewer();
}

/**
 * El elemento ha sido modificado
 * @param[in] pElement elemento modificado
 */
void Viewer2D::UpdateElement(Element *pElement) {
   // REPORT_DEBUG("D:Elemento modificado");
}

/**
 * Se le setea la lista de elementos
 * @param[in] pList nueva lista de elementos
 */
void Viewer2D::DoSetElementList(LayerList *pList) {
#ifdef __RENDER_VISUALIZATION_USING_MASK__
   LayerList *plist = new LayerList;
   VectorElement *pvectormask = dynamic_cast<VectorElement*>(
         Element::Create("VectorElement", __RENDER_VISUALIZATION_USING_MASK__));
   pvectormask->SetName("D:Rendered Mask");
   pvectormask->SetStyle(VectorStyleManager::Instance().
         GetDefaultStyle("mask", Vector::Polygon)->GetWkt());
   pvectormask->Activate(true);
   plist->AddElement(pvectormask);
   pMaskRenderer_ = new RenderizationManager(plist, pWorld_);
   pMaskRenderer_->SetCanvas(pMask_);
#endif  // __RENDER_VISUALIZATION_USING_MASK__
   // creo el renderizador y le paso la lista y el mundo
   pRenderer_ = new RenderizationManager(pList, pWorld_);
   // Se lo seteo al renderizador
   pRenderer_->SetCanvas(pScreenCanvas_);
   pRenderer_->SetMask(pMask_);
   // Esta listo para renderizar

   // -----------------------------------------------------

   // Creo el observador del mundo
   pWorldViewer_ = new WorldView<Viewer2D>(this, &Viewer2D::UpdateViewers);
   pWorld_->RegisterViewer(pWorldViewer_);
   // Creo el observador de la lista
   pListViewer_ = new ListView<Viewer2D>(this, &Viewer2D::UpdateViewers);
   pList->RegisterViewer(pListViewer_);
   // creo el  observador de elementos
   pElementViewer_ = new CallbackView<Viewer2D>(this, &Viewer2D::UpdateViewers);
}

// --------------------------------- EVENTOS --------------------------------

/** evento OnSize */
/**
 * \todo Implementar el sistema Timer/Resize para deferir la modificacion del WW
 */
void Viewer2D::OnSize() {
   pResizeTimer_->Start(TIMER_WAIT_TIME_MS, wxTIMER_ONE_SHOT);
}

/** evento OnIdle */
void Viewer2D::OnIdle() {
}

/**
 * evento OnTimer (para la modificacion del World frente a un resize)
 */
void Viewer2D::OnTimer() {
   if (RenderizationManager::GetRendering()) {
      REPORT_DEBUG("D:Disparando timer porque hay proceso renderizando");
      pResizeTimer_->Start(TIMER_WAIT_TIME_MS, wxTIMER_ONE_SHOT);
      return;
   }
   REPORT_DEBUG("D:VIEWER2D::ONTIMER");
   // Cambio el tamanio del viewport
   ChangeViewport();
}

/** evento OnTimer (para la modificacion del World frente a un resize) */
void Viewer2D::OnScrollTimer() {
   REPORT_DEBUG("D:VIEWER2D::ONSCROLLTIMER");
   // Cambio el tamanio del viewport
   ChangeWindow(newScrollWindow_);
}

/** evento de scroll */
/**
 * Dependiendo del scroll utilizado se se hace la transformacion del
 * mundo correspondiente.
 * @param[in] Event evento de scroll
 */
void Viewer2D::OnScrollRelease(wxScrollWinEvent &Event) {
   REPORT_DEBUG("D:VIEWER2D::ONSCROLLRELEASE");
   double oldulx = windowUlx_, olduly = windowUly_;
   switch (Event.GetOrientation()) {
      case wxHORIZONTAL: {
         double newulx, dummy = 0;
         pWorld_->V2WTransform(Event.GetPosition() - lastX_, 0, newulx, dummy);
         lastX_ = Event.GetPosition();
         windowUlx_ = newulx;
         break;
      }
      case wxVERTICAL: {
         double newuly, dummy = 0;
         pWorld_->V2WTransform(0, Event.GetPosition() - lastY_, dummy, newuly);
         lastY_ = Event.GetPosition();
         windowUly_ = newuly;
         break;
      }
      default:
         break;
   }
   int x, y;
   pScrolledWin_->GetClientSize(&x, &y);
   double winlrx = 0, winlry = 0;
   pWorld_->V2WTransform(x, y, winlrx, winlry);
   Subset window(windowUlx_, windowUly_, windowUlx_ + winlrx - oldulx,
                 windowUly_ + winlry - olduly);
   ChangeWindow(window);
   Event.Skip();
}

/**
 * Modifica la ventana
 * @param[in] Event evento de scroll
 */
void Viewer2D::OnScrollPageUp(wxScrollWinEvent &Event) {
   REPORT_DEBUG("D:VIEWER2D::ONSCROLLPAGEUP");
   AdjustScrollbars(Event.GetOrientation(),
                    -pScrolledWin_->GetScrollThumb(Event.GetOrientation()));
   Event.Skip();
}

/**
 * Modifica la ventana
 * @param[in] Event evento de scroll
 */
void Viewer2D::OnScrollPageDown(wxScrollWinEvent &Event) {
   REPORT_DEBUG("D:VIEWER2D::ONSCROLLPAGEDOWN");
   AdjustScrollbars(Event.GetOrientation(),
                    pScrolledWin_->GetScrollThumb(Event.GetOrientation()));
}

/**
 * Modifica la ventana
 * @param[in] Event evento de scroll
 */
void Viewer2D::OnScrollLineUp(wxScrollWinEvent &Event) {
   REPORT_DEBUG("D:VIEWER2D::ONSCROLLLINEUP");
   AdjustScrollbars(Event.GetOrientation(), -1);
   Event.Skip();
}

/**
 * Modifica la ventana
 * @param[in] Event evento de scroll
 */
void Viewer2D::OnScrollLineDown(wxScrollWinEvent &Event) {
   REPORT_DEBUG("D:VIEWER2D::ONSCROLLLINEDOWN");
   AdjustScrollbars(Event.GetOrientation(), 1);
   Event.Skip();
}

/**
 * Modifica la ventana
 * @param[in] Event evento de scroll
 */
void Viewer2D::OnScrollTop(wxScrollWinEvent &Event) {
   REPORT_DEBUG("D:VIEWER2D::ONMSCROLLTOP");
   // REPORT_DEBUG("D:Top");
   Event.Skip();
}

/**
 * Modifica la ventana
 * @param[in] Event evento de scroll
 */
void Viewer2D::OnScrollBottom(wxScrollWinEvent &Event) {
   REPORT_DEBUG("D:VIEWER2D::ONSCROLLBOTTOM");
   // REPORT_DEBUG("D:Bottom");
   Event.Skip();
}

/** Ajusta las barras de scroll */
/**
 * Usando las coordenadas del viewport anterior y la orientacion e incremento
 * calcula la nueva ventana.
 * Finalmente inicializa pScrollTimer_ para que cambie la ventana.
 * @param[in] Orientation orientacion del movimiento
 * @param[in] Increment magnitud del desplazamiento
 */
void Viewer2D::AdjustScrollbars(int Orientation, int Increment) {
   int x, y;
   x = pScrolledWin_->GetScrollPos(wxHORIZONTAL);
   y = pScrolledWin_->GetScrollPos(wxVERTICAL);
   // REPORT_DEBUG("D:Real FROM: ScrollPosition %d;%d",x,y);
   Coordinates previouslr;
   pWorld_->V2WTransform(x, y, previouslr.x_, previouslr.y_);
   switch (Orientation) {
      case wxHORIZONTAL: {
         x += Increment;
         x = std::max(x, 0);
#ifndef __WXGTK__
         x = std::min(
               x,
               pScrolledWin_->GetScrollRange(Orientation)
                     - pScrolledWin_->GetScrollThumb(Orientation));
#else
         x = std::min(x, pScrolledWin_->GetScrollRange(Orientation));
#endif
         break;
      }
      case wxVERTICAL: {
         y += Increment;
         y = std::max(y, 0);
#ifndef __WXGTK__
         y = std::min(
               y,
               pScrolledWin_->GetScrollRange(Orientation)
                     - pScrolledWin_->GetScrollThumb(Orientation));
#else
         y = std::min(y, pScrolledWin_->GetScrollRange(Orientation));
#endif
         break;
      }
      default:
         break;
   }
   // REPORT_DEBUG("D:TO: ScrollPosition %d;%d",x,y);
   Coordinates afterlr;
   pWorld_->V2WTransform(x, y, afterlr.x_, afterlr.y_);
   Dimension dim(Subset(previouslr, afterlr));
   newScrollWindow_ = newScrollWindow_ + dim;
   pScrollTimer_->Start(2 * TIMER_WAIT_TIME_MS, wxTIMER_ONE_SHOT);
}

/**
 * evento de cierre de ventana que contiene a la herramienta
 * @param[in] Event evento que cierra la ventana
 */
void Viewer2D::OnClose(wxCloseEvent &Event) {
   // REPORT_DEBUG("D:Cerrando, desconectando timer OnResize");
   pResizeTimer_->Stop();
   pEventHandler_->Disconnect(WXTIMER_ID, wxEVT_TIMER,
                              wxTimerEventHandler(Viewer2DEvent::OnTimer));
   pScrollTimer_->Stop();
   pEventHandler_->Disconnect(WX_SCROLL_TIMER_ID, wxEVT_TIMER,
                              wxTimerEventHandler(Viewer2DEvent::OnScrollTimer));
}

/**
 * Cambia el tamano del viewport al del screen canvas
 * @param[in] PreventUpdate indica si hay que actualizar los viewers luego de la
 * modificacion.
 */
void Viewer2D::ChangeViewport(bool PreventUpdate) {
   // previene la renderizacion si la ventana no esta siendo mostrada
   // if (!pToolWindow_->IsShown() || !pToolWindow_->IsShownOnScreen()) return;
   int x = 0, y = 0;
   pScreenCanvas_->GetScreenSize(x, y);
   int vpx = 0, vpy = 0;
   pWorld_->GetViewport(vpx, vpy);
   if (vpx != x || vpy != y) {
      TRY
      {
         pWorld_->PreventNextViewerUpdate(PreventUpdate);
         pWorld_->SetViewport(x, y);
      }
      CATCH {
         pWorld_->SetViewport(vpx, vpy);
      }
   } else {
      REPORT_DEBUG("D:Viewport == ScreenCanvas");
   }
   AdjustVirtualSize();
}

/** Ajusta los tamanios virtuales */
void Viewer2D::AdjustVirtualSize() {
   Subset window;
   pWorld_->GetWindow(window);
   windowUlx_ = window.ul_.x_;
   windowUly_ = window.ul_.y_;

   Subset world;
   pWorld_->GetWorld(world);
   int vulx = 0, vuly = 0, vlrx = 0, vlry = 0, winulx = 0, winuly = 0, winlrx = 0,
         winlry = 0;
   // el maximo entre las coordenadas del mundo y la ventana
   pWorld_->W2VTransform(world.ul_.x_, world.ul_.y_, vulx, vuly);
   pWorld_->W2VTransform(windowUlx_, windowUly_, winulx, winuly);
   vulx = std::min(vulx, winulx);
   vuly = std::min(vuly, winuly);
   pWorld_->W2VTransform(world.lr_.x_, world.lr_.y_, vlrx, vlry);
   pWorld_->W2VTransform(window.lr_.x_, window.lr_.y_, winlrx, winlry);
   vlrx = std::max(vlrx, winlrx);
   vlry = std::max(vlry, winlry);

   lastX_ = -vulx;
   lastY_ = -vuly;

// Codigo para debuguear la posicion
#ifdef __UNUSED_CODE__
   int tempx = 0, tempy = 0, tempxv = 0, tempyv = 0;
   REPORT_DEBUG("D:Cambiando Tamano virtual a %d,%d", vlrx-vulx, vlry-vuly);
   tempx = pScrolledWin_->GetScrollPos(wxHORIZONTAL);
   tempy = pScrolledWin_->GetScrollPos(wxVERTICAL);
   REPORT_DEBUG("D:Posicion anterior del thumb: %d;%d", tempx, tempy);
   tempxv = pScrolledWin_->GetScrollThumb(wxHORIZONTAL);
   tempyv = pScrolledWin_->GetScrollThumb(wxVERTICAL);
   REPORT_DEBUG("D:Tamanio del thumb: %d;%d", tempxv, tempyv);

   REPORT_DEBUG("D:Cambiando posicion del thumb %d;%d", lastX_, lastY_);
#endif

   pScrolledWin_->Freeze();
   pScrolledWin_->SetScrollbar(wxHORIZONTAL, lastX_, winlrx - winulx, vlrx - vulx,
                               true);
   pScrolledWin_->SetScrollbar(wxVERTICAL, lastY_, winlry - winuly, vlry - vuly, true);
   pScrolledWin_->SetVirtualSize(vlrx - vulx, vlry - vuly);
   // (Pablo-TCK #10637): saqué el #ifdef __WXMSW__ que encerraba a la línea de abajo:
   // ( pScrolledWin_->Scroll(lastX_, lastY_); )
   pScrolledWin_->Scroll(lastX_, lastY_);
   pScrolledWin_->Thaw();
}

/** Obtiene la transformacion asociada **/
CoordinatesTransformation* Viewer2D::GetTransformation() {
   return pTransform_;
}
}
