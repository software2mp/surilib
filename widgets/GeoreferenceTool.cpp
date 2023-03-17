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

#include <cstddef>
#include <string>

#include "suri/GenericTool.h"
#include "suri/GeoreferenceTool.h"
#include "suri/DataViewManager.h"
#include "suri/Command.h"
#include "suri/Widget.h"
#include "suri/GeoreferenceTask.h"
#include "GeometrySelectionEvent.h"
#include "suri/EventCommand.h"
#include "suri/ToolCommand.h"
#include "CompositeCommand.h"
#include "GcpEditionEvent.h"
#include "suri/ParameterCollection.h"
#include "GeoreferenceTaskEvent.h"
#include "GeoreferenceEditionEvent.h"
#include "suri/ReprojectionTool.h"
#include "suri/DynCommandButton.h"
#include "resources.h"

#define tooltip_GEOREFERENCE_TOOLTIP_DESC "Iniciar / Finalizar edicion de puntos de control"

namespace suri {

/**
 * Ctor.
 */
GeoreferenceTool::GeoreferenceTool(DataViewManager* pDataViewManager)
   : GenericTool(pDataViewManager) {
   configurationVariableName_ = "georeference";
   pGeoreferenceTask_ = new GeoreferenceTask();
   pGeoreferenceTask_->SetFeatureSelector(GetFeatureSelection());
   Initialize();
}

/**
 * Dtor
 */
GeoreferenceTool::~GeoreferenceTool() {
   Finalize();
}

/**
 * Ejecucion del comando
 */
void GeoreferenceTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("NewGcpFile")) {
      ExecuteNewGcpFile();
   } else if (id == GetId("TaskMenu") && pToolCommand->IsActive()) {
      ExecuteStartTask();
   } else if (id == GetId("TaskMenu") && !pToolCommand->IsActive()) {
      ExecuteEndTask();
   } else if (id == GetId("AddGcp") && !pToolCommand->IsActive()) {
      ExecuteEndAddGcps();
   } else if (id == GetId("StartProcess")) {
      ReprojectionTool tool(GetDataViewManager());
      tool.ExecuteGeoreference();
   } else if (id == GetId("OpenGcpFile")) {
      ExecuteOpenGcpFile();
   }
}

/**
 * Creacion del Comando
 */
Command* GeoreferenceTool::CreateCommand(const std::string& CommandName) {
   Command* pcommand = NULL;
   if (CommandName == "SelectGcp") {
      // Configuro evento que maneja seleccion
      GeometrySelectionEvent* pevent = new GeometrySelectionEvent(
            GetViewportManager(), GetViewcontextManager(), GetFeatureSelection());

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
      pcomposite->SetLocation("ID_GEOREFERENCE_TOOL_PANEL");

      pcommand = pcomposite;

   } else if (CommandName == "NewGcpFile") {
      // Configuro evento que inicializa creacion
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_CREATE_PCT_FILE));
      pcommand->SetDecoration(tool_DRAW_POINT);
      pcommand->SetLocation("ID_GEOREFERENCE_TOOL_PANEL");

   } else if (CommandName == "OpenGcpFile") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_OPEN_PCT_FILE));
      pcommand->SetDecoration(tool_OPEN_GCP);
      pcommand->SetLocation("ID_GEOREFERENCE_TOOL_PANEL");
   } else if (CommandName == "StartProcess") {
      // Configuro evento que inicializa creacion
      pcommand = new ToolCommand(this);
      pcommand->SetType("Button");
      pcommand->SetDescription(_(tooltip_START_GEOREFERENCE_PROCESS));
      pcommand->SetDecoration(button_START_COREGISTER_PROCESS);
      pcommand->SetLocation("ID_GEOREFERENCE_TOOL_PANEL");

   } else if (CommandName == "TaskMenu") {
      // Configuro evento que maneja seleccion
      GeoreferenceTaskEvent* pevent = new GeoreferenceTaskEvent(GetViewportManager(),
                                                            GetViewcontextManager(),
                                                            pGeoreferenceTask_);
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
      pcommand->SetDescription(_(tooltip_GEOREFERENCE_TOOLTIP_DESC));
      pcommand->SetGroupId(-64);
      pcommand->SetDecoration(button_START_ANIMATION_SRC);
      pcommand->SetLocation("ID_GEOREFERENCE_TOOL_PANEL|ID_IMAGE_TO_MAP_GCPS_MENUTIEM");
      pcommand->SetAssociatedTaskId("GeoreferenceTask");

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
      GeoreferenceEditionEvent* pevent = new GeoreferenceEditionEvent(
            GetViewportManager(), GetViewcontextManager(), pGeoreferenceTask_,
            (CommandName == "EditGcp"));

      SetMouseEvent(pevent);
      SetPaintEvent(pevent);

      Command* peventcommand = new EventCommand(pevent, pevent);
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
      pcomposite->SetLocation("ID_GEOREFERENCE_TOOL_PANEL");

      pcommand = pcomposite;
   }
   return pcommand;
}

