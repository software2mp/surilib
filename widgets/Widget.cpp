/*! \file Widget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/Widget.h"

// Includes suri
#include "suri/messages.h"
#include "wxmacros.h"
#include "suri/MouseEventInterface.h"
#include "suri/PaintEventInterface.h"
#include "suri/CommandEventInterface.h"
#include "PaintEvent.h"
#include "MouseEvent.h"
#include "CommandEvent.h"

// Includes wx
#include "wx/wx.h"
#include "wx/window.h"
#include "wx/event.h"
#include "wx/xrc/xmlres.h"
#include "wx/artprov.h"
#include <wx/dcbuffer.h>
#include <wx/settings.h>

// Includes estandar
#include <stack>
#include <deque>

// Defines
/** Define el ancho minimo de una ventana. */
#define MIN_WINDOW_WIDTH 125
/** Define el alto minimo de una ventana. */
#define MIN_WINDOW_HEIGHT 24

#ifdef __DEBUG__
#include <iostream>
#endif

/** Variable para evitar registrar constantemente los Handlers de wx */
bool wxImageHandlersInitialized = false;

/** namespace suri */
namespace suri {
/*!
 *  Se implementa el objeto wxEventHandler con un metodo para atrapar el cierre
 * de la ventana y forwardearlo a un metodo de la herramienta.
 */
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ToolEventHandler, Widget)
/** forward de evento de destruccion */
   IMPLEMENT_EVENT_CALLBACK(OnDestroy, OnDestroy(event); , wxWindowDestroyEvent)
   IMPLEMENT_EVENT_CALLBACK(OnMouseLeave, OnMouseLeave(), wxMouseEvent)
#ifdef __SHOW_WIDGET_FIT_BUTTON__
   IMPLEMENT_EVENT_CALLBACK(OnFitButton, OnFitButton(), wxCommandEvent)
#endif
#ifdef __SHOW_WIDGET_LAYOUT_BUTTON__
   IMPLEMENT_EVENT_CALLBACK(OnLayoutButton, OnLayoutButton(), wxCommandEvent)
#endif

END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** set estatico que almacena los widgets que fueron destruidos por un evento OnDestroy */
std::set<Widget*> Widget::destroyedWidgets_;
/** set estatico que almacena los widgets que existen */
std::set<Widget*> Widget::validWidgets_;

/*!
 *  Constructor de la herramienta base.
 *
 * @param[in] pParent puntero a una ventana padre. En caso de ser NULL, se crea
 *  un wxFrame para que contenga a la herramienta.
 *  @return instancia de la clase Widget.
 */
Widget::Widget(wxWindow *pParent) :
      pToolWindow_(NULL), pParentWindow_(pParent), wantedWidth_(MIN_WINDOW_WIDTH),
      wantedHeight_(MIN_WINDOW_HEIGHT), windowTitle_(_(caption_TOOL)),
      isAutonomous_(false), pToolFrame_(NULL), createWithMiniframe_(false),
      pMiniFrame_(NULL), isExiting_(false), NEW_EVENT_OBJECT(ToolEventHandler) {
   validWidgets_.insert(this);
}

/*!
 * Destructor.
 * Se desconecta de los eventos y si esta contenida en otra ventana
 */
Widget::~Widget() {
   REPORT_DEBUG("D:Destructor : %s(%p),pToolWindow_(%p),pParentWindow_(%p)",
                GetWindowTitle().c_str(), this, pToolWindow_, pParentWindow_);
   validWidgets_.erase(this);
   if (!isExiting_) {
      FLUSH();
      isExiting_ = true;
      destroyedWidgets_.erase(this);

      // Elimino los handlers asociados
      while (PopCommandEvent()) {
      }
      while (PopPaintEvent()) {
      }
      while (PopMouseEvent()) {
      }

      DISCONNECT_TOOL_EVENT(ToolEventHandler, OnDestroy, wxEVT_DESTROY,
                            wxWindowDestroyEventHandler);
      DELETE_EVENT_OBJECT;
      // Si pToolWindow_ esta contenida en otra ventana
      if (!isAutonomous_) {
         RemoveControl(pToolWindow_);
      } else if (pParentWindow_) {
         // Cierro el parent
         pParentWindow_->Close();
      }
   } else {
      REPORT_DEBUG("D:Recursion en la destruccion de \"%s\".", windowTitle_.c_str());
   }
   REPORT_DEBUG("D:Fin Destructor : %s(%p),pToolWindow_(%p),pParentWindow_(%p)",
                GetWindowTitle().c_str(), this, pToolWindow_, pParentWindow_);
}

/** utilizado para determinar si hay que ajustar el tamano de la ventana */
/** haciendo fits y layouts */
#define __DONT_ADJUST_CREATED_TOOL__
#undef __DONT_DETERMINE_WINDOW_SIZE_USING_FRAME__

#ifdef __UNUSED_CODE__
// Respeta el tamano de la ventana previa a agregar el nuevo widget
// Las barras de herramientas no se ven
// construccion rapida (pocos artefactos)
#define __DONT_ADJUST_CREATED_TOOL__
#undef __DONT_DETERMINE_WINDOW_SIZE_USING_FRAME__
#endif   // __UNUSED_CODE__
#ifdef __UNUSED_CODE__
// Respeta el tamano de la ventana previa a agregar el nuevo widget
// La barra de herramientas del viewer sale, las de dibujo no (solo las de mapa)
// construccion lenta (muchos artefactos)
#undef __DONT_ADJUST_CREATED_TOOL__
#define __DONT_DETERMINE_WINDOW_SIZE_USING_FRAME__
#endif

/*!
 * Crea la herramienta
 *   Usa el metodo virtual CreateToolWindow y agrega la ventana al padre
 *   Si el padre es nulo, crea una ventana simple y le agrega la herramienta
 * @param[in] Show determina si se muestra o no la herramienta al ser creada
 * @param[in] pParentFrame: puntero al frame padre de la ventana.
 * @return true si pudo crear la ventana
 * @return false si no pudo crear la ventana
 */
bool Widget::CreateTool(wxTopLevelWindow *pParentFrame, bool Show) {
   if (pToolWindow_) {
      REPORT_AND_FAIL_VALUE("D:Herramienta \"%s\" ya creada.", false,
                            windowTitle_.c_str());
   }
   // Inicializo los recursos de la clase derivada
   bool result = false;
   // En caso de no tener parent, crea un wxFrame y le agrega el control
   if (pParentWindow_ == NULL) {
      result = CreateSimpleToolWindow(pParentFrame);
#if defined (__SHOW_WIDGET_FIT_BUTTON__) || defined (__SHOW_WIDGET_LAYOUT_BUTTON__)
      wxSizer *psizer = new wxBoxSizer(wxHORIZONTAL);
      if ( pParentWindow_ && pParentWindow_->GetSizer() ) {
         pParentWindow_->GetSizer()->Add(psizer, 0, wxALL|wxALIGN_RIGHT, 5);
      }
#endif
#ifdef __SHOW_WIDGET_FIT_BUTTON__
      if ( psizer ) {
         wxButton *pbutton = new wxButton(pParentWindow_, wxID_ANY, wxT("D:Fit") );
         psizer->Add(pbutton, 0, wxALL|wxALIGN_RIGHT, 5);
         pbutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
               wxCommandEventHandler(ToolEventHandler::OnFitButton),
               NULL, pEventHandler_);
      } else {
         REPORT_ERROR("D: Sin sizer en el parent");
      }
#endif   // __SHOW_WIDGET_FIT_BUTTON__
#ifdef __SHOW_WIDGET_LAYOUT_BUTTON__
      if ( psizer ) {
         wxButton *pbutton = new wxButton(pParentWindow_, wxID_ANY, wxT("D:Layout") );
         psizer->Add(pbutton, 0, wxALL|wxALIGN_RIGHT, 5);
         pbutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
               wxCommandEventHandler(ToolEventHandler::OnLayoutButton),
               NULL, pEventHandler_);
      } else {
         REPORT_ERROR("D: Sin sizer en el parent");
      }
