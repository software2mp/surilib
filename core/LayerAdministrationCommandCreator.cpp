/*! \file LayerAdministrationCommandCreator.h */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

// Includes Suri
#include "suri/LayerAdministrationCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {
/** constructor **/
LayerAdministrationCommandCreator::LayerAdministrationCommandCreator() {
}

/** destrcutor **/
LayerAdministrationCommandCreator::~LayerAdministrationCommandCreator() {
}
/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* LayerAdministrationCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command* pcommand = NULL;
	if (CommandName == "AddLayer") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu|Button");
		pcommand->SetDescription(tooltip_LAYER_ADD_BUTTON);
		pcommand->SetDecoration(tool_LAYER_ADD);
		pcommand->SetLocation("ID_ADD_LAYER_MENUTIEM|ID_CONATINERLEFT");
	} else if (CommandName == "AddRemoteLayer") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetDescription(tooltip_LAYER_ADD_REMOTE_BUTTON);
		pcommand->SetDecoration(tool_LAYER_ADD_REMOTE);
		pcommand->SetLocation("ID_ADD_REMOTE_LAYER_MENUTIEM");
	} else if (CommandName == "RemoveLayer") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu|Button");
		pcommand->SetDescription(tooltip_LAYER_REMOVE_BUTTON);
		pcommand->SetDecoration(tool_LAYER_REMOVE);
		pcommand->SetLocation("ID_REMOVE_LAYER_MENUTIEM|ID_CONATINERLEFT");
	} else if (CommandName == "ShowLayerProperties") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetDescription(tooltip_LAYER_SHOW_PROPERTIES_BUTTON);
		pcommand->SetDecoration(tool_LAYER_SHOW_PROPERTIES);
		pcommand->SetLocation("ID_LAYER_PROPERTIES_MENUTIEM");
		pcommand->SetType("Menu");
	}

	return pcommand;
}


}  /** namespace core **/
}  /** namespace suri **/

