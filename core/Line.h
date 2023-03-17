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

#ifndef LINE_H_
#define LINE_H_

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "Geometry.h"
#include "Point.h"

namespace suri {

/**
 * Geometry de tipo linea.
 * Para que sea valida debe contener por lo menos dos puntos.
 */
class Line : public Geometry {
public:
   /** Ctor */
   explicit Line(const std::string &SpatialReference = "");
   /** Ctor */
   explicit Line(const Line &ALine);
   /** Dtor */
   virtual ~Line();

   /** Indica si la geometria es valida */
   virtual bool IsValid() const;
   /** Retorna una copia de la geometria. Tambie copia subgeometrias */
   virtual Geometry* Clone() const;
   /** Configura el srs en el que estan los puntos de la geometria */
   virtual void AssignSpatialReference(const std::string &SpatialReference);

   /** Indica si la geometria esta cerrada, solo valida si es poligono */
   virtual bool IsClosed() const;

   /** Agrega un punto al final de la linea */
   virtual bool AppendPoint(Point* pPoint);
   /** Agrega un punto en la posicion indicada */
   virtual bool InsertPointBefore(Point* pPoint,
                  const SuriObject::UuidType &ReferencePointId);
   /** Elimina un punto del vector */
   virtual bool DeletePoint(const SuriObject::UuidType &PointId);
   /** Saca un punto del vector sin eliminarlo */
   virtual Point* RemovePoint(const SuriObject::UuidType &PointId);
   /** Cambia de posicion un punto en el vector. */
   virtual bool MovePoint(const SuriObject::UuidType &PointId,
                                             Coordinates NewPosition);
   /** Retorla listado de puntos en vector */
   virtual std::vector<SuriObject::UuidType> GetPoints() const;
   /** Retorna punto de linea con id correspondiente */
   virtual const Point* GetPoint(const SuriObject::UuidType &PointId) const;
   /** Retorna punto de linea con id correspondiente */
   virtual Point* GetPoint(const SuriObject::UuidType &PointId);
   /** Retorna la cantidad de puntos en el vector */
   virtual int GetSize() const;

protected:
   typedef std::vector<Point*>::const_iterator ConstPointIterator;
   typedef std::vector<Point*>::iterator PointIterator;

   /** Busca un punto a partir del id */
   ConstPointIterator FindPoint(const SuriObject::UuidType &PointId) const;
   /** Busca un punto a partir del id */
   PointIterator FindPoint(const SuriObject::UuidType &PointId);
   /** Valida el punto */
   bool ValidatePoint(const Point* pPoint);

   /** Vector con puntos */
   std::vector<Point*> pointVector_;
private:
   static const int MINIMUN_LINE_POINTCOUNT = 2;
};

} /* namespace suri */
#endif /* LINE_H_ */
