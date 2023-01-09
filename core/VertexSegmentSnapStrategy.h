/*! \file VertexSegmentSnapStrategy.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VERTEXSEGMENTSNAPSTRATEGY_H_
#define VERTEXSEGMENTSNAPSTRATEGY_H_

// Includes Suri
#include "SnapInterface.h"

namespace suri {

// Modifica las coordenadas de un punto acoplandolo al punto mas cercano de la geometria/vertice
class VertexSegmentSnapStrategy : public SnapInterface {
public:
   /** Constructor */
   VertexSegmentSnapStrategy();
   /** Destructor */
   virtual ~VertexSegmentSnapStrategy();
   /** Modifica las coord de un punto acoplandolo al punto mas cercano de la geometria/vertice */
   virtual bool DoSnap(Coordinates &ViewportPosition, OGRPoint* PhantomPoint);
   /** Retorna el tipo de estrategia que utiliza el snapping */
   virtual std::string GetStrategyType();

private:
   /** Configura el entorno de la estrategia a utilizar */
   void ConfigureStrategyEnvironment(SnapInterface* pSnap);
};

} /** namespace suri */

#endif /* VERTEXSEGMENTSNAPSTRATEGY_H_ */
