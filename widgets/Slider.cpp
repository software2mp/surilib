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

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/Slider.h"
#include "suri/LabelText.h"

// Includes Wx
// Defines
// forwards

namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(SliderEventHandler, Slider)
      IMPLEMENT_EVENT_CALLBACK(OnValueChanged, OnValueChanged(event),  wxScrollEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Constructor
 * @param pViewerToolbar referencia a la ventana que contendra el slider
 * @param Tooltip tooltip a configurar
 * @param Flag flag para la configuracion de eventos
 */
Slider::Slider(wxWindow *pViewerToolbar, const std::string& Tooltip, int Flag) :
      Widget(pViewerToolbar), enabled_(true), NEW_EVENT_OBJECT(SliderEventHandler) {
}

/** Destructor **/
Slider::~Slider() {
   DELETE_EVENT_OBJECT;
}

/** Se crea la ventana de herramienta en las clases derivadas */
bool Slider::CreateToolWindow() {
   pSlider_ = new wxSlider(pParentWindow_, wxID_ANY, 0, 0, 0);
   pToolWindow_ = pSlider_;
   pSlider_->SetToolTip(windowTitle_);
   pSlider_->Enable(enabled_);
   pSlider_->Connect(wxEVT_SCROLL_CHANGED,
                     wxScrollEventHandler(SliderEventHandler::OnValueChanged), NULL,
                  pEventHandler_);
   OnCreateToolWindow();
   return true;
}

/** Metodo que se llama cuando se ejecuta CreateToolWindow **/
void Slider::OnCreateToolWindow() {
}

/** Obtiene la seleccion activa en el control grafico. */
int Slider::GetValue() const {
   return pSlider_->GetValue();
}

/** Indica la seleccion activa en el control grafico. */
void Slider::SetValue(int Value) {
   pSlider_->SetValue(Value);
}

/** Metodo que se llama cuando cambia el valor actual en el control grafico.  */
void Slider::OnValueChanged(wxScrollEvent &Event) {
}

/** Setea la propiedad enabled_ */
void Slider::SetEnabled(bool Enabled) {
   enabled_ = Enabled;
   pSlider_->Enable(Enabled);
}

/** Verifica si el boton se puede o no habilitar */
bool Slider::IsEnabled() const {
   return enabled_;
}

/** Metodo que indica si posee un label asociado **/
bool Slider::HasLabel() const {
   return !label_.empty();
}

/** Metodo que obtiene el label **/
LabelText* Slider::GetLabel() const {
   return new LabelText(pParentWindow_, label_);
}
} /** namespace suri */
