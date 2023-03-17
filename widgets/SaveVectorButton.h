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

#ifndef SAVEVECTORBUTTON_H_
#define SAVEVECTORBUTTON_H_

// Includes Suri
#include "suri/Button.h"
#include "GeometryElementEditor.h"

/** namespace suri */
namespace suri {
// Forwards
class VectorElementEditor;

/** Button que salva el vector en edicion. */
/**
 * Clase que crea un boton para salvar el vector en edicion para las
 * herramientas de edicion de vectores.
 */
class SaveVectorButton : public Button {
public:
   /** Ctor. */
   SaveVectorButton(wxWindow *pToolbar, GeometryElementEditor* pGeometryElementEditor,
                    VectorElementEditor* pVectorElementEditor = NULL);
   /** Dtor. */
   virtual ~SaveVectorButton();
   /** Verifica el estado de activacion del boton */
   virtual bool IsEnabled();
   /** Asigna el vector en edicion */
   void SetVectorElementEditor(VectorElementEditor* pVectorElementEditor) {
      pVectorElementEditor_ = pVectorElementEditor;
   }
   void SetGeometryElementEditor(GeometryElementEditor* pGeoEditor) {
      pGeoEditor_ = pGeoEditor;
   }
protected:
   /** Elimina el ultimo punto ingresado en VectorEditor. */
   virtual void OnButtonClicked(wxCommandEvent &Event);
private:
   GeometryElementEditor* pGeoEditor_;
   VectorElementEditor* pVectorElementEditor_; /*! VectorEditor */
};
}  // namespace suri

#endif /* SAVEVECTORBUTTON_H_ */
