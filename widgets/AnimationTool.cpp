/*! \file AnimationTool.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>
#include <list>
#include <vector>
#include <utility>

// Includes suri
#include "suri/AnimationTool.h"
#include "suri/LayerList.h"
#include "suri/ViewerWidget.h"
#include "suri/ProgressManager.h"
#include "suri/messages.h"
#include "resources.h"
#include "suri/DataViewManager.h"
#include "suri/SuriObject.h"
#include "suri/NodePath.h"
#include "suri/HtmlTreeWidget.h"
#include "suri/DynamicViewerWidget.h"
// Includes Wx
#include "wx/sizer.h"
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/valgen.h"

// Includes app

// Defines
/** Define ID del Timer */
#define WXTIMER_ID wxID_HIGHEST+2

#ifdef __FINE_TOOL_ENABLE_CHECK__
#  define IS_TOOL_ENABLED active_ &&                                          \
   pList_->GetSelectedCount(animatedGroup_)>1 &&      \
   !IsIncompatibleSelected() && !HasActiveNotSelected()
#else
/** Define condiciones para habilitar la herramienta */
#  define IS_TOOL_ENABLED active_ && pList_->GetSelectedCount(animatedGroup_)>1
#endif
/** Permite obtener el siguiente elemento a partir de un elemento dado */
#define SAFE_CURRENT_ELEMENT(elemcount)   currentElement_; currentElement_ = \
   (elemcount == 0 ? currentElement_ : currentElement_ < 0 ? currentElement_%elemcount ==  \
   0 ? 0 : (currentElement_ % elemcount)+elemcount : currentElement_ % elemcount)

