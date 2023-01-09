/*! \file ContextGroupNodeFilter.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CONTEXTGROUPNODEFILTER_H_
#define CONTEXTGROUPNODEFILTER_H_

#include "suri/NodeFilterInterface.h"
#include "suri/SuriObject.h"
// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

class ContextGroupNodeFilter: public suri::NodeFilterInterface {
public:
	ContextGroupNodeFilter(const SuriObject::UuidType& FilterId);
	virtual ~ContextGroupNodeFilter();
	   /**
	 * Clona instancia de filtro.
	 * @return copia de filtro.
	 */
	virtual NodeFilterInterface* Clone() const;
	/**
	 * Retorna true si el nodo esta filtrado.
	 * @param[in] pNode nodo a analizar
	 * @return bool que indica si hay que filtrarlo
	 */
	virtual bool IsFiltred(TreeNodeInterface* pNode) const;
private:
	SuriObject::UuidType filterId_;
};

} /** namespace suri */

#endif /* CONTEXTGROUPNODEFILTER_H_ */
