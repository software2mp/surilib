/*! \file VertexSnapStrategy.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VERTEXSNAPSTRATEGY_H_
#define VERTEXSNAPSTRATEGY_H_

// Includes Suri
#include "SnapInterface.h"

namespace suri {

// Modifica las coordenadas de un punto acoplandolo al vertice mas cercano de la geometria
class VertexSnapStrategy : public SnapInterface {
public:
   /** Constructor */
   VertexSnapStrategy();
   /** Destructor */
   virtual ~VertexSnapStrategy();
   /* Modifica las coordenadas de un punto acoplandolo al vertice mas cercano de la geometria */
   virtual bool DoSnap(Coordinates &ViewportPosition, OGRPoint* PhantomPoint);
   /** Retorna el tipo de estrategia que utiliza el snapping */
   virtual std::string GetStrategyType();
};

} /** namespace suri */

#endif /* VERTEXSNAPSTRATEGY_H_ */
