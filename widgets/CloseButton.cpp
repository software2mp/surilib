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

#include "CloseButton.h"

// Includes standard

// Includes suri
#include "suri/messages.h"
#include "resources.h"
#include "VectorElementEditor.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] pToolbar : ventana donde se inserta el boton.
 * @param[in] pVectorElementEditor : elemento para la edicion del vector.
 */
CloseButton::CloseButton(wxWindow *pToolbar, VectorElementEditor* pVectorElementEditor) :
      Button(pToolbar, wxT(button_GPS_CLOSE), wxT(tooltip_CLOSE_GEOMETRY)),
      pVectorElementEditor_(pVectorElementEditor) {
}

/** dtor */
CloseButton::~CloseButton() {
}

/**
 * Cierra el feature abierto.
 * @param[in] Event evento click sobre el boton
 */
void CloseButton::OnButtonClicked(wxCommandEvent &Event) {
   pVectorElementEditor_->CloseFeature();
}
}  // namespace suri
