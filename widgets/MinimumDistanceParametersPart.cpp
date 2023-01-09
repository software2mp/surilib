/*! \file MinimumDistanceParametersPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Standard

// Includes Suri
#include "MinimumDistanceParametersPart.h"
#include "suri/FactoryHelpers.h"
#include "suri/messages.h"
#include "suri/AuxiliaryFunctions.h"
#include "MinimumDistanceAlgorithm.h"

// Includes Wx
#include "wx/clrpicker.h"
#include "wx/xrc/xmlres.h"

namespace suri {

/* /! Macro para registrar la clase */
AUTO_REGISTER_CLASS(SupervisedParameterSelectionPart, MinimumDistanceParametersPart, 0)

const double MinimumDistanceParametersPart::DEFAULT_THRESHOLD = 30;
const VectorStyle::Color MinimumDistanceParametersPart::DEFAULT_COLOR =
                                                VectorStyle::Color(0, 0, 0, 0);

MinimumDistanceParametersPart::MinimumDistanceParametersPart() :
               ClusterClassificationPart(new MinimumDistanceAlgorithm()) {
}

MinimumDistanceParametersPart::~MinimumDistanceParametersPart() {
}

/**
 * Crea la ventana de la parte
 * @return true si pudo crear la ventana
 */
bool MinimumDistanceParametersPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(
         pParentWindow_, wxT("ID_SINGLE_CLASSIFICATION_PARAMS_PANEL"));
   return true;
}

/**
 * Inicializa los datos mostrados
 */
void MinimumDistanceParametersPart::SetInitialValues() {
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
std::string MinimumDistanceParametersPart::GetAlgorithmName() {
   return _(caption_MINIMUM_DISTANCE);
}


// --- Metodos Internos ---

/**
 * Cambia el color del control. Tranforma el valor en VectorStyle::Color
 * a formato que entiende el control
 * @param[in] Colour color que tendra el control wxColourPickerCtrl
 */
void MinimumDistanceParametersPart::SetColourCtrlValue(VectorStyle::Color Colour) {
   wxColourPickerCtrl *pcolorctrl =
         XRCCTRL(*pToolWindow_, wxT("ID_COLOR_PICKER"), wxColourPickerCtrl);
   wxColour colour(Colour.red_, Colour.green_, Colour.blue_, Colour.alpha_);
   pcolorctrl->SetColour(colour);
}

/**
 * Obtiene el color del control. Tranforma el valor a VectorStyle::Color
 * @return color tiene el control wxColourPickerCtrl
 */
VectorStyle::Color MinimumDistanceParametersPart::GetColourCtrlValue() {
   wxColourPickerCtrl *pcolorctrl =
         XRCCTRL(*pToolWindow_, wxT("ID_COLOR_PICKER"), wxColourPickerCtrl);
   wxColour color = pcolorctrl->GetColour();
   return VectorStyle::Color(color.Red(), color.Green(), color.Blue(), color.Alpha());
}

/**
 * Cambia el valor del threshold.
 * @param[in] Threshold nuevo valor del slider, debe estar entre 0 y 100
 */
void MinimumDistanceParametersPart::SetThresholdCtrlValue(double Threshold) {
   GET_CONTROL(*pToolWindow_, wxT("ID_THRESHOLD_TEXT"), wxTextCtrl)->
                              SetValue(wxString::Format(wxT("%g"), Threshold));
}

/** Retorna el valor del threshold */
double MinimumDistanceParametersPart::GetThresholdCtrlValue() {
   std::string value = USE_CONTROL(*pToolWindow_, "ID_THRESHOLD_TEXT",
                                            wxTextCtrl, GetValue(), "").c_str();
   return StringToNumber<double>(value);
}

}
