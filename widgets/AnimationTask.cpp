/*! \file AnimationTask.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <list>
#include <string>
#include <iterator>

// Includes Suri
#include "suri/AnimationTask.h"
#include "suri/AnimationTool.h"
#include "suri/Element.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/TreeSelectionManagerInterface.h"
#include "suri/NodePath.h"
#include "suri/SuriObject.h"
#include "suri/DataViewManager.h"

// Includes Wx
// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/valgen.h"

// Defines
/** Define ID del Timer */
#define WXTIMER_ID wxID_HIGHEST+2

// forwards

namespace suri {

/** Nombre de la tarea **/
const std::string AnimationTask::TASKNAME = "AnimationTask";

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(AnimationTaskEvent, AnimationTask)
   IMPLEMENT_EVENT_CALLBACK(OnTimer, OnTimer(), wxTimerEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * @param pParent
 * @param pDataManager
 * @param ListGroup
 */
AnimationTask::AnimationTask(wxWindow* pParent, DataViewManager* pDataManager,
                             int ListGroup) :
      NEW_EVENT_OBJECT(AnimationTaskEvent), started_(false),
      pDataManager_(pDataManager), pAnimationTimer_(NULL), framesPerSecond_(1),
      currentFrame_(-1), pCurrentElement_(NULL), stop_(false), group_(ListGroup),
      rendering_(false) {
}

/** Destructor **/
AnimationTask::~AnimationTask() {
   pAnimationTimer_->Stop();
   pEventHandler_->Disconnect(WXTIMER_ID, wxEVT_TIMER,
                              wxTimerEventHandler(AnimationTaskEvent::OnTimer));
   delete pAnimationTimer_;
   // elimino el helper de eventos
   DELETE_EVENT_OBJECT;
}

/** Comienza la tarea */
bool AnimationTask::Start() {
   if (started_)
      return false;
   TreeSelectionManagerInterface* pselectmgr =
         pDataManager_->GetSelectionManager();
   selectlist_ = pselectmgr->GetSelectedNodes();
   if (selectlist_.size() == 0)
      return false;
   ConfigureBlocking();
   pEventHandler_->Connect(WXTIMER_ID, wxEVT_TIMER,
                           wxTimerEventHandler(AnimationTaskEvent::OnTimer));
   started_ = true;
   return true;
}
/** Finaliza la tarea. Intenta guardar los cambios. */
bool AnimationTask::End() {
   if (!started_)
      return false;
   ConfigureBlocking(false);
   Animate(false);
   started_ = false;
   return true;
}
/** Cancela la tarea sin guardar */
bool AnimationTask::Cancel() {
   started_ = false;
   return false;
}
/** Indica si la tarea esta activa */
bool AnimationTask::IsActive() const {
   return started_;
}

/** Avanza un frame hacia atras**/
bool AnimationTask::BackwardAnimation() {
   if (started_) {
      currentFrame_--;
      ShowNextFrame();
   } else {
      return false;
   }
   return true;
}

/** Avanza un frame hacia adelante **/
bool AnimationTask::ForwardAnimation() {
   if (started_) {
      currentFrame_++;
      ShowNextFrame();
   } else {
      return false;
   }
   return true;
}

/** Inicia/frena una animacion **/
bool AnimationTask::Animate(bool Start) {
   if (!started_)
      return false;
   if (Start) {
      if (!pAnimationTimer_) {
         pAnimationTimer_ = new wxTimer(pEventHandler_, WXTIMER_ID);
      }
      if (pAnimationTimer_->IsRunning()) {
         stop_ = true;
         return true;
      } else {
         // Se inicia el primer Timer
         currentFrame_ = 0;
         pAnimationTimer_->Start(1, wxTIMER_ONE_SHOT);
      }
   } else {
      if (pAnimationTimer_)
         pAnimationTimer_->Stop();
      if (pCurrentElement_) {
         pCurrentElement_->Activate(false);
      }
      currentFrame_ = 0;
   }
   stop_ = !Start;
   return true;
}

/** Indica la velocidad de animacion **/
bool AnimationTask::SetFrameRate(int Rate) {
   framesPerSecond_ = Rate;
   return true;
}

/** Evento OnTimer que cambia la el elemento activo */
void AnimationTask::OnTimer() {
   if (rendering_ || !pAnimationTimer_) {
      return;
   }
   if (stop_) {
      --currentFrame_;
      pAnimationTimer_->Stop();
      return;
   }
   if (!pCurrentElement_)
      pCurrentElement_ = GetNextElement();
   rendering_ = true;
   ShowNextFrame();
   rendering_ = false;
   ++currentFrame_;
   pAnimationTimer_->Start(1000 / framesPerSecond_, wxTIMER_ONE_SHOT);
}

/** Obtiene el elemento a mostrar **/
Element* AnimationTask::GetNextElement() {
   if (!pDataManager_)
      return NULL;
   Element* pelement = NULL;
   ViewcontextInterface* pviewctx =
         pDataManager_->GetViewcontextManager()->GetSelectedViewcontext();
   if (!pviewctx)
      return NULL;

   /** cantidad de frames en modulo size de la lista */
   currentFrame_ = currentFrame_ % selectlist_.size();
   std::list<NodePath>::iterator it = selectlist_.begin();
   advance(it, currentFrame_);
   if (it != selectlist_.end()) {
      NodePath path = *it;
      SuriObject::UuidType id = path.GetLastPathNode()->GetContent();
      LayerInterface* player = pviewctx->GetAssociatedLayer(id);
      if (player)
         pelement = player->GetElement();
   }
   return pelement;
}

/** Muestra el frame que corresponde al siguiente de la animacion **/
void AnimationTask::ShowNextFrame() {
   if (currentFrame_ == -1 || !pCurrentElement_)
      return;
   // Para que dibuje solo en el Activate(true) siguente
   pCurrentElement_->PreventNextViewerUpdate();
   pCurrentElement_->Activate(false);
   Element* pshowingelement = GetNextElement();
   if (pshowingelement) {
      REPORT_DEBUG("Animando elemento %d", currentFrame_);
      pshowingelement->Activate(true);
      pCurrentElement_ = pshowingelement;
   }
}

/** Configura el bloqueo las fuente de datos involucradas en la tarea
 *  @param[in] Block boolean que indica si se bloquea o desbloquean las fuentes
 *  de datos**/
void AnimationTask::ConfigureBlocking(bool Block) {
   std::list<NodePath>::iterator it =  selectlist_.begin();
   DatasourceManagerInterface* pdmgr = pDataManager_->GetDatasourceManager();
   if (!pdmgr)
      return;
   for (; it != selectlist_.end(); ++it) {
      NodePath path = *it;
      SuriObject::UuidType id = path.GetLastPathNode()->GetContent();
      if (Block)
         pdmgr->BlockDatasource(id);
      else
         pdmgr->UnblockDatasource(id);
   }
}
} /** namespace suri */
