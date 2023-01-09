/*! \file FilterProcessCommandExecutionHandler.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/DataViewManager.h"
#include "suri/FilterProcess.h"
#include "suri/FilterProcessCommandExecutionHandler.h"
#include "suri/GenericTool.h"
#include "suri/ProcessNotebookWidget.h"
#include "suri/ToolSupport.h"
#include "suri/ToolSupport.h"
#include "suri/ProcessNotebookWidget.h"
// Includes Suri
// Includes Wx
// Defines
// forwards

using namespace suri;
using namespace suri::core;

/** Constructor **/
FilterProcessCommandExecutionHandler::FilterProcessCommandExecutionHandler
   (DataViewManager* pDataView): pDataView_(pDataView) {}

/** Destructor **/
FilterProcessCommandExecutionHandler::~FilterProcessCommandExecutionHandler(){}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool FilterProcessCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   int commandId = pCommand->GetId();
   if (commandId != pTool->GetId("ApplyFilter"))
      return false;
   RasterElement* praster = NULL;
   Subset viewerwindow;
   LoadInputProcessParameters(pDataView_, praster, viewerwindow);
   if (!praster){
         SHOW_WARNING(message_PROCESS_START_ERROR);
         return false;
   }
   if (praster != NULL) {
      int flags = KERNEL_FILTER;
      FilterProcess process(praster, viewerwindow, flags, pDataView_);
      RunProcess(pDataView_, caption_FILTER_EXPORTER_TOOL, &process);
   }
   return true;
}


/** Constructor **/
FilterLibraryCommandExecutionHandler::FilterLibraryCommandExecutionHandler
   (DataViewManager* pDataView): pDataView_(pDataView) {}

/** Destructor **/
FilterLibraryCommandExecutionHandler::~FilterLibraryCommandExecutionHandler(){}

bool FilterLibraryCommandExecutionHandler::Execute(const Command* pCommand,
                                                   GenericTool* pTool) {
   RasterElement* praster = NULL;
   Subset viewerwindow;
   int commandId = pCommand->GetId();
   if (commandId != pTool->GetId("FilterLibrary"))
      return false;
   LoadInputProcessParameters(pDataView_, praster, viewerwindow);
   if (!praster){
         SHOW_WARNING(message_PROCESS_START_ERROR);
         return false;
   }
   int flags = KERNEL_FILTER;
   FilterProcess process(praster, viewerwindow, flags, pDataView_);
   process.SetProcessName(FilterProcess::kChildProcessName);
   RunProcess(pDataView_, caption_FILTER_EXPORTER_TOOL, &process);
   return true;
}
