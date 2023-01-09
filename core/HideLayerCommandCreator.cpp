/*! \file AddCsv:CommandCreator.h */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

// Includes Suri
#include "suri/HideLayerCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {

/** constructor **/
HideLayerCommandCreator::HideLayerCommandCreator() {
}

/** destructor **/
HideLayerCommandCreator::~HideLayerCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* HideLayerCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command* pcommand = NULL;
	if (CommandName == "HideLayers") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetDescription(tooltip_LAYER_HIDE_BUTTON);
		pcommand->SetDecoration(tool_LAYER_HIDE);
		pcommand->SetLocation("ID_HIDE_LAYER_MENUTIEM");
	} else if (CommandName == "HideLayersInAllViewcontexts") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetDescription(tooltip_LAYER_HIDE_ALL_BUTTON);
		pcommand->SetLocation("ID_HIDE_LAYER_ALL_MENUTIEM");
	}
	return pcommand;
}

}	/** namespace core */
}  /** namespace suri */
