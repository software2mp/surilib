/*! \file TrimOperation.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TRIMOPERATION_H_
#define TRIMOPERATION_H_

// Includes Estandar
// Includes Suri
#include "VectorOperation.h"
// Includes Wx
// Defines

namespace suri {
// forwards
class VectorDatasource;

class TrimOperation : public suri::VectorOperation {
public:
   /** Constructor */
   TrimOperation();
   /** Destructor */
   virtual ~TrimOperation();
protected:
   /** Procesa las fuentes de datos de entrada generando la diferencia de las capas en una nueva
    * fuente de datos */
   virtual VectorDatasource* DoProcessDatasource(
         VectorOperation::OperationType& Operation,
         std::vector<DatasourceInterface*>& Datasources);
};

} /** namespace suri */

#endif /* TRIMOPERATION_H_ */
