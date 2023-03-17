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

#include "SuriObjectOrderChangeGroupNotification.h"
#include "SuriObjectGroupManager.h"
#include "TreeNodeAuxiliaryFunctions.h"

namespace suri {

SuriObjectOrderChangeGroupNotification::SuriObjectOrderChangeGroupNotification(
                                          SuriObject::UuidType ObjectId,
                                          SuriObject::UuidType DestinationObjectId) {
   objectId_ = ObjectId;
   destinationObjectId_ = DestinationObjectId;
}

SuriObjectOrderChangeGroupNotification::~SuriObjectOrderChangeGroupNotification() {
}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool SuriObjectOrderChangeGroupNotification::ApplyNotification(
                                             ObserverInterface* pObserver) {
   // Es necesario realizar el casteo hacia un SuriObjectGroupManager
   // para no aplicar el cambio sobre cualquier observer
   SuriObjectGroupManager* pgroupmanager =
                  dynamic_cast<SuriObjectGroupManager*>(pObserver);
   bool success = false;
   if (pgroupmanager) {
      MutableWorkGroupInterface* pworkgroup =
                                pgroupmanager->GetRootMutableWorkGroup();
      NodePath nodepath = pworkgroup->SearchContent(objectId_);
      if (!nodepath.IsValid())
         return false;

      // Si no hay nodo destino trato de moverlo al final del arbol.
      TreeNodeInterface* pdestinationgroup = pworkgroup->GetRoot();
      int destinationindex = pdestinationgroup->GetSize();

      // Busco nodo destino
      if (destinationObjectId_ != SuriObject::NullUuid) {
         TreeNodeInterface* pdestinationnode = pworkgroup->SearchContent(
                                      destinationObjectId_).GetLastPathNode();
         if (!pdestinationnode)
            return false;
         pdestinationgroup = pdestinationnode->GetParent();
         destinationindex = GetIndexOfChild(pdestinationgroup, pdestinationnode);
      }

      success = pworkgroup->MoveNode(nodepath,
                                  pworkgroup->GetPathToRoot(pdestinationgroup),
                                  destinationindex).IsValid();
   }
   return success;
}

} /* namespace suri */
