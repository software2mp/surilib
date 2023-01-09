/*! \file CommandButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "suri/CommandButton.h"

// Includes Wx
// Defines
// forwards

namespace suri {

CommandButton::CommandButton(Command *pCommand) :
      Button(NULL, pCommand->GetDecoration(), pCommand->GetDescription(), BUTTON_NULL),
      pCommand_(pCommand) {
}

CommandButton::~CommandButton() {
}

/** Setea la propiedad enabled_ */
void CommandButton::SetEnabled(bool Enabled) {
   pCommand_->SetEnabled(Enabled);
}
/** Verifica si el boton se puede o no habilitar */
bool CommandButton::IsEnabled() {
   return pCommand_->IsEnabled();
}
void CommandButton::OnButtonClicked(wxCommandEvent &Event) {
   pCommand_->Execute();
   // El boton se destruye al cerrar y volver abrir un
   // proyecto. Cuando wx trata de procesar el evento OnClick,
   // el boton fue destruido y se genera un SIGSEGV,
   // Se agrego esta linea para que los botones abrir proyecto
   // y nuevo proyecto no sigan propagando los eventos.
   Event.Skip(false);
}

} /* namespace suri */
