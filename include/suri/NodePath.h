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

#ifndef GROUPPATH_H_
#define GROUPPATH_H_

// Includes standard
#include <vector>

// Includes suri
#include "suri/TreeNodeInterface.h"

namespace suri {

/**
 * Permite recorrer los grupos que contienen a un nodo. El path puede ser
 * relativo a cualquiera de los grupos.
 */
class NodePath {
public:
   /** Ctor de Copia */
   NodePath(const NodePath &Path);
   /** Ctor */
   NodePath(TreeNodeInterface* pLastNode, TreeNodeInterface* pRoot);
   /** Dtor */
   ~NodePath();

   /** Retorna path al nodo en posicion indicada. */
   NodePath GetPathToParentNode(int Index) const;
   /** Retorna el nodo en posicion indicada de path. */
   TreeNodeInterface* GetPathNode(int Index) const;
   /** Retorna el nodo al que dirige el path. */
   TreeNodeInterface* GetLastPathNode() const;
   /** Retorna la cantidad de nodos en path. */
   int GetPathCount() const;
   /** Informa si el path es valido */
   bool IsValid() const;
   /** Informa si el nodo al que referencia el path puede tener hijos */
   bool AllowsChildren() const;

   /** Compara dos paths */
   bool Compare(const NodePath &OtherPath) const;

   /** Compara dos paths. */
   bool operator<(const NodePath &OtherPath) const;

private:
   /** Vector con nodos en path */
   std::vector<TreeNodeInterface*> pathContent_;
};

} /* namespace suri */

#endif /* GROUPPATH_H_ */
