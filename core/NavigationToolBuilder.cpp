/*! \file NavigationToolBuilder.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Suri
#include "suri/NavigationToolBuilder.h"
#include "suri/NavigationToolCommandCreator.h"
#include "suri/NavigationToolCommandExecutionHandler.h"
#include "suri/GenericTool.h"

namespace suri {
namespace core {

NavigationToolBuilder::NavigationToolBuilder(Navigator *pNavigator) : pNavigator_(pNavigator) {
}

NavigationToolBuilder::~NavigationToolBuilder() {
}

/**
 * Metodo que crea y configura una herramienta con los hanlders para la ejecucion
 * (CommandExecutionHandlers) y las acciones correspondientes (Commands).
 */
ToolInterface* NavigationToolBuilder::BuildTool(DataViewManager* pDataViewManager) {
   GenericTool* ptool = new GenericTool(pDataViewManager);
   /** Creadores de commands **/
   ptool->AddCommandCreator(new ViewportLinkCommandCreator);
   ptool->AddCommandCreator(new NavigationCommandCreator);
   ptool->AddCommandCreator(new FullExtentCommandCreator);
   ptool->AddCommandCreator(new LayerExtentCommandCreator);
   ptool->AddCommandCreator(new Zoom1on1CommandCreator);
   ptool->AddCommandCreator(new SaveViewCommandCreator);
   ptool->AddCommandCreator(new DragCommandCreator(pDataViewManager, ptool));
   ptool->AddCommandCreator(new MultiZoomCommandCreator(pDataViewManager, ptool));
   ptool->AddCommandCreator(new ZoomInCommandCreator);
   ptool->AddCommandCreator(new ZoomOutCommandCreator);
   ptool->AddCommandCreator(new SphericalCameraCommandCreator);
   ptool->AddCommandCreator(new EyeCameraCommandCreator);

   /** Administradores de ejecucion */
   ptool->AddCommandExecutionHandler(new ViewportLinkCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(
         new NavigationCommandExecutionHandler(pDataViewManager, ptool, pNavigator_));
   ptool->AddCommandExecutionHandler(new FullExtentCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new LayerExtentCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new Zoom1on1CommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new SaveViewCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new ZoomInCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new ZoomOutCommandExecutionHandler(pDataViewManager));
   ptool->AddCommandExecutionHandler(new CameraCommandExecutionHandler(pDataViewManager));

   ptool->SetConfigurationVariableName("navigation");
   ptool->Initialize();
   return ptool;
}

} /** namespace core */
} /** namespace suri */
