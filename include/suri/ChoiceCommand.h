/*! \file ChoiceCommand.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CHOICECOMMAND_H_
#define CHOICECOMMAND_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "Choice.h"
#include "Command.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 *  Clase que representa un commando asicoado a un control grafico de tipo choice.
 *  Contiene un command, el cual configura con la seleccion del choice.
 */
class ChoiceCommand : public suri::Choice {
public:
   /** Constructor **/
   explicit ChoiceCommand(Command* pCommand);
   /** Destructor **/
   virtual ~ChoiceCommand();
   /** clave para la configuracion de opciones del choice **/
   static const std::string OptionInputParameterKey;
   /** clave que indica el valor seleccionado al iniciar el control grafico **/
   static const std::string InitialSelectionParameterKey;
   /** clave que indica la clave asociada al parametro label **/
   static const std::string LabelParameterKey;
   /** clave que indica la seleccion actual del choice **/
   static const std::string SelectionParameterKey;

protected:
   /** Metodo que se llama cuando cambia la seleccion en el control grafico.  */
   virtual void OnSelectionChanged(wxCommandEvent &Event);
   /** Metodo que se llama cuando se ejecuta CreateToolWindow **/
   virtual void OnCreateToolWindow();

private:
   /** Comando asociado al choice*/
   Command* pCommand_;
};

} /** namespace suri */

#endif /* CHOICECOMMAND_H_ */
