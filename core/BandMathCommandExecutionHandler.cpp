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

// Include estandard
#include <string>

// Includes Suri
#include "suri/BandMathCommandExecutionHandler.h"
#include "suri/BandMathProcess.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "suri/IndexProcess.h"

using namespace suri;
using namespace suri::core;


/** Constructor **/
BandMathProcessCommandExecutionHandler::BandMathProcessCommandExecutionHandler(DataViewManager* pDataView) 
									: pDataView_(pDataView) {
}

/** Destructor **/
BandMathProcessCommandExecutionHandler::~BandMathProcessCommandExecutionHandler() {
}
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool BandMathProcessCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("BandMath"))
		return false;
	RasterElement* praster = NULL;
   Subset viewerwindow;
	LoadInputProcessParameters(pDataView_, praster, viewerwindow);
	if(!praster){
	   SHOW_WARNING(message_PROCESS_START_ERROR);
	   return false;
	}
	if (praster != NULL) {
	   BandMathProcess process(praster, viewerwindow, pDataView_);
		RunProcess(pDataView_, caption_BAND_MATH_PART, &process);
	}

	return true;
}

/** Constructor **/
IndexProcessCommandExecutionHandler::IndexProcessCommandExecutionHandler(DataViewManager* pDataView) 
									: pDataView_(pDataView) {
}

/** Destructor **/
IndexProcessCommandExecutionHandler::~IndexProcessCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool IndexProcessCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("Indexes"))
		return false;
	RasterElement* praster = NULL;
   Subset viewerwindow;
	LoadInputProcessParameters(pDataView_, praster, viewerwindow);
	if (!praster){
	   SHOW_WARNING(message_PROCESS_START_ERROR);
	   return false;
	}
	if (praster != NULL) {
	   IndexProcess process(praster, viewerwindow, pDataView_);
		RunProcess(pDataView_, caption_INDEX_EXPORTER_TOOL, &process);
	}
	return true;
}
