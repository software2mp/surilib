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

#include "suri/ToggleButton.h"

// Includes suri
#include "suri/messages.h"
#include "wxToggleBitmapButton.h"

/** namespace suri */
namespace suri {
// ----------------------------- BOTON TIPO TOGGLE -----------------------------
/**
 * Constructor.
 * @param[in] pParent Ventana donde se incorpora el boton
 * @param[in] ButtonTooltip Texto de la ayuda emergente del boton
 * @param[in] Flag Mascara que define los eventos de mouse que se conectan sobre
 *            la ventana del visualizador
 * @return Instancia de la clase ToggleButton.
 */
ToggleButton::ToggleButton(wxWindow *pParent, wxString ButtonTooltip, int Flag) :
      Button(pParent, ButtonTooltip, Flag), pButton_(NULL) {
   eventType_ = wxEVT_COMMAND_TOGGLEBUTTON_CLICKED;
}

/**
 * Constructor.
 * @param[in] pParent Ventana donde se incorpora el boton
 * @param[in] Bitmap mapa de bits para setear al boton
 * @param[in] ButtonTooltip Texto de la ayuda emergente del boton
 * @param[in] Flag Mascara que define los eventos de mouse que se conectan sobre
 *            la ventana del visualizador
 * @return Instancia de la clase ToggleButton.
 */
ToggleButton::ToggleButton(wxWindow *pParent, const wxBitmap &Bitmap,
                           wxString ButtonTooltip, int Flag) :
      Button(pParent, Bitmap, ButtonTooltip, Flag), pButton_(NULL) {
   eventType_ = wxEVT_COMMAND_TOGGLEBUTTON_CLICKED;
}

/**
 * Constructor.
 * @param[in] pParent Ventana donde se incorpora el boton
 * @param[in] Bitmap string de recurso para obtener el bitmap
 * @param[in] ButtonTooltip Texto de la ayuda emergente del boton
 * @param[in] Flag Mascara que define los eventos de mouse que se conectan sobre
 *            la ventana del visualizador
 * @return Instancia de la clase ToggleButton.
 */
ToggleButton::ToggleButton(wxWindow *pParent, const wxString &Bitmap,
                           wxString ButtonTooltip, int Flag) :
      Button(pParent, Bitmap, ButtonTooltip, Flag), pButton_(NULL) {
   eventType_ = wxEVT_COMMAND_TOGGLEBUTTON_CLICKED;
}

/* guarda en state_ el valor de pButton y retorna Button::GetValue()
 * @return el estado del boton.
 */
bool ToggleButton::GetValue() const {
   const_cast<ToggleButton*>(this)->state_ = pButton_->GetValue();
   return Button::GetValue();
}

/**
 * Evento click en boton
 * @param[in] Event evento generado por wx.
 */
void ToggleButton::OnButtonClicked(wxCommandEvent &Event) {
   SetValue(GetValue());
}

/*
 * Evento asignar valor
 * @param[in] State estado a setear al boton.
 * @return true.
 */
bool ToggleButton::DoSetValue(bool State) {
   if (pButton_->GetValue() != State) pButton_->SetValue(State);
   return true;
}

/*
 * Evento crear ToolWindow
 * @return true.
 */
bool ToggleButton::DoCreateToolWindow() {
   pButton_ = new wxToggleBitmapButton(pParentWindow_, wxID_ANY, GetBitmap());
   pToolWindow_ = pButton_;
   return true;
}
}
// ----------------------------- BOTON TIPO TOGGLE -----------------------------

