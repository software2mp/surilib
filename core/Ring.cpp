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

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "Ring.h"

namespace suri {

/** Ctor */
Ring::Ring(const std::string &SpatialReference) : Line(SpatialReference) {
}

/** Ctor */
Ring::Ring(const Ring &ARing) : Line(ARing) {
}

/** Dtor */
Ring::~Ring () {
}

/**
 * Indica si la geometria es valida
 * @return bool que indica si el anillo es valido
 */
bool Ring::IsValid() const {
   return GetSize() >= MINIMUN_LINE_POINTCOUNT;
}

/**
 * Retorna una copia de la geometria. Tambien copia subgeometrias
 * @return copia del anillo.
 */
Geometry* Ring::Clone() const {
   return new Ring(*this);
}

/**
 * Indica si la geometria esta cerrada, solo valida si es poligono
 * @return siempre true
 */
bool Ring::IsClosed() const {
   return true;
}

/**
 * Retorna listado de puntos en vector
 * @return vector con el id de los puntos en vector. El primer
 * se repite al final ya que geometria esta cerrada.
 */
std::vector<SuriObject::UuidType> Ring::GetPoints() const {
   std::vector<SuriObject::UuidType> points = Line::GetPoints();
   if (points.size() > 1)
      points.push_back(points[0]);
   return points;
}

/**
 * Retorna la cantidad de puntos en el vector
 * @return Cantidad de puntos. Incluye el punto final.
 */
int Ring::GetSize() const {
   return Line::GetSize() + 1;
}

} /* namespace suri */
