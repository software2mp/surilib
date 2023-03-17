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
#include "suri/ClassConverterCommandExecutionHandler.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "EnclosureManipulator.h"
#include "VectorDatasource.h"
#include "suri/ToolSupport.h"
#include "suri/DataViewManager.h"
#include "suri/ViewcontextManagerInterface.h"
#include "suri/SuriObject.h"
#include "EnclosureValidator.h"
#include "suri/LayerStateChangeNotification.h"

namespace suri {

namespace core {

/** Constructor **/
ClassConverterCommandExecutionHandler::ClassConverterCommandExecutionHandler(
										DataViewManager* pDataView) : pDataView_(pDataView) {
}

/** Destructor **/
ClassConverterCommandExecutionHandler::~ClassConverterCommandExecutionHandler() {
}
/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar 
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool ClassConverterCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
	if (pCommand->GetId() != pTool->GetId("TrainClassConverter") &&
      pCommand->GetId() != pTool->GetId("TestClassConverter"))
		return false;
   // TODO(Gabriel - TCK #7308): Falta distinguir entre areas de entrenamiento y test
   if (!pDataView_) {
		SHOW_ERROR(_(message_CANT_CONVERT_TO_CLASS));
		return true;
	}
   ViewcontextInterface* pviewcontext = pDataView_->GetViewcontextManager()->GetSelectedViewcontext();
   std::list<SuriObject::UuidType> selectedlayers = pviewcontext->GetSelectedLayers();
   EnclosureManipulator manipulator(pCommand->GetId() == pTool->GetId("TrainClassConverter"));
   EnclosureValidator validator;
   bool success = true;
   for (std::list<SuriObject::UuidType>::iterator layer = selectedlayers.begin();
         layer != selectedlayers.end(); ++layer) {
      DatasourceInterface* pvectords = pviewcontext->GetAssociatedDatasource(*layer);
      if (pvectords && validator.IsValidDatasourceType(pvectords) &&
                                                            manipulator.AddNature(pvectords)) {
     	   /** TODO(Gabriel - TCK #6127)
          * Se debe replicar el addnature a las capas para que de esta
          *  forma no importa que capa tome para guardar los proyectos
          *  se encuentre la metadata en el xml que indica que es valida
          *  como area de clasificacion
          */
        std::vector<SuriObject::UuidType> layerids =
               pDataView_->GetViewcontextManager()->GetAssociatedLayer(pvectords->GetId());
         std::vector<SuriObject::UuidType>::iterator it = layerids.begin();
         for (; it != layerids.end(); ++it) {
            LayerInterface* player =
                  pDataView_->GetViewcontextManager()->GetLayer(*it);
            manipulator.AddNature(player->GetElement());
         }       
      } else {
         std::string errmsg = pvectords->GetName() + ": ";
         errmsg += _(message_CANT_CONVERT_TO_CLASS);
         SHOW_ERROR(errmsg.c_str());
         success = false;
      }  
   }      
   /** Fix para el ticket #6364
    * Notificamos que hay un cambio de status en el elemento.
   */
   LayerStateChangeNotification changenotif;
   pDataView_->GetDatasourceManager()->Notify(&changenotif);
	return success;
}

}  /** namespace core **/
}  /** namespace suri **/