#endif   // __SHOW_WIDGET_LAYOUT_BUTTON__
   } else {
      wxWindow *pparent = pParentWindow_;
      wxWindow *pminiframeparent = pParentWindow_;
      if (createWithMiniframe_) {
         pMiniFrame_ = wxXmlResource::Get()->LoadPanel(pminiframeparent,
                                                       wxT("ID_FRAME_WIDGET_PANEL"));
         if (pMiniFrame_) {
            AddControl(pMiniFrame_, pminiframeparent);
            pparent = XRCCTRL(*pMiniFrame_, wxT("ID_WIDGET_PANEL"), wxPanel);
            pParentWindow_ = pparent ? pparent : pParentWindow_;
            wxStaticText *pcaption =
                  XRCCTRL(*pMiniFrame_, wxT("ID_TITLEBAR_CAPTION_TEXT"), wxStaticText);
            pcaption->SetLabel(GetWindowTitle());
         }
      }

      result = CreateToolWindow();
      if (!result || !pToolWindow_) {
         REPORT_AND_FAIL_VALUE("D:Error al crear la herramienta.", false);
      }
      AddControl(pToolWindow_, pParentWindow_);
      if (createWithMiniframe_) {
         pParentWindow_ = pminiframeparent;
      }
      if (false) {
         DEBUG_SIZE(windowTitle_.c_str(), pToolWindow_, pToolWindow_->GetBestSize());
         DEBUG_SIZE(windowTitle_.c_str(), pToolWindow_, pToolWindow_->GetSize());
//      pToolWindow_->Hide();
// Calcula y fuerza el tamano de la ventana metiendo el widget en un frame
// ajustando y volviendo a ponerlo en el parent.
#ifndef __DONT_DETERMINE_WINDOW_SIZE_USING_FRAME__
         // determina el tamanio real de pToolWindow_ ----------------------------
// Este codigo se utiliza para determinar la dimension de los controles
// en el caso GTK + contextos agregados fuera de un CreateToolWindow
// genera un bug asincronico de X
#ifndef __WXGTK__
         wxFrame *pframe = new wxFrame(NULL, wxID_ANY, wxT(""), wxDefaultPosition,
               wxDefaultSize, wxFRAME_TOOL_WINDOW);
         wxSizer *psizer = new wxBoxSizer(wxVERTICAL);
         pframe->SetSizer(psizer);
         pToolWindow_->Reparent(pframe);
         psizer->Add(pToolWindow_, 1, wxEXPAND, 0);
         pframe->Fit();
#endif   // __WXGTK__
         pToolWindow_->Fit();
         wxSize size = pToolWindow_->GetSize();
         pToolWindow_->SetMinSize(
               wxSize(std::max(wantedWidth_, size.GetWidth()),
                      std::max(wantedHeight_, size.GetHeight())));
         DEBUG_SIZE(pToolWindow_->GetName().c_str(), pToolWindow_,
                    pToolWindow_->GetSize());
// Este codigo se utiliza para determinar la dimension de los controles
// en el caso GTK + contextos agregados fuera de un CreateToolWindow
// genera un bug asincronico de X
#ifndef __WXGTK__
         psizer->Detach(pToolWindow_);
         pToolWindow_->Reparent(pParentWindow_);
         pframe->Destroy();
#endif   // __WXGTK__
         // FIN determina el tamanio real de pToolWindow_ -------------------------

#endif   // __DONT_DETERMINE_WINDOW_SIZE_USING_FRAME__
      }
   }
// ajusta el tamano de la ventana haciendo fits y layouts
#ifndef __DONT_ADJUST_CREATED_TOOL__
   pToolWindow_->GetParent()->Fit();

   /* ----------------------------------------------------------------------------
    * Se comenta la linea: pParentWindow_->SetClientSize(pToolWindow_->GetSize());
    * ya que cuando se esta trabajando con las clases PartContainerWidget y Part
    * (y sus derivados) hace que el control donde se insertan los Part tome el
    * tamanio del Part y eso hace que los Part no ocupen el total de la ventana
    * padre.
    * Como efecto secundario, el comentar esta linea hace que el MainWindow no
    * aparezca centrado.
    * --------------------------------------------------------------------------*/
   pParentWindow_->Layout();
   if ( GetFrame() ) {
      GetFrame()->Show(Show);
   }
   pParentWindow_->Layout();

#endif
#ifdef __WXGTK__
   pToolWindow_->Show();
#endif
   if (isAutonomous_ && GetFrame()) {
      GetFrame()->Show(Show);
   }
   // Conecta eventos
   CONNECT_TOOL_EVENT(ToolEventHandler, OnDestroy, wxEVT_DESTROY,
                      wxWindowDestroyEventHandler);
   CONNECT_TOOL_EVENT(ToolEventHandler, OnMouseLeave, wxEVT_LEAVE_WINDOW,
                      wxMouseEventHandler);
   return result;
}

/**
 * Establece el titulo de la ventana.
 *
 * @param[in] WindowTitle Titulo de la ventana.
 */
void Widget::SetWindowTitle(const wxString& WindowTitle) {
   /**
    * todo(gabriel - tck #5694): fix temporal para que no aparezca el titulo en simur.
    * esto debe mejorarse y poder configurarse desde el archivo de configuracion de la aplicacion
    * Si no tiene miniframe rompe
    */
   if (!pMiniFrame_)
      return;
   wxStaticText *pcaption = XRCCTRL(*pMiniFrame_, wxT("ID_TITLEBAR_CAPTION_TEXT"),
                                    wxStaticText);

   if (!WindowTitle.IsEmpty() && pcaption != NULL) {
      windowTitle_ = WindowTitle;
      pcaption->SetLabel(windowTitle_);
   }
}

/*!
 *  Retorna un bitmap con el icono de la ventana.
 * Si se pasa solo X o solo Y el otro parametro tomara un valor que conserve
 * la relacion entre lados con la imagen.
 * Al final de ser necesario hace Rescale.
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono
 * @param[in] Y alto del icono
 */
void Widget::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   if (X < 0 && Y < 0) {
      ToolBitmap = wxArtProvider::GetBitmap(wxART_MISSING_IMAGE);
      return;
   }
   ToolBitmap = wxArtProvider::GetBitmap(wxART_MISSING_IMAGE, wxART_OTHER,
                                         wxSize(X, Y));

   if (X > 0 && Y < 0) {
      Y = static_cast<int>((double) ToolBitmap.GetWidth() * X / ToolBitmap.GetHeight());
   }
   if (X < 0 && Y > 0) {
      X = static_cast<int>((double) ToolBitmap.GetHeight() * Y / ToolBitmap.GetWidth());
   }

   ESCALE_BITMAP(ToolBitmap, X, Y);
}

