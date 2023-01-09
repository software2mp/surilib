/*! \file ToggleButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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

