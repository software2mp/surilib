/*! \file MergeLayersCommandExecutionHandler.h */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */


#ifndef __MERGE_LAYER_COMMAND_EXEC_HANDLER_H__
#define __MERGE_LAYER_COMMAND_EXEC_HANDLER_H__

// Includes Suri
#include "suri/CommandExecutionHandlerInterface.h"
#include "suri/DataViewManager.h"

namespace suri {

namespace core {


/** Clase encargada de administrar la ejecucion del comando MergeLayer **/
class MergeLayersCommandExecutionHandler : public CommandExecutionHandlerInterface {

	/** constructor copia **/
	MergeLayersCommandExecutionHandler(const MergeLayersCommandExecutionHandler&);

public:
	/** Constructor **/
	MergeLayersCommandExecutionHandler(DataViewManager* pDataView);
	/** Destructor **/
	virtual ~MergeLayersCommandExecutionHandler();
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
