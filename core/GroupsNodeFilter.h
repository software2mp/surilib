/** \file GroupsNodeFilter.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GROUPSNODEFILTER_H_
#define GROUPSNODEFILTER_H_

// Includes suri
#include "suri/NodeFilterInterface.h"

namespace suri {

/**
 * Un TreeIterator configurado con este filtro ignorara los nodos que sean
 * grupos(puedan tener hijos).
 */
class GroupsNodeFilter : public NodeFilterInterface {
public:
   /** Ctor */
   explicit GroupsNodeFilter(NodeFilterInterface* pDecoratedFilter = NULL);
   /** Dtor */
   virtual ~GroupsNodeFilter();

   /** Clona instancia de filtro. */
   virtual NodeFilterInterface* Clone() const;
   /** Retorna true si el nodo esta filtrado.  */
   virtual bool IsFiltred(TreeNodeInterface* pNode) const;

private:
   /** Filtro que esta decorando este filtro */
   NodeFilterInterface* pDecoratedFilter_;
};

} /* namespace suri */
#endif /* GROUPSNODEFILTER_H_ */
