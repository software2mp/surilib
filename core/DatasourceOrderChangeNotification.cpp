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

// Includes Suri
#include "suri/DatasourceInterface.h"
#include "DatasourceOrderChangeNotification.h"
#include "DefaultViewcontext.h"
#include "suri/LayerInterface.h"
#include "suri/messages.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
DatasourceOrderChangeNotification::DatasourceOrderChangeNotification(
      DatasourceInterface *pDatasource, DatasourceInterface *pDatasourceDestination) :
      pDatasource_(pDatasource), pDatasourceDestination_(pDatasourceDestination) {
}

/**
 * Destructor
 */
DatasourceOrderChangeNotification::~DatasourceOrderChangeNotification() {
}

/**
 * Metodo que ejecuta una accion sobre el observador a ser notificado
 */
bool DatasourceOrderChangeNotification::ApplyNotification(ObserverInterface* pObserver) {
   // Es necesario realizar el casteo hace un viewcontext, ya que es necesario
   // aplicar la notificacion sobre una instancia de viewcontext y no sobre cualquier
   // obeserver
   ViewcontextInterface* pviewcontext = dynamic_cast<ViewcontextInterface*>(pObserver);
   if (pviewcontext == NULL) {
      return false;
   }
   LayerInterface *player = pviewcontext->GetAssociatedLayer(pDatasource_->GetId());
   if (!player) {
      return false;
   }
   if (pDatasourceDestination_ != NULL) {
      LayerInterface *playerdestination = pviewcontext->GetAssociatedLayer(
            pDatasourceDestination_->GetId());
      if (!playerdestination)
         return false;
      pviewcontext->OrderChangeLayer(player, playerdestination);
   } else {
      pviewcontext->MoveLayerToEnd(player);
   }
   return true;
}

} /** namespace suri */