/**
 * Destruccion del Comando
 */
void GeoreferenceTool::DestroyCommand(Command*& pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Creacion del Widget
 */
Widget* GeoreferenceTool::CreateWidget(const std::string& Widget) {
   return NULL;
}

/**
 * Inicia la tarea de georreferenciacion
 */
bool GeoreferenceTool::StartTask(const std::string &FileName) {
   if (pGeoreferenceTask_->IsActive()) return false;

   ParameterCollection params;
   params.AddValue<DataViewManager*>("DataView", GetDataViewManager());
   params.AddValue<std::string>("FileName", FileName);
   params.AddValue<GeoreferenceTask*>("Task", pGeoreferenceTask_);
   pTaskConfig_ = GetTaskManager()->GetTaskConfig(GeoreferenceTask::kTaskName, params);
   if (pTaskConfig_)
      pTaskConfig_->Configure();

   return pGeoreferenceTask_->Start();
}

/**
 * Crea una capa vectorial en memoria de tipo correcto
 */
void GeoreferenceTool::ExecuteNewGcpFile() {
   if (pGeoreferenceTask_->IsActive()) {
      SHOW_WARNING(_(message_INCOMPLETE_DATA_ENTRY));
   } else {
      std::string filename;
      wxFileDialog dlg(NULL, _(caption_GCP_FILE), wxT(""), wxT(""),
                       filter_SAVE_GCP, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

      if (dlg.ShowModal() == wxID_OK) {
         filename = dlg.GetPath();

         // TODO(Ramiro - TCK #2988) : Permitir que se pisen archivos.
         if (filename.empty()) {
            SHOW_ERROR(message_GCP_NEWFILENAME_ERROR);
            return;
         }

         if (wxFileName::FileExists(filename) && !wxRemoveFile(filename.c_str())) {
            SHOW_ERROR(message_GCP_NEWFILENAME_ERROR);
         }

         if (StartTask(filename)) {
            pTaskCommand_->SetActive(true);
         }
      }
   }
}

/** Abre un archivo de gcp existente e inicia la tarea de toma de puntos**/
void GeoreferenceTool::ExecuteOpenGcpFile() {
   if (pGeoreferenceTask_->IsActive())
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
     if (!StartTask(filename))
        pTaskCommand_->SetActive(false);
     else
        pTaskCommand_->SetActive(true);
}

/**
 * Inicia la tarea de edicion vectorial con vector seleccionado
 */
void GeoreferenceTool::ExecuteStartTask() {
   if (!pGeoreferenceTask_->IsActive()) {
      std::string filename;
      wxFileDialog dlg(NULL, _(caption_GCP_FILE), wxT(""), wxT(""),
                       filter_SAVE_GCP, wxFD_OPEN);

      if (dlg.ShowModal() != wxID_OK) {
         pTaskCommand_->SetActive(false);
      } else {
         filename = dlg.GetPath();

         if (filename.empty() || !wxFileName::FileExists(filename)) {
            SHOW_ERROR(message_GCP_OPENFILENAME_ERROR);
            pTaskCommand_->SetActive(false);
         } else {
            if (!StartTask(filename)) pTaskCommand_->SetActive(false);
         }
      }
   }
}

/**
 * Finaliza la tarea de edicion vectorial
 */
void GeoreferenceTool::ExecuteEndTask() {
   if (pGeoreferenceTask_->IsActive() && pTaskConfig_) {
      pGeoreferenceTask_->End();
      pTaskConfig_->Clear();
      delete pTaskConfig_;
      pTaskConfig_ = NULL;
   }
}

/**
 * Finaliza la edicion grafica de features, no finaliza la tarea.
 */
void GeoreferenceTool::ExecuteEndAddGcps() const {
   pGeoreferenceTask_->EndFeatureEdition();
}

}  // namespace suri
