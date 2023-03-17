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

#ifndef ZOOMIMAGESCALEBUTTON_H_
#define ZOOMIMAGESCALEBUTTON_H_

// Includes suri
#include "suri/Button.h"
#include "resources.h"
#include "suri/RasterSpatialModel.h"
#include "suri/Viewer2D.h"

/** namespace suri */
namespace suri {
/** Button que cambia la ventana para que muestre todo el mundo */
/**
 * Calcula un window para que la imagen activa tenga una relacion entre el
 * pixel de la imagen y la pantalla de 1:1. Mantiene el centro de la ventana.
 */
class ZoomImageScaleButton : public Button {
   /** Ctor. de Copia. */
   ZoomImageScaleButton(const ZoomImageScaleButton &ZoomFullExtentButton);

public:
   /** Ctor */
   ZoomImageScaleButton(wxWindow *pToolbar, Viewer2D *pViewer) :
         Button(pToolbar, wxT(button_SIMUR_ZOOM_ONE_TO_ONE), wxT(tooltip_ZOOM_ONE_TO_ONE)),
         pViewer_(pViewer) {
   }
   /** Dtor */
   virtual ~ZoomImageScaleButton() {
   }
protected:
   /** Cambia la ventana para mostrar un pixel de imagen por pixel de pantalla */
   virtual void OnButtonClicked(wxCommandEvent &Event);

private:
   Viewer2D *pViewer_; /*! Viewer */
};
}

#endif /*ZOOMIMAGESCALEBUTTON_H_*/
