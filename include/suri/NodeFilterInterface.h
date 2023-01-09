/** \file NodeFilterInterface.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef NODEFILTERINTERFACE_H_
#define NODEFILTERINTERFACE_H_

// Includes suri
#include "suri/TreeNodeInterface.h"

namespace suri {

/** 
 * Clase que deben implementar todos los filtros que actuan sobre el arbol 
 */
class NodeFilterInterface {
public:
   /** Dtor */
   virtual ~NodeFilterInterface() {}
   /** 
    * Clona instancia de filtro. 
    * @return copia de filtro.
    */
   virtual NodeFilterInterface* Clone() const = 0;
   /** 
    * Retorna true si el nodo esta filtrado. 
    * @param[in] pNode nodo a analizar
    * @return bool que indica si hay que filtrarlo
    */
   virtual bool IsFiltred(TreeNodeInterface* pNode) const = 0;
};

} /* namespace suri */

#endif /* NODEFILTERINTERFACE_H_ */
