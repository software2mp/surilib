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
