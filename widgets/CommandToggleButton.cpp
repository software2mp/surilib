/*! \file CommandToggleButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/CommandToggleButton.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

CommandToggleButton::CommandToggleButton(Command *pCommand) :
      ToggleButton(NULL, pCommand->GetDecoration(), pCommand->GetDescription(),
                   BUTTON_NULL),
      pCommand_(pCommand) {
}

CommandToggleButton::~CommandToggleButton() {
}

/*!
 *  Setea la propiedad enabled_
 */
void CommandToggleButton::SetEnabled(bool Enabled) {
   pCommand_->SetEnabled(Enabled);
}
/** Verifica si el boton se puede o no habilitar */
bool CommandToggleButton::IsEnabled() {
   return pCommand_->IsEnabled();
}

/**
 * Evento que habilita el estado del control wx del boton en base al
 * retorno del metodo IsEnabled.
 * @param[in] Event evento UIUpdate
 */
void CommandToggleButton::OnUIUpdate(wxUpdateUIEvent &Event) {
   Button::OnUIUpdate(Event);
   ToggleButton::DoSetValue(pCommand_->IsActive());
}

bool CommandToggleButton::GetValue() const {
   return pCommand_->IsActive();
}

/**
 * Evento click en boton
 * @param[in] Event evento generado por wx.
 */
void CommandToggleButton::OnButtonClicked(wxCommandEvent &Event) {
   SetValue(!GetValue());
}

// ! Evento asignar valor
bool CommandToggleButton::DoSetValue(bool State) {
   ToggleButton::DoSetValue(State);
   if (State != pCommand_->IsActive()) {
      pCommand_->SetActive(State);
      return pCommand_->Execute();
   }
   return true;
}

} /* namespace suri */
