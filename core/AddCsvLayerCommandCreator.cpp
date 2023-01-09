/*! \file AddCsvCommandCreator.cpp */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

// Includes Suri
#include "suri/AddCsvLayerCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {

/** constructor **/
AddCsvLayerCommandCreator::AddCsvLayerCommandCreator() {
}

/** destructor **/
AddCsvLayerCommandCreator::~AddCsvLayerCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* AddCsvLayerCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command* pcommand = NULL;
	if (CommandName == "AddCsvLayer") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetDescription(tooltip_LAYER_ADD_CSV_BUTTON);
		pcommand->SetDecoration(tool_LAYER_ADD);
		pcommand->SetLocation("ID_ADD_CSV_LAYER_MENUTIEM");
	}
	return pcommand;
}

}	/** namespace core */
}  /** namespace suri */
