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

#ifndef GEOMETRYEDITOR_H_
#define GEOMETRYEDITOR_H_

// Includes standard
#include <vector>
#include <set>
#include <string>

// Includes suri
#include "Geometry.h"
#include "suri/SuriObject.h"
#include "suri/Subset.h"
#include "suri/Coordinates.h"
#include "suri/RasterSpatialModel.h"

namespace suri {

// fordwards
class Point;
class Line;
class Ring;

/**
 * Facade que permite editar una geometria.
 * Para simplificar la edicion, trata a la geomtria como un listado de puntos.
 * Dependiendo del tipo concreto de geometria estan soportadas las operaciones de
 * agregar/remover puntos.
 */
class SimpleGeometryEditor {
   /** Ctor */
   explicit SimpleGeometryEditor(Ring* pRing);
   /** Ctor */
   explicit SimpleGeometryEditor(Line* pLine);
   /** Ctor */
   explicit SimpleGeometryEditor(Point* pPoint);

public:
   enum GeometryType {SimplePoint = 0, SimpleLine, SimpleRing};

   /** Dtor */
   virtual ~SimpleGeometryEditor();

   /** Retorna los puntos en la geometria */
   std::vector<SuriObject::UuidType> GetPoints() const;
   /** Retorna la coordenada del punto con id indicado */
   bool GetPointCoordinate(const SuriObject::UuidType &PointId,
                                Coordinates &PointCoordinate) const;
   /** Retorna los puntos contenidos en el subset */
   std::set<SuriObject::UuidType> FindPoints(const Subset &SearchSubset,
                                      const std::string &SubsetSrs) const;

   /** Mueve un punto en la geoemtria */
   bool MovePoint(const SuriObject::UuidType &PointId,
                                 const Coordinates &NewCoordinate);
   /** Elimina un punto de la geometria */
   bool DeletePoint(const SuriObject::UuidType &PointId);
   /** Agrega un punto antes del punto indicado */
   SuriObject::UuidType InsertPointBefore(const Coordinates &PointCoordinate,
                                 const SuriObject::UuidType &ReferencePointId);
   /** Agrega un punto al final de la geometria */
   SuriObject::UuidType AppendPoint(const Coordinates &PointCoordinate);

   /** Configura estilo de la geometria completa */
   bool SetStyle(VectorStyle* pNewStyle);
   /** Configura el estilo de un punto en la geometria */
   bool SetStyle(const SuriObject::UuidType &PointId, VectorStyle* pNewStyle);
   /** Retorna el estilo que corresponde al la geometria indicada */
   VectorStyle* GetStyle(const VectorStyle* pDefaultStyle,
              const SuriObject::UuidType &PointId = SuriObject::NullUuid);
   /** Retorna el tipo de la geometria editada */
   GeometryType GetType();
   /** Retorna el srs de la geometria editada */
   std::string GetSpatialReference();

   /** Asigna un srs a la geometria. Transforma coordenadas para que esten en srs */
   bool AddSpatialReference(RasterSpatialModel* pRasterSpatialModel,
                                         const std::string &SpatialModelSrs);

   /** 
    * Inicializa un editor con la geometria que recibe por parametro 
    * Si la geometria es un poligono, trabaja con el primer anillo.
    * Si la geometria es un punto, no permite agregar/remover puntos.
    */
   static SimpleGeometryEditor* InitializeEditor(Geometry* pGeometry);

private:
   /** Indica si se puede modificar la longitud de la lista */
   bool CanChangeLineSize();
   /** Retorna la posicion de un punto en la linea */
   int GetPointPosition(const SuriObject::UuidType &PointId);
   /** Linea en edicion */
   Line* pEditedLine_;
   /** Tipo de geometria editado */
   GeometryType type_;
};

} /* namespace suri */

#endif /* GEOMETRYEDITOR_H_ */
