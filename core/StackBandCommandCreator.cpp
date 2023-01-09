/*! \file StackBandCommandCreator.cpp */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

// Includes Suri
#include "suri/StackBandCommandCreator.h"
#include "suri/ToolCommand.h"

namespace suri {

namespace core {

/** constructor **/
StackBandCommandCreator::StackBandCommandCreator() {
}

/** destructor **/
StackBandCommandCreator::~StackBandCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* StackBandCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command* pcommand = NULL;
	if (CommandName == "StackBands") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetLocation("ID_BAND_STACKING_MENUTIEM");
	}
	return pcommand;
}

}	/** namespace core */
}  /** namespace suri */
