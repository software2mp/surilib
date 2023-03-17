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

// Includes Estandar
#include <string>

// Includes Suri
#include "Query.h"
#include "resources.h"
#include "Viewer2DProperties.h"
#include "suri/messages.h"
#include "suri/Viewer2D.h"
#include "suri/InfoTool.h"
#include "suri/DataTypes.h"
#include "suri/ToolCommand.h"
#include "suri/EventCommand.h"
#include "suri/PartContainerWidget.h"
#include "MeassureWidget.h"
#include "suri/DynamicViewerWidget.h"
#include "suri/ViewerPropertiesPart.h"

namespace suri {

/**
 * Constructor
 * @param[in]  pDataViewManager: Puntero al DataViewManager
 */
InfoTool::InfoTool(DataViewManager* pDataViewManager) :
      GenericTool(pDataViewManager) {
   configurationVariableName_ = "info";
   Initialize();
}

/**
 * Destructor
 */
InfoTool::~InfoTool() {
}

/**
 * Ejecuta un comando
 */
void InfoTool::Execute(const Command* pToolCommand) {
   int id = pToolCommand->GetId();
   if (id == GetId("HotlinkQuery")) {
      // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   } else if (id == GetId("Query")) {
      // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   } else if (id == GetId("QuickMeassure")) {
      Viewer2D* pviewer =
            dynamic_cast<Viewer2D*>(GetViewportManager()->GetSelectedViewport());
      if (!pviewer) {
         DynamicViewerWidget* pdynamic =
               dynamic_cast<DynamicViewerWidget*>(GetViewportManager()->GetSelectedViewport());
         pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer(DynamicViewerWidget::Raster));
      }
      if (pviewer) {
         if (pToolCommand->IsActive()) {
            pviewer->GetMeasureWidget()->ShowWidget();
         } else {
            pviewer->GetMeasureWidget()->HideWidget();
         }
      }
   } else if (id == GetId("ViewportProperties")) {
      ViewerWidget *pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
      Part *pviewportproperties = NULL;
      if (dynamic_cast<DynamicViewerWidget*>(pviewer))
         pviewportproperties = new ViewerPropertiesPart(
               dynamic_cast<DynamicViewerWidget*>(pviewer),
               GetLibraryManager(), GetDataViewManager());

      if (pviewportproperties) {
         PartContainerWidget *pcontainer = new PartContainerWidget(pviewportproperties,
               _(caption_VIEWPORT_PROPERTIES));
         pcontainer->ShowModal();
      }
   }
}

/**
 * Crea un comando
 */
Command* InfoTool::CreateCommand(const std::string &CommandName) {
   Command* pcommand = NULL;
   if (CommandName == "Query") {
      pQuery_ = new Query(GetViewportManager(), GetViewcontextManager(),
                          GetLibraryManager());
      SetMouseEvent(pQuery_);
      SetPaintEvent(pQuery_);
      pcommand = new EventCommand(pQuery_, pQuery_, SuriObject::NullIntUuid,
                                  "ToggleButton|Menu", tool_QUERY, tooltip_PIXEL_QUERY,
                                  "ID_CONATINERMID|ID_PIXEL_QUERY_MENUTIEM");
   } else if (CommandName == "QuickMeassure") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("ToggleButton|Menu");
      pcommand->SetGroupId(-64);
      pcommand->SetDescription(tooltip_QUICK_MEASUREMENT);
      pcommand->SetDecoration(tool_MEASURE_WIDGET);
      pcommand->SetLocation("ID_CONATINERMID|ID_MEASSURE_MENUTIEM");
   } else if (CommandName == "ViewportProperties") {
      pcommand = new ToolCommand(this);
      pcommand->SetType("Menu");
      pcommand->SetDescription(tooltip_VIEWPORT_LINK);
      pcommand->SetDecoration(icon_VIEWPORT_PROPERTIES);
      pcommand->SetLocation("ID_VIEWER_PROPERTIES_MENUTIEM");
   }
   return pcommand;
}

/**
 * Destruye un comando
 */
void InfoTool::DestroyCommand(Command* &pCommand) const {
   delete pCommand;
   pCommand = NULL;
}

/**
 * Crea el widget
 */
Widget* InfoTool::CreateWidget(const std::string &Widget) {
   // TODO(Sebastian - TCK #878): Deteccion de codigo sin implementar
   return NULL;
}

} /* namespace suri */