// forward
/** inicializa los recursos */
void InitAnimationToolXmlResource();

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(AnimationEvent, AnimationTool)
   IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
   IMPLEMENT_EVENT_CALLBACK(OnTimer, OnTimer(), wxTimerEvent)
   IMPLEMENT_EVENT_CALLBACK(OnBackwardsButtonClick, OnBackwardsButtonClick(event),
                            wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnStopButtonClick, OnStopButtonClick(event),
                            wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnStartPauseButtonClick, OnStartPauseButtonClick(event),
                            wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnForwardButtonClick, OnForwardButtonClick(event),
                            wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRightMouseButtonClick, OnRightMouseButtonClick(event),
                                                         wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnLeftMouseButtonClick, OnLeftMouseButtonClick(event),
                                                         wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnSwapperButtonClick, OnSwapperButtonClick(event),
                                                             wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
// ------------------------------------------------------------------------------
/**
 * Constructor.
 * @param[in]	pListWindow: ventanas que se deben deshabilitar cuando se esta
 * animando y habilitar cuando no se esta animando.
 * @param[in]	pContextWindow: puntero a la ventana del contexto asociado
 * @param[in]	ListGroup: nro de grupo de elementos a animar.
 * @param[in]	pParent:  puntero a ventana padre que la contiene.
 */
AnimationTool::AnimationTool(wxWindow *pListWindow, wxWindow *pContextWindow,
                             int ListGroup, wxWindow *pParent,
                             DataViewManager* pDataViewManager,
                             HtmlTreeWidget* pTreeWidget) :
      Widget(pParent), NEW_EVENT_OBJECT(AnimationEvent),
      pAnimationTimer_(NULL), currentElement_(-1), animatedGroup_(ListGroup),
      pCurrentElement_(NULL), isStillRendering_(false), framesPerSecond_(1),
      shouldStop_(true),
      pDataViewManager_(pDataViewManager), pTreeWidget_(pTreeWidget) {
   windowTitle_ = _(caption_ANIMATION_TOOL);
   toolName_ = GetWindowTitle();
   dependentWindows_.insert(pListWindow);
   dependentWindows_.insert(pContextWindow);
   activeSwap_ = false;
}

// ------------------------------------------------------------------------------
/**
 * Detiene el timer, desconecta eventos y los elimina.
 */
AnimationTool::~AnimationTool() {
   if (pAnimationTimer_ != NULL) {
      pAnimationTimer_->Stop();
      pEventHandler_->Disconnect(WXTIMER_ID, wxEVT_TIMER,
                              wxTimerEventHandler(AnimationEvent::OnTimer));
      delete pAnimationTimer_;
      // elimino el helper de eventos
      DELETE_EVENT_OBJECT;
   }
}

// ------------------------------------------------------------------------------
/**
 * Creacion de la ventana que permite animar elementos.
 * Tambien crea el timer que realiza la animacion.
 * Crea (carga) un toolbar:
 *  - 1 slider (fps) para modificar la velocidad de animacion
 *  - 4 botones (atras, stop, play, adelante), para controlar la animacion.
 * Asigna los eventos a cada control. Se establece un validador al slider
 * con el objetivo de obtener/establecer los valores del mismo con el idiom que
 * propone Wx.
 * @return true Siempre true.
 */
bool AnimationTool::CreateToolWindow() {
   // tamanio minimo deseado de la ventana del visualizador en modo
   // standalone (isAutonomous_==true)
   wantedWidth_ = 200;
   wantedHeight_ = 0;
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_ANIMATION_TOOL"));
   // Forma no estandar de conectarme al evento del timer
   pAnimationTimer_ = new wxTimer(pEventHandler_, WXTIMER_ID);
   pEventHandler_->Connect(WXTIMER_ID, wxEVT_TIMER,
                           wxTimerEventHandler(AnimationEvent::OnTimer));
   wxStaticText *ptext = XRCCTRL(*pToolWindow_, wxT("ID_FPS_LABEL"), wxStaticText);
   if (ptext) {
      ptext->Connect(wxEVT_UPDATE_UI,
                     wxUpdateUIEventHandler(AnimationEvent::OnUIUpdate), NULL,
                     pEventHandler_);
   }
   wxSlider *pslider = XRCCTRL(*pToolWindow_, wxT("ID_SPEED_SLIDER"), wxSlider);
   if (pslider) {
      pslider->SetValidator(wxGenericValidator(&framesPerSecond_));
   }
   controls_.push_back(pslider);
   wxBitmapButton *pbutton = XRCCTRL(*pToolWindow_, wxT("wxID_BACKWARD"),
                                     wxBitmapButton);
   if (pbutton) {
      pbutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                       wxCommandEventHandler(AnimationEvent::OnBackwardsButtonClick),
                       NULL, pEventHandler_);
   }
   controls_.push_back(pbutton);
   pbutton = XRCCTRL(*pToolWindow_, wxT("wxID_STOP"), wxBitmapButton);
   if (pbutton) {
      pbutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                       wxCommandEventHandler(AnimationEvent::OnStopButtonClick), NULL,
                       pEventHandler_);
   }
   controls_.push_back(pbutton);
   pbutton = XRCCTRL(*pToolWindow_, wxT("ID_START_STOP"), wxBitmapButton);
   if (pbutton) {
      pbutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                       wxCommandEventHandler(AnimationEvent::OnStartPauseButtonClick),
                       NULL, pEventHandler_);
   }
   controls_.push_back(pbutton);
   pbutton = XRCCTRL(*pToolWindow_, wxT("wxID_FORWARD"), wxBitmapButton);
   if (pbutton) {
      pbutton->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                       wxCommandEventHandler(AnimationEvent::OnForwardButtonClick),
                       NULL, pEventHandler_);
   }
   controls_.push_back(pbutton);
   ptoggle_ = XRCCTRL(*pToolWindow_, wxT("wxID_SWAPPER"), wxBitmapButton);
   if (ptoggle_) {
         ptoggle_->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                          wxCommandEventHandler(AnimationEvent::OnSwapperButtonClick),
                          NULL, pEventHandler_);
   }
   return true;
}





// ------------------------------------------------------------------------------
/** Devuelve titulo de la ventana */
const wxString AnimationTool::GetWindowTitle() const {
   return _(caption_ANIMATION_TOOL);
}

// ------------------------------------------------------------------------------
/**
 * \post	Con herramienta inactiva, setea el bitmap de play al boton de
 *       play/pause y habilita las ventanas pasadas en el ctor.(Ver param. pListWindow)
 * \post	Con herramienta activa, obtiene el ultimo elemento seleccionado
 * @param[in] Active estado de activacion de la herramienta
 * @return informa si la operacion se pudo realizar
 */
bool AnimationTool::DoSetActive(bool Active) {
   // si no hay elementos retorno.
   if (!pList_ || selectednodes_.size() == 0) {
      REPORT_DEBUG("D:Lista no inicializada.");
      return false;
   }
   if (!Active && pAnimationTimer_ != NULL) {
         pAnimationTimer_->Stop();

         wxBitmapButton* pbutton = XRCCTRL(*pToolWindow_, wxT("ID_START_STOP"),
                                        wxBitmapButton);
      wxBitmap bmp;
      GET_BITMAP_RESOURCE(wxT(button_START_ANIMATION), bmp);
      // Establece el boton de Start
      pbutton->SetBitmapLabel(bmp);

      EnableControls(true);
   } else {
      pCurrentElement_ = GetActiveSelected();
   }

   return true;
}

// ------------------------------------------------------------------------------
/**
 * Agrega la ventana del visualizador al conjunto de ventanas
 * dependientes de la herramienta, este conjunto es el que se habilita o
 * deshabilita segun la accion que este realizando la herramienta.
 * \pre pViewer debe ser no nulo.
 * @param[in]	pViewer: visualizador del contexto asociado.
 */
