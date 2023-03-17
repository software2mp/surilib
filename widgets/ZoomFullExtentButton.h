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

#ifndef ZOOMFULLEXTENTBUTTON_H_
#define ZOOMFULLEXTENTBUTTON_H_

// Includes suri
#include "suri/Button.h"

/** namespace suri */
namespace suri {
/** Button que cambia la ventana para que muestre todo el mundo */
/**
 * Modifica el window del viewer para que coincida con el extent del raster.
 */
class ZoomFullExtentButton : public Button {
   /** Ctor. de Copia. */
   ZoomFullExtentButton(const ZoomFullExtentButton &ZoomFullExtentButton);

public:
   /** Ctor */
   ZoomFullExtentButton(wxWindow *pToolbar, Viewer2D *pViewer) :
         Button(pToolbar, wxT(button_SIMUR_ZOOM_FULL_EXTENT)), pViewer_(pViewer) {
      windowTitle_ = _(tooltip_ZOOM_FULL_EXTENT);
   }
   /** Dtor */
   virtual ~ZoomFullExtentButton() {
   }
protected:
   /** Cambia la ventana para mostrar todo el mundo */
   virtual void OnButtonClicked(wxCommandEvent &Event) {
      Subset world;
      pViewer_->GetWorld()->GetWorld(world);
      pViewer_->ChangeWindow(world);
   }
private:
   Viewer2D *pViewer_; /*! Viewer */
};
}

#endif /*ZOOMFULLEXTENTBUTTON_H_*/
