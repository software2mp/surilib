/** \file TreeIterator.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
