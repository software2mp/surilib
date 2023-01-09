/*! \file ExportLayerCommandExecutionHandler.cpp */

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
#include "suri/ExportLayerCommandExecutionHandler.h"
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

namespace suri {

namespace core {

/** Constructor **/
ExportLayerCommandExecutionHandler::ExportLayerCommandExecutionHandler(
										DataViewManager* pDataView) : pDataView_(pDataView) {
}

/** Destructor **/
ExportLayerCommandExecutionHandler::~ExportLayerCommandExecutionHandler() {
}
 
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool ExportLayerCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   RasterElement* praster = ToolSupport::GetRaster(pDataView_->GetViewcontextManager());
   if (pCommand->GetId() != pTool->GetId("ExportLayer"))
		return false;
   Subset viewerwindow;
   bool showmask = true;
   if (praster != NULL) {
      ToolSupport::GetSubset(pDataView_->GetViewportManager(), viewerwindow, praster);
      FileExporterProcess process(praster, viewerwindow, pDataView_);
      ProcessAdaptLayer* ppal = process.GetProcessAdaptLayer();
      ppal->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr, showmask);
      process.OnProcessAdaptLayerSet();
      suri::ProcessNotebookWidget* pprocesswidget = new suri::ProcessNotebookWidget(
            &process, caption_LAYER_EXPORT, pDataView_->GetLibraryManager());
      if (pprocesswidget->CreateTool() && pprocesswidget->ShowModal() == wxID_OK) {
         Element* pOutputElement = NULL;
         ppal->GetAttribute(ProcessAdaptLayer::OutputElementKeyAttr, pOutputElement);
         DatasourceInterface* pdatasource = NULL;
         const char *pdatasourcetypes[] = { "RasterDatasource", "VectorDatasource" };
         for (int i = 0; i < 2 && pdatasource == NULL; ++i)
            pdatasource = DatasourceInterface::Create(pdatasourcetypes[i], pOutputElement);
         if (pdatasource)
            pDataView_->GetDatasourceManager()->AddDatasource(pdatasource);
      }
   } else {
      SHOW_WARNING(message_PROCESS_START_ERROR);
   }
	return true;
}

}  /** namespace core **/
}  /** namespace suri **/
