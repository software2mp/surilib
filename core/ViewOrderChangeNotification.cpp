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
#include "ViewOrderChangeNotification.h"
#include "LayerTreeEventHandler.h"
#include "suri/Element.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
ViewOrderChangeNotification::ViewOrderChangeNotification(Element *pElement, Element *pElementDest,
                                                         const int &Movetype) :
      moveType_(Movetype), pElement_(pElement), pElementDest_(pElementDest) {
}

/**
 * Destructor
 */
ViewOrderChangeNotification::~ViewOrderChangeNotification() {
}

/**
 *  Indica al handler asociado que mueva la lista
 *  @param[in] Observer que es notificado el cambia de orden
 *  @return true en caso de poder cambiar el orden, false en caso contrario
 */
bool ViewOrderChangeNotification::ApplyNotification(ObserverInterface* pObserver) {
   LayerTreeEventHandler *phandler = dynamic_cast<LayerTreeEventHandler *>(pObserver);
   if (phandler == NULL)
      return false;
   switch (moveType_) {
      case MoveBefore:
         phandler->MoveNodeBefore(pElement_, pElementDest_);
         break;
      case MoveAfter:
         phandler->MoveNodeAfter(pElement_, pElementDest_);
         break;
      case MoveUnder:
         phandler->MoveNodeUnder(pElement_, pElementDest_);
         break;
      default:
         return false;
   }
   return true;
}

} /** namespace suri */
