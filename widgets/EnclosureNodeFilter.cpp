/** \file EnclosureNodeFilter.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes suri
#include "EnclosureNodeFilter.h"
#include "suri/DatasourceManagerInterface.h"
#include "EnclosureValidator.h"

namespace suri {

/** Ctor */
EnclosureNodeFilter::EnclosureNodeFilter(
                  DatasourceManagerInterface* pDatasourceManager) {
   pDatasourceManager_ = pDatasourceManager;
}

/** Dtor */
EnclosureNodeFilter::~EnclosureNodeFilter() {
}


/**
 * Clona instancia de filtro.
 * @return copia de filtro.
 */
NodeFilterInterface* EnclosureNodeFilter::Clone() const {
   return new EnclosureNodeFilter(pDatasourceManager_);
}

/**
 * Retorna true si el nodo esta filtrado.
 * @param[in] pNode nodo a analizar
 * @return bool que indica si hay que filtrarlo
 */
bool EnclosureNodeFilter::IsFiltred(TreeNodeInterface* pNode) const {
   if (!pNode)
      return false;
   DatasourceInterface* pdatasource =
         pDatasourceManager_->GetDatasource(pNode->GetContent());

   EnclosureValidator enclosurevalidator;
   return !enclosurevalidator.IsValid(pdatasource);
}

} /* namespace suri */
