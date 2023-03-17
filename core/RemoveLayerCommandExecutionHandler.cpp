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

// Includes Suri
#include "suri/RemoveLayerCommandExecutionHandler.h"
#include "suri/DataViewManager.h"
#include "suri/GenericTool.h"
#include "suri/Command.h"
#include "suri/TreeSelectionManagerInterface.h"
#include "HtmlTreeGroupChangedNotification.h"
namespace suri {

namespace core {

/** Constructor **/
RemoveLayerCommandExecutionHandler::RemoveLayerCommandExecutionHandler(DataViewManager* pDataView) 
							: pDataView_(pDataView) {
}

/** Destructor **/
RemoveLayerCommandExecutionHandler::~RemoveLayerCommandExecutionHandler() {
}
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool RemoveLayerCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("RemoveLayer"))
		return false;
	// Elmino capas seleccionadas
   std::list<suri::NodePath> selectednodes = pDataView_->GetSelectionManager()->GetSelectedNodes();
   std::list<suri::NodePath>::iterator it = selectednodes.begin();
   bool removeresult = true;
   for (; it != selectednodes.end() && removeresult; ++it)
      if (!it->AllowsChildren())
         removeresult = pDataView_->GetDatasourceManager()->RemoveObject(
               it->GetLastPathNode()->GetContent());

   if (removeresult) {
   	// Elimino grupos seleccionados
	   selectednodes = pDataView_->GetSelectionManager()->GetSelectedNodes();
	   removeresult = true;
	   it = selectednodes.begin();
	   for (; it != selectednodes.end() && removeresult; ++it) {
	      if (it->AllowsChildren())
   	      removeresult = pDataView_->GetGroupManager()->RemoveGroup(*it);
		}
      HtmlTreeGroupChangedNotification htmltreegroupnotification(pDataView_->GetGroupManager());
      pDataView_->GetGroupManager()->Notify(&htmltreegroupnotification);
		if (!removeresult) 
			SHOW_ERROR("No se puede elminar el grupo");
   } else {
		SHOW_ERROR(_(message_LAYER_BLOCK));
	}
return true;
}


}  /** namespace core **/
}  /** namespace suri **/
