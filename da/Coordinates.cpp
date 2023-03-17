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

#include "suri/Coordinates.h"

// Includes standard
#include <cmath>

// Includes suri
#include "suri/AuxiliaryFunctions.h"

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] X Coordenada X
 * @param[in] Y Coordenada Y
 * @param[in] Z Coordenada Z
 * @return instancia de la clase Coordinates
 */
Coordinates::Coordinates(const double &X, const double &Y, const double &Z) :
      x_(X), y_(Y), z_(Z) {
}

/** Destructor */
Coordinates::~Coordinates() {
}

/**
 * Proyecta la coordenada en el plano x-y (hace z=0.0)
 * @return Coordenadas en el plano x-y
 */
Coordinates Coordinates::Make2D() const {
   return Coordinates(x_, y_, 0.0);
}

/**
 * Compara la coordenada con otra (compara x,y,z)
 * @param[in] Coord Coordenada a comparar
 * @return True si son iguales
 */
bool Coordinates::IsEqual(const Coordinates &Coord) const {
   // Se aumenta la precision porque quiero distinguir centesimas de segundo
   return FLOAT_COMPARE_WITH_PRECISION(x_, Coord.x_, 0.000000001, 0.000000001)
         && FLOAT_COMPARE_WITH_PRECISION(y_, Coord.y_, 0.000000001, 0.000000001)
         && FLOAT_COMPARE_WITH_PRECISION(z_, Coord.z_, 0.000000001, 0.000000001);
}

/**
 * Compara la coordenada con otra despreciando la coordenada vertical (z)
 * @param[in] Coord Coordenada a comparar
 * @return True si son iguales en el plano x-y
 */
bool Coordinates::IsEqual2D(const Coordinates &Coord) const {
   return FLOAT_COMPARE_WITH_PRECISION(x_, Coord.x_, 0.000000001, 0.000000001)
         && FLOAT_COMPARE_WITH_PRECISION(y_, Coord.y_, 0.000000001, 0.000000001);
}
} /*namespace suri*/

