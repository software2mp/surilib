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

#ifndef DELETEBUTTON_H_
#define DELETEBUTTON_H_

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
/** Declara el objeto de eventos para los que heredan de CBaseTool */
DECLARE_EVENT_OBJECT(DeleteButtonEvent);

/** Boton que elimina el ultimo punto ingresado en el vector */
/**
 * Boton para las herramientas de edicion de vectores que no responden a eventos
 * de mouse (GPS). Permite eliminar el ultimo punto de un feature abierto del
 * vector en edicion. El boton se esconde si no hay puntos para eliminar.
 */
class DeleteButton : public suri::Button {
public:
   /** Declara el objeto de eventos como clase friend */
   FRIEND_EVENT_OBJECT(DeleteButtonEvent);
   /** ctor */
   DeleteButton(wxWindow *pToolbar, VectorElementEditor* pVectorElementEditor);
   /** dtor */
   virtual ~DeleteButton();
protected:
   /** Elimina el ultimo punto ingresado en VectorEditor. */
   virtual void OnButtonClicked(wxCommandEvent &Event);
// Metodo de creacion del boton-herramienta
   virtual bool CreateToolWindow();
   /** Esconde boton cuando no hay puntos en vector en edicion */
   virtual void OnHideWithNoPointsUiUpdate(wxUpdateUIEvent &Event);
private:
   EVENT_OBJECT_PROPERTY(DeleteButtonEvent); /*! Objeto para conectar eventos */
   VectorElementEditor* pVectorElementEditor_; /*! VectorElementEditor que se */
   /* utilza para eliminar el punto */
};
}  // namespace suri

#endif /*DELETEBUTTON_H_*/
