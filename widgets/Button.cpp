/*! \file Button.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Include suri
#include "suri/Button.h"
#include "suri/messages.h"
#include "suri/ViewerWidget.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Configuration.h"

/** namespace suri */
namespace suri {
// -------------------------- EVENTOS DE LOS BOTONES ---------------------------
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ButtonEvent, Button)
   IMPLEMENT_EVENT_CALLBACK(OnButtonClicked, OnButtonClicked(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLeft, OnLeft(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLeftUp, OnLeftUp(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLeftDown, OnLeftDown(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLeftDoubleClick, OnLeftDoubleClick(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRight, OnRight(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRightUp, OnRightUp(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRightDown, OnRightDown(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRightDoubleClick, OnRightDoubleClick(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnMiddle, OnMiddle(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnMiddleUp, OnMiddleUp(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnMiddleDown, OnMiddleDown(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnMiddleDoubleClick, OnMiddleDoubleClick(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnEnterWindow, OnEnterWindow(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLeaveWindow, OnLeaveWindow(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnWheel, OnWheel(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnMouseMove, OnMouseMove(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnMouse, OnMouse(event), wxMouseEvent)
   IMPLEMENT_EVENT_CALLBACK(OnPaint, OnPaint(event), wxPaintEvent)
   IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
/** \endcond */
END_IMPLEMENT_EVENT_OBJECT
/** Macro para ejecutar metodos previos al evento */
#define PRE_EVENT(PreExecute) do {                                            \
      if ( !GetValue() ) { return; }                                                   \
      PreExecute; } while ( 0 )

/** Macro para ejecutar metodos posteriores al evento */
#define POST_EVENT(PostExecute)  do {                                         \
      PostExecute; } while ( 0 )

/** Macro para ejecutar metodos durante el evento */
#define ON_EVENT(PreExecute, EventName, PostExecute) do {                       \
      /*Si no hay elementos activos no hace nada*/                               \
      if ( pList_&&pList_->GetActiveCount(true)==0 ) {                                \
         return; }                                                                 \
      PRE_EVENT(PreExecute);                                                     \
      REPORT_DEBUG("D:db = %d ; drag = %d ; inside = %d",                        \
                   debounceLeft_, isDraggingLeft_, insideLeft_);                       \
      REPORT_DEBUG(# EventName);                                                  \
      Do ## EventName(Event);                                                      \
      POST_EVENT(PostExecute); } while ( 0 )

/** Macro para modificar los estados del boton */
#define FLAGS(State, Button)   do {                                            \
      debounce ## Button ## _ = isDragging ## Button ## _ = inside ## Button ## _ = State;   \
}  while ( 0 )

/**
 * @param[in] pViewerToolbar Ventana donde se incorpora el boton
 * @param[in] ButtonTooltip Texto de la ayuda emergente del boton
 * @param[in] Flag Mascara que define los eventos de mouse que se conectan sobre
 *            la ventana del visualizador
 * \todo Pensar un mecanismo para conectar eventos de teclado
 */
Button::Button(wxWindow *pViewerToolbar, wxString ButtonTooltip, int Flag) :
      Widget(pViewerToolbar), pViewerWindow_(NULL), pViewer_(NULL), pList_(NULL),
      state_(false), enabled_(true), eventMask_(Flag), pBitmap_(NULL),
      debounceLeft_(false), isDraggingLeft_(false), insideLeft_(false),
      debounceMiddle_(false), isDraggingMiddle_(false), insideMiddle_(false),
      debounceRight_(false), isDraggingRight_(false), insideRight_(false),
      eventType_(wxEVT_COMMAND_BUTTON_CLICKED), NEW_EVENT_OBJECT(ButtonEvent)
{
   // -8 porque da en forma empirica para winXP
   wantedWidth_ = -1;
   wantedHeight_ = BUTTON_SIZE - 8;
   windowTitle_ = _(ButtonTooltip);
}

/**
 * @param[in] pViewerToolbar Ventana donde se incorpora el boton
 * @param[in] Bitmap mapa de bits para setear al boton
 * @param[in] ButtonTooltip Texto de la ayuda emergente del boton
 * @param[in] Flag Mascara que define los eventos de mouse que se conectan sobre
 *            la ventana del visualizador
 * \todo Pensar un mecanismo para conectar eventos de teclado
 */
Button::Button(wxWindow *pViewerToolbar, const wxBitmap &Bitmap, wxString ButtonTooltip,
               int Flag) :
      Widget(pViewerToolbar), pViewerWindow_(NULL), pViewer_(NULL), pList_(NULL),
      state_(false), enabled_(true), eventMask_(Flag), pBitmap_(new wxBitmap(Bitmap)),
      debounceLeft_(false), isDraggingLeft_(false), insideLeft_(false),
      debounceMiddle_(false), isDraggingMiddle_(false), insideMiddle_(false),
      debounceRight_(false), isDraggingRight_(false), insideRight_(false),
      eventType_(wxEVT_COMMAND_BUTTON_CLICKED), NEW_EVENT_OBJECT(ButtonEvent)
{
   // -8 porque da en forma empirica para winXP
   wantedWidth_ = -1;
   wantedHeight_ = BUTTON_SIZE - 8;
   windowTitle_ = _(ButtonTooltip);
}

/**
 * @param[in] pViewerToolbar Ventana donde se incorpora el boton
 * @param[in] Bitmap string de recurso para obtener el bitmap
 * @param[in] ButtonTooltip Texto de la ayuda emergente del boton
 * @param[in] Flag Mascara que define los eventos de mouse que se conectan sobre
 *            la ventana del visualizador
 * \todo Pensar un mecanismo para conectar eventos de teclado
 */
Button::Button(wxWindow *pViewerToolbar, const wxString &Bitmap, wxString ButtonTooltip,
               int Flag) :
      Widget(pViewerToolbar), pViewerWindow_(NULL), pViewer_(NULL), pList_(NULL),
      state_(false), enabled_(true), eventMask_(Flag), pBitmap_(NULL),
      debounceLeft_(false), isDraggingLeft_(false), insideLeft_(false),
      debounceMiddle_(false), isDraggingMiddle_(false), insideMiddle_(false),
      debounceRight_(false), isDraggingRight_(false), insideRight_(false),
      eventType_(wxEVT_COMMAND_BUTTON_CLICKED), NEW_EVENT_OBJECT(ButtonEvent)
{
   // -8 porque da en forma empirica para winXP
   wantedWidth_ = -1;
   wantedHeight_ = BUTTON_SIZE - 8;
   windowTitle_ = _(ButtonTooltip);
   pBitmap_ = new wxBitmap;
   GET_BITMAP_RESOURCE(Bitmap.c_str(), *pBitmap_);
}

Button::~Button() {
   RemoveHandlerFromViewerWindow();
   delete pBitmap_;
   DELETE_EVENT_OBJECT
   ;
}

/**
 * Asigna el cursor de mouse default al viewer
 * @param[in] State estado de activacion de la herramienta
 * \pre Se debe haber inicializado la ventana de Viewer con
 *      SetViewerWindow()
 * \post El cursor sobre la ventana del viewer cambia a default
 *      (flechita).
 */
void Button::DoSetCursor(bool State) {
   // TCK - 4447 Como se reactivo la llamada, se comento el contenido
   // para que en SOPI no se ejecute.
   if (pViewerWindow_) {
      pViewerWindow_->SetCursor(wxCursor(wxCURSOR_DEFAULT));
   }
}

/**
 * Metodo de creacion del boton-herramienta
 * Conecto el evento del mouse, luego usando eventMask_ decido que otros
 * eventos conectar.
 * @return bool que indica si se pudo crear la ventana
 * \todo: Ver que pasa con doble click que genera Down-Up-DClick-Up
 */
bool Button::CreateToolWindow() {
   bool result = DoCreateToolWindow();
   if (!pToolWindow_) {
      REPORT_AND_FAIL_VALUE("D:No se creo el boton", false);
   }
   pToolWindow_->Enable(IsEnabled());
   /** Se comenta esta linea ya que acomoda la dimension de varios 
    *  botones en la toolbar principal para entorno unity **/
   // pToolWindow_->SetMaxSize(wxSize(BUTTON_SIZE, BUTTON_SIZE));
   pToolWindow_->SetToolTip(GetWindowTitle());
   // Conecto el evento que maneja el boton
   pToolWindow_->Connect(eventType_,
                         wxCommandEventHandler(ButtonEvent::OnButtonClicked), NULL,
                         pEventHandler_);
   // Conecto el evento OnUIUpdate
   pToolWindow_->Connect(wxEVT_UPDATE_UI,
                         wxUpdateUIEventHandler(ButtonEvent::OnUIUpdate), NULL,
                         pEventHandler_);

   if (eventMask_ != BUTTON_NULL) {
      if (!pViewerWindow_) {
         REPORT_AND_FAIL_VALUE("D:No se definio la ventana de viewer", false);
      }
      // conecto los eventos deseados si el deseado no es todos los eventos
      if (eventMask_ & BUTTON_LEFT_DOWN) {
         pViewerWindow_->Connect(wxEVT_LEFT_DOWN,
                                 wxMouseEventHandler(ButtonEvent::OnLeftDown), NULL,
                                 pEventHandler_);
      }
      if (eventMask_ & BUTTON_LEFT_UP) {
         pViewerWindow_->Connect(wxEVT_LEFT_UP,
                                 wxMouseEventHandler(ButtonEvent::OnLeftUp), NULL,
                                 pEventHandler_);
      }
      if (eventMask_ & BUTTON_MIDDLE_DOWN) {
         pViewerWindow_->Connect(wxEVT_MIDDLE_DOWN,
                                 wxMouseEventHandler(ButtonEvent::OnMiddleDown), NULL,
                                 pEventHandler_);
      }
      if (eventMask_ & BUTTON_MIDDLE_UP) {
         pViewerWindow_->Connect(wxEVT_MIDDLE_UP,
                                 wxMouseEventHandler(ButtonEvent::OnMiddleUp), NULL,
                                 pEventHandler_);
      }
      if (eventMask_ & BUTTON_RIGHT_DOWN) {
         pViewerWindow_->Connect(wxEVT_RIGHT_DOWN,
                                 wxMouseEventHandler(ButtonEvent::OnRightDown), NULL,
                                 pEventHandler_);
      }
      if (eventMask_ & BUTTON_RIGHT_UP) {
         pViewerWindow_->Connect(wxEVT_RIGHT_UP,
                                 wxMouseEventHandler(ButtonEvent::OnRightUp), NULL,
                                 pEventHandler_);
      }
      if (eventMask_ & BUTTON_LEFT_DOUBLECLICK) {
         pViewerWindow_->Connect(wxEVT_LEFT_DCLICK,
                                 wxMouseEventHandler(ButtonEvent::OnLeftDoubleClick),
                                 NULL, pEventHandler_);
      }
      if (eventMask_ & BUTTON_MIDDLE_DOUBLECLICK) {
         pViewerWindow_->Connect(wxEVT_MIDDLE_DCLICK,
                                 wxMouseEventHandler(ButtonEvent::OnMiddleDoubleClick),
                                 NULL, pEventHandler_);
      }
      if (eventMask_ & BUTTON_RIGHT_DOUBLECLICK) {
         pViewerWindow_->Connect(wxEVT_RIGHT_DCLICK,
                                 wxMouseEventHandler(ButtonEvent::OnRightDoubleClick),
                                 NULL, pEventHandler_);
      }
      if (eventMask_ & BUTTON_ENTER) {
         pViewerWindow_->Connect(wxEVT_ENTER_WINDOW,
                                 wxMouseEventHandler(ButtonEvent::OnEnterWindow), NULL,
                                 pEventHandler_);
      }
      if (eventMask_ & BUTTON_LEAVE) {
         pViewerWindow_->Connect(wxEVT_LEAVE_WINDOW,
                                 wxMouseEventHandler(ButtonEvent::OnLeaveWindow), NULL,
                                 pEventHandler_);
      }
      if (eventMask_ & BUTTON_WHEEL) {
         pViewerWindow_->Connect(wxEVT_MOUSEWHEEL,
                                 wxMouseEventHandler(ButtonEvent::OnWheel), NULL,
                                 pEventHandler_);
      }
      // si hay algun evento de boton, activa el de boton movimiento
      if ((eventMask_ & BUTTON_MOVE) || (eventMask_ & BUTTON_ANY_BUTTON)) {
         pViewerWindow_->Connect(wxEVT_MOTION,
                                 wxMouseEventHandler(ButtonEvent::OnMouseMove), NULL,
                                 pEventHandler_);
      }
      if (eventMask_ & WINDOW_PAINT) {
         AddHandlerToViewerWindowBeginning();

         pEventHandler_->Connect(wxEVT_PAINT, wxPaintEventHandler(ButtonEvent::OnPaint),
                                 NULL);
      }
   }
   return result;
}

/**
 * setea el estado del boton
 * @param[in] State estado del boton
 */
bool Button::SetValue(bool State) {
   if (!DoSetValue(State)) {
      return false;
   }
   state_ = State;
   // TCK - 4447 Se descomento para que en SIMUR se configure el cursor
   DoSetCursor(state_);
   FLAGS(false, Left);
   FLAGS(false, Middle);
   FLAGS(false, Right);
   return true;
}

/**
 * Setea la ventana del viewer
 * @param[in] pViewer viewer
 */
void Button::SetViewer(ViewerWidget *pViewer) {
   pViewer_ = pViewer;
   pViewerWindow_ = pViewer_ ? pViewer_->GetWindow() : NULL;
}

/**
 * retorna un bitmap para poner en el boton
 * @return bitmap para el boton
 */
wxBitmap Button::GetBitmap() {
   wxBitmap bitmap(DoGetBitmap());
   // si no es valido, obtengo uno default
   if (!bitmap.IsOk()) {
      bitmap = wxArtProvider::GetBitmap(wxART_MISSING_IMAGE, wxART_OTHER);
   }
   // en GTK hace escala solo
#ifndef __WXGTK__
   int newwidth = bitmap.GetWidth();
   int newheight = bitmap.GetHeight();

   if (wantedWidth_ > 0 && wantedHeight_ > 0) {
      newwidth = wantedWidth_;
      newheight = wantedHeight_;
   } else if (wantedWidth_ < 0 && wantedHeight_ > 0) {
      double scalingfactor = wantedHeight_ / (double)bitmap.GetHeight();
      newwidth = SURI_ROUND(int, bitmap.GetWidth() * scalingfactor);
      newheight = wantedHeight_;
   }

   if (newwidth != bitmap.GetWidth() || newheight != bitmap.GetHeight()) {
      wxImage img = bitmap.ConvertToImage();
      img.Rescale(newwidth, newheight, wxIMAGE_QUALITY_HIGH);
      bitmap = wxBitmap(img);
   }
#endif
   return bitmap;
}

/**
 * Modifica el estado del atributo enabled_
 * @param[in] Enabled estado de activacion de la ventana del boton
 */
void Button::SetEnabled(bool Enabled) {
   enabled_ = Enabled;
}

/**
 * Metodo que verifica las condiciones de activacion del boton.
 * En la clase base devuelve el estado de enabled_ para que los botones
 * previos a que se incorpore la propiedad enabled_ mantengan la misma
 * funcionalidad. Este metodo permite incorporar al boton la logica para
 * su habilitacion.
 * @return enabled_ propiedad que indica si hay que activar la ventana del boton
 */
bool Button::IsEnabled() {
   return enabled_;
}
/**
 * Evento que habilita el estado del control wx del boton en base al
 * retorno del metodo IsEnabled.
 * @param[in] Event evento UIUpdate
 */
void Button::OnUIUpdate(wxUpdateUIEvent &Event) {
   // TODO(Gabriel - #6012): Se comenta ya que no permite desactivar botones
   // de forma manual (hago pbutton->SetEnable(false) y este cambio
   // no se ve replicado (se refresca el boton y se vuelve a activar)
   if (Configuration::GetParameter("app_short_name", "") == wxT("SoPI")) {
      bool enabled = IsEnabled();
      if (pToolWindow_->IsEnabled() != enabled) {
         Event.Enable(enabled);
         pToolWindow_->Enable(enabled);
      }
   }
}

/**
 * Crea un wxBitmapButton con bitmap obtenido de GetBitmap
 * @return si tubo exito en la operacion
 */
bool Button::DoCreateToolWindow() {
   pToolWindow_ = new wxBitmapButton(pParentWindow_, wxID_ANY, GetBitmap());
   return pToolWindow_ != NULL;
}

/**
 * Devuelve el bitmap que tendra el boton
 * Debe ser sobreescrito por los botones para cambiar el icono
 * @return pBitmap_ propiedad con la imagen del boton
 * @return wxNullBitmap si no existe la imagen del boton
 */
wxBitmap Button::DoGetBitmap() {
   if (pBitmap_ && pBitmap_->IsOk()) {
      return *pBitmap_;
   }
   return wxNullBitmap;
}

/**
 * Agrega pEventHandler_ al comienzo de la pila de handlers de ViewerWindow.
 * Luego de llamar a este metodo hay que llamar a RemoveHandlerFromViewerWindow,
 * caso contrario se acumulan los handlers en pViewerWindow_
 */
void Button::AddHandlerToViewerWindowBeginning() {
   // Guardo el handler al comienzo de la lista para que se ejecute ultimo.
   std::list<wxEvtHandler*> list;
   wxEvtHandler* aux = pViewerWindow_->PopEventHandler();
   while (aux != NULL) {
      list.push_back(aux);
      aux = pViewerWindow_->PopEventHandler();
   }
   pViewerWindow_->SetEventHandler(pEventHandler_);
   while (!list.empty()) {
      pViewerWindow_->PushEventHandler(list.front());
      list.pop_front();
   }
}

/** Elimina el pEventHandler_ de la pila de handlers de ViewerWindow */
void Button::RemoveHandlerFromViewerWindow() {
   // Elimino el handler del viewer window
   if (pViewerWindow_) {
      std::list<wxEvtHandler*> list;
      wxEvtHandler* aux = pViewerWindow_->PopEventHandler();
      while (aux != NULL) {
         if (aux != pEventHandler_) {
            list.push_back(aux);
         }
         aux = pViewerWindow_->PopEventHandler();
      }
      pViewerWindow_->SetEventHandler(pEventHandler_);
      while (!list.empty()) {
         pViewerWindow_->PushEventHandler(list.front());
         list.pop_front();
      }
   }
}

/**
 * Metodo que se ejecuta al ante el evento Left del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnLeft(wxMouseEvent &Event) {
   ON_EVENT(debounceLeft_=true, OnLeft, FLAGS(false, Left));
}

/**
 * Metodo que se ejectura ante el evento LeftUp del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnLeftUp(wxMouseEvent &Event) {
   ON_EVENT(, OnLeftUp, FLAGS(false, Left));
}

/**
 * Metodo que se ejectura ante el evento LeftDown del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnLeftDown(wxMouseEvent &Event) {
   ON_EVENT(debounceLeft_ = insideLeft_ = true, OnLeftDown, );
}

/**
 * Metodo que se ejectura ante el evento LeftDoubleClick del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnLeftDoubleClick(wxMouseEvent &Event) {
   // si la secuencia es down-dclick-up => al setear en falso despues del Do
   // no se ejecutaria lo de OnLeftUp (si esta protegido por un debounce)
   ON_EVENT(, OnLeftDoubleClick, FLAGS(false, Left));
}

/**
 * Metodo que se ejectura ante el evento Right del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnRight(wxMouseEvent &Event) {
   ON_EVENT(debounceLeft_=true, OnRight, debounceLeft_ = isDraggingLeft_ = false);
}

/**
 * Metodo que se ejectura ante el evento RightUp del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnRightUp(wxMouseEvent &Event) {
   ON_EVENT(, OnRightUp, FLAGS(false, Right));
}

/**
 * Metodo que se ejectura ante el evento RightDown del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnRightDown(wxMouseEvent &Event) {
   ON_EVENT(debounceRight_ = insideRight_ = true, OnRightDown, );
}

/**
 * Metodo que se ejectura ante el evento RightDoubleClick del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnRightDoubleClick(wxMouseEvent &Event) {
   ON_EVENT(, OnRightDoubleClick, FLAGS(false, Right));
}

/**
 * Metodo que se ejectura ante el evento Middle del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnMiddle(wxMouseEvent &Event) {
   ON_EVENT(debounceLeft_=true, OnMiddle, debounceLeft_ = isDraggingLeft_ = false);
}

/**
 * Metodo que se ejectura ante el evento MiddleUp del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnMiddleUp(wxMouseEvent &Event) {
   ON_EVENT(, OnMiddleUp, FLAGS(false, Middle));
}

/**
 * Metodo que se ejectura ante el evento MiddleDown del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnMiddleDown(wxMouseEvent &Event) {
   ON_EVENT(debounceMiddle_ = insideMiddle_ = true, OnMiddleDown, );
}

/**
 * Metodo que se ejectura ante el evento MiddleDoubleClick del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnMiddleDoubleClick(wxMouseEvent &Event) {
   ON_EVENT(, OnMiddleDoubleClick, FLAGS(false, Middle));
}

/**
 * Metodo que se ejectura ante el evento EnterWindow del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnEnterWindow(wxMouseEvent &Event) {
   ON_EVENT(FLAGS(false, Left); FLAGS(false, Middle); FLAGS(false, Right),
            OnEnterWindow, );
   // deseo que los eventos de este tipo se sigan propagando
   Event.Skip(false);
}

/**
 * Metodo que se ejectura ante el evento LeaveWindow del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnLeaveWindow(wxMouseEvent &Event) {
   ON_EVENT(, OnLeaveWindow,
            FLAGS(false, Left); FLAGS(false, Middle); FLAGS(false, Right));
   // deseo que los eventos de este tipo se sigan propagando
   Event.Skip(false);
}

/**
 * Metodo que se ejectura ante el evento Wheel del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnWheel(wxMouseEvent &Event) {
   ON_EVENT(, OnWheel, );
}

/**
 * Metodo que se ejectura ante el evento MouseMove del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnMouseMove(wxMouseEvent &Event) {
   // Solo ejecuto si hay evento de movimiento o boton asociado
   if ((eventMask_ & BUTTON_ANY_BUTTON) || (eventMask_ & BUTTON_MOVE)) {
      PRE_EVENT(debounceLeft_=debounceMiddle_=debounceRight_=false);
      DoOnMouseMove(Event);
      // si isDraggingLeft_ es falso y esta haciendose un drag -> isDraggingLeft_ = true
      // luego, hasta un clickup isDraggingLeft_ no deberia hacerse falso
      POST_EVENT(
            isDraggingLeft_=insideLeft_&&Event.LeftIsDown()&&Event.Dragging(); isDraggingMiddle_=insideMiddle_&&Event.MiddleIsDown()&&Event.Dragging(); isDraggingRight_=insideRight_&&Event.RightIsDown()&&Event.Dragging());
   }
}

/**
 * Metodo que se ejectura ante el evento Mouse del mouse sobre el viewer
 * @param[in] Event evento
 */
void Button::OnMouse(wxMouseEvent &Event) {
   DoOnMouse(Event);
}

/**
 * Metodo que se ejectura ante el evento Paint sobre el viewer
 * @param[in] Event evento
 */
void Button::OnPaint(wxPaintEvent &Event) {
   // ON_EVENT(,OnPaint,);
   DoOnPaint(Event);
}
}

