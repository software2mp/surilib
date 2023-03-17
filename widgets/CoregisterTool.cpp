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

// Includes stadard
#include <string>

// Includes suri
#include "suri/CoregisterTool.h"
#include "GeometrySelectionEvent.h"
#include "CompositeCommand.h"
#include "suri/ToolCommand.h"
#include "suri/EventCommand.h"
#include "resources.h"
#include "VectorEditionTaskEvent.h"
#include "suri/CoregisterTask.h"
#include "CoregisterTaskEvent.h"
#include "suri/ParameterCollection.h"
#include "suri/messages.h"
#include "GcpEditionEvent.h"
#include "suri/ReprojectionTool.h"
#include "suri/DynCommandButton.h"

std::string OpenSaveFileDialog();

namespace suri {

#define tooltip_COREGISTER_TOOLTIP_DESC "Iniciar / Finalizar edicion de puntos de control"

/** Ctor */
CoregisterTool::CoregisterTool(DataViewManager* pDataViewManager) :
                                         GenericTool(pDataViewManager) {
   gcpAutoGenerateEnabled_ = false;
   configurationVariableName_ = "coregister";
   pCoregisterTask_ = new CoregisterTask();
   pCoregisterTask_->SetFeatureSelector(GetFeatureSelection());
   Initialize();
}

/** Dtor */
CoregisterTool::~CoregisterTool() {
   Finalize();
}

/**
 * Si el comando indicado, se encuentra entre la lista de ejecutables, se ejecutara
 * @param[in]  pToolCommand: Comando a ejecutar
 */
void CoregisterTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("NewGcpFile")) {
      ExecuteNewGcpFile();
   } else if (id == GetId("AutoGenerateGcpFile")) {
      ExecuteAutoGenerateGcpFile();
   } else if (id == GetId("OpenGcpFile")) {
      ExecuteOpenGcpFile();
   } else if (id == GetId("TaskMenu") && pToolCommand->IsActive()) {
      ExecuteStartTask();
   } else if (id == GetId("TaskMenu") && !pToolCommand->IsActive()) {
      ExecuteEndTask();
   } else if (id == GetId("AddGcp") && !pToolCommand->IsActive()) {
      ExecuteEndAddGcps();
   } else if (id == GetId("StartProcess")) {
      ReprojectionTool tool(GetDataViewManager());
      tool.ExecuteCoregister();
   }
}

/**
 * Creacion del Comando
 * @param[in] CommandName nombre del comando
 * @return referencia a comando creado
 * @return NULL si el nombre es invalido
 */
