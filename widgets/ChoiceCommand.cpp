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
#include <vector>

// Includes Suri
#include "suri/ChoiceCommand.h"
#include "suri/ParameterCollection.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** clave para la configuracion de opciones del choice **/
const std::string ChoiceCommand::OptionInputParameterKey = "ChoiceOptions";
/** clave que indica el valor seleccionado al iniciar el control grafico **/
const std::string ChoiceCommand::InitialSelectionParameterKey = "InitialSelection";
/** clave que indica la clave asociada al parametro label **/
const std::string ChoiceCommand::LabelParameterKey = "Label";
/** clave que indica la seleccion actual del choice **/
const std::string ChoiceCommand::SelectionParameterKey = "ChoiceSelection";

/**
 * Constructor
 * @param pCommand command asociado un choice
 */
ChoiceCommand::ChoiceCommand(Command* pCommand) :
      Choice(NULL, pCommand->GetDescription(), 0), pCommand_(pCommand) {
   std::string label;
   ParameterCollection* pparams = pCommand_->GetInputParameters();
   if (pparams && pparams->GetValue<std::string>(LabelParameterKey, label)) {
      label_ = label;
   }
}

/** Destructor **/
ChoiceCommand::~ChoiceCommand() {
   ParameterCollection* pparams = pCommand_->GetInputParameters();
   std::vector < std::string > *poptions = NULL;
   pparams->GetValue<std::vector<std::string>*>(OptionInputParameterKey,
                                                            poptions);
   delete poptions;
}

/** Metodo que se llama cuando cambia la seleccion en el control grafico.  */
void ChoiceCommand::OnSelectionChanged(wxCommandEvent &Event) {
   std::string selection = pChoice_->GetStringSelection().c_str();
   ParameterCollection* pparams = new ParameterCollection;
   pparams->AddValue<std::string>(SelectionParameterKey, selection);
   pCommand_->SetOutputParameters(pparams);
   pCommand_->Execute();
}

/** Metodo que se llama cuando se ejecuta CreateToolWindow **/
void ChoiceCommand::OnCreateToolWindow() {
   ParameterCollection* pparams = pCommand_->GetInputParameters();
   std::vector < std::string > *poptions = NULL;
   if (pparams
         && pparams->GetValue<std::vector<std::string>*>(OptionInputParameterKey,
                                                         poptions) && poptions) {
      std::vector<std::string>::iterator it = poptions->begin();
      for (; it != poptions->end(); ++it) {
         pChoice_->Append((*it).c_str());
      }
      std::string selectvalue;
      if (pparams->GetValue<std::string>(InitialSelectionParameterKey, selectvalue)) {
         pChoice_->SetStringSelection(selectvalue.c_str());
         pparams->AddValue<std::string>(SelectionParameterKey, selectvalue);
      }
   }
}
} /** namespace suri */
