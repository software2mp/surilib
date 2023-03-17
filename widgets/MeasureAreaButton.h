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

#ifndef MEASUREAREABUTTON_H_
#define MEASUREAREABUTTON_H_

#include "MeassureAreaElementEditor.h"
#include "suri/ToggleButton.h"

#include "wx/wx.h"
#include "wx/bmpbuttn.h"
#include "wx/xrc/xmlres.h"

namespace suri {

/**
 * Boton para activar la herramienta de medicion de area y perimetro.
 */
class MeasureAreaButton : public ToggleButton {
public:
   /**
    * Ctor.
    */
   MeasureAreaButton(wxWindow *pParent, const wxString &Bitmap,
                     wxString ButtonTooltip, int Flags, MeassureAreaElementEditor* pEditor) :
         ToggleButton(pParent, Bitmap, ButtonTooltip, Flags), pEditor_(pEditor) {
   }

protected:
   virtual bool DoSetValue(bool State) {
      if (State && pEditor_ && !pEditor_->IsActive()) {
         pEditor_->Start();
      } else if (!State && pEditor_ && pEditor_->IsActive()) {
         pEditor_->Stop();
      }
      return ToggleButton::DoSetValue(State);
   }

   /** Metodo ejecutado al levantar/soltar el boton izquierdo del mouse. */
   virtual void DoOnLeftUp(wxMouseEvent &Event) { }

   /** Metodo ejecutado al levantar/soltar el boton derecho del mouse. */
   virtual void DoOnRightUp(wxMouseEvent &Event) { }

   /** Elimina el ultimo punto agregado al feature. */
   virtual void DoOnMiddleUp(wxMouseEvent &Event) { }

private:
   MeassureAreaElementEditor* pEditor_;
};

}  // namespace suri

#endif  // MEASUREAREABUTTON_H_
