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

#include "suri/CommandToggleButton.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

CommandToggleButton::CommandToggleButton(Command *pCommand) :
      ToggleButton(NULL, pCommand->GetDecoration(), pCommand->GetDescription(),
                   BUTTON_NULL),
      pCommand_(pCommand) {
}

CommandToggleButton::~CommandToggleButton() {
}

/*!
 *  Setea la propiedad enabled_
 */
void CommandToggleButton::SetEnabled(bool Enabled) {
   pCommand_->SetEnabled(Enabled);
}
/** Verifica si el boton se puede o no habilitar */
bool CommandToggleButton::IsEnabled() {
   return pCommand_->IsEnabled();
}

/**
 * Evento que habilita el estado del control wx del boton en base al
 * retorno del metodo IsEnabled.
 * @param[in] Event evento UIUpdate
 */
void CommandToggleButton::OnUIUpdate(wxUpdateUIEvent &Event) {
   Button::OnUIUpdate(Event);
   ToggleButton::DoSetValue(pCommand_->IsActive());
}

bool CommandToggleButton::GetValue() const {
   return pCommand_->IsActive();
}

/**
 * Evento click en boton
 * @param[in] Event evento generado por wx.
 */
void CommandToggleButton::OnButtonClicked(wxCommandEvent &Event) {
   SetValue(!GetValue());
}

// ! Evento asignar valor
bool CommandToggleButton::DoSetValue(bool State) {
   ToggleButton::DoSetValue(State);
   if (State != pCommand_->IsActive()) {
      pCommand_->SetActive(State);
      return pCommand_->Execute();
   }
   return true;
}

} /* namespace suri */
