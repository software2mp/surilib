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
#include <vector>

// Includes Suri
#include "suri/QueryCommandExecutionHandler.h"
#include "Query.h"
#include "suri/Command.h"
#include "suri/GenericTool.h"
#include "suri/DataViewManager.h"
#include "suri/ViewerWidget.h"

namespace suri {
namespace core {

/** Constructor */
QueryCommandExecutionHandler::QueryCommandExecutionHandler(DataViewManager* pDataView) :
      pDataView_(pDataView), pQuery_(new Query(pDataView->GetViewportManager(),
                                               pDataView->GetViewcontextManager(),
                                               pDataView->GetLibraryManager())) {
}

/** Destructor */
QueryCommandExecutionHandler::~QueryCommandExecutionHandler() {
   delete pQuery_;
}

/**
 * Metodo que en funcion del Command que se pasa por parametro se encarga de administrar
 * la ejecucion del mismo (en caso de que pueda manejar el tipo de comando).
 * @return true en caso de poder manejar la ejecucion del comando
 * @return false en caso contrario
 */
bool QueryCommandExecutionHandler::Execute(const Command* pCommand, GenericTool* pTool) {
   int id = pCommand->GetId();
   if (id != pTool->GetId("Query"))
      return false;
   std::vector<SuriObject::UuidType> ids = pDataView_->GetViewportManager()->GetViewportIds();
   ViewerWidget* pviewer = NULL;
   for (size_t i = 0; i < ids.size(); ++i) {
      pviewer = dynamic_cast<ViewerWidget*>(pDataView_->GetViewportManager()->GetViewport(ids[i]));
      if (pTool->IsActive(pCommand)) {
         pviewer->PushMouseEvent(pQuery_);
         pviewer->PushPaintEvent(pQuery_);
      } else {
         pviewer->PopMouseEvent(pQuery_);
         pviewer->PopPaintEvent(pQuery_);
      }
   }
   return true;
}

} /** namespace core */
} /** namespace suri */
