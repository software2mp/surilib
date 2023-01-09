/*! \file MaxLikelihoodPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "MaxLikelihoodPart.h"

// Includes standard
#include <math.h>
#include <string>
#include <vector>

// Includes Suri
#include "suri/FactoryHelpers.h"
#include "suri/messages.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
#include "wx/clrpicker.h"
#include "wx/slider.h"
#include "wx/xrc/xmlres.h"
// Defines
#define NOCLASSVALUE 0

/** Numero Pi */
#define PI 3.14159265

/** inicializa los recursos de la ventana */
void InitMaxLikelihoodParametersXmlResource();

/** namespace suri */
namespace suri {
/** Macro para registrar la clase */
AUTO_REGISTER_CLASS(SupervisedParameterSelectionPart, MaxLikelihoodPart, 0)

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(MaxLikelihoodPartWidgetEvent, MaxLikelihoodPart)
IMPLEMENT_EVENT_CALLBACK(OnScroll, OnScroll(event), wxScrollEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

const double MaxLikelihoodPart::DEFAULT_THRESHOLD = 0.25;
const VectorStyle::Color MaxLikelihoodPart::DEFAULT_COLOR =
                                          VectorStyle::Color(0, 0, 0, 0);

/**
 * Ctor.
 * \attention el color pasado por parametro es responsabilidad de la clase
 * que llama al ctor.
 */
MaxLikelihoodPart::MaxLikelihoodPart() :
      ClusterClassificationPart(new MaxLikelihoodAlgorithm()),
      NEW_EVENT_OBJECT(MaxLikelihoodPartWidgetEvent) {
   pAlgorithm_->SetThreshold(DEFAULT_THRESHOLD);
}

/** Dtor */
MaxLikelihoodPart::~MaxLikelihoodPart() {
}

// --- Metodos de Part ---

/**
 * Crea la ventana de la parte
 * @return true si pudo crear la ventana
 */
bool MaxLikelihoodPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_MAXLIKELIHOOD_PARAMS"));

   GET_CONTROL(*pToolWindow_, wxT("ID_THRESHOLD"), wxSlider)
      ->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(
            MaxLikelihoodPartWidgetEvent::OnScroll),
                NULL, pEventHandler_);

   GET_CONTROL(*pToolWindow_, wxT("ID_THRESHOLD"), wxSlider)
      ->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(
            MaxLikelihoodPartWidgetEvent::OnScroll),
                NULL, pEventHandler_);

   return true;
}

/**
 * Inicializa los datos mostrados
 */
void MaxLikelihoodPart::SetInitialValues() {
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
std::string MaxLikelihoodPart::GetAlgorithmName() {
   return _(caption_MAXIMUN_LIKELIHOOD);
}


// --- Metodos Internos ---

/**
 * Cambia el color del control. Tranforma el valor en VectorStyle::Color
 * a formato que entiende el control
 * @param[in] Colour color que tendra el control wxColourPickerCtrl
 */
void MaxLikelihoodPart::SetColourCtrlValue(VectorStyle::Color Colour) {
   wxColourPickerCtrl *pcolorctrl =
         XRCCTRL(*pToolWindow_, wxT("ID_COLOR_PICKER"), wxColourPickerCtrl);

   wxColour colour(Colour.red_, Colour.green_, Colour.blue_, Colour.alpha_);
   pcolorctrl->SetColour(colour);
}

/**
 * Obtiene el color del control. Tranforma el valor a VectorStyle::Color
 * @return color tiene el control wxColourPickerCtrl
 */
VectorStyle::Color MaxLikelihoodPart::GetColourCtrlValue() {
   wxColourPickerCtrl *pcolorctrl =
         XRCCTRL(*pToolWindow_, wxT("ID_COLOR_PICKER"), wxColourPickerCtrl);

   wxColour color = pcolorctrl->GetColour();

   return VectorStyle::Color(color.Red(), color.Green(), color.Blue(), color.Alpha());
}

/**
 * Cambia el valor del slider.
 * @param[in] Threshold nuevo valor del slider, debe estar entre 0 y 100
 */
void MaxLikelihoodPart::SetThresholdCtrlValue(double Threshold) {
   wxSlider *pslider = XRCCTRL(*pToolWindow_, wxT("ID_THRESHOLD"), wxSlider);

   // Modifico el slider para que el desplazamiento sea logaritmico
   // Para que no termine en 99 tengo este caso especial.
   int value = SURI_ROUND(int, Threshold);
   if (Threshold < 100) {
      value = SURI_ROUND(int, log10(Threshold+1)*50);
   }

   if (value >= 0 && value <= 100) {
      pslider->SetValue(SURI_TRUNC(int, value));
      GET_CONTROL(*pToolWindow_, wxT("ID_SELECTED_THRESHOLD_TEXT"), wxTextCtrl)
         ->ChangeValue(wxString::Format("%i", value).c_str());
   }
}

/** Retorna el valor del slider */
double MaxLikelihoodPart::GetThresholdCtrlValue() {
   wxSlider *pslider = XRCCTRL(*pToolWindow_, wxT("ID_THRESHOLD"), wxSlider);
   // Para que no termine en 99 tengo este caso especial.
   if (pslider->GetValue() == 100) {
      return 100;
   }
   // Modifico el slider para que el desplazamiento sea logaritmico
   double value = ((double) pslider->GetValue()) / 50;
   return pow(10, value) - 1;
}

/**
 * modifica el control que informa la posicion
 * @param[in] Event evento scroll sobre el slider para seleccionar el umbral
 */
void MaxLikelihoodPart::OnScroll(wxScrollEvent& Event) {
   // Para asignarle el label a la barra se usa escala lineal y no logaritmica
   std::string percentasstring = wxString::Format("%i", Event.GetPosition()).c_str();

   GET_CONTROL(*pToolWindow_, wxT("ID_SELECTED_THRESHOLD_TEXT"), wxTextCtrl)
                                       ->ChangeValue(percentasstring.c_str());
}

}  // namespace

