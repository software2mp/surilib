/*! \file InfoToolBuilder.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Suri
#include "suri/InfoToolBuilder.h"
#include "suri/GenericTool.h"
#include "suri/InfoToolCommandCreator.h"
#include "suri/QuickMeassureCommandExecutionHandler.h"
#include "suri/ViewportPropertiesCommandExecutionHandler.h"
#include "suri/QueryCommandExecutionHandler.h"

namespace suri {
namespace core {

/** Constructor */
InfoToolBuilder::InfoToolBuilder(suri::MeassureWidget* pQuickMeasure) :
      pQuickMeasure_(pQuickMeasure) {
}

/** Destructor */
InfoToolBuilder::~InfoToolBuilder() {
}

/**
 * Metodo que crea y configura una herramienta con los hanlders para la ejecucion
 * (CommandExecutionHandlers) y las acciones correspondientes (Commands).
 */
ToolInterface* InfoToolBuilder::BuildTool(DataViewManager* pDataView) {
   GenericTool* ptool = new GenericTool(pDataView);
   /** Creadores de commands **/
   ptool->AddCommandCreator(new InfoToolCommandCreator(pDataView));

   /** Administradores de ejecucion */
   ptool->AddCommandExecutionHandler(
         new QueryCommandExecutionHandler(pDataView));
   ptool->AddCommandExecutionHandler(
         new QuickMeassureCommandExecutionHandler(pDataView, pQuickMeasure_));
   ptool->AddCommandExecutionHandler(
         new ViewportPropertiesCommandExecutionHandler(pDataView));

   ptool->SetConfigurationVariableName("info");
   ptool->Initialize();
   return ptool;
}

} /** namespace core */
} /** namespace suri */
