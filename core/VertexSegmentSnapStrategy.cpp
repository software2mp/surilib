/*! \file VertexSegmentSnapStrategy.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Suri
#include "VertexSegmentSnapStrategy.h"
#include "VertexSnapStrategy.h"
#include "SegmentSnapStrategy.h"

namespace suri {

/** Constructor */
VertexSegmentSnapStrategy::VertexSegmentSnapStrategy() {
}

/** Destructor */
VertexSegmentSnapStrategy::~VertexSegmentSnapStrategy() {
}

/**
 * Modifica las coordenadas de un punto acoplandolo al punto mas cercano de la geometria/vertice
 * @param[in] ViewportPosition punto fantasma que quiere agregarse
 * @param[in] PhantomPoint punto fantasma que quiere agregarse con referencia espacial
 */
bool VertexSegmentSnapStrategy::DoSnap(Coordinates &ViewportPosition, OGRPoint* PhantomPoint) {
   VertexSnapStrategy* vertex = new VertexSnapStrategy();
   ConfigureStrategyEnvironment(vertex);
   if (!vertex->DoSnap(ViewportPosition, PhantomPoint)) {
      SegmentSnapStrategy* segment = new SegmentSnapStrategy();
      ConfigureStrategyEnvironment(segment);
      segment->DoSnap(ViewportPosition, PhantomPoint);
      delete segment;
   }
   delete vertex;
   return true;
}

/**
 * Configura el entorno de la estrategia a utilizar
 * param[in] pSnap estrategia concreta
 */
void VertexSegmentSnapStrategy::ConfigureStrategyEnvironment(SnapInterface* pSnap) {
   pSnap->SetWorld(pWorld_);
   pSnap->SetVertexGeometries(vertexgeometries_);
   pSnap->SetVertexBufferGeometries(vertexbuffers_);
   pSnap->SetSegmentGeometries(segmentgeometries_);
   pSnap->SetSegmentBufferGeometries(segmentbuffers_);
}

/** Retorna el tipo de estrategia que utiliza el snapping */
std::string VertexSegmentSnapStrategy::GetStrategyType() {
   return "VertexSegment";
}

} /** namespace suri */
