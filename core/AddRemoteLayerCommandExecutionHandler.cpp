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

// Includes estandard
#include <string>

// Includes Suri
#include "suri/AddRemoteLayerCommandExecutionHandler.h"
#include "suri/GenericTool.h"
#include "suri/Command.h"
#include "WxsLayerImporter.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DatasourceInterface.h"

namespace suri {

namespace core {

/** Constructor **/
AddRemoteLayerCommandExecutionHandler::AddRemoteLayerCommandExecutionHandler(DataViewManager* pDataView) 
														: pDataView_(pDataView) {
}

/** Destructor **/
AddRemoteLayerCommandExecutionHandler::~AddRemoteLayerCommandExecutionHandler() {
}
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool AddRemoteLayerCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("AddRemoteLayer"))	
		return false;	
	std::string factorystring;
	Option metadata;
	WxsLayerImporter* pimportwidget = new WxsLayerImporter(pDataView_, factorystring, metadata);
	if (pimportwidget->CreateTool() && pimportwidget->ShowModal() == wxID_OK) {
		// se comenta hasta que este terminada la implementacion
		suri::DatasourceManagerInterface* dm = pDataView_->GetDatasourceManager();
		const char* pdatasourcetypes[] = { "RasterDatasource", "VectorDatasource" };
		DatasourceInterface* pdatasource = NULL;
		for (int i = 0; i < 2 && pdatasource == NULL; ++i)
			pdatasource = suri::DatasourceInterface::Create(pdatasourcetypes[i], factorystring, metadata);
		if (pdatasource != NULL && !dm->AddDatasource(pdatasource))
			SHOW_ERROR(message_ADD_ELEMENT_ERROR);                                    
	}
	return true;
}


}  /** namespace core **/
}  /** namespace suri **/
