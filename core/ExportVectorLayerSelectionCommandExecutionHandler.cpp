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
#include "suri/ExportVectorLayerSelectionCommandExecutionHandler.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "VectorDatasource.h"
#include "suri/ToolSupport.h"
#include "suri/DataViewManager.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/SuriObject.h"
#include "suri/DatasourceInterface.h"
#include "suri/RasterElement.h"
#include "suri/Subset.h"
#include "suri/RasterProcess.h"
#include "suri/ProcessInterface.h"
#include "suri/FileExporterProcess.h"
#include "suri/ProcessNotebookWidget.h"
#include "Filter.h"
#include "FilteredDatasource.h"
#include "suri/FileVectorExporterProcess.h"

// Defines
#define FEATURE_ID_FIELD_NAME "FID"

namespace suri {

namespace core {

/** Constructor **/
ExportVectorLayerSelectionCommandExecutionHandler::ExportVectorLayerSelectionCommandExecutionHandler(
										DataViewManager* pDataView) : pDataView_(pDataView) {
}

/** Destructor **/
ExportVectorLayerSelectionCommandExecutionHandler::~ExportVectorLayerSelectionCommandExecutionHandler() {
}
 
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool ExportVectorLayerSelectionCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("ExportLayerSelection"))
		return false;
	VectorDatasource* pvector = ToolSupport::GetVectorDatasource(
         pDataView_->GetDatasourceManager(),
         pDataView_->GetViewcontextManager());
   World* pworld = ToolSupport::GetWorld(pDataView_->GetViewportManager());
   Element* pelement = NULL;
   ProcessAdaptLayer* pal=NULL;
   if (pvector) {
      bool showspatial = false;
      Subset viewerwindow;
      if (!pworld) {
         pworld = new World();
         pworld->SetSpatialReference(pvector->GetSpatialReference());
         Subset extent;
         pvector->GetElement()->GetElementExtent(extent);
         pworld->SetWorld(extent);
         pworld->SetWindow(extent);
      }
      pworld->GetWindow(viewerwindow);
      std::vector<DatasourceInterface*> datasources;
      FilteredDatasource* pvectorselection = new FilteredDatasource();
      pvectorselection->SetDatasource(pvector);
      Filter* pfilter = new Filter();
      pfilter->SetFilteredName(pvector->GetActiveLayerName());
      pfilter->SetIdFieldName(FEATURE_ID_FIELD_NAME);
      std::vector<FeatureIdType> fids =
            pDataView_->GetFeatureSelection()->GetSelectedFeatures(
                  pvector->GetId());
      std::vector<FeatureIdType>::iterator it = fids.begin();
      for (; it != fids.end(); ++it) {
         pfilter->AddEqualConstraint(*it);
      }
      pvectorselection->SetFilter(pfilter);
      datasources.push_back(pvectorselection);
      FileVectorExporterProcess process(datasources, viewerwindow, pelement,
                                               pDataView_);
      pal = process.GetProcessAdaptLayer();
      pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr,
                                     showspatial);
      process.OnProcessAdaptLayerSet();
      suri::ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
            &process, caption_VECTOR_LAYER_SELECTION_EXPORT, pDataView_->GetLibraryManager());
      if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
         DatasourceInterface* pdatasource = NULL;
         const char *pdatasourcetypes[] = { "VectorDatasource" };
         for (int i = 0; i < 1 && pdatasource == NULL; ++i)
            pdatasource = DatasourceInterface::Create(pdatasourcetypes[i], pelement);
         if (pdatasource)
            pDataView_->GetDatasourceManager()->AddDatasource(pdatasource);
      }
   } else {
      SHOW_WARNING(message_SELECTION_EXPORT_PROCESS_START_ERROR);
   }
	return true;
}

}  /** namespace core **/
}  /** namespace suri **/