/*!
 *  Regresa un puntero a la ventana de la herramienta.
 *
 * \post Si pToolWidnow_ == NULL, se creara la ventana usando CreateTool().
 * @return Puntero a pToolWindow_
 */
wxWindow *Widget::GetWindow() {
   if (!pToolWindow_) {
      CreateTool();
   }
   return pToolWindow_;
}

/*!
 * Retorna el frame dentro del cual se encuentra la ventana
 * @return frame dentro del cual se encuentra la ventana
 */
wxTopLevelWindow *Widget::GetFrame() const {
   if (pToolFrame_) {
      return pToolFrame_;
   }
   wxWindow *pparent = pParentWindow_;
   while (dynamic_cast<wxTopLevelWindow*>(pparent) == NULL && pparent != NULL) {
      pparent = pparent->GetParent();
   }
   wxTopLevelWindow *pframe = dynamic_cast<wxTopLevelWindow*>(pparent);
   if (!pframe) {
      REPORT_DEBUG("D:No se encontro el frame de \"%s\".", windowTitle_.c_str());
   }
   const_cast<Widget*>(this)->pToolFrame_ = pframe;
   if (!pframe) {
      REPORT_DEBUG("D:pToolFrame_ == NULL");
   }
   return pframe;
}

/*!
 * Agrega Componentes
 * @param[in] pTool Widget que se quiere agregar
 * @param[in] ParentControl nombre del control en el que se agrega el
 * widget, el control debe ser estar contenido en pToolWindow_.
 * @return informa si se pudo agregar el widget al control
 */
bool Widget::AddControl(Widget *pTool, const wxString &ParentControl) {
   return AddControl(pTool, ParentControl, ControlArrangement());
}

/*!
 * Agrega Componentes
 * @param[in] pTool Widget que se quiere agregar
 * @param[in] ParentControl nombre del control en el que se agrega el
 * widget, el control debe ser estar contenido en pToolWindow_.
 * @param[in] Arrangement Tiene los parametros de distribucion del control
 * @return informa si se pudo agregar el widget al control
 */
bool Widget::AddControl(Widget *pTool, const wxString &ParentControl,
                        const ControlArrangement &Arrangement) {
   if (!pTool) {
      return false;
   }REPORT_DEBUG("D:AddControl() %s <- %s(%p)",
                 ParentControl.c_str(), pTool->GetWindowTitle().c_str(), pTool);
   // intenta encontrar por nombre de ventana primero y luego por ID XRC
   // permite tener widgets generados por codigo
   wxWindow *pparent =
         pToolWindow_ ? pToolWindow_->FindWindowByName(ParentControl, pToolWindow_) :
                        NULL;
   if (!AddControl(pTool,
                   pparent ? pparent : XRCCTRL(*pToolWindow_, ParentControl, wxWindow),
                   Arrangement)) {
      REPORT_AND_FAIL_VALUE("D:No se pudo insertar %s en %s", false,
                            pTool->GetWindowTitle().c_str(), ParentControl.c_str());
   }
   return true;
}

/*!
 *  Agrega un control en la ventana.
 * @param[in] pTool ventana de la herramienta  que se quiere agregar
 * @param[in] pParentControl ventana del control en el que se quiere agregar
 * el widget.
 * @return informa si se pudo agregar el widget al control
 */
bool Widget::AddControl(Widget *pTool, wxWindow *pParentControl) {
   return AddControl(pTool, pParentControl, ControlArrangement());
}

/*!
 * Agrega Componentes
 * @param[in] pTool Widget que se quiere agregar
 * @param[in] pParentControl ventana del control en el que se quiere agregar
 * el widget.
 * @param[in] Arrangement Tiene los parametros de distribucion del control
 * @return informa si se pudo agregar el widget al control
 */
bool Widget::AddControl(Widget *pTool, wxWindow *pParentControl,
                        const ControlArrangement &Arrangement) {
   if (!pTool) {
      return false;
   }
   if (!pParentControl) {
      REPORT_AND_FAIL_VALUE("D:Error al insertar control. %s.", false,
                            pTool->GetWindowTitle().c_str());
   }REPORT_DEBUG(
         "D:AddControl() %s(%p) <- %s(%p)",
         pParentControl->GetName().c_str(), pParentControl, pTool->GetWindowTitle().c_str(), pTool);

   // si todavia no esta creada le asigno el parent que va a tener y la crea
   if (!pTool->pToolWindow_) {
      pTool->pParentWindow_ = pParentControl;
      pTool->createWithMiniframe_ = pTool->createWithMiniframe_
            || Arrangement.GetMiniFrame();
      pTool->CreateTool();
   }
   // define que control tiene que agregar, si el miniframe o pToolWindow.
   // se las rebusca asi para evitar cambiar toda la logica de GetWindow()
   wxWindow *paddedtoolwindow = pTool->pMiniFrame_;
   if (!paddedtoolwindow) {
      paddedtoolwindow = pTool->GetWindow();
   }
   return AddControl(paddedtoolwindow, pParentControl, Arrangement);
}

/*!
 * Elimina una herramienta que fue agregada a este widget (puede ser
 * si misma)
 * @param[in] pTool Es la herramienta que se quiere remover
 * @return true si pudo eliminar la herramienta del widget.
 * @return false si no pudo eliminar la herramienta del widget.
 */
bool Widget::RemoveControl(Widget *pTool) {
   /** Se verifica que el pToolWindow_ sea NULL para no llamar el GetWindow
    *  que crea una nueva ventana en caso de que sea NULL.
    *  No se mete este control dentro de RemoveControl para que no impacte
    *  el return false.
    *  El Widget no se destruye inmediamente, sino que se destruye en el proximo ciclo
   **/
   if (pTool && (!pTool->pToolWindow_ || RemoveControl(pTool->GetWindow()))) {
      destroyedWidgets_.insert(pTool);
   } else {
      return false;
   }
   return true;
}

bool Widget::Show() {
   return this->Show(pToolWindow_, true);
}

bool Widget::Show(wxWindow *pWindow, bool Show) {
   if (pWindow && pWindow->GetParent()) {
      if (pWindow->GetName() == wxT("ID_WIDGET_PANEL")) {
         pWindow = pWindow->GetParent()->GetParent();  // Deberia ser wxT("ID_FRAME_WIDGET_PANEL")
      }
      if (pWindow) {
         // now, modify the sizer which contain that window
         wxSizer *psizer = pWindow->GetContainingSizer();
         if (psizer) {
            wxSizerItem *pitem = psizer->GetItem(pWindow);
            if (pitem) {
               pitem->Show(Show);
               psizer->Layout();
               pWindow->GetParent()->Layout();
               return true;
            }
         }
      }
   }
   return false;
}

/*!
 * Muestra u oculta el control con WidgetId
 *
 * @param[in] WidgetId nombre del ID de la herramienta que se quiere desplegar.
 * @param[in] Show Flag para indicar si se debe mostrar u ocultar el control.
 * @return true si pudo encontrar y aplicar el cambio en el control
 */
bool Widget::Show(const std::string &WidgetId, bool Show) {
   wxWindow *pwindow = XRCCTRL(*pToolWindow_, WidgetId.c_str(), wxWindow);
   return this->Show(pwindow, Show);
}

