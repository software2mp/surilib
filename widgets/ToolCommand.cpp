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
// Includes Suri
#include "suri/ToolCommand.h"
#include "suri/ParameterCollection.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/GuiStateChangeMediator.h"
#include "suri/StateChangeNotification.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Constructor
 * @param pTool herramienta que se quiere manejar con el command
 * @param Id id del comando
 */
ToolCommand::ToolCommand(ToolInterface *pTool, const CommandIdType &Id) :
      pTool_(pTool), id_(Id), groupId_(Command::Standalone), type_(""), enable_(true),
      active_(false), pInputParams_(NULL), pOutputParams_(NULL) {
}

/** Destructor **/
ToolCommand::~ToolCommand() {
   std::string commandid = IntToString(GetId());
   suri::StateChangeNotification notif(GuiStateChangeMediator::DeleteCommand, commandid);
   Notify(&notif);

   delete pInputParams_;
   delete pOutputParams_;
}

/** Ejecuta el comando **/
bool ToolCommand::Execute() {
   pTool_->Execute(this);
   return true;
}

/** Obtiene el id del comando */
ToolCommand::CommandIdType ToolCommand::GetId() const {
   return id_;
}

/** Obtiene el id del grupo al que pertenece el comando **/
Command::CommandExclusionGroupType ToolCommand::GetGroupId() const {
   return groupId_;
}

/** Obtiene el tipo de comando */
ToolCommand::CommandType ToolCommand::GetType() const {
   return type_;
}

/** Tooltip / Help */
const std::string ToolCommand::GetDescription() const {
   return description_;
}
/** bitmap asociado */
const std::string ToolCommand::GetDecoration() const {
   return decoration_;
}

/** ruta (xpath, ramas, etc) del menu donde aparece, hasta el texto del item. */
const std::string ToolCommand::GetLocation() const {
   return location_;
}

void ToolCommand::SetId(const CommandIdType &Id) {
   id_ = Id;
}

void ToolCommand::SetGroupId(const CommandExclusionGroupType &GroupId) {
   groupId_ = GroupId;
}

void ToolCommand::SetType(const CommandType &Type) {
   type_ = Type;
}

void ToolCommand::SetDescription(const std::string &Description) {
   description_ = Description;
}

void ToolCommand::SetDecoration(const std::string &Decoration) {
   decoration_ = Decoration;
}
void ToolCommand::SetLocation(const std::string &Location) {
   location_ = Location;
}

bool ToolCommand::IsActive() const {
   return active_ || pTool_->IsActive(this);
}
void ToolCommand::SetActive(bool Active) {
   if (active_ != Active) {
      pTool_->SetActive(this, Active);
      active_ = Active;

      std::string commandid = IntToString(GetId());
      std::string action = active_ ? suri::GuiStateChangeMediator::Activate :
                                       suri::GuiStateChangeMediator::Deactivate;
      suri::StateChangeNotification notif(action, commandid);
      Notify(&notif);
   }
}

bool ToolCommand::IsEnabled() const {
   return pTool_->IsEnabled(this) || enable_;
}

void ToolCommand::SetEnabled(bool Enable) {
   pTool_->SetEnabled(this, Enable);
   enable_ = Enable;
}

/** Indica los parametros de entrada al command (ej. opciones para un command de tipo choice) */
void ToolCommand::SetInputParameters(ParameterCollection* pParameters) {
   if (pInputParams_)
      delete pInputParams_;
   pInputParams_ = pParameters;
}

/** Obtiene los parametros de entrada del command*/
ParameterCollection* ToolCommand::GetInputParameters() const {
   return pInputParams_;
}

/** Indica los parametros de salida al command (ej. seleccion de un choice)*/
void ToolCommand::SetOutputParameters(ParameterCollection* pParameters) {
   if (pOutputParams_)
      delete pOutputParams_;
   pOutputParams_ = pParameters;
}

/** Obtiene los parametros de salida del command */
ParameterCollection* ToolCommand::GetOutputParameters() const {
   return pOutputParams_;
}

/** Obtiene el nombre de la tarea asociada (si existe alguna) */
std::string ToolCommand::GetAssociatedTaskId() const {
   return associatedTaskId_;
}

/** Establece el nombre de la tarea asociada (si existe alguna) */
void ToolCommand::SetAssociatedTaskId(std::string TaskId) {
   associatedTaskId_ = TaskId;
}

} /* namespace suri */
