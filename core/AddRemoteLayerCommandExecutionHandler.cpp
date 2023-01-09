/*! \file AddRemoteLayerCommandExecutionHandler.cpp */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */

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
