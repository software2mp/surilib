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

#ifndef EXTENT_H_
#define EXTENT_H_

// Includes estandar

// Includes Suri
#include "suri/Subset.h"
#include "suri/Coordinates.h"

// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Clase que representa una extension a partir de un subset. */
/**
 * Clase que recibe un subset y transforma las coordenadas UL y LR en coordenadas
 * minimo y maximo.
 */
class Extent {
public:
   /** Ctor. */
   Extent(const Subset &InputSubset);
   /** Ctor. */
   Extent(double MinX, double MinY, double MaxX, double MaxY);
   /** Dtor. */
   virtual ~Extent();
   Coordinates min_; /*! Coordenada con los valores minimos para x e y */
   Coordinates max_; /*! Coordenada con los valores minimos para x e y */
};

/** Calcula la interseccion de dos Subset */
Extent Intersect(const Extent &Extent1, const Extent &Extent2);
/** Operador igualdad */
bool operator==(const Extent &Extent1, const Extent &Extent2);
}

#endif /* EXTENT_H_ */