bool AnimationTool::DoSetViewer(ViewerWidget *pViewer) {
   AddDependentWindow(pViewer->GetWindow());
   return true;
}

// ------------------------------------------------------------------------------
/** Indica si esta habilitado o no */
/**
 * Este metodo establece el estado de la herramienta a activo o desactivo y
 * devuelve el estado establecido.
 * @return true si los elementos seleccionados cumplen el criterio para la animacion.
 * @return false en caso contrario.
 */
bool AnimationTool::IsEnabled() {
   std::list<suri::NodePath> selectednodes =
         pDataViewManager_->GetSelectionManager()->GetSelectedNodes();
   if ((selectednodes.size() > 1) && !activeSwap_) {
      EnableControls(true);
      UpdateLayerList();
   } else if (activeSwap_) {
      EnableControls(false);
   }
   if (GetActiveViewPortsIds().size()>1) {
      EnableToggle(true);
   } else {
      EnableToggle(false);
   }
   return true;
}

/** Actualiza la lista de imagenes seleccionadas **/
void AnimationTool::UpdateLayerList() {
   std::list < suri::NodePath > newselection =
         pDataViewManager_->GetSelectionManager()->GetSelectedNodes();
   selectednodes_ = newselection;
}

// ------------------------------------------------------------------------------
/**
 * @pre Si hay un elemento seleccionado.
 * @post activa el elemento.
 * Si no se cumple la precondicion habilita las ventanas pasadas en el ctor.(Ver param. pListWindow)
 */
void AnimationTool::ShowCurrentElement() {
   if (pCurrentElement_) {
      isStillRendering_ = true;
      pCurrentElement_->Activate(true);
      isStillRendering_ = false;
      if (pTreeWidget_) {
         pTreeWidget_->UpgradeControlContent();
         pTreeWidget_->ForceRefresh();
      }
   } else {
      // Si no encuentro el siguiente elemento (retorna NULL)
      EnableControls(true);
   }
}

// ------------------------------------------------------------------------------
/**
 * Evento UIUpdate, se ejecuta cuando no hay otros eventos de usuario en la cola
 * de eventos de Wx. Lee el valor del slider de velocidad(fps) y carga dicho
 * valor en la TextCtrl.
 * @param[in]	Event: evento.
 */
void AnimationTool::OnUIUpdate(wxUpdateUIEvent &Event) {
   pToolWindow_->TransferDataFromWindow();
   Event.SetText(wxString::Format("%2d", framesPerSecond_));
}

// ------------------------------------------------------------------------------
/**
 * Evento de Timer. Es el que realiza la animacion de los elementos,
 * deseleccionando el activo, seleccionando el siguiente. y activandolo.
 */
void AnimationTool::OnTimer() {
   if (isStillRendering_) {
      REPORT_DEBUG("D:Saltea timer");
      return;
   }REPORT_DEBUG("D:Timer");
   // si no esta activa o debe detener, freno y salgo
   if ((!active_ || shouldStop_) && pAnimationTimer_ != NULL) {
      pAnimationTimer_->Stop();
      EnableControls(true);
      return;
   }

   // si hay activo seleccionado lo desactiva
   pCurrentElement_ = GetActiveSelected();
   if (pCurrentElement_) {
   // Para que dibuje solo en el Activate(true) siguente
      pCurrentElement_->PreventNextViewerUpdate();
      pCurrentElement_->Activate(false);
   }

   // Obtiene el proximo elemento seleccionado
   pCurrentElement_ = GetNextSelected();

   // Despliega el pCurrentElement_
   ShowCurrentElement();

   // si el timer esta activo vuelve a tirarlo con la velocidad deseada
   REPORT_DEBUG("D:Reiniciando el timer");

   // Si no debo frenar el timer dispara uno nuevo
   if (!shouldStop_) {
      wxTheApp->Yield();
      pToolWindow_->TransferDataFromWindow();
      pAnimationTimer_->Start(1000 / framesPerSecond_, wxTIMER_ONE_SHOT);
   }
}

// ------------------------------------------------------------------------------
/**
 * Metodo asociado a eventos de click sobre el boton (wxID_BACKWARD).
 * Este metodo setea el bitmap de play al boton de play/pause y habilita las
 * ventanas pasadas en el ctor.(Ver param. pListWindow) si se esta ejecutando la
 * animacion.
 * Desactiva el elemento seleccionado y muestra el elemento anterior.
 * @param[in]	Event: evento.
 */
