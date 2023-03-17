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

#ifndef POLYGON_H_
#define POLYGON_H_

// Includes standard
#include <vector>
#include <string>

// Include suri
#include "Geometry.h"
#include "Ring.h"

namespace suri {

/**
 * Geometry de tipo poligono. Un poligono esta compuesto por un anillo.
 * Para que el anillo sea valido debe estar cerrado y tener al menos 4 puntos.
 */
class Polygon : public Geometry {
public:
   /** Ctor */
   explicit Polygon(const std::string &SpatialReference = "");
   /** Ctor */
   explicit Polygon(const Polygon &APolygon);
   /** Dtor */
   virtual ~Polygon();

   /** Indica si la geometria es valida */
   virtual bool IsValid() const;
   /** Retorna una copia de la geometria. Tambie copia subgeometrias */
   virtual Geometry* Clone() const;
   /** Configura el srs de la geometria */
   virtual void AssignSpatialReference(const std::string &SpatialReference);

   /** Indica si la geometria esta cerrada, solo valida si es poligono */
   virtual bool IsClosed() const;
   /** Fuerza a que se cierre la geometria. Solo para poligonos */
   virtual bool CloseRings();

   /** Agrega un anillo al vector */
   bool AppendRing(Ring* pRing);
   /** Elimina un anillo del vector */
   bool RemoveRing(SuriObject::UuidType RingId);
   /** Retorla listado de anillos en vector */
   std::vector<SuriObject::UuidType> GetRings() const;
   /** Retorna la linea con id indicado */
   Ring* GetRing(SuriObject::UuidType RingId);
   /** Retorna la linea con id indicado */
   const Ring* GetRing(SuriObject::UuidType RingId) const;

private:
   typedef std::vector<Ring*>::const_iterator RingIterator;

   /** Busca un punto a partir del id */
   RingIterator FindRing(SuriObject::UuidType RingId) const;
   /** Valida el anillo */
   bool ValidateRing(const Ring* pPoint);

   /** Vector con puntos */
   std::vector<Ring*> ringVector_;

   static const int MINIMUN_RING_POINTCOUNT = 3;
};

} /* namespace suri */
#endif /* POLYGON_H_ */
