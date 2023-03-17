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

#ifndef TOGGLEBUTTON_H_
#define TOGGLEBUTTON_H_

// Includes suri
#include "suri/Button.h"

class wxToggleBitmapButton;

/** namespace suri */
namespace suri {
/** Button que queda precionado */
/**
 * Especializacion de Button para utilizar un boton que queda presionado.
 */
class ToggleButton : public Button {
   /** Ctor. de Copia. */
   ToggleButton(const ToggleButton &ToggleButton);

public:
   /** Ctor */
   ToggleButton(wxWindow *pParent, wxString ButtonTooltip, int Flag);
   /** Ctor */
   ToggleButton(wxWindow *pParent, const wxBitmap &Bitmap, wxString ButtonTooltip =
         wxT(tooltip_BUTTON),
                int Flag = BUTTON_NULL);
   /** Ctor */
   ToggleButton(wxWindow *pParent, const wxString &Bitmap, wxString ButtonTooltip =
         wxT(tooltip_BUTTON),
                int Flag = BUTTON_NULL);
   /** Dtor */
   virtual ~ToggleButton() {
   }

   virtual bool GetValue() const;

protected:
   /** Evento click en boton */
   virtual void OnButtonClicked(wxCommandEvent &Event);
   /** Evento asignar valor */
   virtual bool DoSetValue(bool State);
   /** Evento crear ToolWindow */
   virtual bool DoCreateToolWindow();
private:
   wxToggleBitmapButton *pButton_; /*! Boton con un bitmap que tiene 2 estados */
};
}

#endif /*TOGGLEBUTTONTOOL_H_*/
