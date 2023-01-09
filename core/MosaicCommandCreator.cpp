/*! \file MosaicCommandCreator.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Suri
#include "suri/MosaicCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"

namespace suri {

namespace core {

/** Constructor */
MosaicCommandCreator::MosaicCommandCreator() {
}

/** Destructor */
MosaicCommandCreator::~MosaicCommandCreator() {
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* MosaicCommandCreator::DoCreateCommand(
      const std::string& CommandName, ToolInterface* pTool) {
   Command* pcommand = NULL;
   if (CommandName == "Mosaic") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Menu");
      pcommand->SetDescription(caption_MOSAIC);
      pcommand->SetLocation("ID_RASTER_MOSAIC_MENUTIEM");
   }
   return pcommand;
}

}  /** namespace core */
} /** namespace suri */
