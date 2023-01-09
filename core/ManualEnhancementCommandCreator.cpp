/*! \file ManualEnhancementCommandCreator.h */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

// Includes Suri
#include "suri/ManualEnhancementCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {

/** constructor **/
ManualEnhancementCommandCreator::ManualEnhancementCommandCreator() {
}

/** destructor **/
ManualEnhancementCommandCreator::~ManualEnhancementCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* ManualEnhancementCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command* pcommand = NULL;
	if (CommandName == "ManualEnhancement") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetLocation("ID_MANUAL_ENHANCEMENT_MENUITEM");
	} 
	return pcommand;
}

}	/** namespace core */
}  /** namespace suri */
