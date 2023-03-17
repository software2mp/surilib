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
#include "suri/LibraryToolBuilder.h"
#include "suri/LibraryToolCommandCreator.h"
#include "suri/LibraryToolCommandExecutionHandler.h"
#include "suri/GenericTool.h"

namespace suri {

namespace core {

/** Constructor **/
LibraryToolBuilder::LibraryToolBuilder() {
}

/** Destructor **/
LibraryToolBuilder::~LibraryToolBuilder() {
}

/** Metodo que crea y configura una herramienta con los hanlders para la ejecucion
 * (CommandExecutionHandlers)
 * y las acciones correspondientes (Commands). **/
ToolInterface* LibraryToolBuilder::BuildTool(DataViewManager* pDataViewManager) {
	GenericTool* ptool = new GenericTool(pDataViewManager);
	/** Creadores de commands **/
	ptool->AddCommandCreator(new SpatialReferenceLibraryCommandCreator);	
	ptool->AddCommandCreator(new DatumLibraryCommandCreator);	
	ptool->AddCommandCreator(new EllipsoidLibraryCommandCreator);	
	ptool->AddCommandCreator(new DensitySliceLibraryCommandCreator);
	ptool->AddCommandCreator(new WmsLibraryCommandCreator);
	ptool->AddCommandCreator(new VectorStyleLibraryCommandCreator);
	ptool->AddCommandCreator(new SpectralSignLibraryCommandCreator);
	ptool->AddCommandCreator(new ConvolutionfiltersLibraryCommandCreator);
	ptool->AddCommandCreator(new EquationLibraryCommandCreator);
	ptool->AddCommandCreator(new IndexLibraryCommandCreator);

	/** Administradores de ejecucion */
	ptool->AddCommandExecutionHandler(new SpatialReferenceLibraryCommandExecutionHandler(pDataViewManager));
	ptool->AddCommandExecutionHandler(new DatumLibraryCommandExecutionHandler(pDataViewManager));
	ptool->AddCommandExecutionHandler(new EllipsoidLibraryCommandExecutionHandler(pDataViewManager));
	ptool->AddCommandExecutionHandler(new DensitySliceLibraryCommandExecutionHandler(pDataViewManager));
	ptool->AddCommandExecutionHandler(new WmsLibraryCommandExecutionHandler(pDataViewManager));
	ptool->AddCommandExecutionHandler(new VectorStyleLibraryCommandExecutionHandler(pDataViewManager));
	ptool->AddCommandExecutionHandler(new SpectralSignLibraryCommandExecutionHandler(pDataViewManager));
	ptool->AddCommandExecutionHandler(new ConvolutionFiltersLibraryCommandExecutionHandler(pDataViewManager));
	ptool->AddCommandExecutionHandler(new EquationLibraryCommandExecutionHandler(pDataViewManager));
	ptool->AddCommandExecutionHandler(new IndexLibraryCommandExecutionHandler(pDataViewManager));

	ptool->SetConfigurationVariableName("library");
	ptool->Initialize();
	return ptool;
}

}  /** namespace core **/
}  /** namespace suri **/
