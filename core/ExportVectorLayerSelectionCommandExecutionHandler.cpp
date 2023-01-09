/*! \file ExportVectorLayerSelectionCommandExecutionHandler.cpp */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */

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
