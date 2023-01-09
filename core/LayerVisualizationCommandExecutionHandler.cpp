/*! \file LayerVisualizationCommandExecutionHandler.cpp */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */

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
