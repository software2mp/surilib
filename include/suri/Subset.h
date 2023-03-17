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

#ifndef SUBSET_H_
#define SUBSET_H_

// Includes suri
#include <ostream>
#include "Coordinates.h"

/** namespace suri */
#include <suri/Serializable.h>

namespace suri {
/** Clase que guarda coordenadas limites de un rectangulo */
/**
 *  Al estar definido por las coordenadas UpperLeft y LowerRight
 * puede inferirse de esta informacion el sentido de los ejes del
 * sistema coordenado en el que esta expresado el Subset.
 * \see Dimension
 */
class Subset : public Serializable{
public:
   /** ctor */
   Subset() : ul_(0, 0, 0), lr_(0, 0, 0) {
   }
   /** ctor */
   Subset(double Ulx, double Uly, double Lrx, double Lry);
   /** ctor */
   Subset(const Coordinates &UL, const Coordinates &LR);
   /** Verifica si el punto cae adentro */
   bool IsInside(const Coordinates &Point) const;
   /** Encuentra el punto mas cercano dentro del subset */
   Coordinates NearestPoint(const Coordinates &Point);
   /** Verfica que sea un subset valido */
   bool IsValid() const;
   /** Obtiene el centro */
   Coordinates GetCenter() const;

   /** Calcula la interseccion de dos Subset */
   Subset Intersect(const Subset &other) const;
   /**
    * Extrae un vector de coordenadas con una determinada distancia
    * entre cada una.
    */
   static std::vector<Coordinates> ExtractCoordinates(const Subset& SubsetP,
                                                      int PointsCountP, int DeltaP);

   friend std::ostream & operator<<(std::ostream &os, const Subset& Asubset);

   wxXmlNode* SerializeXml() const;

   static Subset* DeserializeXml(wxXmlNode* pRoot);
   /** Devuelve el subset desplazado a la coordenada 0,0 **/
   Subset ToOrigin();

   Coordinates ul_; /*! coordenada de vertice superior izquierdo */
   Coordinates lr_; /*! coordenada de vertice inferior derecho */
};

/** Operador igualdad */
bool operator==(const Subset &Lhs, const Subset &Rhs);

/** Operador desigualdad */
bool operator!=(const Subset &Lhs, const Subset &Rhs);

/** Calcula la interseccion de dos Subset */
Subset Intersect(const Subset& Subset1, const Subset &Subset2);

/** operator suma de una coordenada a un subset (corrimiento) */
Subset operator+(const Subset &Lhs, const Coordinates &Rhs);
/** operador de suma de un entero a un subset (lo hace crecer) */
Subset operator+(const Subset &Lhs, double Buffer);

/** Retorna un Subset que contiene a Subset1 y Subset2 */
Subset Join(const Subset &Subset1, const Subset &Subset2);

/** Obtiene el subset representado por un stream **/
std::ostream & operator<<(std::ostream &os, const Subset& Asubset);

}

#endif /*SUBSET_H_*/
