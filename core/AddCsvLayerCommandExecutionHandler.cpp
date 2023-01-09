/*! \file AddCsvLayerCommandExecutionHandler.cpp */

/**                                                                                                 
 *  Biblioteca de procesamiento GeoEspacial SuriLib. 
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L. 
 *                                                                                                  
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/) 
 *  
 */

// Include estandard
#include <string>

// Includes Suri
#include "suri/AddCsvLayerCommandExecutionHandler.h"
#include "suri/DataViewManager.h"
#include "suri/Command.h"
#include "TableImportXyPart.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/GenericTool.h"

namespace suri {

namespace core {

/** Constructor **/
AddCsvLayerCommandExecutionHandler::AddCsvLayerCommandExecutionHandler(
      DataViewManager* pDataView) : pDataView_(pDataView) {
}

/** Destructor **/
AddCsvLayerCommandExecutionHandler::~AddCsvLayerCommandExecutionHandler() {
}
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool AddCsvLayerCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   if (pCommand->GetId() != pTool->GetId("AddCsvLayer")) {
      return false;
   }
   suri::DatasourceManagerInterface* dm = pDataView_->GetDatasourceManager();
   TableImportXyPart* ptableimportxy = new TableImportXyPart(pDataView_->GetLibraryManager(), dm);
   ptableimportxy->CreateTool();
   ptableimportxy->ShowModal();
   return true;
}


}  /** namespace core **/
}  /** namespace suri **/
