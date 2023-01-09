/*! \file BandMathCommandExecutionHandler.cpp */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */

// Include estandard
#include <string>

// Includes Suri
#include "suri/BandMathCommandExecutionHandler.h"
#include "suri/BandMathProcess.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "suri/IndexProcess.h"

using namespace suri;
using namespace suri::core;


/** Constructor **/
BandMathProcessCommandExecutionHandler::BandMathProcessCommandExecutionHandler(DataViewManager* pDataView) 
									: pDataView_(pDataView) {
}

/** Destructor **/
BandMathProcessCommandExecutionHandler::~BandMathProcessCommandExecutionHandler() {
}
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool BandMathProcessCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("BandMath"))
		return false;
	RasterElement* praster = NULL;
   Subset viewerwindow;
	LoadInputProcessParameters(pDataView_, praster, viewerwindow);
	if(!praster){
	   SHOW_WARNING(message_PROCESS_START_ERROR);
	   return false;
	}
	if (praster != NULL) {
	   BandMathProcess process(praster, viewerwindow, pDataView_);
		RunProcess(pDataView_, caption_BAND_MATH_PART, &process);
	}

	return true;
}

/** Constructor **/
IndexProcessCommandExecutionHandler::IndexProcessCommandExecutionHandler(DataViewManager* pDataView) 
									: pDataView_(pDataView) {
}

/** Destructor **/
IndexProcessCommandExecutionHandler::~IndexProcessCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool IndexProcessCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("Indexes"))
		return false;
	RasterElement* praster = NULL;
   Subset viewerwindow;
	LoadInputProcessParameters(pDataView_, praster, viewerwindow);
	if (!praster){
	   SHOW_WARNING(message_PROCESS_START_ERROR);
	   return false;
	}
	if (praster != NULL) {
	   IndexProcess process(praster, viewerwindow, pDataView_);
		RunProcess(pDataView_, caption_INDEX_EXPORTER_TOOL, &process);
	}
	return true;
}
