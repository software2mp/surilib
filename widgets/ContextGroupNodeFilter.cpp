/*! \file ContextGroupNodeFilter.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */



// Includes Estandar
// Includes Suri
#include "ContextGroupNodeFilter.h"
#include "DefaultTreeNode.h"


// Includes Wx
// Defines
// forwards

namespace suri {


ContextGroupNodeFilter::ContextGroupNodeFilter(
		const SuriObject::UuidType& FilterId) :
		filterId_(FilterId) {
}

ContextGroupNodeFilter::~ContextGroupNodeFilter() {
}
/**
* Clona instancia de filtro.
* @return copia de filtro.
*/
NodeFilterInterface* ContextGroupNodeFilter::Clone() const {
	return new ContextGroupNodeFilter(filterId_);
}
/**
* Retorna true si el nodo esta filtrado.
* @param[in] pNode nodo a analizar
* @return bool que indica si hay que filtrarlo
*/
bool ContextGroupNodeFilter::IsFiltred(TreeNodeInterface* pNode) const {
   if (pNode && !pNode->AllowsChildren())
      return false;
   DefaultTreeNode* ptreenode = dynamic_cast<DefaultTreeNode*>(pNode);
   if (!ptreenode)
      return false;

   std::vector<SuriObject::UuidType> vlist = ptreenode->GetAssociatedIds();
   std::vector<SuriObject::UuidType>::iterator vit = vlist.begin();
   for (; vit != vlist.end(); ++vit) {
      if((*vit).compare(filterId_) == 0)
         return false;
   }
   return true;
}
} /** namespace suri */