/** Oculta el Widget ppal */
bool Widget::Hide() {
   return Show(pToolWindow_, false);
}

/*!
 * Oculta el control con WidgetId. ver Widget::Show()
 *
 * @param[in] WidgetId nombre del ID de la herramienta que se quiere desplegar.
 * @return true si pudo encontrar y aplicar el cambio en el control
 */
bool Widget::Hide(const std::string &WidgetId) {
   return Show(WidgetId, false);
}

/** Habilita la herramienta que se indica en WidgetId */
bool Widget::Enable() {
   return Enable(pToolWindow_, true);
}

/** Habilita la Ventana */
bool Widget::Enable(wxWindow *pWindow, bool Enabled) {
   return pWindow && pWindow->Enable(Enabled);
}

/*!
 * Habilita la herramienta que se indica en WidgetId
 *
 * @param[in] WidgetId nombre del ID de la herramienta que se quiere desplegar.
 * @param[in] Enabled Flag para indicar si se debe habilitar el control.
 * @return true si pudo encontrar y aplicar el cambio en el control
 */
bool Widget::Enable(const std::string &WidgetId, bool Enabled) {
   return Enable(XRCCTRL(*pToolWindow_, WidgetId.c_str(), wxWindow), Enabled);
}

/*!
 * Inhabilita la herramienta que se indica en WidgetId. Ver Widget::Enable()
 *
 * @param[in] WidgetId nombre del ID de la herramienta que se quiere desplegar.
 * @return true si pudo encontrar y aplicar el cambio en el control
 */
bool Widget::Disable(const std::string &WidgetId) {
   return Enable(WidgetId, false);
}

/** Inhabilita Widget ppal */
bool Widget::Disable() {
   return Enable(pToolWindow_, false);
}

/*!
 * Marca en el miniframe el foco del widget.
 * \todo: Conectar (opcionalmente) eventos de on-focus para control automatico.
 * @param[in] Focused Estado de foco del miniframe.
 */
void Widget::Focus(bool Focused) {
   if (pMiniFrame_) {
      wxWindow *ptitlebar = XRCCTRL(*pMiniFrame_, "ID_TITLEBAR_PANEL", wxWindow);
      if (ptitlebar) {
         wxColour colour(wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION));
         if (!Focused)
            colour = wxSystemSettings::GetColour(wxSYS_COLOUR_INACTIVECAPTION);
         ptitlebar->SetBackgroundColour(colour);
         ptitlebar->Update();
         ptitlebar->Refresh();
      }
   }
   DoOnFocus(Focused);
}

/** 
 * Metodo polimorfico que se llama cuando se hace foco sobre un Widget
 * @param[in] Focused Estado del foco del miniframe
 **/
void Widget::DoOnFocus(bool Focused) {
}

/*!
 *  Agrega un handler de eventos a la cadena de pToolWindow_.
 *  \todo: Hacer que haya eventos que no se pueden bloquear y que se acomoden
 *         primero en la lista.
 * @param[in] pMouseEvent Puntero a una instancia de handler de eventos de mouse.
 *            pMouseEvent debera ser eliminado por el codigo cliente.
 */
void Widget::PushMouseEvent(MouseEventInterface *pMouseEvent) {
   if (!pToolWindow_ || !pMouseEvent) return;
   pToolWindow_->PushEventHandler(new MouseEvent(pMouseEvent));
}

/*!
 *
 */
MouseEventInterface *Widget::PopMouseEvent(MouseEventInterface *pMouseEvent) {
   if (!pToolWindow_) return NULL;
   wxEvtHandler *pev = pToolWindow_->PopEventHandler();
   std::stack<wxEvtHandler*> handlers;
   MouseEventInterface *phandler = NULL;
   while (pev) {
      MouseEvent *pmouse = dynamic_cast<MouseEvent*>(pev);
      if (pmouse) {
         phandler = pmouse->GetHandler();
         if (!pMouseEvent || pMouseEvent == phandler) {
            delete pmouse;
            break;
         } else {
            handlers.push(pev);
            pev = pToolWindow_->PopEventHandler();
         }
      } else {
         handlers.push(pev);
         pev = pToolWindow_->PopEventHandler();
      }
   }
   while (!handlers.empty()) {
      pToolWindow_->PushEventHandler(handlers.top());
      handlers.pop();
   }
   return phandler;
}

/*!
 *  \todo: Hacer que haya eventos que no se pueden bloquear y que se acomoden
 *         primero en la lista.
 *
 * @param pPaintEvent
 * @param DeepSearch
 */
void Widget::PushPaintEvent(PaintEventInterface *pPaintEvent, bool DeepSearch) {
   if (!pToolWindow_ || !pPaintEvent) return;
   pToolWindow_->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
   wxEvtHandler *pev = pToolWindow_->GetEventHandler();
   while (pev) {
      PaintEvent *ppaint = dynamic_cast<PaintEvent*>(pev);
      if (ppaint) {
         ppaint->Push(pPaintEvent);
         return;
      }
      if (DeepSearch)
         pev = pev->GetNextHandler();
      else
         pev = NULL;
   }
   if (!pev) {
      // Guardo el handler al comienzo de la lista para que se ejecute ultimo.
      std::stack<wxEvtHandler*> handlers;
      wxEvtHandler* paux = pToolWindow_->PopEventHandler();
      // no saco el handler original (pToolWindow_)
      while (paux) {
         handlers.push(paux);
         paux = pToolWindow_->PopEventHandler();
      }
      PaintEvent* pPaint = new PaintEvent(pPaintEvent);
      pToolWindow_->PushEventHandler(pPaint);

      while (!handlers.empty()) {
         pToolWindow_->PushEventHandler(handlers.top());
         handlers.pop();
      }
   }
}

/*!
 *
 */
PaintEventInterface *Widget::PopPaintEvent(PaintEventInterface *pPaintEvent) {
   if (!pToolWindow_) return NULL;
   wxEvtHandler *pev = pToolWindow_->PopEventHandler();
   std::stack<wxEvtHandler*> handlers;
   PaintEventInterface *phandler = NULL;
   bool deleted = false;
   while (pev) {
      PaintEvent *ppaint = dynamic_cast<PaintEvent*>(pev);
      if (ppaint) {
         std::stack<PaintEventInterface*> paintstack;
         phandler = ppaint->GetHandler();
         while (phandler) {
            // si encontro lo que buscaba o el primer handler
            if (!pPaintEvent || pPaintEvent == phandler) {
               ppaint->Pop();
               if (!ppaint->GetHandler() && paintstack.empty()) {
                  delete ppaint;
                  deleted = true;
               }
               break;
            }
            paintstack.push(phandler);
            ppaint->Pop();
            phandler = ppaint->GetHandler();
         }
         // reconstruyo los handlers dentro de paintevent
         while (!paintstack.empty()) {
            ppaint->Push(paintstack.top());
            paintstack.pop();
         }
      }
      if (!deleted) {
         handlers.push(pev);
      }
      deleted = false;
      pev = pToolWindow_->PopEventHandler();
   }
   // reconstruyo los paintevents
   while (!handlers.empty()) {
      pToolWindow_->PushEventHandler(handlers.top());
      handlers.pop();
   }
   return phandler;
}

/*!
 *  \todo: Hacer que haya eventos que no se pueden bloquear y que se acomoden
 *         primero en la lista.
 * @param pCommandEvent
 */
