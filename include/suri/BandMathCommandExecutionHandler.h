/*! \file BandMathCommandExecutionHandler.h */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */


#ifndef __BAND_MATH_COMMAND_EXEC_HANDLER_H__
#define __BAND_MATH_COMMAND_EXEC_HANDLER_H__

// Includes Suri
#include "suri/CommandExecutionHandlerInterface.h"
#include "suri/DataViewManager.h"

namespace suri {

namespace core {


/** Clase encargada de administrar la ejecucion del comando BandMathProcess **/
class BandMathProcessCommandExecutionHandler : public CommandExecutionHandlerInterface {

	/** constructor copia **/
	BandMathProcessCommandExecutionHandler(const BandMathProcessCommandExecutionHandler&);

public:
	/** Constructor **/
	BandMathProcessCommandExecutionHandler(DataViewManager* pDataView);
	/** Destructor **/
	virtual ~BandMathProcessCommandExecutionHandler();
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

/** Clase encargada de administrar la ejecucion del comando Indexes **/
class IndexProcessCommandExecutionHandler : public CommandExecutionHandlerInterface {

	/** constructor copia **/
	IndexProcessCommandExecutionHandler(const IndexProcessCommandExecutionHandler&);

public:
	/** Constructor **/
	IndexProcessCommandExecutionHandler(DataViewManager* pDataView);
	/** Destructor **/
	virtual ~IndexProcessCommandExecutionHandler();
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
