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

#ifndef ZOOMBUTTON_H_
#define ZOOMBUTTON_H_

// Include standard
#include <cmath>

// Includes suri
#include "suri/ToggleButton.h"
#include "suri/Coordinates.h"

/** namespace suri */
namespace suri {
class Viewer2D;

/** ToggleButton que realiza zoomIn y zoomOut */
/**
 * Cuando se activa el boton espera eventos de click sobre el
 * viewer y modifica la ventana. Permite hacer zoom(in/out) eligiendo
 * un recuadro o haciendo click directamente.
 * Botton izquierdo: zoom in
 * Boton derecho: zoom out
 */
class ZoomButton : public ToggleButton {
   /** Ctor. de Copia. */
   ZoomButton(const ZoomButton &ZoomButton);

public:
   /** Ctor */
   ZoomButton(wxWindow *pToolbar, Viewer2D *pViewer);
   /** Dtor */
   virtual ~ZoomButton();
protected:
   /** Cambia cursor a magnificador */
   virtual void DoSetCursor(bool State);
   /** si se sale de la ventana se finaliza el zoom */
   virtual void DoOnLeaveWindow(wxMouseEvent &Event);
   /** inicializa el zoom in */
   virtual void DoOnLeftDown(wxMouseEvent &Event);
   /** realiza zoom in en viewer2D */
   virtual void DoOnLeftUp(wxMouseEvent &Event);
   /** inicializa el zoom out */
   virtual void DoOnRightDown(wxMouseEvent &Event);
   /** realiza zoom out en viewer2D */
   virtual void DoOnRightUp(wxMouseEvent &Event);
   /** dibuja el rectangulo si alguno de los dos zoom esta activado */
   virtual void DoOnMouseMove(wxMouseEvent &Event);
private:
   /** Dibuja un rectangulo en el window del viewer en las coordenadas indicadas */
   void DrawRectangleWithInvertedColor(const Coordinates &Point1,
                                       const Coordinates &Point2);
   /** Calcula rectangulo que tendra la ventana si se suelta el mouse en ese momento. */
   Coordinates GetNewPointCoordinates(wxMouseEvent &Event);

   Viewer2D *pViewer_; /*! Viewer */
   long initialDragX_; /*! Coordenada X del punto de inicio del drag */
   long initialDragY_; /*! Coordenada Y del punto de inicio del drag */
   long lastDrawX_; /*! Coordenada X del punto de finalizacion del drag */
   long lastDrawY_; /*! Coordenada Y del punto de finalizacion del drag */
   bool zoomInActivated_; /*! Flag : Estoy haciendo zoom in */
   bool zoomOutActivated_; /*! Flag : Estoy haciendo zoom out */
   bool drawRectangle_; /*! Flag : indica si se debe dibujar el rectangulo de */
   /* zoom */
};
}

#endif /*ZOOMBUTTON_H_*/