void Widget::PushCommandEvent(CommandEventInterface *pCommandEvent) {
   if (!isAutonomous_ || !pToolFrame_ || !pCommandEvent) return;
   pToolFrame_->PushEventHandler(new CommandEvent(pCommandEvent));
}

/** Elimina el handler de eventos command */
CommandEventInterface *Widget::PopCommandEvent(CommandEventInterface *pCommandEvent) {
   if (!pToolWindow_) return NULL;
   wxEvtHandler *pev = pToolWindow_->PopEventHandler();
   std::stack<wxEvtHandler*> handlers;
   CommandEventInterface *phandler = NULL;
   while (pev) {
      CommandEvent *pcommand = dynamic_cast<CommandEvent*>(pev);
      if (pcommand) {
         phandler = pcommand->GetHandler();
         if (!pCommandEvent || pCommandEvent == phandler) {
            delete pcommand;
            break;
         } else {
            handlers.push(pev);
            pev = pToolWindow_->PopEventHandler();
         }
      } else {
         handlers.push(pev);
         pev = pToolWindow_->PopEventHandler();
      }
   }
   while (!handlers.empty()) {
      pToolWindow_->PushEventHandler(handlers.top());
      handlers.pop();
   }
   return phandler;
}

/*!
 *  Crea un wxTopLevelWindow sencillo con el panel de la herramienta adentro
 *
 *  @param[in] pParentFrame: puntero al frame padre de la ventana.
 *  @return true si pudo crear la ventana.
 *  @return false si no pudo crear la ventana.
 */
bool Widget::CreateSimpleToolWindow(wxTopLevelWindow *pParentFrame) {
   if (pParentWindow_ || pToolWindow_) {
      REPORT_AND_FAIL_VALUE("D:Herramienta ya creada.", false);
   }
   isAutonomous_ = true;
   // Creo el frame
   wxTopLevelWindow *pframe = CreateContainingWindow(pParentFrame);
   pframe->SetParent(pParentFrame);
//   pframe->Layout();
//   pframe->Fit();
// Le agrego un sizer
   pframe->SetSizer(new wxBoxSizer(wxVERTICAL));
   // Seteo el icono
   wxBitmap bmp;
   GetWindowIcon(bmp, 16, 16);
   if (bmp.IsOk()) {
      wxIcon icn;
      icn.CopyFromBitmap(bmp);
      pframe->SetIcon(icn);
   }
   pToolFrame_ = pframe;
   bool result = CreateToolWindow();
   //  Me aseguro que la herramienta haya seteado el puntero
   assert(pToolWindow_ != NULL);
   AddControl(pToolWindow_, pToolWindow_->GetParent());
   GetFrame()->Fit();
   GetFrame()->Layout();
   return result;
#ifdef __UNUSED_CODE__
   //  Agrego la herramienta
   sizer->Add(pToolWindow_, 1, wxGROW, 0);

   // Setea tamanio minimo para la ventana creada, no es exacto
//   pParentWindow_->SetClientSize(pToolWindow_->GetSize());
   int w, h;
   pParentWindow_->GetSize(&w, &h);
   int cw, ch;
   pParentWindow_->GetClientSize(&cw, &ch);
   // decoraciones
   int dw, dh;
   dw = w-cw;
   dh = h-ch;
   // elige el mayor entre el tamanio necesario de ventana
   // y el tamanio deseado
   w = std::max(wantedWidth_-dw, w);
   h = std::max(wantedHeight_-dh, h);
//   pParentWindow_->SetMinSize(wxSize(w,h));
   pframe->SetFocus();
   pframe->Center();
   return result;
#endif   // __UNUSED_CODE__
}

/*!
 * crea el wxFrame que contiene a la ventana.
 * @param[in] pParentFrame puntero al frame padre de la ventana
 * @return wxFrame que contiene a la ventana.
 */
wxTopLevelWindow* Widget::CreateContainingWindow(wxTopLevelWindow *pParentFrame) {
   // Creo el frame
   wxFrame *pframe = new wxFrame;
   pframe->Create(pParentFrame, wxID_ANY, GetWindowTitle(), wxDefaultPosition,
                  wxSize(0, 0), wxDEFAULT_FRAME_STYLE, wxT("ID_FRMEMPTY"));
   // Creo y agrego la ventana de la herramienta
   //  para que lo pueda usar la clase derivada en CreateToolWindow()
   pParentWindow_ = pframe;
   pToolFrame_ = pframe;
   //  Creo el statusbar
   pframe->CreateStatusBar(2, wxST_SIZEGRIP | wxNO_BORDER, wxID_ANY,
                           wxT("ID_STATUSBAR"));
   return pframe;
}

/*!
 *  Agrega un control en la ventana.
 * @param[in] pToolWindow ventana de la herramienta  que se quiere agregar
 * @param[in] pParentControl ventana del control en el que se quiere agregar
 * el widget.
 * @return informa si se pudo agregar el widget al control
 */
bool Widget::AddControl(wxWindow *pToolWindow, wxWindow *pParentControl) {
   return AddControl(pToolWindow, pParentControl, ControlArrangement());
}

/*!
 *  Agrega un control en la ventana.
 * \todo: tener en cuenta el tamanio del wxWindow padre y no del wxFrame y hacer
 * el calculo correcto para el tamanio y asi evitar tener tamanios minimos
 * incorrectos.
 * @param[in] pToolWindow ventana de la herramienta  que se quiere agregar
 * @param[in] pParentControl ventana del control en el que se quiere agregar
 * el widget.
 * @param[in] Arrangement Tiene los parametros de distribucion del control
 * @return informa si se pudo agregar el widget al control
 */