void AnimationTool::OnBackwardsButtonClick(wxCommandEvent &Event) {
   if (IsIncompatibleSelected() || HasActiveNotSelected()) {
      return;
   }

   if (pAnimationTimer_->IsRunning()) {
      EnableControls(true);
      wxBitmapButton* pbutton = XRCCTRL(*pToolWindow_, wxT("ID_START_STOP"),
                                        wxBitmapButton);
      wxBitmap bmp;
      GET_BITMAP_RESOURCE(wxT(button_START_ANIMATION), bmp);
      pbutton->SetBitmapLabel(bmp);
   }
   BackwardAnimation();
}


// ------------------------------------------------------------------------------
/**
 * Metodo asociado a eventos de click sobre el boton (wxID_STOP).
 * Si la animacion esta corriendo, la detiene y setea el bitmap de play al boton
 * de play/pause.
 * Deselecciona el elemento activo y muestra el primer elemento de la lista.
 * @param[in]	Event: evento
 */
void AnimationTool::OnStopButtonClick(wxCommandEvent &Event) {
   if (pAnimationTimer_->IsRunning()) {
      EnableControls(true);
      active_ = false;
      // Si frena establece el boton de start
      wxBitmapButton* pbutton = XRCCTRL(*pToolWindow_, wxT("ID_START_STOP"),
                                        wxBitmapButton);
      wxBitmap bmp;
      GET_BITMAP_RESOURCE(wxT(button_START_ANIMATION), bmp);
      pbutton->SetBitmapLabel(bmp);
   }
}

// ------------------------------------------------------------------------------
/**
 * Metodo asociado a eventos de click sobre el boton (ID_START_STOP).
 * Activa/Pausa la animacion.
 * Verifica que los elementos seleccionados sean compatibles(tipo y grupo).
 * @pre animacion no esta corriendo y se presiona el boton
 * @post establece el bitmap de pausa y ejecuta la animacion
 * @pre animacion esta corriendo y se presiona el boton
 * @post establece el bitmap de play y detiene la animacion
 * @param[in]	Event: evento
 */
void AnimationTool::OnStartPauseButtonClick(wxCommandEvent &Event) {
   if (!pAnimationTimer_->IsRunning()) {
      UpdateLayerList();
      if (IsIncompatibleSelected() || HasActiveNotSelected()) {
          return;
       }
   }
   wxBitmapButton* pbutton = XRCCTRL(*pToolWindow_, wxT("ID_START_STOP"),
                                     wxBitmapButton);
   wxBitmap bmp;

   if (pAnimationTimer_->IsRunning()) {
      EnableControls(true);
      GET_BITMAP_RESOURCE(wxT(button_START_ANIMATION), bmp);
      // Establece el boton de Start
   } else {
      EnableControls(false);
      GET_BITMAP_RESOURCE(wxT(button_PAUSE_ANIMATION), bmp);
      // Establece el boton de Pause
   }
   pbutton->SetBitmapLabel(bmp);
   StartAnimation();
}

// ------------------------------------------------------------------------------
/**
 * Metodo asociado a eventos de click sobre el boton (wxID_FORWARD).
 * Este metodo setea el bitmap de play al boton de play/pause y habilita las
 * ventanas pasadas en el ctor.(Ver param. pListWindow) si se esta ejecutando la
 * animacion.
 * Desactiva el elemento seleccionado y muestra el elemento siguiente.
 * @param[in]	Event: evento.
 */
void AnimationTool::OnForwardButtonClick(wxCommandEvent &Event) {
   if (IsIncompatibleSelected() || HasActiveNotSelected()) {
      return;
   }

   if (pAnimationTimer_->IsRunning()) {
      EnableControls(true);
      // Si frena establece el boton de start
      wxBitmapButton* pbutton = XRCCTRL(*pToolWindow_, wxT("ID_START_STOP"),
                                        wxBitmapButton);
      wxBitmap bmp;
      GET_BITMAP_RESOURCE(wxT(button_START_ANIMATION), bmp);
      pbutton->SetBitmapLabel(bmp);
   }
   ForwardAnimation();
}

// ------------------------------------------------------------------------------
/**
 * Metodo para des/habilitar los controles pasados en el ctor.(param. pListWindow).
 * Para Enabled == false modifica el ProgressManager para usar uno nulo ya que el
 * estandar ocasiona problemas por desbloquear ventanas sin verificar si estaban
 * desactivadas al iniciar.
 * \post shouldStop_ toma el valor de Enabled para detener el Timer en la siguiente
 *       iteracion si se habilitan los controles.
 * @param[in]	Enabled: indica si debe habilitar o no los controles.
 */
