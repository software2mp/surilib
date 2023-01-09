/*! \file StatisticsToolCommandCreator.cpp */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

// Includes Suri
#include "suri/StatisticsToolCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {

/** constructor **/
GlobalStatisticsCommandCreator::GlobalStatisticsCommandCreator() {
}

/** destructor **/
GlobalStatisticsCommandCreator::~GlobalStatisticsCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* GlobalStatisticsCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command *pcommand = NULL;
	if (CommandName == "GlobalStatistics") {     
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");    
		pcommand->SetDescription(tooltip_STATISTICS_EXPORTER_BUTTON); 
		pcommand->SetDecoration(tool_STATISTICS_EXPORTER);
		pcommand->SetLocation("ID_GLOBAL_STATISTICS_MENUTIEM"); 
	}    
	return pcommand;
}

/** constructor **/
ClassStatisticsCommandCreator::ClassStatisticsCommandCreator() {
}

/** destructor **/
ClassStatisticsCommandCreator::~ClassStatisticsCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* ClassStatisticsCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command *pcommand = NULL;
	if (CommandName == "ClassStatistics") {     
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");    
		pcommand->SetDescription(tooltip_CLASS_STATISTICS_EXPORTER_BUTTON); 
		pcommand->SetDecoration(tool_STATISTICS_EXPORTER);
		pcommand->SetLocation("ID_CLASS_STATISTICS_MENUTIEM"); 
	}    
	return pcommand;
}

/** constructor **/
ClassStatisticsExporterCommandCreator::ClassStatisticsExporterCommandCreator() {
}

/** destructor **/
ClassStatisticsExporterCommandCreator::~ClassStatisticsExporterCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* ClassStatisticsExporterCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command *pcommand = NULL;
	if (CommandName == "ClassStatisticsExporter") {     
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");    
		pcommand->SetDescription(tooltip_CLASS_STATISTICS_EXPORTER_BUTTON); 
		pcommand->SetDecoration(tool_STATISTICS_EXPORTER);
		pcommand->SetLocation("ID_CLASS_STATISTICS_EXPORTER_MENUTIEM"); 
	}    
	return pcommand;
}
}	/** namespace core */
}  /** namespace suri */
