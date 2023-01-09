/** \file GroupsNodeFilter.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "GroupsNodeFilter.h"

namespace suri {

/** Ctor */
GroupsNodeFilter::GroupsNodeFilter (NodeFilterInterface* pDecoratedFilter) {
   pDecoratedFilter_ = pDecoratedFilter;
}

/** Dtor */
GroupsNodeFilter::~GroupsNodeFilter () {
}

/**
 * Clona instancia de filtro.
 * @return copia de filtro.
 */
NodeFilterInterface* GroupsNodeFilter::Clone() const {
   return new GroupsNodeFilter(pDecoratedFilter_);
}

/**
 * Retorna true si el nodo esta filtrado.
 * @param[in] pNode nodo a analizar
 * @return bool que indica si hay que filtrarlo
 */
bool GroupsNodeFilter::IsFiltred(TreeNodeInterface* pNode) const {
   return pNode->AllowsChildren() || (pDecoratedFilter_ &&
                                          pDecoratedFilter_->IsFiltred(pNode));
}

} /* namespace suri */
