/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
