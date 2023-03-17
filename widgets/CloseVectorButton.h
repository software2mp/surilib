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

#ifndef CLOSEVECTORBUTTON_H_
#define CLOSEVECTORBUTTON_H_

// Includes Suri
#include "suri/Button.h"
#include "GeometryElementEditor.h"
// Forwards

/** namespace suri */
namespace suri {
// Forwards
class VectorElementEditor;

/** Boton que permite descartar el vector en edicion. */
/**
 * Boton que sirve para descartar un vector en edicion, en las herramientas de
 * edicion de vectores. Se activa solo cuando hay un vector en edicion.
 */
class CloseVectorButton : public Button {
public:
   /** Ctor. */
   CloseVectorButton(wxWindow *pToolbar, GeometryElementEditor* pGeometryElementEditor,
                     VectorElementEditor* pVectorElementEditor = NULL);
   /** Ctor - sobrecargado para soportar la nueva geomentria. GeometryElementEditor */

   /** Dtor. */
   virtual ~CloseVectorButton();
   /** Verifica el estado de activacion del boton */
   virtual bool IsEnabled();
   /** Permite asignar el VectorElementEditor sobre el que se esta editando */
   void SetVectorElementEditor(VectorElementEditor* pVectorElementEditor) {
      pVectorElementEditor_ = pVectorElementEditor;
   }
   void SetGeometryElementEditor(GeometryElementEditor* pGeometryElementEditor) {
      pGeometryElementEditor_ = pGeometryElementEditor;
   }

protected:
   /** Elimina el ultimo punto ingresado en VectorEditor. */
   virtual void OnButtonClicked(wxCommandEvent &Event);
private:
   GeometryElementEditor* pGeometryElementEditor_;
   VectorElementEditor* pVectorElementEditor_; /*! Vector en edicion */

};
}  // namespace suri

#endif /* CLOSEVECTORBUTTON_H_ */