void AnimationTool::EnableControls(bool Enabled) {
   // Hablilita o deshabilita todas las ventantas del vector dependentWindows_
   // Tool::EnableControls(Enabled);
   std::vector<wxWindow*>::const_iterator it = controls_.begin();
   for (; it != controls_.end(); ++it) {
         (*it)->Enable(Enabled);
   }
   // Establece el progressManager
   if (Enabled) {
      ProgressManager::SetCreateFunction(WxProgressManager);
   } else {
      ProgressManager::SetCreateFunction(NullProgressManager);
   }

   // Determina si debe parar o no en el OnTimer siguente
   shouldStop_ = Enabled;
}

void AnimationTool::EnableToggle(bool Enabled) {
     ptoggle_->Enable(Enabled);
     shouldStop_ = Enabled;
}
// ------------------------------------------------------------------------------
/**
 * Obtiene el elemento siguiente, a partir del elemento seleccionado.
 * @return puntero al siguente elemento seleccionado del grupo animado.
 * @return NULL en caso de error o que no haya elementos seleccionados en
 *         el grupo.
 * \attention el puntero retornado no debe ser eliminado.
 */
Element *AnimationTool::GetNextSelected() {
//   if (pList_->GetSelectedCount(animatedGroup_) == 0) {
//      return NULL;
//   }
//   int elemcount = pList_->GetElementCount(animatedGroup_);
   int elemcount = selectednodes_.size();
   if (elemcount == 0) {
      return NULL;
   }
   ++SAFE_CURRENT_ELEMENT(elemcount);
//   Element *pelement = pList_->GetElement(currentElement_, animatedGroup_);
//   while (!pList_->GetElementData(pelement)->selected_) {
//      ++SAFE_CURRENT_ELEMENT(elemcount);
//      pelement = pList_->GetElement(currentElement_, animatedGroup_);
//   }
   std::list<suri::NodePath>::iterator it = selectednodes_.begin();
   std::advance(it, currentElement_);
   if (it == selectednodes_.end())
      return NULL;
   Element *pelement = NULL;
   if (!it->AllowsChildren()) {
      suri::ViewcontextInterface* pviewcontext =
            pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
      if (pviewcontext) {
         suri::LayerInterface* player = pviewcontext->GetAssociatedLayer(
               it->GetLastPathNode()->GetContent());
         if (player) {
            pelement = player->GetElement();
         }
      }
   }
   return pelement;
}

// ------------------------------------------------------------------------------
/**
 * Obtiene el elemento anterior al elemento seleccionado del grupo animado
 * @return puntero al elemento anterior al seleccionado del grupo animado
 * @return NULL en caso de error o que no haya elementos seleccionados en el grupo.
 * \attention el puntero retornado no debe ser eliminado.
 */
Element *AnimationTool::GetPrevSelected() {
//   if (pList_->GetSelectedCount(animatedGroup_) == 0) {
//      return NULL;
//   }
//   int elemcount = pList_->GetElementCount(animatedGroup_);
   int elemcount = selectednodes_.size();
   if (elemcount == 0) {
      return NULL;
   }
   if (currentElement_ < 0) {
      GetNextSelected();  // entra a la lista
   }
   --SAFE_CURRENT_ELEMENT(elemcount);
//   Element *pelement = pList_->GetElement(currentElement_, animatedGroup_);
//   while (!pList_->GetElementData(pelement)->selected_) {
//      --SAFE_CURRENT_ELEMENT(elemcount);
//      pelement = pList_->GetElement(currentElement_, animatedGroup_);
//   }
   std::list<suri::NodePath>::iterator it = selectednodes_.begin();
   std::advance(it, currentElement_);
   if (it == selectednodes_.end())
      return NULL;
   Element *pelement = NULL;
   if (!it->AllowsChildren()) {
      suri::ViewcontextInterface* pviewcontext =
            pDataViewManager_->GetViewcontextManager()->GetSelectedViewcontext();
      if (pviewcontext) {
         suri::LayerInterface* player = pviewcontext->GetAssociatedLayer(
               it->GetLastPathNode()->GetContent());
         if (player) {
            pelement = player->GetElement();
         }
      }
   }
   return pelement;
}

// ------------------------------------------------------------------------------
/**
 * Obtiene el elemento activo que este seleccionado.
 * @return puntero al elemento activo seleccionado
 * @return NULL en caso de error.que no haya elemento seleccionado
 * \post Si no hay elemento seleccionado establece currentElement_ = -1 que es la
 *       condicion inicial para la propiedad.
 * \attention el puntero retornado no debe ser eliminado.
 */
