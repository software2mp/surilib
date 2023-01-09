/*! \file IntersectionOperation.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef INTERSECTIONOPERATION_H_
#define INTERSECTIONOPERATION_H_

// Includes Estandar
// Includes Suri
#include "VectorOperation.h"
// Includes Wx
// Defines

namespace suri {
// forwards
class VectorDatasource;

class IntersectionOperation : public suri::VectorOperation {
public:
   /** Constructor */
   IntersectionOperation();
   /** Destructor */
   virtual ~IntersectionOperation();
protected:
   /** Procesa las fuentes de datos de entrada generando la interseccion de ambas
    * en una nueva fuente de datos */
   virtual VectorDatasource* DoProcessDatasource(
         VectorOperation::OperationType& Operation,
         std::vector<DatasourceInterface*>& Datasources);
};

} /** namespace suri */

#endif /* INTERSECTIONOPERATION_H_ */
