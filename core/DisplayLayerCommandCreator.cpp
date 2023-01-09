/*! \file DisplayLayerCommandCreator.h */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

// Includes Suri
#include "suri/DisplayLayerCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {

/** constructor **/
DisplayLayerCommandCreator::DisplayLayerCommandCreator() {
}

/** destructor **/
DisplayLayerCommandCreator::~DisplayLayerCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* DisplayLayerCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command* pcommand = NULL;
	if (CommandName == "DisplayLayers") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetDescription(tooltip_LAYER_DISPLAY_BUTTON);
		pcommand->SetDecoration(tool_LAYER_DISPLAY);
		pcommand->SetLocation("ID_SHOW_LAYER_MENUTIEM");
	} else if (CommandName == "DisplayLayersInAllViewcontexts") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetDescription(tooltip_LAYER_DISPLAY_ALL_BUTTON);
		pcommand->SetLocation("ID_SHOW_LAYER_ALL_MENUTIEM");
	}
return pcommand;
}

}	/** namespace core */
}  /** namespace suri */
