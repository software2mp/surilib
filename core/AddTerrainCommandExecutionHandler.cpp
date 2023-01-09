/*! \file AddTerrainCommandExecutionHandler.cpp */

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
#include "suri/AddTerrainCommandExecutionHandler.h"
#include "suri/DataViewManager.h"
#include "suri/GenericTool.h"
#include "suri/Command.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/TerrainElement.h"

namespace suri {

namespace core {

/** Constructor **/
AddTerrainCommandExecutionHandler::AddTerrainCommandExecutionHandler(DataViewManager* pDataView) 
													: pDataView_(pDataView) {
}

/** Destructor **/
AddTerrainCommandExecutionHandler::~AddTerrainCommandExecutionHandler() {
}
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool AddTerrainCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("AddTerrainLayer"))
		return false;
   ViewcontextInterface* pvc = pDataView_->GetViewcontextManager()->GetSelectedViewcontext();
   if (pvc && pvc->GetType() != suri::ViewcontextInterface::V3D) {
      SHOW_ERROR("Solo es posible agregar un MDE en el modo de visualizacion 3D");
      return true;
   }
	std::string filter = filter_VIEWER_3D_ELEMENTS;
	suri::DatasourceManagerInterface* dm = pDataView_->GetDatasourceManager();
   wxFileDialog filedialog(NULL, _(caption_SELECT_ELEMENT), wxT(""), wxT(""), filter);
   if (filedialog.ShowModal() == wxID_OK) {
      std::string filename = filedialog.GetPath().c_str();
      Element* pelement = Element::Create("TerrainElement", filename);
      if (pelement)
         pelement->SetName(wxFileName(filename).GetName());
      const char* pdatasourcetypes[] = { "RasterDatasource" };
      DatasourceInterface *pdatasource = NULL;
      for (size_t i = 0; i < ARRAY_SIZE(pdatasourcetypes) && pdatasource == NULL; ++i)
         pdatasource = suri::DatasourceInterface::Create(pdatasourcetypes[i], pelement);
      if (!dm->AddDatasource(pdatasource))
         SHOW_ERROR(message_ADD_ELEMENT_ERROR);
   }
	return true;
}


}  /** namespace core **/
}  /** namespace suri **/
