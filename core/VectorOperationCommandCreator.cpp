/*! \file VectorOperationCommandCreator.cpp */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

// Includes Suri
#include "suri/VectorOperationCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {

/** constructor **/
VectorOperationCommandCreator::VectorOperationCommandCreator() {
}

/** destructor **/
VectorOperationCommandCreator::~VectorOperationCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* VectorOperationCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command* pcommand = NULL;
	if (CommandName == "ExportVectorOperationLayer") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetDescription(caption_BUFFER_CALCULATION);
		pcommand->SetLocation("ID_EXPORT_VECTOR_OPERATION_LAYER_MENUTIEM");
	}
	return pcommand;
}

}	/** namespace core */
}  /** namespace suri */
