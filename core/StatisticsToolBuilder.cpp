/*! \file StatisticsToolBuilder.h */                                                                          
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
 */    

// Includes Suri
#include "suri/StatisticsToolBuilder.h"
#include "suri/GenericTool.h"
#include "suri/StatisticsToolCommandExecutionHandler.h"
#include "suri/StatisticsToolCommandCreator.h"

namespace suri {

namespace core {

/** Constructor **/
StatisticsToolBuilder::StatisticsToolBuilder() {
}

/** Destructor **/
StatisticsToolBuilder::~StatisticsToolBuilder() {
}

/** Metodo que crea y configura una herramienta con los hanlders para la ejecucion
 * (CommandExecutionHandlers)
 * y las acciones correspondientes (Commands). **/
ToolInterface* StatisticsToolBuilder::BuildTool(DataViewManager* pDataViewManager) {
	GenericTool* ptool = new GenericTool(pDataViewManager);
	/** Creadores de commands **/
	ptool->AddCommandCreator(new GlobalStatisticsCommandCreator);
	ptool->AddCommandCreator(new ClassStatisticsCommandCreator);
	ptool->AddCommandCreator(new ClassStatisticsExporterCommandCreator);
/** Administradores de ejecucion */
	ptool->AddCommandExecutionHandler(new GlobalStatisticsCommandExecutionHandler(pDataViewManager));
	ptool->AddCommandExecutionHandler(new ClassStatisticsCommandExecutionHandler(pDataViewManager));
	ptool->AddCommandExecutionHandler(new ClassStatisticsExporterCommandExecutionHandler(pDataViewManager));
	ptool->SetConfigurationVariableName("statistics");
	ptool->Initialize();
	return ptool;
}

}  /** namespace core **/
}  /** namespace suri **/
