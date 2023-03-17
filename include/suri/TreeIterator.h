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

#ifndef TREEITERATOR_H_
#define TREEITERATOR_H_

// Includes suri
#include "suri/NodePath.h"
#include "suri/SuriObject.h"
#include "suri/TreeNodeInterface.h"
#include "suri/NodeFilterInterface.h"

namespace suri {

/**
 * Permite recorrer los nodos de un arbol secuencialmente
 */
class TreeIterator {
public:
   /** Ctor */
   TreeIterator(TreeNodeInterface* pRootNode, TreeNodeInterface* pCurrentNode,
                                                  NodeFilterInterface* pFilter);
   /** Ctor */
   TreeIterator(TreeNodeInterface* pRootNode, NodeFilterInterface* pFilter);
   /** Dtor */
   virtual ~TreeIterator();

   /** Se mueve al nodo siguiente */
   virtual void NextNode();
   /** Indica si el iterador apunta a un nodo valido */
   virtual bool IsValid();
   /** Retorna el path al nodo apuntado */
   virtual NodePath GetPath();

   /** Devuelve el nodo actual */
   TreeNodeInterface* GetCurrent();

protected:
   /** Se mueve al nodo siguiente */
   virtual void MoveToNextNode();
   /** Puntero a la raiz del arbol */
   TreeNodeInterface* pRootNode_;
   /** Puntero al nodo actual */
   TreeNodeInterface* pCurrentNode_;
   /** Filtro que hay que aplicar al arbol */
   NodeFilterInterface* pFilter_;
};

} /* namespace suri */
#endif /* TREEITERATOR_H_ */
