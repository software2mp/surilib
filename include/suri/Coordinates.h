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

#ifndef COORDINATES_H_
#define COORDINATES_H_

#include <vector>

/** namespace suri */
namespace suri {
/** Clase con coordenadas X, Y, Z */
/**
 *  Esta clase contiene las coordenadas de un punto 2D/3D
 *  Permite comparar si dos coordenadas son iguales, y transformar una coordenada
 *  3D en 2D (hace Z=0.0).
 */
class Coordinates {
public:

   /** Constructor */
   Coordinates(const double &X = 0.0, const double &Y = 0.0, const double &Z = 0.0);

   /** Destructor */
   ~Coordinates();

   /** Proyecta la coordenada en el plano x-y (hace z=0.0) */
   Coordinates Make2D() const;

   /** Compara la coordenada con otra */
   bool IsEqual(const Coordinates &Coord) const;

   /** Compara la coordenada con otra despreciando la coordenada vertical (z) */
   bool IsEqual2D(const Coordinates &Coord) const;

   double x_; /*! Coordenada X */
   double y_; /*! Coordenada Y */
   double z_; /*! Coordenada Z */
protected:
private:
};

/** Operador igualdad */
bool operator==(const Coordinates &Lhs, const Coordinates &Rhs);

/** Operador desigualdad */
bool operator!=(const Coordinates &Lhs, const Coordinates &Rhs);

/** Lista de coordenadas */
typedef std::vector<Coordinates> CoordinateListType;
} /*namespace suri*/

#endif /*COORDINATES_H_*/
