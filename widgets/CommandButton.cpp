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
#include "suri/CommandButton.h"

// Includes Wx
// Defines
// forwards

namespace suri {

CommandButton::CommandButton(Command *pCommand) :
      Button(NULL, pCommand->GetDecoration(), pCommand->GetDescription(), BUTTON_NULL),
      pCommand_(pCommand) {
}

CommandButton::~CommandButton() {
}

/** Setea la propiedad enabled_ */
void CommandButton::SetEnabled(bool Enabled) {
   pCommand_->SetEnabled(Enabled);
}
/** Verifica si el boton se puede o no habilitar */
bool CommandButton::IsEnabled() {
   return pCommand_->IsEnabled();
}
void CommandButton::OnButtonClicked(wxCommandEvent &Event) {
   pCommand_->Execute();
   // El boton se destruye al cerrar y volver abrir un
   // proyecto. Cuando wx trata de procesar el evento OnClick,
   // el boton fue destruido y se genera un SIGSEGV,
   // Se agrego esta linea para que los botones abrir proyecto
   // y nuevo proyecto no sigan propagando los eventos.
   Event.Skip(false);
}

} /* namespace suri */
