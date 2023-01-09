/*! \file StatisticsToolCommandExecutionHandler.h */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */


#ifndef __STATISTICS_TOOL_COMMAND_EXEC_HANDLER_H__
#define __STATISTICS_TOOL_COMMAND_EXEC_HANDLER_H__

// Includes Suri
#include "suri/CommandExecutionHandlerInterface.h"
#include "suri/DataViewManager.h"

namespace suri {

namespace core {


/** Clase encargada de administrar la ejecucion de los comandos asociados al 
 * calculo de estadisticas globales raster 
**/
class GlobalStatisticsCommandExecutionHandler : public CommandExecutionHandlerInterface {

	/** constructor copia **/
	GlobalStatisticsCommandExecutionHandler(const GlobalStatisticsCommandExecutionHandler&);

public:
	/** Constructor **/
	GlobalStatisticsCommandExecutionHandler(DataViewManager* pDataView);
	/** Destructor **/
	virtual ~GlobalStatisticsCommandExecutionHandler();
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

/** Clase encargada de administrar la ejecucion de los comandos asociados al 
 * calculo de estadisticas clasificadas raster 
**/
class ClassStatisticsCommandExecutionHandler : public CommandExecutionHandlerInterface {

	/** constructor copia **/
	ClassStatisticsCommandExecutionHandler(const ClassStatisticsCommandExecutionHandler&);

public:
	/** Constructor **/
	ClassStatisticsCommandExecutionHandler(DataViewManager* pDataView);
	/** Destructor **/
	virtual ~ClassStatisticsCommandExecutionHandler();
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

/** Clase encargada de administrar la ejecucion del comando responsable de la exportacion
 *  de estadisticas clasificadas raster 
**/
class ClassStatisticsExporterCommandExecutionHandler : public CommandExecutionHandlerInterface {

	/** constructor copia **/
	ClassStatisticsExporterCommandExecutionHandler(const ClassStatisticsExporterCommandExecutionHandler&);

public:
	/** Constructor **/
	ClassStatisticsExporterCommandExecutionHandler(DataViewManager* pDataView);
	/** Destructor **/
	virtual ~ClassStatisticsExporterCommandExecutionHandler();
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
