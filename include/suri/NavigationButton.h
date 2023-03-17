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

#ifndef NAVIGATIONBUTTON_H_
#define NAVIGATIONBUTTON_H_

#include "suri/ToggleButton.h"

/** namespace suri */
namespace suri {

class GoButton;
class CrossHairPaintEvent;
class ViewerWidget;

/** Activa NavigationWidget para recorrer el mundo y muestra cruz si corresponde */
/**
 * Agrega GoButton a NavigationWidget(Activandose si no estaba visible).
 * Escucha por el evento Paint y dibuja la cruz si las coordenadas Navigator
 * estan en la ventana.
 */
class NavigationButton : public ToggleButton {
public:
   /** ctor */
   NavigationButton(wxWindow *pToolbar, ViewerWidget* pViewer);

   /** dtor */
   virtual ~NavigationButton();

   /** Setea el estado del boton. Ingresa/Saca boton de NavigationWidget. */
   virtual bool DoSetValue(bool State);

   /** Evita que cambie el cursor al navegar */
   void DoSetCursor(bool State) { }

private:
   ViewerWidget* pViewer_;  // Se usa para obtener el navegador y para mostrar cruz
   GoButton* pGoButton_;  // Boton que se agrega al NavigationWidget para ir al destino deseado
   bool drawCross_;  // Determina si se tiene que dibujar la cruz
   CrossHairPaintEvent* pCrossHair_;
};
}

#endif /*NAVIGATIONBUTTON_H_*/
