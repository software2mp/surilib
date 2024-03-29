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

#include "ObjectAditionGroupNotification.h"
#include "SuriObjectGroupManager.h"

namespace suri {

/**
 * Ctor.
 */
ObjectAditionGroupNotification::ObjectAditionGroupNotification(
      SuriObject::UuidType ObjectId, SuriObject::UuidType Predecessor, 
      ViewcontextInterface::ViewcontextType CType) : objectId_(ObjectId), 
		predecessor_(Predecessor), ctype_(CType) {
}

/**
 * Dtor.
 */
ObjectAditionGroupNotification::~ObjectAditionGroupNotification() {

}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool ObjectAditionGroupNotification::ApplyNotification(
                                       ObserverInterface* pObserver) {
   // Es necesario realizar el casteo hacia un SuriObjectGroupManager
   // para no aplicar el cambio sobre cualquier observer
   SuriObjectGroupManager* pgroupmanager =
                  dynamic_cast<SuriObjectGroupManager*>(pObserver);
   bool success = false;
   if (pgroupmanager) {
      MutableWorkGroupInterface* pworkgroup =
                                pgroupmanager->GetRootMutableWorkGroup();
      NodePath spath = pworkgroup->SearchContent(objectId_);
      if (!spath.IsValid()) {
			int parentindex = pworkgroup->GetRootSize();
			if (predecessor_.compare(SuriObject::NullUuid) != 0) {
				parentindex = 0;
				TreeIterator iterator = pworkgroup->GetIterator(pworkgroup->GetRootPath());
				TreeNodeInterface* pnode = iterator.GetCurrent();
				int index = 0;
				while (pnode) {
					if (pnode->GetContent().compare(predecessor_) == 0) {
						parentindex = index > 0 ? index - 1 : index;
						break;
					}
					iterator.NextNode();
					pnode = iterator.GetCurrent();
					++index;
				}
			}
         pworkgroup->InsertObject(objectId_, pworkgroup->GetRootPath(),
                                  parentindex, GetContextType());
      }
      success = true;
   }
   return success;
}

} /* namespace suri */
