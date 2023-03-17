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
#include "SegmentSnapStrategy.h"
#include "suri/Coordinates.h"
#include "suri/World.h"
#include "suri/AuxiliaryFunctions.h"

namespace suri {

/** Constructor */
SegmentSnapStrategy::SegmentSnapStrategy() : index_(-1) {
}

/** Destructor */
SegmentSnapStrategy::~SegmentSnapStrategy() {
}

/**
 * Modifica las coordenadas de un punto acoplandolo al punto mas cercano de la geometria
 * @param[in] ViewportPosition punto fantasma que quiere agregarse
 * @param[in] PhantomPoint punto fantasma que quiere agregarse con referencia espacial
 */
bool SegmentSnapStrategy::DoSnap(Coordinates &ViewportPosition, OGRPoint* PhantomPoint) {
   // Itero las geometrias existentes para ver si hay interseccion
   index_ = -1;
   if (!FindNearestGeometryIndex(PhantomPoint))
      return false;

   int position = 0;
   std::vector<OGRPoint*> vertex;
   GetGeometryVertex(PhantomPoint, vertex, position);

   // Obtengo el punto que se quiere crear
   Coordinates phanthomcoorda, phanthomcoordb;
   pWorld_->Transform(ViewportPosition, phanthomcoorda);
   pWorld_->Transform(ViewportPosition, phanthomcoordb);

   /**
    * Obtengo la ecuacion de dos segmentos:
    * 1. Segmento que corresponde al vertice mas cercano respecto al punto fantasma y
    * al vertice anterior.
    * Si position es 0 el vertice anterior es el ultimo vertice del poligono (vertex.size() - 1).
    * 2. Segmento que corresponde al vertice mas cercano respecto al punto fantasma y
    * al vertice posterior.
    * Si position es vertex.size() el vertice posterior es 0.
    */

   // Segmento #1
   double nearx = vertex[position]->getX();
   double neary = vertex[position]->getY();
   Coordinates nearvertex(nearx, neary);

   double previousx =
         (position == 0) ? vertex[vertex.size() - 1]->getX() : vertex[position - 1]->getX();
   double previousy =
         (position == 0) ? vertex[vertex.size() - 1]->getY() : vertex[position - 1]->getY();
   Coordinates previousvertex(previousx, previousy);
   GetIntersectionPoint(PhantomPoint, nearvertex, previousvertex, phanthomcoorda);

   // Segmento #2
   double nextx =
         (position == (int)vertex.size() - 1) ? vertex[0]->getX() : vertex[position + 1]->getX();
   double nexty =
         (position == (int)vertex.size() - 1) ? vertex[0]->getY() : vertex[position + 1]->getY();
   GetIntersectionPoint(PhantomPoint, nearvertex, Coordinates(nextx, nexty), phanthomcoordb);

   double distancea = GetDistance(PhantomPoint, phanthomcoorda.x_, phanthomcoorda.y_);
   double distanceb = GetDistance(PhantomPoint, phanthomcoordb.x_, phanthomcoordb.y_);

   if (distancea > distanceb || distancea == distanceb) {
      if (IsInDomain(nearx, nextx, phanthomcoordb.x_) ||
            IsInDomain(neary, nexty, phanthomcoordb.y_))
         pWorld_->InverseTransform(phanthomcoordb, ViewportPosition);
      else if (IsInDomain(nearx, previousx, phanthomcoorda.x_) ||
            IsInDomain(neary, previousy, phanthomcoorda.y_))
         pWorld_->InverseTransform(phanthomcoorda, ViewportPosition);
      else
         pWorld_->InverseTransform(nearvertex, ViewportPosition);
   } else if (distanceb > distancea) {
      if (IsInDomain(nearx, previousx, phanthomcoorda.x_) ||
            IsInDomain(neary, previousy, phanthomcoorda.y_))
         pWorld_->InverseTransform(phanthomcoorda, ViewportPosition);
      else if (IsInDomain(nearx, nextx, phanthomcoordb.x_) ||
            IsInDomain(neary, nexty, phanthomcoordb.y_))
         pWorld_->InverseTransform(phanthomcoordb, ViewportPosition);
      else
         pWorld_->InverseTransform(nearvertex, ViewportPosition);
   }

   for (size_t i = 0; i < vertex.size(); ++i)
      delete vertex[i];

   return true;
}

/**
 * Retorna un valor booleano que indica si encontro una geometria dentro del area de tolerancia
 * @param[in] PhantomPoint punto original
 * @return true si encontro una geometria que aplique, false en caso contrario
 */
bool SegmentSnapStrategy::FindNearestGeometryIndex(OGRPoint* PhantomPoint) {
   bool finded = false;
   double distance = 0;
   for (size_t geom = 0; geom < segmentbuffers_.size(); ++geom) {
      if (PhantomPoint->Within(segmentbuffers_[geom])) {
         if (index_ == -1)
            distance = PhantomPoint->Distance(segmentgeometries_[geom]);
         if (PhantomPoint->Distance(segmentgeometries_[geom]) <= distance) {
            distance = PhantomPoint->Distance(segmentgeometries_[geom]);
            index_ = geom;
            finded = true;
         }
      }
   }
   return finded;
}

/**
 * Obtiene un vector con todos los vertices de la geometria mas cercana al punto fantasma
 * @param[in] Vertex referencia al vector con los vertices
 * @param[in] Position referencia a la posicion del vertice mas cercano
 */
void SegmentSnapStrategy::GetGeometryVertex(
      OGRPoint* PhantomPoint, std::vector<OGRPoint*> &Vertex, int &Position) {
   OGRwkbGeometryType type = segmentgeometries_[index_]->getGeometryType();
   OGRLinearRing* pring = NULL; // Este puntero no debe ser borrado ya que forma parte de la geom
   int lenix = -1;
   switch (type) {
      case wkbPolygon:
      {
         pring = static_cast<OGRPolygon*>(segmentgeometries_[index_])->getExteriorRing();
         lenix = pring->getNumPoints() - 1;
         break;
      }
      case wkbLineString:
      {
         lenix = static_cast<OGRLineString*>(segmentgeometries_[index_])->getNumPoints();
         break;
      }
      default:
         break;
   }

   double min = std::numeric_limits<double>::max();
   // Itero todos los vertices de la geometria
   for (int ix = 0; ix < lenix; ++ix) {
      OGRPoint* point = new OGRPoint();
      switch (type) {
         case wkbPolygon:
         {
            pring->getPoint(ix, point);
            break;
         }
         case wkbLineString:
         {
            static_cast<OGRLineString*>(segmentgeometries_[index_])->getPoint(ix, point);
            break;
         }
         default: {
            point = static_cast<OGRPoint*>(segmentgeometries_[index_]);
            break;
         }
      }
      // Calculo la distancia entre el punto fantasma y cada uno de los vertices
      double distance = sqrt(
            pow(PhantomPoint->getX() - point->getX(), 2.0)
                  + pow(PhantomPoint->getY() - point->getY(), 2.0));
      // Guardo la posicion del vertice mas cercano al punto fantasma
      if (distance < min) {
         min = distance;
         Position = ix;
      }
      Vertex.push_back(point);
   }
}

/** Retorna el tipo de estrategia que utiliza el snapping */
std::string SegmentSnapStrategy::GetStrategyType() {
   return "Segment";
}

/**
 * Obtiene el punto de interseccion del punto fantasma con un segmento
 * @param[in] PhantomPoint puntero al punto fantasma
 * @param[in] NearVertex coordenadas del vertice mas cercano al punto fantasma
 * @param[in] OtherVertex coordenadas del vertice anterior/posterior al vertice mas cercano
 * @param[in] Intersection referencia a las coordenadas de la interseccion
 */
void SegmentSnapStrategy::GetIntersectionPoint(OGRPoint* PhantomPoint, Coordinates NearVertex,
                                               Coordinates OtherVertex, Coordinates &Intersection) {
   double m = (OtherVertex.y_ - NearVertex.y_) / (OtherVertex.x_ - NearVertex.x_);

   // Si m no existe la funcion es x = b con lo cual unicamente tengo la coordenada X
   if (SURI_ISNAN(m)) {
      Intersection.x_ = NearVertex.x_;
      return;
   }

   if (std::isinf(m) || m == 0) {
      OGRGeometry* xprojection = PhantomPoint->clone();
      OGRGeometry* yprojection = PhantomPoint->clone();
      static_cast<OGRPoint*>(xprojection)->setX(NearVertex.x_);
      static_cast<OGRPoint*>(yprojection)->setY(NearVertex.y_);

      if (segmentgeometries_[index_]->Intersects(xprojection)) {
         Intersection.x_ = NearVertex.x_;
      } else if (segmentgeometries_[index_]->Intersects(yprojection)) {
         Intersection.y_ = NearVertex.y_;
      } else {
         Intersection.x_ = NearVertex.x_;
         Intersection.y_ = NearVertex.y_;
      }

      delete xprojection;
      delete yprojection;
      return;
   }

   double b = NearVertex.y_ - (m * NearVertex.x_);

   // Obtengo la pendiente de la recta perpendicular
   double mp = -1 / m;

   // Utilizo la ecuacion de la recta que pasa por (phanthomcoord.x_, phanthomcoord.y_)
   // y = neary + m(x - nearx)
   double bp = -1 * mp * Intersection.x_ + Intersection.y_;

   // Obtengo el primer punto de interseccion
   Intersection.x_ = (bp - b) / ( m - mp);
   Intersection.y_ = mp * Intersection.x_ + bp;
}

/**
 * Retorna la distancia entre un punto y una geometria
 * @param[in] Geometry geometria origen
 * @param[in] X coordenadas en X del punto
 * @param[in] Y coordenadas en Y del punto
  */
double SegmentSnapStrategy::GetDistance(OGRGeometry* Geometry, double X, double Y) {
   OGRPoint* intersection = new OGRPoint();
   intersection->setX(X);
   intersection->setY(Y);
   return Geometry->Distance(intersection);
}

/**
 * Retorna un booleano que indica si el punto especificado se encuentra en el dominio de una recta
 * @param[in] StartPoint punto inicial del dominio de la recta
 * @param[in] EndPoint punto final del dominio de la recta
 * @param[in] Point punto que quiero saber si se encuentra en la recta
 * @return success true el punto especificado se encuentra en el dominio de una recta
 */
bool SegmentSnapStrategy::IsInDomain(double StartPoint, double EndPoint, double Point) {
   bool success = false;
   std::vector<double> domain;
   domain.push_back(StartPoint);
   domain.push_back(EndPoint);
   std::sort(domain.begin(), domain.end());
   if (Point > domain[0] && Point < domain[1])
      success = true;
   return success;
}

} /** namespace suri */
