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

// Includes standard
#include <algorithm>
#include <vector>
// Includes suri
#include "suri/NodePath.h"
#include "TreeNodeAuxiliaryFunctions.h"
#include "logmacros.h"

namespace suri {

/** Ctor de Copia */
NodePath::NodePath(const NodePath &Path) {
   pathContent_.assign(Path.pathContent_.begin(), Path.pathContent_.end());
}

/** Ctor */
NodePath::NodePath(TreeNodeInterface* pLastNode, TreeNodeInterface* pRoot) {
   // Configuro path
   TreeNodeInterface* pcurrentnode = pLastNode;
   while (pcurrentnode != NULL && pcurrentnode != pRoot) {
      pathContent_.insert(pathContent_.begin(), pcurrentnode);
      pcurrentnode = pcurrentnode->GetParent();
   }
   if (pRoot != NULL)
      pathContent_.insert(pathContent_.begin(), pcurrentnode);

   // Valido path generado
   if (pcurrentnode != pRoot)
      pathContent_.clear();
}

/** Dtor */
NodePath::~NodePath() {
}

/** Retorna path al nodo en posicion indicada. */
NodePath NodePath::GetPathToParentNode(int Index) const {
   if (!IsValid() || Index >= static_cast<int>(pathContent_.size()))
      return NodePath(NULL, NULL);
   return NodePath(pathContent_[Index], pathContent_[0]);
}

/**
 * Retorna el nodo en posicion indicada de path.
 * @param[in] Index Posicion dentro de path donde 0 es el root.
 * @return TreeNodeInterface* Nodo en posicion indicada
 */
TreeNodeInterface* NodePath::GetPathNode(int Index) const {
   return pathContent_[Index];
}

/**
 * Retorna el nodo al que dirige el path.
 * @return TreeNodeInterface* Nodo al que dirige el path
 */
TreeNodeInterface* NodePath::GetLastPathNode() const {
   if (pathContent_.size() > 0)
      return pathContent_[pathContent_.size() - 1];
   return NULL;
}

/**
 * Retorna la cantidad de nodos en path.
 * @return int cantidad de nodos en path
 */
int NodePath::GetPathCount() const {
   return pathContent_.size();
}

/**
 * Informa si el path es valido
 * @return bool que indica si el path es valido
 */
bool NodePath::IsValid() const {
   bool IsValid = (pathContent_.size() > 0);
   // Se recorren los nodos padre
   for (size_t i = 0; i < pathContent_.size() - 1 && IsValid; i++)
      IsValid = (GetIndexOfChild(pathContent_[i], pathContent_[i + 1]) >= 0) &&
                           (pathContent_[i] == pathContent_[i + 1]->GetParent());

   return IsValid;
}

/** Informa si el path corresponde a una hoja */
bool NodePath::AllowsChildren() const {
   if (!IsValid()) {
      REPORT_DEBUG("D: Es path es invalido");
      return false;
   }
   return GetLastPathNode()->AllowsChildren();
}

/** Compara dos paths */
bool NodePath::Compare(const NodePath &OtherPath) const {
   if (!IsValid() || !OtherPath.IsValid() || OtherPath.GetPathCount() < 2 ||
            GetPathCount() < 2 || pathContent_[0] != OtherPath.pathContent_[0]) {
      REPORT_DEBUG("D: Se trato de comparar dos paths. Donde al menos uno era invalido");
      return false;
   }

   std::vector<TreeNodeInterface*>::const_iterator it1 = pathContent_.begin();
   std::vector<TreeNodeInterface*>::const_iterator it2 = OtherPath.pathContent_.begin();
   for (; it1 != pathContent_.end() && it2 != OtherPath.pathContent_.end()
                                             && *it1 == *it2; it1++,
                                                               it2++);

   return it1 == pathContent_.end() && it2 == OtherPath.pathContent_.end();
}

/** Compara dos paths. */
bool NodePath::operator<(const NodePath &OtherPath) const {
   if (!IsValid() || !OtherPath.IsValid() || OtherPath.GetPathCount() < 2 ||
            GetPathCount() < 2 || pathContent_[0] != OtherPath.pathContent_[0]) {
      REPORT_DEBUG("D: Se trato de comparar dos paths. Donde al menos uno era invalido");
      return false;
   }

   int count = std::min(GetPathCount(), OtherPath.GetPathCount());

   bool lessthan = false,
         finished = false;
   for (int i = 0; i < (count - 1) && !finished; i++) {
      int thispathindex = GetIndexOfChild(pathContent_[i], pathContent_[i + 1]);
      int otherpathindex = GetIndexOfChild(OtherPath.pathContent_[i],
                                              OtherPath.pathContent_[i + 1]);
      finished = (thispathindex == otherpathindex);
      lessthan = (thispathindex < otherpathindex);
   }

   return lessthan;
}

} /** namespace */
