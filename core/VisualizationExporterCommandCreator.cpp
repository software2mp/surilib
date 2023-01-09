/*! \file VisualizationExporterCommandCreator.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Suri
#include "suri/VisualizationExporterCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"

namespace suri {

namespace core {

/** Constructor */
VisualizationExporterCommandCreator::VisualizationExporterCommandCreator() {
}

/** Destructor */
VisualizationExporterCommandCreator::~VisualizationExporterCommandCreator() {
}

/**
 * Creacion de un comando a partir de su nombre (siempre y cuando el nombre
 * del comando sea del dominio de la clase.
 **/
Command* VisualizationExporterCommandCreator::DoCreateCommand(
      const std::string& CommandName, ToolInterface* pTool) {
   Command* pcommand = NULL;
   if (CommandName == "VisualizationExporterProcess") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Menu");
      pcommand->SetDescription(caption_VISUALIZATION_EXPORT);
      pcommand->SetLocation("ID_EXPORT_VISUALIZATION_MENUTIEM");
   }
   return pcommand;
}

}  /** namespace core */
} /** namespace suri */
