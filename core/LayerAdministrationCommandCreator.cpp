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

// Includes Suri
#include "suri/LayerAdministrationCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {
/** constructor **/
LayerAdministrationCommandCreator::LayerAdministrationCommandCreator() {
}

/** destrcutor **/
LayerAdministrationCommandCreator::~LayerAdministrationCommandCreator() {
}
/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* LayerAdministrationCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command* pcommand = NULL;
	if (CommandName == "AddLayer") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu|Button");
		pcommand->SetDescription(tooltip_LAYER_ADD_BUTTON);
		pcommand->SetDecoration(tool_LAYER_ADD);
		pcommand->SetLocation("ID_ADD_LAYER_MENUTIEM|ID_CONATINERLEFT");
	} else if (CommandName == "AddRemoteLayer") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetDescription(tooltip_LAYER_ADD_REMOTE_BUTTON);
		pcommand->SetDecoration(tool_LAYER_ADD_REMOTE);
		pcommand->SetLocation("ID_ADD_REMOTE_LAYER_MENUTIEM");
	} else if (CommandName == "RemoveLayer") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu|Button");
		pcommand->SetDescription(tooltip_LAYER_REMOVE_BUTTON);
		pcommand->SetDecoration(tool_LAYER_REMOVE);
		pcommand->SetLocation("ID_REMOVE_LAYER_MENUTIEM|ID_CONATINERLEFT");
	} else if (CommandName == "ShowLayerProperties") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetDescription(tooltip_LAYER_SHOW_PROPERTIES_BUTTON);
		pcommand->SetDecoration(tool_LAYER_SHOW_PROPERTIES);
		pcommand->SetLocation("ID_LAYER_PROPERTIES_MENUTIEM");
		pcommand->SetType("Menu");
	}

	return pcommand;
}


}  /** namespace core **/
}  /** namespace suri **/

