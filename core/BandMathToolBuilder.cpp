/*! \file BandMathToolBuilder.h */                                                                          
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
 */    

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