bool Widget::AddControl(wxWindow *pToolWindow, wxWindow *pParentControl,
                        const ControlArrangement &Arrangement) {
   if (!pToolWindow_ || !pToolWindow || !pParentControl) {
      return false;
   }
//   pToolWindow->Hide();
//   pToolWindow->Freeze();
   if (pToolWindow->GetParent() != pParentControl) {
      pToolWindow->Reparent(pParentControl);
   }
//   pParentControl->Fit();
   wxTopLevelWindow *ptopframe = GetFrame();
   /*   if (ptopframe)
    ptopframe->Fit();*/
   wxSizer *pparentsizer = pParentControl->GetSizer();
   if (!pparentsizer) {
#ifdef __DEBUG__
      wxWindow *pwin = pParentControl->GetParent();
      std::string namebranch(pParentControl->GetName());
      while (pwin) {
         namebranch = pwin->GetName() + "->" + namebranch;
         pwin = pwin->GetParent();
      }REPORT_ERROR(
            "D: %s:%s no tiene Sizer. Se debe agregar al menos un sizer.",
            GetWindowTitle().c_str(), namebranch.c_str());
#endif   // __DEBUG__
      pParentControl->SetSizer(new wxBoxSizer(wxVERTICAL));
      pparentsizer = pParentControl->GetSizer();
   }
//   wxSize size = pParentControl->GetSize();
//   pToolWindow->SetSize(size);
   wxSizer *pcontainingsizer = pToolWindow->GetContainingSizer();
   if (pcontainingsizer) {
      pcontainingsizer->Detach(pToolWindow);
   }
   pcontainingsizer = pToolWindow->GetContainingSizer();
   // cero, lo pone como primero
   int position = Arrangement.GetPosition();
   int proportion = Arrangement.GetProportion();
   int flags = Arrangement.GetFlags();
   int border = Arrangement.GetBorder();
   if (position == 0) {
      pparentsizer->Prepend(pToolWindow, proportion, flags, border);
   } else {
      // fuera de rango, lo agrega al final
      int count = pparentsizer->GetChildren().GetCount();
      if (position < 0 || position > count) {
         pparentsizer->Add(pToolWindow, proportion, flags, border);
      } else {
         // lo agrega en la posicion deseada
         pparentsizer->Insert(position, pToolWindow, proportion, flags, border);
      }
   }
   ptopframe = GetFrame();
   if (ptopframe) {
//      ptopframe->Fit();
//      ptopframe->Layout();
#ifdef __WXMSW__
//      size = ptopframe->GetSize();
//      ptopframe->SetSize(size.GetWidth()+1,size.GetHeight()+1);
//      ptopframe->SetSize(size.GetWidth(),size.GetHeight());
#endif
   }
   DEBUG_SIZE(pToolWindow->GetName().c_str(), pToolWindow, pToolWindow->GetBestSize());
   DEBUG_SIZE(pToolWindow->GetName().c_str(), pToolWindow, pToolWindow->GetSize());
   DEBUG_SIZE(pParentControl->GetName().c_str(), pParentControl, pParentControl->GetBestSize());
   DEBUG_SIZE(pParentControl->GetName().c_str(), pParentControl, pParentControl->GetSize());

   wxTopLevelWindow *pframe = GetFrame();
   if (pframe) {
      DEBUG_SIZE(pframe->GetName().c_str(), pframe, pframe->GetSize());
   }

   wxWindow *pwin = pToolWindow_;
   while (pwin->GetParent())
      pwin = pwin->GetParent();
//   pwin->InvalidateBestSize();
   /*   pParentControl->SetClientSize(wxSize(std::max(currentsize.GetWidth(),WantedWidth),
    std::max(currentsize.GetHeight(),WantedHeight)));*/
   DEBUG_SIZE(pParentControl->GetName().c_str(), pParentControl, pParentControl->GetSize());
   pToolWindow_->Layout();
   pParentControl->Layout();
   DEBUG_SIZE(pToolWindow_->GetName().c_str(), pToolWindow_, pToolWindow_->GetSize());
//   pwin->Fit();
//   pToolWindow->Thaw();
//   pToolWindow->Show();
   return true;
}

/*!
 * Elimina Componentes
 * @param[in] pControlWindow control a ser eliminado de la ventana en la que se
 * encuentra.
 * @return true.
 */
bool Widget::RemoveControl(wxWindow* pControlWindow) {
   // ventana que se desea remover, puede ser un ancestro de pControlWindow
   // (i.e. miniframe)
   wxWindow *ptopremovedwindow = pControlWindow;
   if (!ptopremovedwindow) {
      return false;
   }
   // se trata de una ventana dentro de un miniframe
   if (ptopremovedwindow->GetParent() &&
   ptopremovedwindow->GetParent()->GetName() == wxT("ID_WIDGET_PANEL"))
   {return RemoveControl(ptopremovedwindow->GetGrandParent() );
}

   // El metodo Hide deberia hacer obsoleto este codigo
#ifndef __UNUSED_CODE__
   ptopremovedwindow->Hide();
   // obtengo el sizer donde vive la ventana (pToolWindow_)
   wxSizer *psizer = ptopremovedwindow->GetContainingSizer();
   // si existe elimino la ventana del mismo
   if (psizer) {
      psizer->Detach(ptopremovedwindow);
      psizer->RecalcSizes();
      psizer->Layout();
      if (GetFrame()) {
         GetFrame()->Layout();
      }
   }
#else
   Hide(ptopremovedwindow);
#endif   // __UNUSED_CODE__
// intenta no utilizar reparent y trucos raros para eliminar un widget
#ifdef __UNUSED_CODE__
   ptopremovedwindow->Destroy();
   return true;
#endif
   // creo un wxFrame dummy y se lo asigno como parent
   // Asi, la puedo sacar de la ventana donde estaba
   REPORT_DEBUG("D:\"%s\" no manejada, creando wxFrame dummy.",
                GetWindowTitle().c_str());
   FLUSH();
   wxTopLevelWindow *pframe = GetFrame();
   wxTopLevelWindow *pdummy =
         !isAutonomous_ ? new wxFrame(pframe ? pframe : wxTheApp->GetTopWindow(),
                                      wxID_ANY, wxT("D:Ventana de destruccion")) :
                          pframe;
#ifdef __DEBUG__
   if (pdummy == pframe) {
      REPORT_DEBUG("D: Usando frame principal de %s", GetWindowTitle().c_str());
   }
#endif
   // la reparenteo
   ptopremovedwindow->Reparent(pdummy);
   // reorganizo los tamanios usando el padre original (y no pdummy)
   if (pframe)
#ifdef __WXGTK__
   {
      if (!pframe->GetParent()) {
         REPORT_DEBUG("D:%s->GetFrame()->GetParent() tiene parent NULL",
                      GetWindowTitle().c_str());
      } else
#endif
      {
#ifdef __UNUSED_CODE__
         // Con este codigo cambiaba el tamanio de ventana cuando se eliminaban botones del
         // NavigationWidget
//      pframe->Layout();
//      pframe->Fit();
         wxSize size = pframe->GetSize();
//      pframe->SetMinSize(wxSize(size.GetWidth(),size.GetHeight()));
#else
//      pframe->SetMinSize(pframe->GetSizer()->GetMinSize());
#endif
      }
#ifdef __WXGTK__
   }
#endif
   // Cierro el parent (en caso de autonoma corresponde y sino
   // el pParentWindow_ es el dummy del if de arriba)
   if (pdummy != pframe) {
      pdummy->Close();
   }
   return true;
}

/*!
 * Evento que limpia el statusbar cuando el mouse sale de la ventana.
 * \note soluciona el problema de los mensajes de error que se mostraban
 * hasta que otro mensaje lo reemplazara. Ahora el mensaje de error (y cualquier
 * mensaje)
 */
void Widget::OnMouseLeave() {
   SHOW_STATUS(wxT(""));
}

/*!
 * Agrega el Widget a la lista y manda a Destruir todos aquellos
 * cuyas ventanas no existen mas.
 * Frena la propagacion del evento.
 * @param[in] Event evento de destruccion de la ventana
 */
void Widget::OnDestroy(wxWindowDestroyEvent &Event) {
   // Me protejo de eventos sobre Widgets invalidos
   if (!Widget::IsValid(this)) {
      return;
   }
   wxString title = windowTitle_;
   REPORT_DEBUG(
         "D:%s(%p)::OnDestroy(),pToolWindow_(%p),pParentWindow_(%p),pEventHandler_(%p).",
         title.c_str(), this, pToolWindow_, pParentWindow_, pEventHandler_);
   // Si se destruyo la ventana (wxWindow) de la herramienta,
   // autodestruirse
   pToolWindow_ = NULL;
   pParentWindow_ = NULL;
   pToolFrame_ = NULL;
   destroyedWidgets_.insert(this);
   while (DeleteWidgets());
   Event.Skip(false);
   REPORT_DEBUG("D:%s(%p)::OnDestroy(return),destroyedWidgets_(%d).",
                title.c_str(), this, destroyedWidgets_.size());
}

