/** \file TreeIterator.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
