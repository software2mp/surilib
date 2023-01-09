/*! \file ExportVectorLayerSelectionCommandExecutionHandler.h */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */


#ifndef __EXPORT_VECTOR_LAYER_SELECTION_COMMAND_EXEC_HANDLER_H_
#define __EXPORT_VECTOR_LAYER_SELECTION_COMMAND_EXEC_HANDLER_H_

// Includes Suri
#include "suri/CommandExecutionHandlerInterface.h"

namespace suri {

// forwards
class DataViewManager;

namespace core {


/** Clase encargada de administrar la ejecucion del comando ExportVectorLayerSelection **/
class ExportVectorLayerSelectionCommandExecutionHandler : public CommandExecutionHandlerInterface {

	/** constructor copia **/
	ExportVectorLayerSelectionCommandExecutionHandler(const ExportVectorLayerSelectionCommandExecutionHandler&);
	
public:
	/** Constructor **/
	ExportVectorLayerSelectionCommandExecutionHandler(DataViewManager* pDataView);
	/** Destructor **/
	virtual ~ExportVectorLayerSelectionCommandExecutionHandler();
	/**
	 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
	 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
    * @return true en caso de poder manejar la ejecucion del comando
    * @return false en caso contrario
    */
	virtual bool Execute(const Command* pCommand, GenericTool* pTool);

private:
	DataViewManager* pDataView_;
};

}  /** namespace core **/
}  /** namespace suri **/
#endif