Command* CoregisterTool::CreateCommand(const std::string &CommandName) {
   Command* pcommand = NULL;
   if (CommandName == "SelectGcp") {
      // Configuro evento que maneja seleccion
      GeometrySelectionEvent* pevent = new GeometrySelectionEvent(
                                 GetViewportManager(), GetViewcontextManager(),
                                 GetFeatureSelection());
      SetMouseEvent(pevent);
      SetPaintEvent(pevent);
      Command* peventcommand = new EventCommand(pevent, pevent);

      // Configuro evento que inicializa seleccion
      Command* ptoolcommand = new ToolCommand(this);

      // Configuro CompositeCommand
      CompositeCommand* pcomposite = new CompositeCommand();
      pcomposite->AppendCommand(ptoolcommand);
      pcomposite->AppendCommand(peventcommand);
      pcomposite->SetType("ToggleButton");
      pcomposite->SetDescription(_(tooltip_SELECT_GCP));
      pcomposite->SetDecoration(tool_SELECT_VECTOR);
      pcomposite->SetLocation("ID_COREGISTER_TOOL_PANEL");

      pcommand = pcomposite;

   } else if (CommandName == "NewGcpFile") {
      // Configuro evento que inicializa creacion
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_CREATE_GCP_FILE));
      pcommand->SetDecoration(tool_DRAW_POINT);
      pcommand->SetLocation("ID_COREGISTER_TOOL_PANEL");
   } else if (CommandName == "AutoGenerateGcpFile") {
      // Configuro evento que inicializa creacion
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_AUTOGENERATE_GCP_FILE));
      pcommand->SetDecoration(tool_DRAW_POINT);
      pcommand->SetLocation("ID_COREGISTER_TOOL_PANEL");
   } else if (CommandName == "OpenGcpFile") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_OPEN_GCP_FILE));
      pcommand->SetDecoration(tool_OPEN_GCP);
      pcommand->SetLocation("ID_COREGISTER_TOOL_PANEL");
   } else if (CommandName == "StartProcess") {
      // Configuro evento que inicializa creacion
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_START_COREGISTER_PROCESS));
      pcommand->SetDecoration(button_START_COREGISTER_PROCESS);
      pcommand->SetLocation("ID_COREGISTER_TOOL_PANEL");

   } else if (CommandName == "TaskMenu") {
      // Configuro evento que maneja seleccion
      CoregisterTaskEvent* pevent = new CoregisterTaskEvent(
                                 GetViewportManager(), GetViewcontextManager(),
                                                           pCoregisterTask_);
      GetFeatureSelection()->Attach(pevent);
      SetPaintEvent(pevent);
      SetMouseEvent(pevent);
      Command* peventcommand = new EventCommand(pevent, pevent);

      // Configuro evento que inicializa seleccion
      Command* ptoolcommand = new ToolCommand(this);

      // Configuro CompositeCommand
      CompositeCommand* pcomposite = new CompositeCommand();
      pcomposite->AppendCommand(ptoolcommand);
      pcomposite->AppendCommand(peventcommand);
      pcommand = pcomposite;

      pcommand->SetType("DynButton|Menu");
      pcommand->SetDescription(_(tooltip_COREGISTER_TOOLTIP_DESC));
      pcommand->SetGroupId(-64);
      pcommand->SetDecoration(button_START_ANIMATION_SRC);
      pcommand->SetLocation("ID_COREGISTER_TOOL_PANEL|ID_IMAGE_TO_IMAGE_GCPS_MENUTIEM");
      pcommand->SetAssociatedTaskId("CoregisterTask");


      ParameterCollection* pparams = new ParameterCollection();
      pparams->AddValue<std::string>(DynCommandButton::kActivatedBitmap,
                                     button_STOP_ANIMATION);
      pparams->AddValue<std::string>(DynCommandButton::kDeactivatedBitmap,
                                     button_START_ANIMATION_SRC);
      pparams->AddValue<std::string>(DynCommandButton::kActivatedTooltip,
                                     tooltip_STOP_COREGISTER_TASK);
      pparams->AddValue<std::string>(DynCommandButton::kDeactivatedTooltip,
                                     tooltip_START_COREGISTER_TASK);
      pcommand->SetInputParameters(pparams);

      pTaskCommand_ = pcommand;
   } else if (CommandName == "AddGcp" || CommandName == "EditGcp") {
      GcpEditionEvent* pevent = new GcpEditionEvent(
                                 GetViewportManager(), GetViewcontextManager(),
                                 pCoregisterTask_, (CommandName == "EditGcp"));
      SetMouseEvent(pevent);
      SetPaintEvent(pevent);
      Command* peventcommand = new EventCommand(pevent, pevent);

      // Configuro evento que inicializa seleccion
      Command* ptoolcommand = new ToolCommand(this);

      // Configuro CompositeCommand
      CompositeCommand* pcomposite = new CompositeCommand();
      pcomposite->AppendCommand(ptoolcommand);
      pcomposite->AppendCommand(peventcommand);
      pcomposite->SetType("ToggleButton");
      if (CommandName == "AddGcp") {
         pcomposite->SetDescription(_(tooltip_ADD_GCP));
         pcomposite->SetDecoration(tool_ADD_GCP_POINT);
      } else {
         pcomposite->SetDescription(_(tooltip_EDIT_VECTOR));
         pcomposite->SetDecoration(tool_EDIT_VECTOR);
      }
      pcomposite->SetLocation("ID_COREGISTER_TOOL_PANEL");

      pcommand = pcomposite;

   } else if (CommandName == "SwitchBaseWarp") {
      // Configuro evento que inicializa creacion
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_SWITCH_BASE_WARP));
      pcommand->SetDecoration(button_SWITCH);
      pcommand->SetLocation("ID_COREGISTER_TOOL_PANEL");

   } else if (CommandName == "Prediction") {
      // Configuro evento que inicializa creacion
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_PREDICTION));
      pcommand->SetDecoration(button_PREDICTION);
      pcommand->SetLocation("ID_COREGISTER_TOOL_PANEL");
   }
   return pcommand;
}

/**
 * Destruccion del Comando
 * @param[out] pCommand comando a destruir
 */
