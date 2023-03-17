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

// Includes Estandard
#include <string>
#include <vector>

// Includes Suri
#include "suri/LayerVisualizationCommandExecutionHandler.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "VectorDatasource.h"
#include "suri/ToolSupport.h"
#include "suri/DataViewManager.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/SuriObject.h"

namespace suri {

namespace core {

/** Constructor **/
LayerVisualizationCommandExecutionHandler::LayerVisualizationCommandExecutionHandler(
										DataViewManager* pDataView) : pDataView_(pDataView) {
}

/** Destructor **/
LayerVisualizationCommandExecutionHandler::~LayerVisualizationCommandExecutionHandler() {
}
 

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool LayerVisualizationCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	int id = pCommand->GetId();
	if (id != pTool->GetId("DisplayLayers") && id != pTool->GetId("HideLayers")) {  
		return false;
	}
	bool display = (id == pTool->GetId("DisplayLayers")) || id == pTool->GetId("DisplayLayersInAllViewcontexts");
	bool allviewcontext = (id == pTool->GetId("DisplayLayersInAllViewcontexts") || 
																	id == pTool->GetId("HideLayersInAllViewcontexts"));
	ViewcontextManagerInterface* pvm = pDataView_->GetViewcontextManager();
	if (!allviewcontext)
		pvm->ActivateSelectedLayersInViewcontext(pvm->GetSelectedViewcontext(), display);
	else 
		pvm->ActivateSelectedLayersInAllViewcontexts(display);
	return true;
}

}  /** namespace core **/
}  /** namespace suri **/
