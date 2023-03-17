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

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/SliderCommand.h"
#include "suri/ParameterCollection.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** clave para la configuracion del valor minimo del slider **/
const std::string SliderCommand::MaxValueInputParameterKey = "SliderMaxValue";
/** clave para la configuracion del valro maximo del slider **/
const std::string SliderCommand::MinValueInputParameterKey = "SliderMinValue";
/** clave para la configuracion del valro maximo del slider **/
const std::string SliderCommand::ValueInputParameterKey = "InitialValue";
/** clave que indica la clave asociada al parametro label **/
const std::string SliderCommand::LabelParameterKey = "Label";
/** clave que indica la seleccion actual del choice **/
const std::string SliderCommand::SelectionParameterKey = "Selection";

/**
 * Constructor
 * @param pCommand command asociado al control grafico slider
 */
SliderCommand::SliderCommand(Command* pCommand) :
      Slider(NULL, pCommand->GetDescription(), 0),
      pCommand_(pCommand) {
   std::string label;
   ParameterCollection* pparams = pCommand_->GetInputParameters();
   if (pparams && pparams->GetValue<std::string>(LabelParameterKey, label)) {
      label_ = label;
   }
}

/** Destructor **/
SliderCommand::~SliderCommand() {
}

/** Metodo que se llama cuando se ejecuta CreateToolWindow **/
void SliderCommand::OnCreateToolWindow() {
   ParameterCollection* pparams = pCommand_->GetInputParameters();
   int min = -1, max = -1;
   if (pparams && pparams->GetValue<int>(MinValueInputParameterKey, min)
         && pparams->GetValue<int>(MaxValueInputParameterKey, max)) {
      pSlider_->SetRange(min, max);
   }
   int value = -1;
   if (pparams->GetValue<int>(ValueInputParameterKey, value)) {
      pSlider_->SetValue(value);
   }
}

/** Metodo que se llama cuando cambia la seleccion en el control grafico.  */
void SliderCommand::OnValueChanged(wxScrollEvent &Event) {
   int selection = pSlider_->GetValue();
   ParameterCollection* pparams = new ParameterCollection;
   pparams->AddValue<int>(SelectionParameterKey, selection);
   pCommand_->SetOutputParameters(pparams);
   pCommand_->Execute();
}
} /** namespace suri */
