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
#include "suri/BandMathToolBuilder.h"
#include "suri/BandMathCommandCreator.h"
#include "suri/BandMathCommandExecutionHandler.h"
#include "suri/FilterProcessCommandExecutionHandler.h"
#include "suri/GenericTool.h"

namespace suri {

namespace core {

/** Constructor **/
BandMathToolBuilder::BandMathToolBuilder() {
}

/** Destructor **/
BandMathToolBuilder::~BandMathToolBuilder() {
}

/** Metodo que crea y configura una herramienta con los hanlders para la ejecucion
 * (CommandExecutionHandlers)
 * y las acciones correspondientes (Commands). **/
ToolInterface* BandMathToolBuilder::BuildTool(DataViewManager* pDataViewManager) {
	GenericTool* ptool = new GenericTool(pDataViewManager);
	/** Creadores de commands **/
	ptool->AddCommandCreator(new BandMathCommandCreator);
	/** Administradores de ejecucion */
	ptool->AddCommandExecutionHandler(new BandMathProcessCommandExecutionHandler(pDataViewManager));
	ptool->AddCommandExecutionHandler(new IndexProcessCommandExecutionHandler(pDataViewManager));
	ptool->AddCommandExecutionHandler(new FilterProcessCommandExecutionHandler(pDataViewManager));
	ptool->AddCommandExecutionHandler(new FilterLibraryCommandExecutionHandler(pDataViewManager));
	ptool->SetConfigurationVariableName("bandmath");
	ptool->Initialize();
	return ptool;
}

}  /** namespace core **/
}  /** namespace suri **/
