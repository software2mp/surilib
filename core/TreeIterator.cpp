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
#include <list>

// Includes suri
#include "suri/TreeIterator.h"
#include "TreeNodeAuxiliaryFunctions.h"

namespace suri {

class NullNodeFilter : public NodeFilterInterface {
public:
   /** Retorna true si el nodo esta filtrado */
   virtual NodeFilterInterface* Clone() const {return new NullNodeFilter();}
   /** Retorna true si el nodo esta filtrado */
   virtual bool IsFiltred(TreeNodeInterface* pNode) const {return false;}
};


/** Ctor */
TreeIterator::TreeIterator(TreeNodeInterface* pRootNode,
                                    TreeNodeInterface* pCurrentNode,
                                               NodeFilterInterface* pFilter) {
   pRootNode_ = pRootNode;
   pCurrentNode_ = pCurrentNode;
   pFilter_ = pFilter ? pFilter->Clone() : new NullNodeFilter();
   if (pCurrentNode_ != NULL && pFilter_->IsFiltred(pCurrentNode_))
      NextNode();
}

/** Ctor */
TreeIterator::TreeIterator(TreeNodeInterface* pRootNode,
                                 NodeFilterInterface* pFilter) {
   pRootNode_ = pRootNode;
   pCurrentNode_ = pRootNode;
   pFilter_ = pFilter ? pFilter->Clone() : new NullNodeFilter();
   if (pCurrentNode_ != NULL && pFilter_->IsFiltred(pCurrentNode_))
      NextNode();
}

/** Dtor */
TreeIterator::~TreeIterator() {
   delete pFilter_;
}

/** Se mueve al nodo siguiente */
void TreeIterator::NextNode() {
   bool isfiltred = true;
   while(pCurrentNode_ != NULL && isfiltred) {
      MoveToNextNode();
      if (IsValid())
         isfiltred = pFilter_->IsFiltred(pCurrentNode_);
   }
}

/**
 * Indica si el iterador apunta a un nodo valido
 * @return bool que indica si el iterador es valido
 */
bool TreeIterator::IsValid() {
   return pCurrentNode_ != NULL && !pFilter_->IsFiltred(pCurrentNode_);
}

/**
 * Retorna el path al nodo apuntado
 * @return NodePath al nodo apuntado
 */
NodePath TreeIterator::GetPath() {
   return NodePath(pCurrentNode_, pRootNode_);
}

/**
 * Devuelve el nodo actual.
 */
TreeNodeInterface* TreeIterator::GetCurrent() {
   return pCurrentNode_;
}

/** Se mueve al nodo siguiente */
void TreeIterator::MoveToNextNode() {
   if (!pRootNode_ || !pCurrentNode_ || pRootNode_->GetSize() == 0) {
      pCurrentNode_ = NULL;
      return;
   }

   TreeNodeInterface* pcurrentnode = pCurrentNode_;

   // Inicializo currentchilds
   std::list<int> currentchilds;
   TreeNodeInterface* pchildnode = pcurrentnode;
   while (pchildnode != pRootNode_ && pchildnode != NULL) {
      TreeNodeInterface* ptempnode = pchildnode->GetParent();
      int index = GetIndexOfChild(ptempnode, pchildnode);
      currentchilds.insert(currentchilds.begin(), index);
      pchildnode = pchildnode->GetParent();
   }
   if (pchildnode == NULL) {
      pCurrentNode_ = NULL;
      return;
   }
   int currentnodeindex = 0;
   if (!currentchilds.empty()) {
      currentnodeindex = currentchilds.back();
      currentchilds.pop_back();
   }

   // Este ciclo itera sobre el arbol en profundidad buscando la siguiente hoja
   bool found = false;
   int direction = 1;  // 1: entrando nodo, 0: siguiente hijo, -1: saliendo nodo
   while (!found && pcurrentnode != NULL) {
      if (direction == 1) {
         if (pcurrentnode->GetSize() > 0) {
            pcurrentnode = pcurrentnode->GetChildAt(0);
            currentchilds.push_back(currentnodeindex);
            currentnodeindex = 0;
            found = true;
         } else {
            direction = 0;
         }
      }

      if (direction == 0) {
         currentnodeindex++;
         if (pcurrentnode->GetParent()->GetSize() > currentnodeindex) {
            pcurrentnode = pcurrentnode->GetParent()->GetChildAt(currentnodeindex);
            direction = 1;
            found = true;
         } else {
            direction = -1;
         }
      }

      if (direction == -1) {
         if (pcurrentnode->GetParent() != pRootNode_) {
            pcurrentnode = pcurrentnode->GetParent();
            currentnodeindex = currentchilds.back();
            currentchilds.pop_back();
            direction = 0;
         } else {
            pcurrentnode = NULL;
         }
      }
   }

   pCurrentNode_ = pcurrentnode;
}

} /* namespace suri */
