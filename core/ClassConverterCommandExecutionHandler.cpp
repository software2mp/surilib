/*! \file ClassConverterCommandExecutionHandler.cpp */

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
