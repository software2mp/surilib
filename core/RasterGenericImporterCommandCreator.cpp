/*! \file RasterGenericImporterCommandCreator.cpp */
/** 
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 * 
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 * 
*/

// Includes Suri
#include "suri/RasterGenericImporterCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"

namespace suri {

namespace core {

/** constructor **/
RasterGenericImporterCommandCreator::RasterGenericImporterCommandCreator() {
}

/** destructor **/
RasterGenericImporterCommandCreator::~RasterGenericImporterCommandCreator() {
}

/** 
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre 
 * del comando sea del dominio de la clase. 
 **/
Command* RasterGenericImporterCommandCreator::DoCreateCommand(const std::string& CommandName, 
																			ToolInterface* pTool) {
	Command* pcommand = NULL;
	if (CommandName == "RasterGenericImporter") {
		pcommand = new ToolCommand(pTool);
		pcommand->SetType("Menu");
		pcommand->SetDescription(caption_RASTER_IMPORTER);
		pcommand->SetLocation("ID_RASTER_GENERIC_IMPORTER_MENUTIEM");
	}
	return pcommand;
}

}	/** namespace core */
}  /** namespace suri */
