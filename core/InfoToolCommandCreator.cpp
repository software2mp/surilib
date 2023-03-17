/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
