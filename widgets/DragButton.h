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

#ifndef DRAGBUTTON_H_
#define DRAGBUTTON_H_

// Includes standard

// Includes suri
#include "suri/ToggleButton.h"
#include "suri/Viewer2D.h"

// Includes Wx

// Includes app

// Defines

/** namespace suri */
namespace suri {
/** ToggleButton que Permite mover la ventana */
/**
 * Se moviliza la ventana y se muestra la parte del mundo que se encuentre dentro
 * de ella.
 * Limita el desplazamiento para no perder la imagen
 * \note hereda de ToggleButton para que se muestre que la opcion activa es la
 *  de Drag
 */
class DragButton : public ToggleButton {
   /** Ctor. de Copia. */
   DragButton(const DragButton &DragButton);

public:
   /** Ctor. */
   DragButton(wxWindow *pToolbar, Viewer2D *pViewer);
   /** Dtor. */
   virtual ~DragButton();
protected:
   /** Cambia cursor a mano */
   virtual void DoSetCursor(bool State);
   /** Cambia subset de la ventana */
   virtual void DoOnLeftUp(wxMouseEvent &Event);
   /** Cambia posicion de la imagen mostrada */
   virtual void DoOnMouseMove(wxMouseEvent &Event);
   /** Si el mouse sale de la pantalla mientras se hace drag la ventana muestra la mitad del dibujo */
   virtual void DoOnLeaveWindow(wxMouseEvent &Event);
private:
   Viewer2D *pViewer_; /*! viewer */
   int initialDragX_; /*! posicion inicial de la ventana en x */
   int initialDragY_; /*! posicion inicial de la ventana en y */
   int initialPosX_; /*! posicion inicial del scroll en x */
   int initialPosY_; /*! posicion inicial del scroll en y */
};
}
#endif /*DRAGBUTTON_H_*/
