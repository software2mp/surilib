/*! \file CommandButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COMMANDBUTTON_H_
#define COMMANDBUTTON_H_

// Includes Estandar

// Includes Suri
#include "Button.h"
#include "suri/Command.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 *
 */
class CommandButton : public suri::Button {
public:
   CommandButton(Command *pCommand);
   virtual ~CommandButton();

   /** Setea la propiedad enabled_ */
   virtual void SetEnabled(bool Enabled = true);
   /** Verifica si el boton se puede o no habilitar */
   virtual bool IsEnabled();
protected:
   void OnButtonClicked(wxCommandEvent &Event);
private:
   suri::Command *pCommand_;
};

} /* namespace suri */

#endif /* COMMANDBUTTON_H_ */
