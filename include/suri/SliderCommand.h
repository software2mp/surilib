/*! \file SliderCommand.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SLIDERCOMMAND_H_
#define SLIDERCOMMAND_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "Slider.h"
#include "Command.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que representa un commando asicoado a un control grafico de tipo slider.
 * Contiene un command, el cual configura con el valor actual del slider.
 */
class SliderCommand : public suri::Slider {
public:
   /** Constructor */
   explicit SliderCommand(Command* pCommand);
   /** Destructor **/
   virtual ~SliderCommand();
   /** clave para la configuracion del valor minimo del slider **/
   static const std::string MaxValueInputParameterKey;
   /** clave para la configuracion del valro maximo del slider **/
   static const std::string MinValueInputParameterKey;
   /** clave para la configuracion del valro maximo del slider **/
   static const std::string ValueInputParameterKey;
   /** clave que indica la clave asociada al parametro label **/
   static const std::string LabelParameterKey;
   /** clave que indica la seleccion actual del choice **/
   static const std::string SelectionParameterKey;

protected:
   /** Metodo que se llama cuando se ejecuta CreateToolWindow **/
   virtual void OnCreateToolWindow();
   /** Metodo que se llama cuando cambia el valor actual en el control grafico.  */
   virtual void OnValueChanged(wxScrollEvent &Event);

private:
   /** comando asociado al slider **/
   Command* pCommand_;
};

} /** namespace suri */

#endif /* SLIDERCOMMAND_H_ */
