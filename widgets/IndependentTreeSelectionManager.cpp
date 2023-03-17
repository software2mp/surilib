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

#include "IndependentTreeSelectionManager.h"

namespace suri {

IndependentTreeSelectionManager::IndependentTreeSelectionManager(
                                 GroupManagerInterface* pGroupManager) {
   pGroupManager_ = pGroupManager;
}

IndependentTreeSelectionManager::~IndependentTreeSelectionManager() {
}

/** Limpia la seleccion de grupos e items */
void IndependentTreeSelectionManager::ResetSelection() {
   selectedNodes_.clear();
}

/** Permite modificar los nodos seleccionados */
void IndependentTreeSelectionManager::ChangeSelection(const NodePath &Path,
                                                               bool Selected) {
   SuriObject::UuidType nodeid = Path.GetLastPathNode()->GetId();
   if (Selected)
      selectedNodes_.insert(nodeid);
   else
      selectedNodes_.erase(nodeid);
}

/** Retorna el path de nodos seleccionados */
std::list<NodePath> IndependentTreeSelectionManager::GetSelectedNodes() {
   std::list<NodePath> selectednodepaths;
   WorkGroupInterface* pworkgroup = pGroupManager_->GetRootWorkGroup();
   NodeUuidSet::iterator it = selectedNodes_.begin();
   while (it != selectedNodes_.end()) {
      NodePath path = pworkgroup->SearchNode(*it);
      if (path.IsValid()) {
         selectednodepaths.push_back(path);
         ++it;
      } else {
         selectedNodes_.erase(it++);
      }
   }
   delete pworkgroup;
   return selectednodepaths;
}

} /* namespace suri */
