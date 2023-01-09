/*! \file ToolGroupManager.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes suri
#include "suri/ToolGroupManager.h"
#include "suri/Button.h"
#include "suri/LayerList.h"
#include "wxToggleBitmapButton.h"
#include "suri/MouseEventInterface.h"
#include "suri/CommandToggleButton.h"
#include "suri/CommandButton.h"
#include "suri/Choice.h"
#include "suri/Slider.h"
#include "suri/LabelText.h"

// Includes wx
#include "wx/wx.h"

/** namespace suri */
namespace suri {
/*!< Boton independiente (no toggleable, simultaneo) */
const ToolGroupManager::ButtonGroupType ToolGroupManager::StandAlone =
      MouseEventInterface::NoneFlag;
const ToolGroupManager::ButtonGroupType ToolGroupManager::Default = Navigation;
const ToolGroupManager::ButtonGroupType ToolGroupManager::LeftMouse =
      MouseEventInterface::Left;
const ToolGroupManager::ButtonGroupType ToolGroupManager::RightMouse =
      MouseEventInterface::Right;
const ToolGroupManager::ButtonGroupType ToolGroupManager::Navigation =
      MouseEventInterface::Left | MouseEventInterface::Right
            | MouseEventInterface::MouseMove;
const ToolGroupManager::ButtonGroupType ToolGroupManager::Edition =
      MouseEventInterface::Left | MouseEventInterface::Right;

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ToolGroupManagerEvent, ToolGroupManager)
   IMPLEMENT_EVENT_CALLBACK(OnButtonClicked, OnButtonClicked(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor
 * @return instancia de la clase ToolGroupManager
 */
ToolGroupManager::ToolGroupManager() :
      pViewerWidget_(NULL), pElementList_(NULL), pToolbar_(NULL),
      pLinkedToolsSet_(NULL), NEW_EVENT_OBJECT(ToolGroupManagerEvent) {
}

/**
 * Agrega el ToolGroupManager al grupo al que pertenece este ToolGroupManager
 * @param[in] pToolGroupManager: puntero a la lista de ToolGroupManager
 *  asocociados.
 * @param[out] pToolGroupManager: si la lista esta vacia la inicia con el
 *  este ToolGroupManager
 */
void ToolGroupManager::Link(ToolGroupManager *pToolGroupManager) {
   // Si no hay lista la creo
   if (!pLinkedToolsSet_) {
      pLinkedToolsSet_ = new ToolGroupManagerSetType;
      pLinkedToolsSet_->insert(this);
   }

   ToolGroupManagerSetType::iterator it = pLinkedToolsSet_->find(pToolGroupManager);
   // si esta en la lista salgo
   if (it != pLinkedToolsSet_->end()) {
      return;
   }

   // Desactivo botones activos en ToolGroupManager que entra al grupo
   ButtonToolMapType::iterator toolit = pToolGroupManager->toolMap_.begin();
   while (toolit != pToolGroupManager->toolMap_.end()) {
      pToolGroupManager->DeactivateButtons(toolit->first);
      toolit++;
   }

   // \todo (danieltesta-#3119): Este fragmento de codigo se ejecuta al cargar y
   // cerrar un proyecto. Al ejecutar el metodo GetActiveButtonTool la aplicacion
   // pincha en este punto. No pareciera tener otro objetivo mas que setear el
   // cursor al boton activo pero por alguna razon todos los punteros en toolMap_
   // son invalidos (En la documentacion se aclara que los punteros NO se deben
   // eliminar externamente)
#ifdef __ENABLE_SET_CURSOR__
   // Seteo el cursor al del boton que queda activo
   toolit = toolMap_.begin();
   Button* buttonaux;
   while (toolit != toolMap_.end()) {
      buttonaux = GetActiveButtonTool(toolit->first);
      if (buttonaux) {
         buttonaux->DoSetCursor(true);
      }
      toolit++;
   }
#endif  // __ENABLE_SET_CURSOR__

   // Agrego ToolGroupManager a set
   pLinkedToolsSet_->insert(pToolGroupManager);
   // Guardo la lista del grupo en el ToolGroupManager pasado por parametro
   pToolGroupManager->SetToolGroupManagerList(pLinkedToolsSet_);
}

/**
 * Sale del grupo en el que se encuentra
 */
void ToolGroupManager::Unlink() {
   // Si la lista no existe salgo
   if (!pLinkedToolsSet_) {
      return;
   }
   // Si esta en la lista lo elimino
   ToolGroupManagerSetType::iterator it = pLinkedToolsSet_->find(this);
   if (it != pLinkedToolsSet_->end()) {
      pLinkedToolsSet_->erase(it);
   }
   // Si la lista esta vacia la elimino
   if (pLinkedToolsSet_->size() == 0) {
      delete pLinkedToolsSet_;
      pLinkedToolsSet_ = NULL;
   }
}

/**
 * Devuelve un puntero al boton activo del grupo indicado.
 * @param[in] Group grupo del boton (StandAlone, Default, Left/RightMouse)
 * @return boton activo del grupo seleccionado
 * @return NULL si no hay boton ningun boton activo
 * \attention el Button* retornado es responsabilidad de esta clase(no eliminar externamente)
 */
Button *ToolGroupManager::GetActiveButtonTool(ButtonGroupType Group) {
   ButtonToolMapType::iterator toolit = toolMap_.find(Group);
   if (toolit == toolMap_.end()) {
      return NULL;
   }
   ButtonSetType::iterator setit = toolit->second.toolSet_.begin();
   while (setit != toolit->second.toolSet_.end()) {
      Button *pbutton = *setit;
      if (pbutton->GetValue()) {
         assert(
               pbutton == toolit->second.pLastSelected_ || !toolit->second.pLastSelected_);
         return pbutton;
      }
      setit++;
   }
   return NULL;
}

/**
 * Guarda el set como grupo al que pertenece, se deslinkea del actual si existe
 * @param[in] LinkedToolsSet: Set con ToolGroupManagers asociados
 */
void ToolGroupManager::SetToolGroupManagerList(
      ToolGroupManagerSetType *LinkedToolsSet) {
   Unlink();
   pLinkedToolsSet_ = LinkedToolsSet;
}

/**
 * Desactiva los botones del tool e informa su estado
 * @param[in] Group: grupo del boton (StandAlone, Default, Left/RightMouse)
 * @return true si pudo desactivar el boton
 * @return false si no pudo desactivar el boton
 */
bool ToolGroupManager::DeactivateButtons(ButtonGroupType Group) {
   if (Group == StandAlone) return true;
   if (Group > 0) {
      bool success = true;
      for (size_t i = 0; i < sizeof(ButtonGroupType) * 8; ++i) {
         ButtonGroupType newgroup = Group & (1 << i);
         ButtonToolMapType::iterator toolit = toolMap_.find(newgroup);
         if (toolit != toolMap_.end()) {
            ButtonSetType::iterator setit = toolit->second.toolSet_.begin();
            // itera por los botones
            while (setit != toolit->second.toolSet_.end()) {
               // desactivo el anterior
               if ((*setit) == toolit->second.pLastSelected_) {
                  success = DeactivateButton((*setit));
               }
               ++setit;
            }
            toolit->second.pLastSelected_ = NULL;
         }
      }
      return success;
   } else {
      ButtonToolMapType::iterator toolit = toolMap_.find(Group);
      bool success = true;
      if (toolit != toolMap_.end()) {
         ButtonSetType::iterator setit = toolit->second.toolSet_.begin();
         // itera por los botones
         while (setit != toolit->second.toolSet_.end()) {
            // desactivo el anterior
            if ((*setit) == toolit->second.pLastSelected_) {
               success = DeactivateButton((*setit));
            }
            setit++;
         }
         toolit->second.pLastSelected_ = NULL;
      }
      return success;
   }


}

/**
 * Agrega un boton de herramienta
 * @param[in] pButton: puntero al boton a agregar
 * @param[in] Position: posicion a donde se va a agregar el boton.
 * @param[in] Group: grupo del boton (StandAlone, Default, Left/RightMouse)
 */
void ToolGroupManager::AddButtonTool(Button *pButton, int Position,
                                     ButtonGroupType Group) {
   if (!pButton) {
      REPORT_ERROR("D:No implementado");
      return;
   }
   if (!pToolbar_) {
      REPORT_AND_FAIL("D:No se inicializo la barra de herramientas del viewer");
   }
   // Verifico que la barra sea horizaontal

   // Inicializo con la ventana de viewer y la lista de elementos
   if (Position != Invisible) {
      pButton->SetViewer(pViewerWidget_);
      pButton->SetElementList(pElementList_);
      // \todo es una forma no estandar de usar AddControl (deberia ser desde el
      // widget que contiene al parent)
      pButton->AddControl(
            pButton,
            pToolbar_,
            ControlArrangement(0, Position, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxLEFT,
                               1));
      if (pToolbar_->GetParent())
         pToolbar_->GetParent()->Layout();
   }

   /**
    * Los nuevos command usan GuiStateChangeMediator en vez de ToolGroupManager 
    * para manejar el uso de comandos conflictivos(usan mouse por ejemplo).
    * Una vez que Simur empiece a usar Commands se puede eliminar/refactorizar 
    * esta clase.
    */
   if (dynamic_cast<CommandToggleButton*>(pButton) == NULL &&
                     dynamic_cast<CommandButton*>(pButton) == NULL) {
      wxWindow *pwindow = NULL;
      pwindow = pButton->GetWindow();
      if (pwindow) {
         // Conecto el evento que maneja los grupos
         pwindow->Connect(pButton->GetButtonEventType(),
                          wxCommandEventHandler(ToolGroupManagerEvent::OnButtonClicked),
                          NULL, pEventHandler_);
      }
   }

   // agrega la herramienta en el mapa
   AddButtonToGroup(pButton, Group);

   // Rearmo ventana
#ifdef __DEBUG__
   int x, y;
   pButton->GetWindow()->GetSize(&x, &y);
   DEBUG_SIZE(pButton->GetWindowTitle().c_str(), pButton->GetWindow(), wxSize(x, y));
   wxWindow *pwin = pToolbar_;
// No modifico los tamanos fuera de AddControl
#ifdef __UNUSED_CODE__
   pwin->InvalidateBestSize();
   pwin->GetSizer()->Layout();
   pwin->Fit();
#endif
   pwin->GetSize(&x, &y);
   DEBUG_SIZE(pButton->GetWindowTitle().c_str(), pToolbar_, wxSize(x, y));
   while (pwin->GetParent())
      pwin = pwin->GetParent();
// No modifico los tamanos fuera de AddControl
#ifdef __UNUSED_CODE__
   pwin->InvalidateBestSize();
   pwin->GetSizer()->Layout();
   pwin->Fit();
#endif
   pwin->GetSize(&x, &y);
   DEBUG_SIZE(pButton->GetWindowTitle().c_str(), pwin, wxSize(x, y));
#endif
   // Permite que las clases que heredan de ToolGroupManager sepan cuando se agrega un boton
   DoOnAddButton(pButton);
}

/**
 * Elimina boton de la herramienta
 * @param[in] pButton que se elimina de herramienta
 */
void ToolGroupManager::RemoveButtonFromTool(Button *pButton) {
   if (!pButton) {
      REPORT_DEBUG("D:El boton que se quiere eliminar es NULL");
      return;
   }
   bool found = false;
   // Busco en todos los grupos el boton y lo saco.
   ButtonToolMapType::iterator toolit = toolMap_.begin();
   while (toolit != toolMap_.end()) {
      ButtonSetType::iterator setit = toolit->second.toolSet_.find(pButton);
      if (setit != toolit->second.toolSet_.end()) {
         // Elimino el boton
         toolit->second.toolSet_.erase(setit);
         found = true;
      }
      ++toolit;
   }
   if (found) {
      // Saco el boton del tool.
      pToolbar_->GetSizer()->Detach(pButton->GetWindow());
      pToolbar_->RemoveChild(pButton->GetWindow());
      pToolbar_->GetSizer()->Layout();
      // Permite que las clases que heredan de ToolGroupManager sepan cuando se saca un boton
      DoOnRemoveButton(pButton);
   }
}

/**
 * Mantiene los botones de herramientas en estado
 * @param[in] Event: evento.
 */
void ToolGroupManager::OnButtonClicked(wxCommandEvent &Event) {
   wxObject *peventobj = Event.GetEventObject();
   Button *pbutton = NULL;
   // Recorro todos los grupos buscando el boton
   ButtonToolMapType::iterator toolit = toolMap_.begin();
   while (toolit != toolMap_.end() && !pbutton) {
#ifdef __STANDALONE_COMMANDTOGGLEBUTTON_SOPI_FIX_
      /**
       * Si se ignoran los botones togle standalone nunca cambia a estado
       * activo ya que ToggleButton::OnClink no modifica estado por su cuenta.
       */
      if (toolit->first != StandAlone) {
#endif
         // itera por los botones
         ButtonSetType::iterator setit = toolit->second.toolSet_.begin();
         while ((setit != toolit->second.toolSet_.end()) && (!pbutton)) {
            // desactivo el anterior
            if ((*setit)->GetWindow() == peventobj) {
               pbutton = (*setit);
#ifndef __STANDALONE_COMMANDTOGGLEBUTTON_SOPI_FIX_
               if (toolit->first == StandAlone &&
                     !dynamic_cast<CommandToggleButton*>(pbutton))
                  pbutton = NULL;
#endif
            }
            ++setit;
         }
#ifdef __STANDALONE_COMMANDTOGGLEBUTTON_SOPI_FIX_
      }
#endif
      ++toolit;
   }

   // Si pudo encontrar el boton le cambia estado de activacion
   if (pbutton) {
      // con los botones nuevos (Command) la logica parece invertida, ya que
      // da deshabilitado aca
      if (dynamic_cast<CommandToggleButton*>(pbutton)) {
         if (!pbutton->GetValue()) {
            ActivateButton(pbutton);
         } else {
            DeactivateButton(pbutton);
         }
      } else {
         if (pbutton->GetValue()) {
            if (!ActivateButton(pbutton))
               DeactivateButton(pbutton);
         } else {
            DeactivateButton(pbutton);
         }
      }
   }
   DoOnButtonClicked(Event);
}

/* Guarda los elementos que necesitan los botones al cargarlos al tool.
 * @param[in] pToolbar Toolbar donde se agregan los botones
 * @param[in] pViewerWidget Widget del viewer
 * @param[in] pElementList Lista de elementos
 */
void ToolGroupManager::InitializeToolbar(wxWindow *pToolbar,
                                         ViewerWidget *pViewerWidget,
                                         LayerList *pElementList) {
   pViewerWidget_ = pViewerWidget;
   pElementList_ = pElementList;
   pToolbar_ = pToolbar;
}

/**
 * Retorna la cantidad de botones en la herramienta. Si un boton esta
 * asociado a varios grupos lo cuenta varias veces
 * @return cantidad de botones en herramienta
 */
int ToolGroupManager::GetButtonCount() {
   int ret = 0;
   ButtonToolMapType::iterator toolit = toolMap_.begin();
   while (toolit != toolMap_.end()) {
      ret += toolit->second.toolSet_.size();
      ++toolit;
   }
   return ret;
}

/**
 * Agrega boton a grupo
 * @param[in] pButton boton que se quiere agregar al grupo
 * @param[in] Group grupo al cual se debe agregar
 */
void ToolGroupManager::AddButtonToGroup(Button *pButton, ButtonGroupType Group) {
   if (Group > 0) {
      if (toolMap_.empty()) {
         for (size_t i = 0; i < sizeof(ButtonGroupType) * 8; ++i) {
            ButtonStateType butstate;
            butstate.toolSet_.insert(pButton);
            // el primer elemento va activo
            butstate.pLastSelected_ = NULL;
            ButtonGroupType mask = 1 << i;
            ButtonGroupType newgroup = Group & mask;
            toolMap_.insert(std::make_pair(newgroup, butstate));
         }
      } else {
         ButtonToolMapType::iterator it = toolMap_.begin();
         while (it != toolMap_.end()) {
            ButtonGroupType currentgroup = it->first;
            if (true || currentgroup > 0) {
               for (size_t i = 0; i < sizeof(ButtonGroupType) * 8; ++i) {
                  ButtonGroupType mask = 1 << i;
                  bool exists = (currentgroup > 0);
                  exists = exists && (currentgroup & Group & mask);
                  if (exists) {
                     pButton->SetValue(false);
                     it->second.toolSet_.insert(pButton);
                  } else {
                     ButtonStateType butstate;
                     butstate.toolSet_.insert(pButton);
                     // el primer elemento va activo
                     butstate.pLastSelected_ = NULL;
                     ButtonGroupType newgroup = Group & mask;
                     toolMap_.insert(std::make_pair(newgroup, butstate));
                  }
               }
            }
            ++it;
         }
      }
   } else {
      ButtonToolMapType::iterator it = toolMap_.find(Group);
      // si no existe el mapa del grupo lo crea
      if (it == toolMap_.end()) {
         ButtonStateType butstate;
         butstate.toolSet_.insert(pButton);
         // el primer elemento va activo
         butstate.pLastSelected_ = NULL;
         toolMap_.insert(std::make_pair(Group, butstate));
      } else {
         pButton->SetValue(false);
         it->second.toolSet_.insert(pButton);
      }
   }
}

/**
 * Desactiva el boton en todos los grupos a los que pertenece. Si el boton
 * esta activo lo desactiva y en todos los grupos al que pertenece pone
 * pLastSelected_=NULL
 * Llama al metodo ButtonDeactivated, que permite a las clases derivadas agregar
 * comportamiento cuando se desactiva un boton.
 * @param[in] pButton boton a desactivar
 * @return true si pudo desactivar el boton.
 * @return false si no pudo desactivar el boton.
 */
bool ToolGroupManager::DeactivateButton(Button *pButton) {
   bool success = false;
   if (pButton->GetValue()) {
      success = pButton->SetValue(false);
   } else {
      success = true;
   }

   // Recorro todos los grupos
   ButtonToolMapType::iterator toolit = toolMap_.begin();
   while (toolit != toolMap_.end()) {
      if (toolit->first != StandAlone) {
         // itera por los botones
         bool found = false;
         ButtonSetType groupbuttons = toolit->second.toolSet_;
         ButtonSetType::iterator setit = groupbuttons.begin();
         while ((setit != groupbuttons.end()) && (!found)) {
            bool value = (*setit)->GetValue();
            value = !value;
            // desactivo el anterior
            if ((*setit) == pButton) {
               found = true;
               if (toolit->second.pLastSelected_ == pButton) {
                  toolit->second.pLastSelected_ = NULL;
               }
               (*setit)->SetValue(false);
            }
            setit++;
         }
      }
      toolit++;
   }

   if (success) {
      ButtonDeactivated(pButton);
   }

/*   ViewerWidget *pviewer = AS_VW(pDataViewManager_->GetViewportManager()->GetSelectedViewport());
   pviewer->GetWindow()->Refresh();*/

   return success;
}

/**
 * Activa el boton en todos los grupos a los que pertenece
 * @param[in] pButton boton que se quiere activar
 * @return bool que indica si la operacion tuvo exito.
 * \post retorna false si no encontro el boton en tool
 * \post retorna false si no pudo activar el boton
 * \post retorna false si no pudo actualizar ToolGroupManager
 * \post retorna true si tuvo exito la operacion
 */
bool ToolGroupManager::ActivateButton(Button *pButton) {
   if (!pButton) {
      return false;
   }REPORT_DEBUG("D:Activando boton %s", pButton->GetWindowTitle().c_str());
   bool success = true;
   bool validoperation = false;
   // Busca en que grupo esta el boton
   Button* buttonactivated = NULL;
   bool found = false;
   ButtonToolMapType::iterator toolit = toolMap_.begin();
   while ((toolit != toolMap_.end()) && (success)) {
      found = false;
      ButtonSetType::iterator setit = toolit->second.toolSet_.begin();
      // itera por los botones
      while ((setit != toolit->second.toolSet_.end()) && (!found)) {
         // desactivo el anterior
         if ((*setit) == pButton) {
            found = true;
            buttonactivated = (*setit);
         }
         setit++;
      }
      // Si el boton esta en el tool y no esta activo lo activo
      if (found) {
         validoperation = true;
         // Siempre trato de setear el valor del boton. Eso ocurre porque este metodo
         // se ejecuta antes que OnButtonClick del boton y solo de esta forma puedo
         // saber si falla la activacion. Como consecuencia tiene que se llama dos veces
         // a SetValue(Aca y en OnButtonClick del boton).
         success = buttonactivated->SetValue(true);
         // Actualizo el toolgroupmanager
         if ((toolit->first != StandAlone) && (success)) {
            // Si el boton no queda presionado no tiene sentido liberar los otros botones en uso
            if (buttonactivated->GetValue()) {
               // itera por todos los tools asociados
               if (pLinkedToolsSet_) {
                  ToolGroupManagerSetType::iterator linkedtoolsit =
                        pLinkedToolsSet_->begin();
                  while ((linkedtoolsit != pLinkedToolsSet_->end()) && (success)) {
                     success = (*linkedtoolsit)->DeactivateButtons(toolit->first);
                     linkedtoolsit++;
                  }
               } else {
                  success = success & DeactivateButtons(toolit->first);
               }
               toolit->second.pLastSelected_ = buttonactivated;
            } else {
               toolit->second.pLastSelected_ = NULL;
            }
         }
      }

      toolit++;
   }

   // Si pudo desactivar el boton anterior
   if (success) {
      ButtonActivated(buttonactivated);
   }

   return validoperation && success;
}

/*** Agrega un control grafico de tipo choice a la posicion que se indica por parametro **/
void ToolGroupManager::AddChoiceTool(Choice* pChoice, int Position) {
   // \todo es una forma no estandar de usar AddControl (deberia ser desde el
   // widget que contiene al parent)
   if (!pChoice)
      return;
   if (pChoice->HasLabel()) {
      LabelText* plabel = pChoice->GetLabel();
      plabel->AddControl(plabel, pToolbar_,
               ControlArrangement(0, Position, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxLEFT,
                                  1));
   }
   pChoice->AddControl(pChoice, pToolbar_,
         ControlArrangement(0, Position, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxLEFT,
                            1));
   tools_.push_back(pChoice);
}

/*** Agrega un control grafico de tipo slider a la posicion que se indica por parametro **/
void ToolGroupManager::AddSliderTool(Slider* pSlider, int Position) {
   // \todo es una forma no estandar de usar AddControl (deberia ser desde el
   // widget que contiene al parent)
   if (!pSlider)
      return;
   if (pSlider->HasLabel()) {
      LabelText* plabel = pSlider->GetLabel();
      plabel->AddControl(plabel, pToolbar_,
                         ControlArrangement(0, Position, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxLEFT,
                                                     1));
   }
   pSlider->AddControl(pSlider, pToolbar_,
         ControlArrangement(0, Position, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxLEFT,
                            1));
   tools_.push_back(pSlider);
}

/** Metodo que se llama cuando se ocasiona un evento sobre un tool
 * (control grafico, salvo boton) **/
void ToolGroupManager::OnToolClicked(wxCommandEvent &Event) {
}

/** Metodo que se llama cuando ocurre un evento sobre un control
 * grafico de tipo scroll (ej. slider) **/
void ToolGroupManager::OnScrollToolEvent(wxScrollEvent &Event) {
}
}
