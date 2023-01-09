/*! \file LibraryToolBuilder.h */                                                                          
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
 */    

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
