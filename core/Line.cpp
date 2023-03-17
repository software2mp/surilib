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

// Includes suri
#include "Line.h"

namespace suri {

/** Ctor */
Line::Line(const std::string &SpatialReference)  : Geometry(SpatialReference) {
}

/** Ctor */
Line::Line(const Line &ALine) : Geometry(ALine) {
   ConstPointIterator it = ALine.pointVector_.begin();
   for (; it != ALine.pointVector_.end(); ++it)
      AppendPoint(new Point(*(*it)));
}

/** Dtor */
Line::~Line() {
   ConstPointIterator it = pointVector_.begin();
   for (; it != pointVector_.end(); ++it)
      delete *it;
   pointVector_.clear();
}


/** 
 * Indica si la geometria es valida.
 * @return bool que indica si la geometria es valida.
 */
bool Line::IsValid() const {
   return GetSize() >= MINIMUN_LINE_POINTCOUNT;
}

/** 
 * Retorna una copia de la geometria. Tambien copia subgeometrias 
 * @return geometria clonada
 */
Geometry* Line::Clone() const {
   Line* pnewline = new Line(*this);
   return pnewline;
}

/**
 * Configura el srs de la geometria
 * @param[in] SpatialReference srs a configurar
 */
void Line::AssignSpatialReference(const std::string &SpatialReference) {
   PointIterator it = pointVector_.begin();
   for (;it != pointVector_.end(); ++it)
      (*it)->AssignSpatialReference(SpatialReference);
   Geometry::AssignSpatialReference(SpatialReference);
}

/**
 * Indica si la geometria esta cerrada, solo valida si es poligono
 * @return bool indicado si la geometria esta cerrada
 */
bool Line::IsClosed() const {
   return false;
}

/**
 * Agrega un punto al vector
 * @param[in] pPoint punto que quiero agregar.
 * \attention El punto pasa a ser responsabilidad de esta clase. No eliminar.
 */
bool Line::AppendPoint(Point* pPoint) {
   if (!ValidatePoint(pPoint))
      return false;
   pointVector_.push_back(pPoint);
   return true;
}

/**
 * Agrega un punto en la posicion indicada
 * @param[in] Position posicion del punto que quiero agregar
 * @param[in] pPoint punto que quiero agregar
 * \attention El punto pasa a ser responsabilidad de esta clase. No eliminar.
 * @return bool que indica si se pudo agregar el punto.
 */
bool Line::InsertPointBefore(Point* pPoint,
                     const SuriObject::UuidType &ReferencePointId) {
   PointIterator it = FindPoint(ReferencePointId);
   if (!ValidatePoint(pPoint) || it == pointVector_.end())
      return false;
   pointVector_.insert(it, pPoint);
   return true;
}

/**
 * Elimina un punto de la linea
 * @param[in] PointId Id del punto que quiero eliminar
 * @return bool que indica si se pudo eliminar el punto.
 */
bool Line::DeletePoint(const SuriObject::UuidType &PointId) {
   Point* ppoint = RemovePoint(PointId);
   delete ppoint;
   return (ppoint != NULL);
}

/**
 * Saca un punto del vector sin eliminarlo
 * @param[in] PointId Id del punto que quiero remover
 * @return puntero al punto removido
 * @return NULL si no pudo encontrar el punto
 */
Point* Line::RemovePoint(const SuriObject::UuidType &PointId) {
   std::vector<Point*>::iterator it = pointVector_.begin();

   for (;it != pointVector_.end() && (*it)->GetId() != PointId; ++it);

   Point* ppoint = NULL;
   if (it != pointVector_.end()) {
      ppoint = *it;
      pointVector_.erase(it);
   }
   return ppoint;
}

/**
 * Cambia de posicion un punto en el vector.
 * @param[in] PointId id del punto que quiero mover.
 * @param[in] NewPosition nueva posicion del punto.
 * @return bool que indica si se puede mover el punto.
 */
bool Line::MovePoint(const SuriObject::UuidType &PointId, Coordinates NewPosition) {
   Line::ConstPointIterator it = FindPoint(PointId);
   if (it != pointVector_.end())
      (*it)->SetCoordinate(NewPosition);
   return true;
}

/**
 * Retorna listado de puntos en vector
 * @return vector con el id de los puntos que contiene
 */
std::vector<SuriObject::UuidType> Line::GetPoints() const {
   std::vector<SuriObject::UuidType> returnvector;
   ConstPointIterator it = pointVector_.begin();
   for (; it != pointVector_.end(); ++it)
      returnvector.push_back((*it)->GetId());
   return returnvector;
}

/**
 * Retorna punto de linea con id pedido
 * @param[in] PointId id del punto que busco
 * @return punto que busco o NULL si no lo encuentra.
 */
const Point* Line::GetPoint(const SuriObject::UuidType &PointId) const {
   ConstPointIterator it = FindPoint(PointId);
   return (it != pointVector_.end()) ? *it : NULL;
}

/**
 * Retorna punto de linea con id pedido
 * @param[in] PointId id del punto que busco
 * @return punto que busco o NULL si no lo encuentra.
 */
Point* Line::GetPoint(const SuriObject::UuidType &PointId) {
   PointIterator it = FindPoint(PointId);
   return (it != pointVector_.end()) ? *it : NULL;
}

/**
 * Retorna la cantidad de puntos en el vector
 * @return cantidad de puntos en vector.
 */
int Line::GetSize() const {
   return pointVector_.size();
}

/**
 * Busca un punto a partir del id
 * @return iterador apuntando a punto con id correspondiente
 */
Line::ConstPointIterator Line::FindPoint(const SuriObject::UuidType &PointId) const {
   ConstPointIterator it = pointVector_.begin();
   while (it != pointVector_.end() && (*it)->GetId() != PointId)
      ++it;
   return it;
}

/** Busca un punto a partir del id */
Line::PointIterator Line::FindPoint(const SuriObject::UuidType &PointId) {
   PointIterator it = pointVector_.begin();
   while (it != pointVector_.end() && (*it)->GetId() != PointId)
      ++it;
   return it;
}

/**
 * Valida el punto
 * @param[in] pPoint punto a validar
 * @return bool que indica si se puede agregar el punto a la geometria.
 */
bool Line::ValidatePoint(const Point* pPoint) {
   if(!pPoint)
      return false;
   Line::ConstPointIterator it = FindPoint(pPoint->GetId());
   return it == pointVector_.end() &&
             pPoint->GetSpatialReference() == GetSpatialReference();
}

} /* namespace suri */