Element* AnimationTool::GetActiveSelected() {
   Element *pelement = NULL;
   Element *pinitialelement = NULL;

   // Si el elemento actual es el activo y seleccionado lo devuelve
   if (pCurrentElement_ && pCurrentElement_->IsActive()) {
         // && pList_->GetElementData(pCurrentElement_)->selected_) {
      return pCurrentElement_;
   }

   // Sino loopea sobre la lista
   pinitialelement = GetNextSelected();  // Obtiene el proximo seleccionado
   pelement = pinitialelement;  // guarda el inicial para que de una vuelta completa
   do {
      if (pelement && pelement->IsActive()
            && pList_->GetElementData(pelement)->selected_) {
         return pelement;
      }

      pelement = GetNextSelected();
   } while (pinitialelement != pelement);

   currentElement_ = -1;
   return NULL;
}

// ------------------------------------------------------------------------------
/**
 * Determina si hay por lo menos un elemento del grupo animado activo que no este seleccionado.
 * @return true si hay por lo menos un elemento activo que no este seleccionado.
 * @return true Si la cantidad de elementos del grupo animado es 0.
 * @return false C.O.C.
 */
bool AnimationTool::HasActiveNotSelected() {
  /* int elemcount = pList_->GetElementCount(animatedGroup_);
   if (elemcount == 0) {
      return true;
   }
   for (int i = 0; i < elemcount; i++) {
      Element *pelement = pList_->GetElement(i, animatedGroup_);
      // si esta activo y no seleccionado retorno verdadero
      bool active=pelement->IsActive();
      bool selected=pelement->GetSelected();///pList_->GetElementData(pelement)->selected_;
      if (pelement &&active
            && !selected) {
         SHOW_INFO(message_ACTIVE_ELEMENT_OUTSIDE_SELECTION_FOR_ANIMATION_ERROR);
         // no se traduce,
         // lo hace SHOW
         return true;
      }
   }*/
   return false;
}

// ------------------------------------------------------------------------------
/**
 * Determina si hay al menos un elemento del grupo animado seleccionado con
 * referencia espacial distinta a la referencia espacial del primer elemento
 * seleccionado.
 * @return true Si existe al menos un elemento del grupo con Spatial Reference distinto
 * @return false Si todos los elementos del grupo tienen la misma referencia espacial.
 * \note La comparacion de referencias espaciales se realiza a nivel string.
 * \todo considerar utilizar transformacion de coordenadas y verificar por la identidad.
 */
bool AnimationTool::IsIncompatibleSelected() {
   bool retval = true;
   int prevcurrent = currentElement_;
   // inicializo en -1 para recorrer la lista
   currentElement_ = -1;
   Element *pelement = GetNextSelected();
   int firstselected = currentElement_;
   if (pelement) {
      retval = false;
      std::string spatialref = pelement->GetSpatialReference().c_str();
      do {
         pelement = GetNextSelected();
         if (!pelement) {
            retval = true;
            break;
         }
         std::string currspatialref = pelement->GetSpatialReference().c_str();
         if (spatialref != currspatialref) {
            retval = true;
            SHOW_INFO(message_INVALID_ELEMENT_SELECTION_FOR_ANIMATION_ERROR);
            // no se traduce, lo
            // hace SHOW_
            REPORT_DEBUG(
                  "D:El elemento %d del grupo %d tiene referencia espacial :"
                  " \n\t\t\t%s\nSe esperaba: \n\t\t\t\%s",
                  currentElement_, animatedGroup_, spatialref.c_str(),
                  currspatialref.c_str());
            break;
         }
      } while (currentElement_ != firstselected);
   }
   currentElement_ = prevcurrent;
   return retval;
}

/** Metodo que realiza una avanza hacia atras un frame */
void AnimationTool::BackwardAnimation() {
   if (IsIncompatibleSelected())
      return;
   if (pAnimationTimer_ && pAnimationTimer_->IsRunning() && pAnimationTimer_ != NULL) {
      pAnimationTimer_->Stop();
   }

   // si hay activo seleccionado lo desactiva
   pCurrentElement_ = GetActiveSelected();
   if (pCurrentElement_) {
      pCurrentElement_->PreventNextViewerUpdate();
      pCurrentElement_->Activate(false);
   }

   // Obtiene el elemento anterior
   pCurrentElement_ = GetPrevSelected();

   // Despliega el pCurrentElement_
   ShowCurrentElement();
}

/** Metodo que realiza una avanza hacia adelante un frame */
void AnimationTool::ForwardAnimation() {
   if (IsIncompatibleSelected())
      return;
   if (pAnimationTimer_ && pAnimationTimer_->IsRunning() && pAnimationTimer_ != NULL) {
      pAnimationTimer_->Stop();
   }

   // si hay activo seleccionado lo desactiva
   pCurrentElement_ = GetActiveSelected();
   if (pCurrentElement_) {
      pCurrentElement_->PreventNextViewerUpdate();
      pCurrentElement_->Activate(false);
   }

   // Obtiene el proximo elemento
   pCurrentElement_ = GetNextSelected();

   // Despliega el pCurrentElement_
   ShowCurrentElement();
}

