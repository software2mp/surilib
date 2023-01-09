/*! \file UnionOperation.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef UNIONOPERATION_H_
#define UNIONOPERATION_H_

// Includes Estandar
// Includes Suri
#include "VectorOperation.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class UnionOperation : public suri::VectorOperation {
public:
   /** Constructor */
   UnionOperation();
   /** Destructor */
   virtual ~UnionOperation();
protected:
   /** Procesa las fuentes de datos de entrada generando la union de ambas
    * en una nueva fuente de datos */
   virtual VectorDatasource* DoProcessDatasource(VectorOperation::OperationType& Operation,
                                                 std::vector<DatasourceInterface*>& Datasources);
   /** Realiza la interseccion de las geometrias sin generar duplicados */
   void DoGroupIntersection(std::multimap<int, OGRGeometry*> PendingGeoms,
                            OGRLayer* pLayer, OGRLayer* pSecondLayer, OGRLayer* pDestLayer);
};

} /** namespace suri */

#endif /* UNIONOPERATION_H_ */
