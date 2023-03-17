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

#ifndef CLOSEBUTTON_H_
#define CLOSEBUTTON_H_

// Includes standard
// Includes Suri
#include "suri/Button.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
// Forwards
class VectorElementEditor;

/** Boton que cierra el feature abierto en el vector que se esta editando */
/**
 * Boton que permite durante la edicion de vectores cerrar el feature abierto.
 * Este boton se agrega a las herramientas de edicion de que no responden a los
 * eventos de mouse (GPS).
 */
class CloseButton : public Button {
public:
   /** ctor */
   CloseButton(wxWindow *pToolbar, VectorElementEditor* pVectorElementEditor);
   /** dtor */
   virtual ~CloseButton();
protected:
   /** Cierra el feature abierto en VectorEditor. */
   virtual void OnButtonClicked(wxCommandEvent &Event);
private:
   VectorElementEditor* pVectorElementEditor_; /*! Elemento para la edicion */
};
}  // namespace suri

#endif /*CLOSEBUTTON_H_*/
