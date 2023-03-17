/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
