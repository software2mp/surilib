/*! \file InfoToolCommandCreator.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Suri
#include "suri/InfoToolCommandCreator.h"
#include "suri/ToolCommand.h"
#include "suri/messages.h"
#include "resources.h"
#include "suri/DataViewManager.h"
#include "suri/EventCommand.h"

namespace suri {
namespace core {

/** Constructor */
InfoToolCommandCreator::InfoToolCommandCreator(DataViewManager* pDataView) :
      pDataView_(pDataView) {
}

/** Destructor */
InfoToolCommandCreator::~InfoToolCommandCreator() {
}

/**
 * Creacion de un comando a partir de su nombre
 * (siempre y cuando el nombre del comando sea del dominio de la clase).
 */
Command* InfoToolCommandCreator::DoCreateCommand(const std::string& CommandName,
                                                 ToolInterface* pTool) {
   Command* pcommand = NULL;
   if (CommandName == "Query") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("ToggleButton|Menu");
      pcommand->SetGroupId(-64);
      pcommand->SetDescription(tooltip_PIXEL_QUERY);
      pcommand->SetDecoration(tool_QUERY);
      pcommand->SetLocation("ID_CONATINERMID|ID_PIXEL_QUERY_MENUTIEM");
   } else if (CommandName == "QuickMeassure") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("ToggleButton|Menu");
      pcommand->SetGroupId(-64);
      pcommand->SetDescription(tooltip_QUICK_MEASUREMENT);
      pcommand->SetDecoration(tool_MEASURE_WIDGET);
      pcommand->SetLocation("ID_CONATINERMID|ID_MEASSURE_MENUTIEM");
   } else if (CommandName == "ViewportProperties") {
      pcommand = new ToolCommand(pTool);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_VIEWPORT_LINK);
      pcommand->SetDecoration(icon_VIEWPORT_PROPERTIES);
      pcommand->SetLocation("ID_VIEWER_PROPERTIES_MENUTIEM");
   }
   return pcommand;
}

} /** namespace core */
} /** namespace suri */
