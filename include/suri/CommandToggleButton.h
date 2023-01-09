/*! \file CommandToggleButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COMMANDTOGGLEBUTTON_H_
#define COMMANDTOGGLEBUTTON_H_

// Includes Estandar
// Includes Suri
#include "ToggleButton.h"
#include "suri/Command.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 *
 */
class CommandToggleButton : public ToggleButton {
public:
   CommandToggleButton(Command *pCommand);
   virtual ~CommandToggleButton();

   /** Setea la propiedad enabled_ */
   virtual void SetEnabled(bool Enabled = true);
   /** Verifica si el boton se puede o no habilitar */
   virtual bool IsEnabled();
   /** Evento que habilita el boton segun el retorno de IsEnabled */
   virtual void OnUIUpdate(wxUpdateUIEvent &Event);
   /** Obtiene el valor del boton */
   virtual bool GetValue() const;
protected:
   /** Evento click en boton */
   virtual void OnButtonClicked(wxCommandEvent &Event);
   /** Evento asignar valor */
   virtual bool DoSetValue(bool State);
private:
   Command *pCommand_;
};

} /* namespace suri */

#endif /* COMMANDTOGGLEBUTTON_H_ */
