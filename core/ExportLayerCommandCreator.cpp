/*! \file ExportLayerCommandCreator.cpp */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

// Includes Suri
#include "suri/ExportLayerCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {

/** constructor **/
ExportLayerCommandCreator::ExportLayerCommandCreator() {
}

/** destructor **/
ExportLayerCommandCreator::~ExportLayerCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* ExportLayerCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command* pcommand = NULL;
	if (CommandName == "ExportLayer") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetDescription(tooltip_LAYER_EXPORT_LAYER_BUTTON);
		pcommand->SetDecoration(tool_LAYER_EXPORT);
		pcommand->SetLocation("ID_EXPORT_LAYER_MENUTIEM");
	} else if (CommandName == "ExportVectorLayer") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetDescription(tooltip_LAYER_EXPORT_LAYER_BUTTON);
		pcommand->SetDecoration(tool_LAYER_EXPORT);
		pcommand->SetLocation("ID_EXPORT_VECTOR_LAYER_MENUTIEM");
	} else if (CommandName == "ExportLayerSelection") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetDescription(tooltip_LAYER_EXPORT_LAYER_BUTTON);
		pcommand->SetLocation("ID_EXPORT_LAYER_SELECTION_MENUTIEM");
	}
	return pcommand;
}

}	/** namespace core */
}  /** namespace suri */
