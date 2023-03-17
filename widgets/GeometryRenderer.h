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

#ifndef GEOMETRYRENDERER_H_
#define GEOMETRYRENDERER_H_

// Inclues standard
#include <vector>
#include <string>

// Includes suri
#include "suri/VectorStyle.h"

// fordwards
class wxDC;
class wxPoint;

namespace suri {

class Mask;
class Geometry;
class World;
class GeometryCollection;

/**
 * Clase que renderiza un listado de gemetrias sobre un wxDC.
 */
class GeometryRenderer {
   /**
    * Estructura que se usa para guardar informacion de renderizacion
    */
   class GeometryData {
   public:
      /** Ctor */
      GeometryData(int PointCount, VectorStyle* pStyle);
      /** Dtor */
      ~GeometryData();

      wxPoint* points_; /** Vector con puntos a renderizar */
      int pointCount_; /** Cantidad de puntos en points_ */
      VectorStyle* pStyle_; /** Estilo de los puntos */
   };

public:
   /** Ctor */
   GeometryRenderer(wxDC* pDC, const World* pWorldWindow);
   /** Dtor */
   virtual ~GeometryRenderer();
   /** Renderiza las geometrias sobre wl wxDC */
   void Render(GeometryCollection* pCollection,
                               const Mask* pMask = NULL);
   /** Renderiza las geometrias en vector sobre wl wxDC */
   void Render(std::vector<Geometry*> &Geometries,
                                const VectorStyle* pDefaultStyle,
                                const Mask* pMask = NULL);

private:
   /** Pinta */
   bool Paint(std::vector<GeometryData*> &Polygons, const Mask* &pMask);
   /** Traza */
   bool Trace(std::vector<GeometryData*> &Lines, const Mask* &pMask);
   /** Pone marcas */
   bool Mark(std::vector<GeometryData*> &Points, const Mask* &pMask);

   /** DC wx sobre el que se renderizan los vectores */
   wxDC* pDC_;
   /** Mundo asociado al dc donde quiero renderizar */
   const World* pWorldWindow_;
};

} /* namespace suri */

#endif /* GEOMETRYRENDERER_H_ */
