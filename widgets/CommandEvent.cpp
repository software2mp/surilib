/*! \file CommandEvent.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "CommandEvent.h"

// Includes Estandar
// Includes Suri
#include "suri/CommandEventInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

CommandEvent::CommandEvent(CommandEventInterface *pParent) :
      pParent_(pParent) {
   if (pParent) {
      CommandEventInterface::CommandEventFlags commandflags =
            pParent_->GetCommandFlags();
      if (commandflags & CommandEventInterface::MenuEvent) {
         Connect(pParent_->GetCommandId(), wxEVT_COMMAND_MENU_SELECTED,
                 wxCommandEventHandler(CommandEvent::OnCommand));
      }
   }
}

CommandEvent::~CommandEvent() {
   Disconnect(pParent_->GetCommandId(), wxEVT_COMMAND_MENU_SELECTED,
              wxCommandEventHandler(CommandEvent::OnCommand));
}

CommandEventInterface *CommandEvent::GetHandler() {
   return pParent_;
}

void CommandEvent::OnCommand(wxCommandEvent &Event) {
   if (!pParent_->IsCommandEnabled()) {
      return;
   }
   pParent_->Execute(Event);
}

} /* namespace suri */
