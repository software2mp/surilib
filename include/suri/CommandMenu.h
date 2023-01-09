/*! \file CommandMenu.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COMMANDMENU_H_
#define COMMANDMENU_H_

// Includes Estandar
// Includes Suri
#include "suri/CommandEventInterface.h"
#include "suri/Command.h"

// Includes Wx
// Defines
// forwards
class wxMenuItem;

namespace suri {

/*!
 *
 */
class CommandMenu : public CommandEventInterface {
public:
   CommandMenu(Command *pCommand, wxMenuItem *pMenuItem);
   virtual ~CommandMenu();

   virtual void Execute(wxCommandEvent&Event);

   virtual CommandEventFlags GetCommandFlags() const;

   virtual void CommandEnabled(bool Enable = true);
   virtual bool IsCommandEnabled() const;

   virtual bool CanProcessEvent(wxCommandEvent &Event) const;
protected:
   virtual int GetCommandId() const;
private:
   Command *pCommand_;
   wxMenuItem *pMenuItem_;
};

} /* namespace suri */

#endif /* COMMANDMENU_H_ */
