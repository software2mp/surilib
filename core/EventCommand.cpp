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
#include "suri/EventCommand.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/GuiStateChangeMediator.h"
#include "suri/StateChangeNotification.h"

// Includes Wx
// Defines
// forwards

namespace suri {

EventCommand::EventCommand(MouseEventInterface *pMouseEvent,
                           PaintEventInterface *pPaintEvent, const CommandIdType &Id,
                           const CommandType &Type, const std::string &Decoration,
                           const std::string &Description, const std::string &Location) :
      id_(Id), type_(Type), decoration_(Decoration), description_(Description),
      location_(Location), pMouseEvent_(pMouseEvent), pPaintEvent_(pPaintEvent),
      active_(false), enabled_(true) {
   if (pMouseEvent_) pMouseEvent_->MouseEnabled(active_);
   if (pPaintEvent_) pPaintEvent_->PaintEnabled(active_);
}

EventCommand::~EventCommand() {
   std::string commandid = IntToString(GetId());
   suri::StateChangeNotification notif(GuiStateChangeMediator::DeleteCommand, commandid);
   Notify(&notif);
}

bool EventCommand::Execute() {
   return true;
}

Command::CommandIdType EventCommand::GetId() const {
   return id_;
}

Command::CommandExclusionGroupType EventCommand::GetGroupId() const {
   return pMouseEvent_ ? pMouseEvent_->GetMouseFlags() : Command::Standalone;
}

Command::CommandType EventCommand::GetType() const {
   return type_;
}

/** Tooltip / Help */
const std::string EventCommand::GetDescription() const {
   return description_;
}
/** bitmap asociado */
const std::string EventCommand::GetDecoration() const {
   return decoration_;
}
/** ruta (xpath, ramas, etc) del menu donde aparece, hasta el texto del item. */
const std::string EventCommand::GetLocation() const {
   return location_;
}

void EventCommand::SetId(const CommandIdType &Id) {
   id_ = Id;
}

void EventCommand::SetGroupId(const CommandExclusionGroupType &GroupId) {
}

void EventCommand::SetType(const CommandType &Type) {
   type_ = Type;
}
void EventCommand::SetDescription(const std::string &Description) {
   description_ = Description;
}
void EventCommand::SetDecoration(const std::string &Decoration) {
   decoration_ = Decoration;
}
void EventCommand::SetLocation(const std::string &Location) {
   location_ = Location;
}

bool EventCommand::IsActive() const {
   return
         enabled_ ? (pMouseEvent_ && pMouseEvent_->IsMouseEnabled())
                          || (pPaintEvent_ && pPaintEvent_->IsPaintEnabled()) :
                    active_;
}
void EventCommand::SetActive(bool Active) {
   if (active_ != Active) {
      if (pMouseEvent_) pMouseEvent_->MouseEnabled(Active);
      if (pPaintEvent_) pPaintEvent_->PaintEnabled(Active);
      active_ = Active;

      std::string commandid = IntToString(GetId());
      std::string action = active_ ? suri::GuiStateChangeMediator::Activate :
                                       suri::GuiStateChangeMediator::Deactivate;
      suri::StateChangeNotification notif(action, commandid);
      Notify(&notif);
   }
}
bool EventCommand::IsEnabled() const {
   return enabled_;
}
void EventCommand::SetEnabled(bool Enable) {
   bool activation = !Enable ? false : active_;
   if (pMouseEvent_) pMouseEvent_->MouseEnabled(activation);
   if (pPaintEvent_) pPaintEvent_->PaintEnabled(activation);
   enabled_ = Enable;
}

/** Indica los parametros de entrada al command (ej. opciones para un command de tipo choice) */
void EventCommand::SetInputParameters(ParameterCollection* pParameters) {
}

/** Obtiene los parametros de entrada del command*/
ParameterCollection* EventCommand::GetInputParameters() const {
   return NULL;
}

/** Indica los parametros de salida al command (ej. seleccion de un choice)*/
void EventCommand::SetOutputParameters(ParameterCollection* pParameters) {
}

/** Obtiene los parametros de salida del command */
ParameterCollection* EventCommand::GetOutputParameters() const{
   return NULL;
}

/**
 * Obtiene el nombre de la tarea asociada (si existe alguna).
 */
std::string EventCommand::GetAssociatedTaskId() const {
   return associatedTaskId_;
}

/**
 * Establece el nombre de la tarea asociada (si existe alguna)
 */
void EventCommand::SetAssociatedTaskId(std::string TaskId) {
   associatedTaskId_ = TaskId;
}

} /* namespace suri */
