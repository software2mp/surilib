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

#ifndef SEGMENTSNAPSTRATEGY_H_
#define SEGMENTSNAPSTRATEGY_H_

// Includes Suri
#include "SnapInterface.h"

namespace suri {

// Modifica las coordenadas de un punto acoplandolo al punto mas cercano de la geometria
class SegmentSnapStrategy : public SnapInterface {
public:
   /** Constructor */
   SegmentSnapStrategy();
   /** Destructor */
   virtual ~SegmentSnapStrategy();
   /** Modifica las coordenadas de un punto acoplandolo al punto mas cercano de la geometria */
   virtual bool DoSnap(Coordinates &ViewportPosition, OGRPoint* PhantomPoint);
   /** Retorna el tipo de estrategia que utiliza el snapping */
   virtual std::string GetStrategyType();

private:
   /** Retorna la distancia entre un punto y una geometria */
   double GetDistance(OGRGeometry* Geometry, double X, double Y);
   /** Retorna un booleano que indica si el punto especificado se encuentra en el dominio */
   bool IsInDomain(double StartPoint, double EndPoint, double Point);
   /** Obtiene el punto de interseccion del punto fantasma con un segmento */
   void GetIntersectionPoint(OGRPoint* PhantomPoint, Coordinates NearVertex,
                             Coordinates OtherVertex, Coordinates &Intersection);
   /**
    * Retorna un valor booleano que indica si encontro una geometria dentro del area de tolerancia
    */
   bool FindNearestGeometryIndex(OGRPoint* PhantomPoint);
   /** Obtiene un vector con todos los vertices de la geometria mas cercana al punto fantasma */
   void GetGeometryVertex(OGRPoint* PhantomPoint, std::vector<OGRPoint*> &Vertex, int &Position);
   /** Indice correspondiente a la geometria mas cercana */
   int index_;
};

} /** namespace suri */

#endif /* SEGMENTSNAPSTRATEGY_H_ */
