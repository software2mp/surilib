/*! \file StackBandCommandExecutionHandler.cpp */

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
