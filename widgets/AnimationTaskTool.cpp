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

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/AnimationTaskTool.h"
#include "suri/ToolCommand.h"
#include "suri/SliderCommand.h"
#include "suri/ParameterCollection.h"
#include "resources.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/ToolSupport.h"
#include "suri/messages.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Constructor
 * @param pDataViewManager
 * @param pParent
 */
AnimationTaskTool::AnimationTaskTool(DataViewManager* pDataViewManager,
                                     wxWindow* pParent) :
      GenericTool(pDataViewManager), pAnimTask_(NULL), pTaskConfig_(NULL) {
   configurationVariableName_ = "animation";
   Initialize();
}

/** Destructor **/
AnimationTaskTool::~AnimationTaskTool() {
   Finalize();
}

/** Ejecucion del comando */
void AnimationTaskTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("StopAnimation")) {
      ExecuteAnimate(false);
   } else if (id == GetId("StartAnimation")) {
      ExecuteAnimate(true);
   } else if (id == GetId("SpeedSlider")) {
      ParameterCollection* pparams = pToolCommand->GetOutputParameters();
      if (pparams) {
         int rate = 1;
         pparams->GetValue<int>(SliderCommand::SelectionParameterKey, rate);
         ExecuteSetFrameRate(rate);
      }
   } else if (id == GetId("BackwardAnimation")) {
      ExecuteBackwardAnimation();
   } else if (id == GetId("ForwardAnimation")) {
      ExecuteForwardAnimation();
   } else if (id == GetId("TaskMenu")) {
      ExecuteTaskMenu(pToolCommand->IsActive());
   }
}

/** Creacion del Comando */
Command* AnimationTaskTool::CreateCommand(const std::string &CommandName) {
   // StopAnimation,StartAnimation,SpeedSlider,BackwardAnimation,ForwardAnimation
   Command* pcommand = NULL;
   if (CommandName.compare("StopAnimation") == 0) {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetGroupId(0);
      pcommand->SetDescription(_(tooltip_STOP_ANIMATION));
      pcommand->SetDecoration(button_STOP_ANIMATION);
      pcommand->SetLocation("ID_ANIMATION_TOOL_PANEL");
   } else if (CommandName.compare("StartAnimation") == 0) {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetGroupId(0);
      pcommand->SetDescription(_(tooltip_START_ANIMATION));
      pcommand->SetDecoration(button_START_ANIMATION);
      pcommand->SetLocation("ID_ANIMATION_TOOL_PANEL");
   } else if (CommandName.compare("SpeedSlider") == 0) {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Slider");
      pcommand->SetGroupId(0);
      pcommand->SetLocation("ID_ANIMATION_TOOL_PANEL");
      ParameterCollection* pparams = new ParameterCollection;
      pparams->AddValue<int>(SliderCommand::MinValueInputParameterKey, 1);
      pparams->AddValue<int>(SliderCommand::MaxValueInputParameterKey, 5);
      pparams->AddValue<int>(SliderCommand::ValueInputParameterKey, 1);
      pparams->AddValue<std::string>(SliderCommand::LabelParameterKey, "Cps");
      pcommand->SetInputParameters(pparams);
   } else if (CommandName.compare("BackwardAnimation") == 0) {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetGroupId(0);
      pcommand->SetDescription(_(tooltip_PREVIOUS_FRAME_ANIMATION));
      pcommand->SetDecoration(button_PREVIOUS_FRAME_ANIMATION);
      pcommand->SetLocation("ID_ANIMATION_TOOL_PANEL");
   } else if (CommandName.compare("ForwardAnimation") == 0) {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetGroupId(0);
      pcommand->SetDescription(_(tooltip_NEXT_FRAME_ANIMATION));
      pcommand->SetDecoration(button_NEXT_FRAME_ANIMATION);
      pcommand->SetLocation("ID_ANIMATION_TOOL_PANEL");
   } else if (CommandName.compare("TaskMenu") == 0) {
      pcommand = new ToolCommand(this);
      pcommand->SetType("ToggleButton|Menu");
      pcommand->SetGroupId(-64);
      pcommand->SetDescription(_(tooltip_TASK_MENU));
      pcommand->SetDecoration(tool_EDIT_TASK);
      pcommand->SetLocation("ID_ANIMATION_TOOL_PANEL|ID_ANIMATE_LAYERS_MENUTIEM");
   }
   return pcommand;
}

/** Destruccion del Comando */
void AnimationTaskTool::DestroyCommand(Command* &pCommand) const {
}

/** Creacion del Widget */
Widget* AnimationTaskTool::CreateWidget(const std::string &Widget) {
   return NULL;
}

/** Inicia la tarea de edicion de mapa */
bool AnimationTaskTool::StartTask() {
   if (!pAnimTask_) {
      pAnimTask_ = new AnimationTask(pParent_, GetDataViewManager());
   }
   ParameterCollection params;
   params.AddValue<DataViewManager*>(TaskConfigBuilder::kDataViewParam, GetDataViewManager());
   params.AddValue<AnimationTask*>(TaskConfigBuilder::kTaskParam, pAnimTask_);
   pTaskConfig_ = GetTaskManager()->GetTaskConfig(AnimationTask::TASKNAME, params);
   if (pTaskConfig_)
      pTaskConfig_->Configure();
   if (pAnimTask_->IsActive())
      return false;
   return pAnimTask_->Start();
}

/** Finaliza la tarea de edicion de mapa **/
bool AnimationTaskTool::EndTask() {
   if (!pAnimTask_ || !pAnimTask_->IsActive())
      return false;

   if (!pAnimTask_->End())
      return false;

   pTaskConfig_->Clear();
   delete pTaskConfig_;
   pTaskConfig_ = NULL;

   return true;
}

/** Avanza un frame hacia atras**/
void AnimationTaskTool::ExecuteBackwardAnimation() {
   if (!pAnimTask_ || !pAnimTask_->IsActive()) {
      return;
   }
   pAnimTask_->BackwardAnimation();
}

/** Avanza un frame hacia adelante **/
void AnimationTaskTool::ExecuteForwardAnimation() {
   if (!pAnimTask_ || !pAnimTask_->IsActive())
         return;
   pAnimTask_->ForwardAnimation();
}
/** Inicia/frena una animacion **/
void AnimationTaskTool::ExecuteAnimate(bool Start) {
   if (!pAnimTask_ || !pAnimTask_->IsActive())
         return;
   pAnimTask_->Animate(Start);
}

/** Indica la velocidad de animacion **/
void AnimationTaskTool::ExecuteSetFrameRate(int Rate) {
   if (!pAnimTask_ || !pAnimTask_->IsActive())
         return;
   pAnimTask_->SetFrameRate(Rate);
}

/** Metodo que se ejecuta cuando se apreta boton de iniciar/finalizar tarea */
void AnimationTaskTool::ExecuteTaskMenu(bool Active) {
   if (Active)
      StartTask();
   else
      EndTask();
}
} /** namespace suri */
