/*! \file ExportBufferCommandCreator.cpp */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

// Includes Suri
#include "suri/ExportBufferCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {

/** constructor **/
ExportBufferCommandCreator::ExportBufferCommandCreator() {
}

/** destructor **/
ExportBufferCommandCreator::~ExportBufferCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* ExportBufferCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command* pcommand = NULL;
	if (CommandName == "ExportBufferVectorLayer") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetDescription(caption_BUFFER_CALCULATION);
		pcommand->SetDecoration(tool_LAYER_EXPORT);
		pcommand->SetLocation("ID_EXPORT_BUFFER_VECTOR_LAYER_MENUTIEM");
	}
	return pcommand;
}

}	/** namespace core */
}  /** namespace suri */