void CoregisterTool::DestroyCommand(Command* &pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Creacion del Widget
 * @return Siempre NULL.
 */
Widget* CoregisterTool::CreateWidget(const std::string &Widget) {
   return NULL;
}

/**
 * Inicia la tarea de edicion de puntos de control
 * @param[in] FileName nombre del archivo con gcps
 * @return bool que indica si tuvo exito.
 */
bool CoregisterTool::StartTask(const std::string &FileName) {

   if (pCoregisterTask_->IsActive()) {
      SHOW_WARNING(_(message_TASK_ACTIVE));
      return false;
   }

   ParameterCollection params;
   params.AddValue<DataViewManager*>("DataView", GetDataViewManager());
   params.AddValue<std::string>("FileName", FileName);
   params.AddValue<CoregisterTask*>("Task", pCoregisterTask_);
   params.AddValue<bool>("GcpAutoGenEnabled", this->gcpAutoGenerateEnabled_);
   pTaskConfig_ = GetTaskManager()->GetTaskConfig(
                                 CoregisterTask::TASKNAME, params);
   if (pTaskConfig_)
      pTaskConfig_->Configure();

   if (!pCoregisterTask_->Start()) {
      SHOW_ERROR(_(message_CANT_START_COREGISTER_TASK));
      return false;
   }
   return true;
}

/** Crea un archivo con gcps nuevo y comienza tarea de co-registro */
void CoregisterTool::ExecuteNewGcpFile() {

   if (pCoregisterTask_->IsActive()) {
      SHOW_WARNING(_(message_TASK_ACTIVE));
      return;
   }

   std::string filename = OpenSaveFileDialog();
   if (filename.empty()) {
      return;
   }

   this->gcpAutoGenerateEnabled_ = false;

   if (StartTask(filename)) {
      pTaskCommand_->SetActive(true);
   } else {
      pTaskCommand_->SetActive(false);
   }
}

void CoregisterTool::ExecuteAutoGenerateGcpFile() {
   if (pCoregisterTask_->IsActive()) {
      SHOW_WARNING(_(message_TASK_ACTIVE));
      return;
   }

   std::string filename = OpenSaveFileDialog();
   if (filename.empty()) {
      return;
   }

   this->gcpAutoGenerateEnabled_ = true;

   if (StartTask(filename)) {
      pTaskCommand_->SetActive(true);
   } else {
      pTaskCommand_->SetActive(false);
   }
}

/** Abre un archivo de gcp existente e inicia la tarea de toma de puntos**/
void CoregisterTool::ExecuteOpenGcpFile() {
   if (pCoregisterTask_->IsActive())
      return;
   std::string filename;
   wxFileDialog dlg(NULL, _(caption_GCP_FILE), wxT(""), wxT(""),
                                            filter_SAVE_GCP, wxFD_OPEN);
   if (dlg.ShowModal() != wxID_OK) {
      pTaskCommand_->SetActive(false);
      return;
   }
   filename = dlg.GetPath();

   if (filename.empty() || !wxFileName::FileExists(filename)) {
      SHOW_ERROR(message_GCP_OPENFILENAME_ERROR);
      pTaskCommand_->SetActive(false);
      return;
   }

   this->gcpAutoGenerateEnabled_ = false;

   if (StartTask(filename)) {
      pTaskCommand_->SetActive(true);
   } else {
      pTaskCommand_->SetActive(false);
   }
}

/** Inicia la tarea de co-registro con un archivo existente */
void CoregisterTool::ExecuteStartTask() {
   if (pCoregisterTask_->IsActive())
      return;
   std::string filename;
   wxFileDialog dlg(NULL, _(caption_GCP_FILE), wxT(""), wxT(""),
                                             filter_SAVE_GCP, wxFD_OPEN);
   if (dlg.ShowModal() != wxID_OK) {
      pTaskCommand_->SetActive(false);
      return;
   }
   filename = dlg.GetPath();

   if (filename.empty() || !wxFileName::FileExists(filename)) {
      SHOW_ERROR(message_GCP_OPENFILENAME_ERROR);
      pTaskCommand_->SetActive(false);
      return;
   }

   this->gcpAutoGenerateEnabled_ = false;

   if (!StartTask(filename))
      pTaskCommand_->SetActive(false);
}

/** Finaliza la tarea de co-registro */
void CoregisterTool::ExecuteEndTask() {
   if (!pCoregisterTask_->IsActive())
      return;
   pCoregisterTask_->End();
   pTaskConfig_->Clear();
   delete pTaskConfig_;
   pTaskConfig_ = NULL;
}

/** Finaliza la edicion grafica de features, no finaliza la tarea. */
void CoregisterTool::ExecuteEndAddGcps() const {
   pCoregisterTask_->EndFeatureEdition();
}

} /* namespace suri */

/**
 *  PRIVADAS
 */

std::string OpenSaveFileDialog() {
   std::string filename("");
   wxFileDialog dlg(NULL, _(caption_GCP_FILE), wxT(""), wxT(""),
                     filter_SAVE_GCP, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
      if (dlg.ShowModal() != wxID_OK)
         return filename;
      filename = dlg.GetPath();

      // TODO(Ramiro - TCK #2988) : Permitir que se pisen archivos.
      if (filename.empty()) {
         SHOW_ERROR(message_GCP_NEWFILENAME_ERROR);
         return "";
      }

      if (wxFileName::FileExists(filename) && !wxRemoveFile(filename.c_str())) {
         SHOW_ERROR(message_GCP_NEWFILENAME_ERROR);
         return "";
      }
      return filename;
}
