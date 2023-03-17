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

// Include estandard
#include <string>

// Includes Suri
#include "suri/MergeLayersCommandExecutionHandler.h"
#include "suri/DataViewManager.h"
#include "suri/Command.h"
#include "TableImportXyPart.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/GenericTool.h"
#include "suri/VectorOperationProcess.h"
#include "suri/ToolSupport.h"
#include "suri/RasterElement.h"
#include "SpatialReference.h"
#include "suri/ProcessNotebookWidget.h"

namespace suri {

namespace core {

/** Constructor **/
MergeLayersCommandExecutionHandler::MergeLayersCommandExecutionHandler(DataViewManager* pDataView) 
									: pDataView_(pDataView) {
}

/** Destructor **/
MergeLayersCommandExecutionHandler::~MergeLayersCommandExecutionHandler() {
}
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool MergeLayersCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("MergeLayers"))
		return false;
   VectorDatasource* pvector = ToolSupport::GetVectorDatasource(
         pDataView_->GetDatasourceManager(),
         pDataView_->GetViewcontextManager());
   RasterElement* praster = ToolSupport::GetActiveRaster(
         pDataView_->GetViewcontextManager(),
         pDataView_->GetViewcontextManager()->GetSelectedViewcontextId());
   if (!praster
         || SpatialReference::IsPixelLineSpatialRef(
               praster->GetSpatialReference().c_str())) {
      SHOW_WARNING(message_CANT_START_PROCESS_WITHOUT_SR);
      return true;
   }
   World* pworld = ToolSupport::GetWorld(pDataView_->GetViewportManager());
   Element* pelement = NULL;
   bool hasdatasources = false;
   if (pvector) {
      bool showspatial = false;
      bool showvectoroperation = true;
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

      // Agrego las fuentes de datos al vector
      std::list<NodePath> selectednodes =
            pDataView_->GetSelectionManager()->GetSelectedNodes();
      std::list<suri::NodePath>::iterator it = selectednodes.begin();
      for (; it != selectednodes.end(); ++it) {
         DatasourceInterface* pds = pDataView_->GetDatasourceManager()->GetDatasource(
               it->GetLastPathNode()->GetContent());
         if (pds != NULL && pds->GetClassId().compare("VectorDatasource") == 0) {
            datasources.push_back(pds);
         }
      }
      // Para iniciar el proceso necesito al menos dos fuentes de datos
      if (datasources.size() > 1)	         
			hasdatasources = true;
      VectorOperationProcess process(datasources, viewerwindow, pelement,
                                            pDataView_);
      ProcessAdaptLayer* pal=process.GetProcessAdaptLayer();
      pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMergeLayersKeyAttr,
                                  showvectoroperation);
      pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr,
                                    showspatial);
      pal->AddAttribute<std::vector<DatasourceInterface*> >(
                ProcessAdaptLayer::VectorOperationDatasourcesKeyAttr, datasources);
      process.SetProcessName("MergeLayersProcess");
      process.OnProcessAdaptLayerSet();
      if (hasdatasources) {
         ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
               &process, caption_MERGE_LAYERS, pDataView_->GetLibraryManager());
         if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
            DatasourceInterface* pdatasource = NULL;
            const char *pdatasourcetypes[] = { "VectorDatasource" };
            for (int i = 0; i < 1 && pdatasource == NULL; ++i)
               pdatasource = DatasourceInterface::Create(pdatasourcetypes[i], pelement);
            if (pdatasource)
               pDataView_->GetDatasourceManager()->AddDatasource(pdatasource);
         }
      } else {
         //TODO: Detallar más el error.
         SHOW_WARNING(message_PROCESS_MERGE_LAYERS_START_ERROR);
      }
   } else {
      SHOW_WARNING(message_PROCESS_MERGE_LAYERS_START_ERROR);
   }
	return true;
}


}  /** namespace core **/
}  /** namespace suri **/
