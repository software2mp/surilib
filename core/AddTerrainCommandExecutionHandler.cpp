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
