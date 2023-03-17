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
#include "suri/LabelText.h"

// Includes Wx
#include "wx/stattext.h"

// Defines
// forwards

namespace suri {

/**
 * Constructor
 * @param Text texto que mostrara el label
 */
LabelText::LabelText(wxWindow *pViewerToolbar, const std::string& Text) :
      Widget(pViewerToolbar), text_(Text), enabled_(true), pTextCtrl_(NULL) {
}

/** Destructor */
LabelText::~LabelText() {
}

/** Se crea la ventana de herramienta en las clases derivadas */
bool LabelText::CreateToolWindow() {
   pTextCtrl_ = new wxStaticText(pParentWindow_, wxID_ANY, text_.c_str());
   pToolWindow_ = pTextCtrl_;
   pTextCtrl_->Enable(enabled_);
   return true;
}

/** Obtiene el texto del control grafico. */
std::string LabelText::GetValue() const {
   return pTextCtrl_->GetLabel().c_str();
}

/** Indica el texto a mostrar por el control grafico. */
void LabelText::SetValue(std::string& Value) {
   pTextCtrl_->SetLabel(Value.c_str());
}
/** Setea la propiedad enabled_ */
void LabelText::SetEnabled(bool Enabled) {
   enabled_ = Enabled;
   pTextCtrl_->Enable(enabled_);
}

/** Verifica si el boton se puede o no habilitar */
bool LabelText::IsEnabled() const {
   return enabled_;
}

} /** namespace suri */
