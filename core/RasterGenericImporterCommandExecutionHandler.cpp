/*! \file RasterGenericImporterCommandExecutionHandler.cpp */

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
#include "suri/RasterGenericImporterCommandExecutionHandler.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "VectorDatasource.h"
#include "suri/ToolSupport.h"
#include "suri/DataViewManager.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/SuriObject.h"
#include "RasterGenericImporterWidget.h"

namespace suri {

namespace core {

/** Constructor **/
RasterGenericImporterCommandExecutionHandler::RasterGenericImporterCommandExecutionHandler(
										DataViewManager* pDataView) : pDataView_(pDataView) {
}

/** Destructor **/
RasterGenericImporterCommandExecutionHandler::~RasterGenericImporterCommandExecutionHandler() {
}
 
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool RasterGenericImporterCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("RasterGenericImporter"))
		return false;
	RasterGenericImporterWidget* prwidget = new RasterGenericImporterWidget(pDataView_->GetDatasourceManager());
	if (prwidget->CreateTool() && prwidget->GetFrame()->EnableCloseButton() 
														&& prwidget->ShowModal() == wxID_OK) { 
	}
	return true;
}

}  /** namespace core **/
}  /** namespace suri **/