/** Metodo que detiene la animacion en curso */
void AnimationTool::StopAnimation() {
   if (pAnimationTimer_ && pAnimationTimer_->IsRunning() && pAnimationTimer_ != NULL) {
       pAnimationTimer_->Stop();
    }

    // si hay activo seleccionado lo desactiva
    pCurrentElement_ = GetActiveSelected();
    if (pCurrentElement_) {
       pCurrentElement_->PreventNextViewerUpdate();
       pCurrentElement_->Activate(false);
    }

    // Obtiene el primer elemento
    currentElement_ = -1;
    pCurrentElement_ = GetNextSelected();
    // Muestra el primer elemento
    ShowCurrentElement();
}

/** Metodo que inicia una animacion */
void AnimationTool::StartAnimation() {
   if (IsIncompatibleSelected())
      return;
   if (!pAnimationTimer_) {
      pAnimationTimer_ = new wxTimer(pEventHandler_, WXTIMER_ID);
      pEventHandler_->Connect(WXTIMER_ID, wxEVT_TIMER,
                              wxTimerEventHandler(AnimationEvent::OnTimer));
   }
   if (pAnimationTimer_->IsRunning() && pAnimationTimer_ != NULL) {
       pAnimationTimer_->Stop();
    } else {
       pAnimationTimer_->Start(1, wxTIMER_ONE_SHOT);
       active_ = true;
    }
}

void AnimationTool::OnSwapperButtonClick(wxCommandEvent &Event) {
   wxBitmap bmp;
   if (!activeSwap_) {
     GET_BITMAP_RESOURCE(wxT(button_SWAPPER_OFF), bmp);
     ptoggle_->SetBitmapLabel(bmp);
     Views_ = GetActiveViewPorts();
     originalState_ = GetActiveRasterIdFromViewPorts();
     lastState_ = originalState_;
     std::vector<ViewportInterface*>::const_iterator it = Views_.begin();
     int x = 0;
     for (; it != Views_.end(); ++it) {
        DynamicViewerWidget* view = dynamic_cast<DynamicViewerWidget*>(*it);
        if (view) {
           swapState_[view->GetViewer()->GetWindow()->GetId()] = std::make_pair(x, x);
           view->GetViewer()->GetWindow()->Connect(wxEVT_RIGHT_DOWN, wxCommandEventHandler
                                                   (AnimationEvent::OnRightMouseButtonClick),
                                                                              NULL, pEventHandler_);
           view->GetViewer()->GetWindow()->Connect(wxEVT_LEFT_DOWN,
                                    wxCommandEventHandler(AnimationEvent::OnLeftMouseButtonClick),
                                    NULL, pEventHandler_);
           x++;
         }
      }
      activeSwap_ = true;
   } else {
      GET_BITMAP_RESOURCE(wxT(button_SWAPPER_ON), bmp);
      ptoggle_->SetBitmapLabel(bmp);
      Views_ = GetActiveViewPorts();
          std::vector<ViewportInterface*>::const_iterator it = Views_.begin();
         for (; it != Views_.end(); ++it) {
            DynamicViewerWidget* view = dynamic_cast<DynamicViewerWidget*>(*it);
            if (view) {
                view->GetViewer()->GetWindow()->Disconnect(wxEVT_RIGHT_DOWN,
                           wxCommandEventHandler(AnimationEvent::OnRightMouseButtonClick),
                           NULL, pEventHandler_);
                view->GetViewer()->GetWindow()->Disconnect(wxEVT_LEFT_DOWN,
                           wxCommandEventHandler(AnimationEvent::OnLeftMouseButtonClick),
                           NULL, pEventHandler_);
             }
          }
      activeSwap_ = false;
   }
}

void AnimationTool::OriginalStateUpdate() {
   std::vector<ViewportInterface*>::const_iterator it = Views_.begin();
   int index = 0;
   for (; it != Views_.end(); ++it) {
      ViewerWidget* viw = dynamic_cast<ViewerWidget*>(*it);
      LayerList* plist = viw->GetList();
      // primer elemento del grupo de los rasters
      Element* pActiveElement = plist->GetActiveElement(0, 6);
      if (pActiveElement) {
         if (pActiveElement->GetUid() != lastState_[index]) {
            // el usuario desplego otra imagen
            originalState_[index] = pActiveElement->GetUid();
            lastState_[index] = pActiveElement->GetUid();
         }
      }
      index++;
   }
}

