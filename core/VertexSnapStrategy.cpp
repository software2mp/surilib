/*! \file VertexSnapStrategy.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <limits>

// Includes Suri
#include "VertexSnapStrategy.h"
#include "suri/Coordinates.h"
#include "suri/World.h"

namespace suri {

/** Constructor */
VertexSnapStrategy::VertexSnapStrategy() {
}

/** Destructor */
VertexSnapStrategy::~VertexSnapStrategy() {
}

/**
 * Modifica las coordenadas de un punto acoplandolo al vertice mas cercano de la geometria
 * @param[in] ViewportPosition punto fantasma que quiere agregarse
 * @param[in] PhantomPoint punto fantasma que quiere agregarse con referencia espacial
 */
bool VertexSnapStrategy::DoSnap(Coordinates &ViewportPosition, OGRPoint* PhantomPoint) {
   // Itero las geometrias existentes para ver si hay interseccion
   int index = -1;
   double min = std::numeric_limits<double>::max();
   for (size_t geom = 0; geom < vertexbuffers_.size(); ++geom) {
      if (!PhantomPoint->Within(vertexbuffers_[geom]))
         continue;
      double distance = PhantomPoint->Distance(vertexgeometries_[geom]);
      if (distance < min) {
         index = geom;
         min = distance;
      }
   }

   if (index < 0)
      return false;

   // Obtengo el punto que se quiere crear
   Coordinates phanthomcoord;
   pWorld_->Transform(ViewportPosition, phanthomcoord);

   // Calculo el punto de interseccion entre ambas rectas m * x + b = mp * x + bp
   phanthomcoord.x_ = static_cast<OGRPoint*>(vertexgeometries_[index])->getX();
   phanthomcoord.y_ = static_cast<OGRPoint*>(vertexgeometries_[index])->getY();

   // Muevo el punto a las coordenadas de la interseccion
   pWorld_->InverseTransform(phanthomcoord, ViewportPosition);
   return true;
}

/** Retorna el tipo de estrategia que utiliza el snapping */
std::string VertexSnapStrategy::GetStrategyType() {
   return "Vertex";
}

} /** namespace suri */
