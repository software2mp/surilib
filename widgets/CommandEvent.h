/*! \file CommandEvent.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COMMANDEVENT_H_
#define COMMANDEVENT_H_

// Includes Estandar

// Includes Suri
#include "suri/CommandEventInterface.h"

// Includes Wx
#include <wx/event.h>

// Defines

// forwards

namespace suri {

/*!
 *
 */
class CommandEvent : public wxEvtHandler {
public:
   CommandEvent(CommandEventInterface *pParent);
   virtual ~CommandEvent();

   virtual void OnCommand(wxCommandEvent &Event);

   CommandEventInterface *GetHandler();
protected:
private:
   CommandEventInterface *pParent_;
};

} /* namespace suri */

#endif /* COMMANDEVENT_H_ */
