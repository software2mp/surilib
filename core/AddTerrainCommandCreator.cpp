/*! \file AddTerrainCommandCreator.h */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

// Includes Suri
#include "suri/AddTerrainCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {

/** constructor **/
AddTerrainCommandCreator::AddTerrainCommandCreator() {
}

/** destructor **/
AddTerrainCommandCreator::~AddTerrainCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* AddTerrainCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command* pcommand = NULL;
	if (CommandName == "AddTerrainLayer") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetDescription(tooltip_LAYER_ADD_TERRAIN_BUTTON);
		pcommand->SetLocation("ID_ADD_TERRAIN_LAYER_MENUTIEM");
	} 
	return pcommand;
}

}	/** namespace core */
}  /** namespace suri */