/*!
 * Destruye los elementos guardados en el set estatico.
 * Evita codigo reentrante durante la destruccion.
 * @return false
 */
bool Widget::DeleteWidgets() {
   static bool preventreentry = false;
   if (preventreentry) {
      REPORT_DEBUG("D:Reentrando");
      return false;
   }
   preventreentry = true;
   REPORT_DEBUG("D: Iniciando Eliminacion de widgets");
   FLUSH();
   std::set<Widget*> widgetstodestroy(destroyedWidgets_);
   destroyedWidgets_.clear();
   std::set<Widget*>::iterator it = widgetstodestroy.begin();
   while (!widgetstodestroy.empty() && IsValid(*it)) {
      Widget *pwidget = *it;
      wxString windowtitle = pwidget->GetWindowTitle();
      REPORT_DEBUG("D: Eliminando herramienta %s(%p)", windowtitle.c_str(), pwidget);
      FLUSH();
      delete pwidget;
      widgetstodestroy.erase(it);
      REPORT_DEBUG("D: Herramienta eliminada %s(%p)", windowtitle.c_str(), pwidget);
      it = widgetstodestroy.begin();
   }
   preventreentry = false;
   REPORT_DEBUG("D: Finalizando Eliminacion de widgets");
   FLUSH();
   return false;
}

/*!
 * Determina si es valido. Para esto realiza una busqueda (alg. find) en la
 * propiedad de la clase validWidgets_.
 * @param[in] pWidget puntero a Widget a validar.
 * @return true en caso de encontrarlo.
 * @return false en caso de no encontrarlo.
 */
bool Widget::IsValid(Widget *pWidget) {
   return validWidgets_.find(pWidget) != validWidgets_.end();
}

#ifdef __SHOW_WIDGET_FIT_BUTTON__
/** Boton Fit en los widgets creados con la ventana padre (pParentWindow_ == NULL) */
void Widget::OnFitButton() {
   if ( GetFrame() ) {
      GetFrame()->Fit();
   }
}
#endif   // __SHOW_WIDGET_FIT_BUTTON__
#ifdef __SHOW_WIDGET_LAYOUT_BUTTON__
/** Boton Fit en los widgets creados con la ventana padre (pParentWindow_ == NULL) */
void Widget::OnLayoutButton() {
   if ( GetFrame() ) {
      GetFrame()->Layout();
   }
}
#endif   // __SHOW_WIDGET_LAYOUT_BUTTON__

#ifdef __WXMSW__
/**
 * Recorre los hijos de una ventana configurando el tamanio de los sizers para
 * que sea igual al de la ventana que los contiene
 * Ver TCK #2572 - El Visualizador 2D no aparece inicalmente en win7.
 */
void Widget::FixSizers(wxWindow* pWindow, const std::string &Spaces) {
   if (!pWindow->IsShown())
      return;

   wxSize windowsize =  pWindow->GetClientSize();
   REPORT_DEBUG("D:Window name: %s", pWindow->GetLabel().c_str());
   REPORT_DEBUG("D:%s Window size: h %d s %d", Spaces.c_str(),
                  windowsize.GetHeight(), windowsize.GetWidth());

   if (pWindow->GetSizer()) {
      wxSize sizersize =  pWindow->GetSizer()->GetSize();
      REPORT_DEBUG("D:%s Sizer size: h %d s %d", Spaces.c_str(),
                  sizersize.GetHeight(), sizersize.GetWidth());

      pWindow->GetSizer()->SetDimension(0, 0,
                  windowsize.GetWidth(), windowsize.GetHeight());

      sizersize =  pWindow->GetSizer()->GetSize();
      REPORT_DEBUG("D:%s Sizer size post: h %d s %d", Spaces.c_str(),
                  sizersize.GetHeight(), sizersize.GetWidth());
   }

   std::string spaces = Spaces + " .";
   wxWindowList& children = pWindow->GetChildren();
   for (size_t i = 0; i < children.size(); i++) {
      FixSizers(children[i], spaces);
   }
}
#endif

}  // namespace suri

