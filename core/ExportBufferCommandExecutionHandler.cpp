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
#include "suri/ExportBufferCommandExecutionHandler.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "VectorDatasource.h"
#include "suri/ToolSupport.h"
#include "suri/DataViewManager.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/SuriObject.h"
#include "suri/ProcessNotebookWidget.h"
#include "suri/Subset.h"
#include "suri/ProcessInterface.h"
#include "SpatialReference.h"
#include "suri/VectorOperationProcess.h"

namespace suri {

namespace core {

/** Constructor **/
ExportBufferCommandExecutionHandler::ExportBufferCommandExecutionHandler(
										DataViewManager* pDataView) : pDataView_(pDataView) {
}

/** Destructor **/
ExportBufferCommandExecutionHandler::~ExportBufferCommandExecutionHandler() {
}
 
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool ExportBufferCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("ExportBufferVectorLayer"))
		return false;
	ViewcontextManagerInterface* pvm = pDataView_->GetViewcontextManager();
	VectorDatasource* pvector = ToolSupport::GetVectorDatasource(
         pDataView_->GetDatasourceManager(), pvm);
  RasterElement* praster = ToolSupport::GetActiveRaster(pvm, pvm->GetSelectedViewcontextId());
	if (!praster
         || SpatialReference::IsPixelLineSpatialRef(
               praster->GetSpatialReference().c_str())) {
      SHOW_WARNING(message_CANT_START_PROCESS_WITHOUT_SR);
      return true;
   }
	World* pworld = ToolSupport::GetWorld(pDataView_->GetViewportManager());
   Element* pelement = NULL;
   if (pvector) {
      bool showspatial = false;
      bool showbuffer = true;
      Subset viewerwindow;
      if (!pworld) {
         pworld = new World();
         pworld->SetSpatialReference(pvector->GetSpatialReference());
         Subset extent;
         pvector->GetElement()->GetElementExtent(extent);
         pworld->SetWorld(extent);
         pworld->SetWindow(extent);
      }
      pworld->GetWorld(viewerwindow);
      std::vector<DatasourceInterface*> datasources;
      VectorDatasource* pvector = ToolSupport::GetVectorDatasource(
            pDataView_->GetDatasourceManager(),
            pDataView_->GetViewcontextManager());

      datasources.push_back(pvector);
      VectorOperationProcess process(datasources, viewerwindow, pelement,
                                               pDataView_);
      ProcessAdaptLayer* pal = process.GetProcessAdaptLayer();
      pal->SetProcessName("VectorBufferProcess");
      pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeBufferKeyAttr,
                                            showbuffer);
      pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr,
                                   showspatial);
      pal->AddAttribute<std::vector<DatasourceInterface*> >(
                       ProcessAdaptLayer::VectorOperationDatasourcesKeyAttr, datasources);
      process.OnProcessAdaptLayerSet();
      suri::ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
            &process, caption_BUFFER_CALCULATION, pDataView_->GetLibraryManager());
      if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
         DatasourceInterface* pdatasource = NULL;
	        const char *pdatasourcetypes[] = { "VectorDatasource" };
         for (int i = 0; i < 1 && pdatasource == NULL; ++i)
            pdatasource = DatasourceInterface::Create(pdatasourcetypes[i], pelement);
         if (pdatasource)
            pDataView_->GetDatasourceManager()->AddDatasource(pdatasource);
      }
   } else {
      SHOW_WARNING(message_PROCESS_EXPORT_VECTOR_LAYER_START_ERROR);
   }
	return true;
}

}  /** namespace core **/
}  /** namespace suri **/
