/** \file EnclosureNodeFilter.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ENCLOSURENODEFILTER_H_
#define ENCLOSURENODEFILTER_H_

// Includes suri
#include "suri/NodeFilterInterface.h"

namespace suri {

class DatasourceManagerInterface;

/**
 * Clase que filtra todos los elementos menos los recintos.
 */
class EnclosureNodeFilter : public NodeFilterInterface {
public:
   /** Ctor */
   explicit EnclosureNodeFilter(DatasourceManagerInterface* pDatasourceManager);
   /** Dtor */
   virtual ~EnclosureNodeFilter();

   /** Clona instancia de filtro. */
   virtual NodeFilterInterface* Clone() const;
   /** Retorna true si el nodo esta filtrado. */
   virtual bool IsFiltred(TreeNodeInterface* pNode) const;

private:
   /** administrador de fuente de datos para la obtencion de las
    * caracteristicas de la fuente de datos a filtrar */
   DatasourceManagerInterface* pDatasourceManager_;
};

} /* namespace suri */
#endif /* ENCLOSURENODEFILTER_H_ */
