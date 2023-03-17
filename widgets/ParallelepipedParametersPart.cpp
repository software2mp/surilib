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

// Includes Standard
#include <string>

// Includes Suri
#include "ParallelepipedParametersPart.h"
#include "suri/FactoryHelpers.h"
#include "suri/messages.h"
#include "suri/AuxiliaryFunctions.h"
#include "SupervisedParameterSelectionPart.h"
#include "ParallelepipedAlgorithm.h"

// Includes Wx
#include "wx/clrpicker.h"
#include "wx/slider.h"
#include "wx/xrc/xmlres.h"

namespace suri {

/* /! Macro para registrar la clase */
AUTO_REGISTER_CLASS(SupervisedParameterSelectionPart, ParallelepipedParametersPart, 0)


const double ParallelepipedParametersPart::DEFAULT_THRESHOLD = 2;
const VectorStyle::Color ParallelepipedParametersPart::DEFAULT_COLOR =
                                                VectorStyle::Color(0, 0, 0, 0);

ParallelepipedParametersPart::ParallelepipedParametersPart() :
            ClusterClassificationPart(new ParallelepipedAlgorithm()) {
}

ParallelepipedParametersPart::~ParallelepipedParametersPart() {
}

/**
 * Crea la ventana de la parte
 * @return true si pudo crear la ventana
 */
bool ParallelepipedParametersPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(
         pParentWindow_, wxT("ID_SINGLE_CLASSIFICATION_PARAMS_PANEL"));

   return true;
}

/**
 * Inicializa los datos mostrados
 */
void ParallelepipedParametersPart::SetInitialValues() {
   color_ = DEFAULT_COLOR;
   pAlgorithm_->SetThreshold(DEFAULT_THRESHOLD);
   SetColourCtrlValue(DEFAULT_COLOR);
   SetThresholdCtrlValue(DEFAULT_THRESHOLD);
}

// --- Metodos de ParametersSelectionPart ---
/**
 * Retorna el nombre del algoritmo que se usara para clasificar
 * @return string con el nombre del algoritmo de clasificacion
 */
std::string ParallelepipedParametersPart::GetAlgorithmName() {
   return _(caption_PARALLELEPIPED);
}

// --- Metodos Internos ---

/**
 * Cambia el color del control. Tranforma el valor en VectorStyle::Color
 * a formato que entiende el control
 * @param[in] Colour color que tendra el control wxColourPickerCtrl
 */
void ParallelepipedParametersPart::SetColourCtrlValue(VectorStyle::Color Colour) {
   wxColourPickerCtrl *pcolorctrl =
         XRCCTRL(*pToolWindow_, wxT("ID_COLOR_PICKER"), wxColourPickerCtrl);
   wxColour colour(Colour.red_, Colour.green_, Colour.blue_, Colour.alpha_);
   pcolorctrl->SetColour(colour);
}

/**
 * Obtiene el color del control. Tranforma el valor a VectorStyle::Color
 * @return color tiene el control wxColourPickerCtrl
 */
VectorStyle::Color ParallelepipedParametersPart::GetColourCtrlValue() {
   wxColourPickerCtrl *pcolorctrl =
         XRCCTRL(*pToolWindow_, wxT("ID_COLOR_PICKER"), wxColourPickerCtrl);
   wxColour color = pcolorctrl->GetColour();
   return VectorStyle::Color(color.Red(), color.Green(), color.Blue(), color.Alpha());
}

/**
 * Cambia el valor del threshold.
 * @param[in] Threshold nuevo valor del slider, debe estar entre 0 y 100
 */
void ParallelepipedParametersPart::SetThresholdCtrlValue(double Threshold) {
   GET_CONTROL(*pToolWindow_, wxT("ID_THRESHOLD_TEXT"), wxTextCtrl)->
                              SetValue(wxString::Format(wxT("%g"), Threshold));
}

/** Retorna el valor del threshold */
double ParallelepipedParametersPart::GetThresholdCtrlValue() {
   std::string value = USE_CONTROL(*pToolWindow_, "ID_THRESHOLD_TEXT",
                                            wxTextCtrl, GetValue(), "").c_str();
   return StringToNumber<double>(value);
}


}