/** @mainpage
 *
 * @page "suri::Widget" Tutorial Widget: HelloWorldTool
 *
 *  Siguiendo la lista de acciones para crear una herramienta que despliegue
 * una ventana con un texto y un boton que la cierre.
 *
 *  Se crea una clase derivada de Widget. Ejemplo del archivo .h:
 *
 * @code
 class HelloWorldTool : public suri::Widget
 {
 public:
 HelloWorldTool();
 virtual ~HelloWorldTool();
 protected:
 private:
 };
 * @endcode
 *
 *  Widget tiene funciones virtuales puras, es decir, tiene la definicion,
 * pero no existe el codigo. Estas funciones son obligatorias de implementar en
 * las clases derivadas.
 *
 *  Se declaran las funciones abstractas CreateToolWindow() y GetWindowTitle().
 * Queda la declaracion e implementacion de la clase de la siguiente manera.
 *
 * Archivo HelloWorldTool.h:
 *
 * @code
 class HelloWorldTool : public suri::Widget
 {
 public:
 HelloWorldTool();
 virtual ~HelloWorldTool();

 virtual bool CreateToolWindow();
 virtual const wxString GetWindowTitle()const;

 protected:
 private:
 };
 * @endcode
 *
 * Archivo HelloWorldTool..cpp:
 *
 * @code
 bool HelloWorldTool::CreateToolWindow()
 {
 wxPanel* ppanel = new wxPanel(pParentWindow_,wxID_ANY);

 wxBoxSizer* psizer = new wxBoxSizer(wxVERTICAL);
 ppanel->SetSizer(psizer);

 wxStaticText* pstatictext = new wxStaticText(ppanel,wxID_STATIC,
 _("Hello World Tool!"));
 psizer->Add(pstatictext,0,wxALIGN_CENTER_HORIZONTAL|wxALL,5);

 wxButton* pokbutton = new wxButton(ppanel,wxID_OK);
 psizer->Add(pokbutton,0,wxALIGN_CENTER_HORIZONTAL|wxALL,5);

 pToolWindow_ = ppanel;
 wantedWidth_ = 100;
 wantedHeight_ = 80;

 return true;
 }

 const wxString HelloWorldTool::GetWindowTitle()const
 {
 return _("Hello World Tool");
 }
 * @endcode
 *
 *  CreateToolWindow() debe crear un wxPanel con los controles y asignarselo a
 * pToolWindow_. El "parent window" de dicho wxPanel debe ser pParentWindow_. En
 * caso de que la creacion haya sido exitosa CreateToolWindow() debe retornar
 * true, en caso contrario false. Tambien puede setear wantedWidth_ y
 * wantedHeight_ como indicadores del tamanio minimo de la ventana (se va a
 * respetar el tamanio minimo debido a los controles si este es mayor que el
 * deseado)
 *
 *  GetWindowTitle() debe retornar el titulo de la ventana.
 *
 *  La herramienta ya se puede usar como esta; es autosuficiente y no produce
 * "leaks". Se la puede destruir con un delete a la herramienta o un Close() de
 * la ventana asociada (mediante el mouse).
 *
 *  Falta poder responder al evento del boton. Para ello se debe crear una clase
 * de ayuda que es la que recibe los eventos de la ventana y los forwardea a la
 * herramienta. La declaracion se realiza utilizando las macros
 * DECLARE_EVENT_OBJECT, FRIEND_EVENT_OBJECT y EVENT_OBJECT_PROPERTY en la
 * declaracion de la clase de la siguiente manera.
 *
 *  Archivo HelloWorldTool.h:
 *
 * @code
 DECLARE_EVENT_OBJECT(HelloWorldEvent);

 class HelloWorldTool : public suri::Widget
 {
 public:
 FRIEND_EVENT_OBJECT(HelloWorldEvent);

 HelloWorldTool();
 virtual ~HelloWorldTool();
 virtual bool CreateToolWindow();
 virtual const wxString GetWindowTitle()const;

 void OnButtonOk(wxCommandEvent &ButtonEvent);
 protected:
 private:
 EVENT_OBJECT_PROPERTY(HelloWorldEvent);
 };
 * @endcode
 *
 *  En respectivo orden a las macros; declaro el nombre de la clase de ayuda,
 * la hago friend de la clase herramienta y le agrego una propiedad privada
 * pEventHandler_ a la herramienta de clase de ayuda.
 *
 *  Al tener una propiedad de este tipo nuevo, es necesario inicializarla con
 * una instancia de la nueva clase, y destruirla al terminar con la herramienta.
 * Para ello se modifican el constructor y destructor de la clase herramienta.
 *
 * Archivo HelloWorldTool.cpp:
 *
 * @code
 HelloWorldTool::HelloWorldTool():NEW_EVENT_OBJECT(HelloWorldEvent)
 {
 }

 HelloWorldTool::~HelloWorldTool()
 {
 DELETE_EVENT_OBJECT;
 }
 * @endcode
 *
 *  Estas macros crean y destruyen la instancia de la clase que forwardea los
 * eventos. Para implementar esta clase de ayuda, se utilizan las macros:
 * START_IMPLEMENT_EVENT_OBJECT, IMPLEMENT_EVENT_CALLBACK y
 * END_IMPLEMENT_EVENT_OBJECT. START_IMPLEMENT_EVENT_OBJECT y
 * END_IMPLEMENT_EVENT_OBJECT abren y cierran la implementacion.
 * IMPLEMENT_EVENT_CALLBACK es la macro usada para forwardear el evento.
 *
 *  Tomando como ejemplo el evento del boton, se tiene que: declarar e
 * implementar la clase de ayuda, declarar e implementar el metodo en la clase
 * herramienta que responda al evento, agregar el metodo que forwardea el evento
 * y conectar el evento.
 *
 *  La primera tarea requiere el siguiente uso de macros, en el archivo
 * HelloWorldTool.cpp:
 *
 * @code
 START_IMPLEMENT_EVENT_OBJECT(HelloWorldEvent,HelloWorldTool)
 IMPLEMENT_EVENT_CALLBACK(OnButtonOk,OnButtonOk(event),wxCommandEvent)
 END_IMPLEMENT_EVENT_OBJECT
 * @endcode
 *
 *  La macro IMPLEMENT_EVENT_CALLBACK, genera un metodo OnButtonOk en la clase
 * de ayuda, que llama a OnButtonOk(event) en la clase herramienta que es del
 * tipo wxCommandEvent.
 *
 *  Dicho metodo OnButtonOk(event) de la clase herramienta debe ser declarado
 * e implementado en la clase herramienta.
 *
 * Archivo HelloWorldTool.h:
 *
 * @code
 ...
 void OnButtonOk(wxCommandEvent &ButtonEvent);
 ...
 * @endcode
 *
 * Archivo HelloWorldTool.cpp:
 *
 * @code
 void HelloWorldTool::OnButtonOk(wxCommandEvent &ButtonEvent)
 {
 pParentWindow_->Close();
 }
 * @endcode
 *
 *  Hasta aca se tiene el metodo que responde al boton y la clase con el metodo
 * que forwardea el evento a dicho metodo. Falta conectar el evento del boton
 * a la clase de ayuda. Para hacerlo se utiliza la macro
 * CONNECT_TOOL_EVENT, como se hizo una conexion de evento, es necesario
 * eliminarla antes de destruir la clase de ayuda con DISCONNECT_TOOL_EVENT.
 *
 *  La conexion se puede realizar en CreateToolWindow() y la desconexion en el
 * destructor antes de eliminar la instancia de la clase de ayuda con la macro
 * DELETE_EVENT_OBJECT.
 *
 *  La herramienta esta completa, para utilizarla se debe crear una instancia
 * y llamar al metodo CreateTool().
 *
 * Esto nos deja con un archivo HelloWorldTool.h:
 *
 * @code
 #ifndef HELLOWORLDTOOL_H_
 #define HELLOWORLDTOOL_H_

 #include "Widget.h"
 #include <wx/wx.h>

 DECLARE_EVENT_OBJECT(HelloWorldEvent);


 class HelloWorldTool : public suri::Widget
 {
 public:
 FRIEND_EVENT_OBJECT(HelloWorldEvent);

 HelloWorldTool();
 virtual ~HelloWorldTool();
 virtual bool CreateToolWindow();
 virtual const wxString GetWindowTitle()const;

 void OnButtonOk(wxCommandEvent &ButtonEvent);
 protected:
 private:
 EVENT_OBJECT_PROPERTY(HelloWorldEvent);
 };

 #endif
 * @endcode
 *
 *  Y un archivo HelloWorldTool.cpp:
 *
 * @code
 #include "HelloWorldTool.h"

 START_IMPLEMENT_EVENT_OBJECT(HelloWorldEvent,HelloWorldTool)
 IMPLEMENT_EVENT_CALLBACK(OnButtonOk,OnButtonOk(event),wxCommandEvent)
 END_IMPLEMENT_EVENT_OBJECT


 HelloWorldTool::HelloWorldTool():NEW_EVENT_OBJECT(HelloWorldEvent)
 {
 }

 HelloWorldTool::~HelloWorldTool()
 {
 DISCONNECT_TOOL_EVENT(HelloWorldEvent,OnButtonOk,
 wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler);
 DELETE_EVENT_OBJECT;
 }

 bool HelloWorldTool::CreateToolWindow()
 {
 wxPanel* ppanel = new wxPanel(pParentWindow_,wxID_ANY);

 wxBoxSizer* psizer = new wxBoxSizer(wxVERTICAL);
 ppanel->SetSizer(psizer);

 wxStaticText* pstatictext = new wxStaticText(ppanel,wxID_STATIC,
 _("Hello World Tool!"));
 psizer->Add(pstatictext,0,wxALIGN_CENTER_HORIZONTAL|wxALL,5);

 wxButton* pokbutton = new wxButton(ppanel,wxID_OK);
 psizer->Add(pokbutton,0,wxALIGN_CENTER_HORIZONTAL|wxALL,5);

 pToolWindow_ = ppanel;

 CONNECT_TOOL_EVENT(HelloWorldEvent,OnButtonOk,wxEVT_COMMAND_BUTTON_CLICKED,
 wxCommandEventHandler);
 return true;
 }

 const wxString HelloWorldTool::GetWindowTitle()const
 {
 return _("Hello World Tool");
 }

 void HelloWorldTool::OnButtonOk(wxCommandEvent &ButtonEvent)
 {
 pParentWindow_->Close();
 }

 * @endcode
 */

