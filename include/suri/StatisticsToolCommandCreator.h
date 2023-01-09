/*! \file StatisticsToolCommandCreator.h */                                                     
/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.                                                
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.                             
 *                                                                                                  
 *  Este producto contiene software desarrollado por                                                
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)                             
 *                                                                                                  
*/ 

#ifndef __STATISTICS_TOOL_COMMAND_CREATOR_H__
#define __STATISTICS_TOOL_COMMAND_CREATOR_H__

// Includes Estandard
#include <string>

// Includes Suri
#include "suri/CommandCreatorInterface.h"

namespace suri {

// forwards
class Command;
class ToolInterface;

namespace core {


/** Clase que encargada de configurar el Command para 
 *  el calculo de estadisticas globales sobre una capa raster
 **/
class GlobalStatisticsCommandCreator : public CommandCreatorInterface {
	/** ctor copia **/
	GlobalStatisticsCommandCreator(const GlobalStatisticsCommandCreator&);

public:
	/** constructor **/
	GlobalStatisticsCommandCreator();
	/** destructor **/
	~GlobalStatisticsCommandCreator();

private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/** Clase que encargada de configurar el Command para 
 *  el calculo de estadisticas clasificadas sobre una capa raster
 **/
class ClassStatisticsCommandCreator : public CommandCreatorInterface {
	/** ctor copia **/
	ClassStatisticsCommandCreator(const ClassStatisticsCommandCreator&);

public:
	/** constructor **/
	ClassStatisticsCommandCreator();
	/** destructor **/
	~ClassStatisticsCommandCreator();

private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

/** Clase que encargada de configurar el Command para la exportacion de estadisticas 
 *  de una capa raster
 **/
class ClassStatisticsExporterCommandCreator : public CommandCreatorInterface {
	/** ctor copia **/
	ClassStatisticsExporterCommandCreator(const ClassStatisticsExporterCommandCreator&);

public:
	/** constructor **/
	ClassStatisticsExporterCommandCreator();
	/** destructor **/
	~ClassStatisticsExporterCommandCreator();

private:
	/** 
	 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
    * del comando sea del dominio de la clase. 
    **/
	virtual Command* DoCreateCommand(const std::string& CommandName, ToolInterface* pTool);
};

}  /** namespace core **/
}  /** namespace suri **/

#endif
