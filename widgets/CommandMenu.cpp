/*! \file CommandMenu.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/CommandMenu.h"

// Includes Estandar
// Includes Suri
// Includes Wx
#include "wx/menu.h"

// Defines
// forwards

namespace suri {

CommandMenu::CommandMenu(Command *pCommand, wxMenuItem *pMenuItem) :
      pCommand_(pCommand), pMenuItem_(pMenuItem) {
}

CommandMenu::~CommandMenu() {

}

void CommandMenu::Execute(wxCommandEvent&Event) {
   pCommand_->SetActive(!pCommand_->IsActive());
   pCommand_->Execute();
}

CommandEventInterface::CommandEventFlags CommandMenu::GetCommandFlags() const {
   return CommandEventInterface::MenuEvent;
}

void CommandMenu::CommandEnabled(bool Enable) {
   pCommand_->SetEnabled(Enable);
}

bool CommandMenu::IsCommandEnabled() const {
   return pCommand_->IsEnabled();
}

int CommandMenu::GetCommandId() const {
   return pMenuItem_->GetId();
}

bool CommandMenu::CanProcessEvent(wxCommandEvent &Event) const {
   return true;
}

} /* namespace suri */
