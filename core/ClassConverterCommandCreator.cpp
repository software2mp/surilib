/*! \file ClassConverterCommandCreator.h */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

// Includes Suri
#include "suri/ClassConverterCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {

/** constructor **/
ClassConverterCommandCreator::ClassConverterCommandCreator() {
}

/** destructor **/
ClassConverterCommandCreator::~ClassConverterCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* ClassConverterCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command* pcommand = NULL;
	if (CommandName == "TrainClassConverter") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetLocation("ID_CONVERT_AREA_TRAIN_MENUTIEM");
	} else if (CommandName == "TestClassConverter") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetLocation("ID_CONVERT_AREA_TEST_MENUTIEM");
   }
	return pcommand;
}

}	/** namespace core */
}  /** namespace suri */
