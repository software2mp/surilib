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

#include "CloseVectorButton.h"

// Includes suri
#include "suri/messages.h"
#include "VectorElementEditor.h"
#include "resources.h"

// Includes App

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] pToolbar toolbar donde se inserta el boton
 * @param[in] pVectorElementEditor elemento para la edicion
 */
CloseVectorButton::CloseVectorButton(wxWindow *pToolbar, GeometryElementEditor* pGeometryElementEditor,
                                     VectorElementEditor* pVectorElementEditor) :
      Button(pToolbar, wxT(button_CLOSE_VECTOR), wxT(tooltip_DISCARD)), pGeometryElementEditor_(pGeometryElementEditor),
      pVectorElementEditor_(pVectorElementEditor) {
}
/**
 * Destructor
 */
CloseVectorButton::~CloseVectorButton() {
}

/**
 * Cierra el vector en edicion.
 * @param[in] Event evento click sobre el boton
 */
void CloseVectorButton::OnButtonClicked(wxCommandEvent &Event) {
   if (pGeometryElementEditor_ )
      pGeometryElementEditor_->CloseVector();
   else if (pVectorElementEditor_)
      pVectorElementEditor_->CloseVector();

   return;
}

/**
 * Verifica las condiciones de activacion del boton.
 * @return true si hay un vector abierto
 */
bool CloseVectorButton::IsEnabled() {

   if (pGeometryElementEditor_)
      return pGeometryElementEditor_->IsEditing();
   else
   {
      /** Si la insancia no es Geometry, por descarte es pVectorElementEditor_ */
      if (pVectorElementEditor_)
         return pVectorElementEditor_->IsVectorOpen();
      else
         return false;
   }
}

}  // namespace suri

