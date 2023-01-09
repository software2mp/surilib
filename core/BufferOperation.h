/*! \file BufferOperation.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef BUFFEROPERATION_H_
#define BUFFEROPERATION_H_

// Includes Estandar
// Includes Suri
#include "VectorOperation.h"
#include "VectorDatasource.h"
// Includes Wx
// Defines

namespace suri {
// forwards
class VectorDatasource;

class BufferOperation : public suri::VectorOperation {
public:
   /** Constructor */
   BufferOperation(const std::string& FieldName, int Nrings, double Distance,
                   double ConversionFactor);
   /** Destructor */
   virtual ~BufferOperation();

protected:
   /** Retorna el valor de la distancia */
   double GetDistance(double Distance, char* pWkt);
   /** Procesa la fuente de datos de entrada generando un buffer en una nueva fuente de datos */
   virtual VectorDatasource* DoProcessDatasource(VectorOperation::OperationType& Operation,
                                                 std::vector<DatasourceInterface*>& Datasources);
   std::string bufferfield_; /** Nombre del campo que posee la distancia del buffer */
   double distancevalue_; /** Valor de la distancia del buffer */
   int nrings_; /** Numero de anillos concentricos */
   double conversionfactor_; /** Factor de conversion */
};

} /** namespace suri */

#endif /* BUFFEROPERATION_H_ */
