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

#ifndef EYECAMBUTTON_H_
#define EYECAMBUTTON_H_

// Includes suri
#include "suri/ToggleButton.h"

/** namespace suri */
namespace suri {
/** ToggleButton que activa la camara con vista sobre terreno */
/**
 * Activa la accion de EyeCam en TerrainGlCanvas
 * Permite cambiar el angulo de la camara con respecto a los 3 ejes.
 * Los movimientos son: Pitch(Cabeceo), Roll(Alabeo) y Yaw(Guinado)
 * \note hereda de ToggleButton para que se muestre que la opcion activa es la
 *       de EyeCam
 */
class EyeCamButton : public ToggleButton {
   /** Ctor. de Copia. */
   EyeCamButton(const EyeCamButton &EyeCamButton);

public:
   /** Ctor */
   EyeCamButton(wxWindow *pToolbar, TerrainGLCanvas *pGlCanvas) :
         ToggleButton(pToolbar, wxT(tool_EYE_CAMERA), wxT(tooltip_EYE_CAMERA),
                      BUTTON_NULL),
         pGlCanvas_(pGlCanvas) {
   }
   /** Dtor */
   virtual ~EyeCamButton() {
   }
protected:
   /** Cambia cursor a flecha */
   virtual void DoSetCursor(bool State) {
      if (State) {
         pViewerWindow_->SetCursor(wxCursor(wxCURSOR_ARROW));
      } else {
         pViewerWindow_->SetCursor(wxCursor(wxCURSOR_DEFAULT));
      }
   }
   /** Activa la accion TerrainGLCanvas::EyeCam en GlCanvas */
   virtual void OnButtonClicked(wxCommandEvent &Event) {
      pGlCanvas_->SetAction(TerrainGLCanvas::EyeCam);
   }
private:
   TerrainGLCanvas *pGlCanvas_; /*! TerrainGLCanvas */
};
}

#endif /*EYECAMBUTTON_H_*/
