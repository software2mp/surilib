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

#ifndef SNAPINTERFACE_H_
#define SNAPINTERFACE_H_

// Includes Estandar
#include <vector>
#include <string>

// Includes Gdal
#include "ogr_geometry.h"

namespace suri {

// forwards
class World;
class Coordinates;

class SnapInterface {
public:
   /** Destructor */
   virtual ~SnapInterface();
   /** Modifica las coordenadas de un punto de acuerdo a la estrategia definida */
   virtual bool DoSnap(Coordinates &ViewportPosition, OGRPoint* PhantomPoint)=0;
   /** Retorna el tipo de estrategia que utiliza el snapping */
   virtual std::string GetStrategyType()=0;
   /** Configura el mundo que se utilizara */
   virtual void SetWorld(World* pWorld);
   /** Configura el vector con las geometrias correspondientes a los segmentos */
   void SetSegmentGeometries(std::vector<OGRGeometry*> SegmentGeometries);
   /** Configura el vector con las geometrias correspondientes a los vertices */
   void SetVertexGeometries(std::vector<OGRGeometry*> VertexGeometries);
   /** Configura el vector con las geometrias correspondientes a los buffers de los vertices */
   void SetVertexBufferGeometries(std::vector<OGRGeometry*> VertexBuffers);
   /** Configura el vector con las geometrias correspondientes a los buffers de los segmentos */
   void SetSegmentBufferGeometries(std::vector<OGRGeometry*> SegmentBuffers);

protected:
   /** Puntero al mundo */
   World* pWorld_;
   /** Vector con geometrias */
   std::vector<OGRGeometry*> segmentbuffers_, vertexbuffers_, segmentgeometries_, vertexgeometries_;
};

} /** namespace suri */

#endif /* SNAPINTERFACE_H_ */
