/*! \file VectorReprojectionCommandExecutionHandler.cpp */

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
#include "suri/VectorReprojectionCommandExecutionHandler.h"
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
#include "suri/FileVectorExporterProcess.h"
#include "suri/LibraryManagerFactory.h"
#include "suri/TransformationFactory.h"

namespace suri {

namespace core {

/** Constructor **/
VectorReprojectionCommandExecutionHandler::VectorReprojectionCommandExecutionHandler(
										DataViewManager* pDataView) : pDataView_(pDataView) {
}

/** Destructor **/
VectorReprojectionCommandExecutionHandler::~VectorReprojectionCommandExecutionHandler() {
}
 
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool VectorReprojectionCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("VectorReprojection"))
		return false;
	VectorDatasource* pvector = ToolSupport::GetVectorDatasource(
         pDataView_->GetDatasourceManager(),
         pDataView_->GetViewcontextManager());
   World* pworld = ToolSupport::GetWorld(pDataView_->GetViewportManager());
   Element* pelement = NULL;
   ProcessAdaptLayer* pal=NULL;
   if (pvector) {
      bool showspatial = false;
      bool showsrselection = true;   
      Library* plibrary = pDataView_->GetLibraryManager()->
                              GetLibraryByCode(LibraryManagerFactory::SpatialReferenceLibraryCode,
                              LibraryManager::AccessKey(true));
      SuriObject::UuidType libraryid = plibrary  ? plibrary->GetId() : SuriObject::NullClassId;
      std::string srin = pvector->GetSpatialReference();
      Subset viewerwindow;
      if (!pworld) {
         pworld = new World();
         pworld->SetSpatialReference(srin);
         Subset extent;
         pvector->GetElement()->GetElementExtent(extent);
         pworld->SetWorld(extent);
         pworld->SetWindow(extent);
      }
      pworld->GetWorld(viewerwindow);
      std::vector<DatasourceInterface*> datasources;
      datasources.push_back(pvector);

      FileVectorExporterProcess process(datasources, viewerwindow, pelement,
                                               pDataView_);
      pal=process.GetProcessAdaptLayer();
      pal->AddAttribute<SuriObject::UuidType>(ProcessAdaptLayer::LibraryIdKeyAttr, libraryid);
      pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeTransformationParametersKeyAttr,
                                   showsrselection);
      pal->AddAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr,
                                    showspatial);
      pal->AddAttribute<std::string>(suri::TransformationFactory::kParamWktIn, srin);
      pal->AddAttribute<std::string>(suri::TransformationFactory::kParamWktOut, srin);
      pal->AddAttribute<std::vector<DatasourceInterface*> >(
                                  ProcessAdaptLayer::InputDatasourcesKeyAttr, datasources);
      process.OnProcessAdaptLayerSet();
      suri::ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
            &process, caption_VECTOR_OPERATION, pDataView_->GetLibraryManager());
      if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
         DatasourceInterface* pdatasource = NULL;
         const char *pdatasourcetypes[] = { "VectorDatasource" };
         for (int i = 0; i < 1 && pdatasource == NULL; ++i)
            pdatasource = DatasourceInterface::Create(pdatasourcetypes[i], pelement);
         if (pdatasource)
            pDataView_->GetDatasourceManager()->AddDatasource(pdatasource);
      }
   } else {
      //TODO: establecer mensaje de error
   }

	return true;
}

}  /** namespace core **/
}  /** namespace suri **/
