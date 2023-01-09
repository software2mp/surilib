/*! \file Slider.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
