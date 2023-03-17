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
#include "suri/QuickMeassureCommandExecutionHandler.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "suri/DataViewManager.h"
#include "suri/Viewer2D.h"
#include "suri/DynamicViewerWidget.h"
#include "MeassureWidget.h"

namespace suri {
namespace core {

/** Constructor **/
QuickMeassureCommandExecutionHandler::QuickMeassureCommandExecutionHandler(
      DataViewManager* pDataView, suri::MeassureWidget* pQuickMeasure) :
            pDataView_(pDataView), pQuickMeasure_(pQuickMeasure) {
}

/** Destructor **/
QuickMeassureCommandExecutionHandler::~QuickMeassureCommandExecutionHandler() {
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool QuickMeassureCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   int id = pCommand->GetId();
   if (id == pTool->GetId("QuickMeassure")) {
      Viewer2D* pviewer =
            dynamic_cast<Viewer2D*>(pDataView_->GetViewportManager()->GetSelectedViewport());
      if (!pviewer) {
         DynamicViewerWidget* pdynamic =
               dynamic_cast<DynamicViewerWidget*>(pDataView_->GetViewportManager()->GetSelectedViewport());
         pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer(DynamicViewerWidget::Raster));
      }
      if (pviewer) {
         if (pCommand->IsActive()) {
            pQuickMeasure_->ShowWidget();
         } else {
            pQuickMeasure_->HideWidget();
         }
      }
   }
   else
      return false;
   return true;
}

} /** namespace core */
} /** namespace suri */
