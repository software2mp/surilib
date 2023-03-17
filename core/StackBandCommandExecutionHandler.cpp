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

// Includes Estandard
#include <string>
#include <vector>

// Includes Suri
#include "suri/StackBandCommandExecutionHandler.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "VectorDatasource.h"
#include "suri/ToolSupport.h"
#include "suri/DataViewManager.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/SuriObject.h"
#include "suri/BandStackingProcess.h"
#include "suri/ProcessNotebookWidget.h"

namespace suri {

namespace core {

/** Constructor **/
StackBandCommandExecutionHandler::StackBandCommandExecutionHandler(
                              DataViewManager* pDataView) : pDataView_(pDataView) {
}

/** Destructor **/
StackBandCommandExecutionHandler::~StackBandCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool StackBandCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   if (pCommand->GetId() != pTool->GetId("StackBands"))
      return false;
   std::vector<RasterElement*> rasters = ToolSupport::GetAllRaster(pDataView_->GetViewcontextManager());
   if (!rasters.empty()) {
      Subset viewerwindow;
      ToolSupport::GetSubset(pDataView_->GetViewportManager(), viewerwindow, rasters[0]);
      std::vector<Element*> elements;
      for (int ix = 0, lenix = rasters.size(); ix < lenix; ++ix) 
         elements.push_back(dynamic_cast<Element*>(rasters[ix]));
      BandStackingProcess process(elements, viewerwindow, pDataView_);
      process.OnProcessAdaptLayerSet();
      ProcessAdaptLayer* pPal = process.GetProcessAdaptLayer();
      suri::ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
            &process, caption_STACK_BANDS, pDataView_->GetLibraryManager());
      if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
         Element* pNewElement = NULL;
         pPal->GetAttribute<Element*>(
                     ProcessAdaptLayer::OutputElementKeyAttr, pNewElement);
         DatasourceInterface* pdatasource = DatasourceInterface::Create("RasterDatasource",
                                                                        pNewElement);
         pDataView_->GetDatasourceManager()->AddDatasource(pdatasource);
      }
   } else {
      SHOW_WARNING(message_PROCESS_EXECUTION_ERROR);
   }

   return true;
}

}  /** namespace core **/
}  /** namespace suri **/