void AnimationTool::Swap(int Id, bool Clockwise) {
   OriginalStateUpdate();
   int operation = 0;
   if (Clockwise){
      operation = 1;
   } else {
      operation = -1;
   }
   std::pair<int, int> next = swapState_[Id];
   ViewerWidget* viw = dynamic_cast<ViewerWidget*>(Views_[next.first]);
   LayerList* list = viw->GetList();
   Element* pactive = list->GetActiveElement(0, 6);  // primer elemento del grupo de los rasters
   if (pactive) {
       pactive->Activate(false);
   }
   int index = next.second+operation;
   if (index >= static_cast<int>(Views_.size())) {
      index = 0;
   } else if (index < 0) {
      index = Views_.size()-1;
   }
   ViewerWidget* viwNext = dynamic_cast<ViewerWidget*>(Views_[index]);
   std::string elementId = "";
   if (index != next.first) {
      LayerList* listNextView = viwNext->GetList();
      Element* pNextelement=  listNextView->GetActiveElement(0, 6);
      if (pNextelement) {
         elementId = pNextelement->GetUid();
      }
   } else {
      if (static_cast<int>(originalState_.size()) > index) {
         elementId = originalState_[index];
      } else {
         elementId = -1;
      }
   }
   Element* pelement = list->GetElement(elementId);
   if (pelement) {
      pelement->Activate(true);
      viw->SetWindowTitle(pelement->GetName());
      lastState_[next.first] = elementId;
   } else {
      list->DisableAll();  // si no hay elemento escondo los shp
   }
   swapState_[Id] = std::make_pair(next.first, index);
}

void AnimationTool::OnRightMouseButtonClick(wxCommandEvent &Event) {
   Swap(Event.GetId(), true);
}
void AnimationTool::OnLeftMouseButtonClick(wxCommandEvent &Event) {
   Swap(Event.GetId(), false);
}

std::vector<std::string> AnimationTool:: GetActiveRasterIdFromViewPorts() {
   std::vector<ViewportInterface*>::const_iterator it = Views_.begin();
   std::vector<std::string> result;
   for (; it != Views_.end(); ++it) {
         ViewerWidget* casted = dynamic_cast<ViewerWidget*>((*it));
         LayerList* list = casted->GetList();
         Element* pelement = list->GetActiveElement(0, 6);
         if (pelement) {
            result.push_back(pelement->GetUid());
         } else {
            result.push_back("");
         }
   }
   return result;
}


/**Obtiene los ids de los viewPorts activos**/
std::vector<std::string> AnimationTool::GetActiveViewPortsIds() {
   ViewportManagerInterface* pViewportManager = pDataViewManager_->GetViewportManager();
   std::vector<std::string> activeViewPorts;
   std::vector<std::string> viewIds = pViewportManager->GetViewportIds();
   std::vector<std::string>::const_iterator it = viewIds.begin();
   for (; it != viewIds.end(); ++it) {
       if ( pViewportManager->IsViewportActive((*it)) ) {
          activeViewPorts.push_back(*it);
       }
   }
   return activeViewPorts;
}

/**
 * Devuelve los ids de los elementos activos de todos los visualizadores en orden
 * un vector<string> por visualizador
 */
std::vector< std::vector<std::string> > AnimationTool::GetActiveLayersIdFromViewPorts() {
   std::vector<ViewportInterface*>::const_iterator it = Views_.begin();
   std::vector< std::vector<std::string> > result;
   for (; it != Views_.end(); ++it) {
      std::vector<std::string> activeElementsPerView;
      ViewerWidget* casted = dynamic_cast<ViewerWidget*>((*it));
      LayerList* list = casted->GetList();
      std::vector<Element*> elements = list->GetRenderizationOrderList();
      std::vector<Element*>::const_iterator it2 = elements.begin();
      for (; it2 != elements.end(); ++it2) {
         if (*it2) {
               if ( (*it2)->IsActive() ) {
                  activeElementsPerView.push_back((*it2)->GetUid());
               }
           }
      }
      result.push_back(activeElementsPerView);
   }
   return result;
}

std::vector<ViewportInterface*> AnimationTool::GetActiveViewPorts() {
   std::vector<ViewportInterface*> actives;
   ViewportManagerInterface* pViewportManager = pDataViewManager_->GetViewportManager();
   std::vector<std::string> ids = GetActiveViewPortsIds();
   std::vector<std::string>::const_iterator it = ids.begin();
   for (; it != ids.end(); ++it) {
      actives.push_back(pViewportManager->GetViewport((*it)));
   }
   return actives;
}

/** Indica la velocidad de la animacion    */
void AnimationTool::SetFrameRate(int Rate) {
   framesPerSecond_ = Rate;
}
}  // namspace suri
