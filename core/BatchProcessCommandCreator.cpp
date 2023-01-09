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
#include "suri/BatchProcessCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {

/** constructor **/
BatchProcessCommandCreator::BatchProcessCommandCreator() {
}

/** destructor **/
BatchProcessCommandCreator::~BatchProcessCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* BatchProcessCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command* pcommand = NULL;
	if (CommandName == "BatchProcess") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Button");
		pcommand->SetDescription("Procesamiento a traves de XML");
      pcommand->SetLocation("ID_CONATINERLEFT");
	}
	return pcommand;
}

}	/** namespace core */
}  /** namespace suri */
