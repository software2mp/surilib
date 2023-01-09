/*! \file RemoveLayerCommandExecutionHandler.cpp */

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
