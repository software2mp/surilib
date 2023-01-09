/*! \file HistogramMatchingCommandCreator.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes Suri
#include "suri/HistogramMatchingCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"

namespace suri {

namespace core {

/** Constructor */
HistogramMatchingCommandCreator::HistogramMatchingCommandCreator() {
}

/** Destructor */
HistogramMatchingCommandCreator::~HistogramMatchingCommandCreator() {
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* HistogramMatchingCommandCreator::DoCreateCommand(
      const std::string& CommandName, ToolInterface* pTool) {
   Command* pcommand = NULL;
   if (CommandName == "HistogramMatching") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Menu");
      pcommand->SetDescription(caption_MOSAIC);
      pcommand->SetLocation("ID_HISTOGRAM_MATCHING_MENUITEM");
   }
   return pcommand;
}

}  /** namespace core */
} /** namespace suri */
