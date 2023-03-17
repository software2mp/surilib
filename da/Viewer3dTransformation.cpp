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
#include <vector>

// Includes Suri
#include "suri/Viewer3dTransformation.h"
#include "TerrainGLCanvas.h"
#include "Terrain.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/** constructor **/
Viewer3dTransformation::Viewer3dTransformation(Viewer3D* pViewer) :
      pViewer_(pViewer) {
}

/** destructor **/
Viewer3dTransformation::~Viewer3dTransformation() {
}

/**
 * Metodo a implementar en la clase derivada (cuando corresponda) para transformar
 * las coordenadas en el formato de origen al formato de salida utilizando como
 * salida un objecto Coordinates.
 */
int Viewer3dTransformation::Transform(Coordinates &PointCoordinates,
                                      bool Inverse) const {
   if (!Inverse && pViewer_) {
      TerrainGLCanvas* pglterrain = pViewer_->GetCanvas();
      Coordinates coords = pglterrain->GetPosition(PointCoordinates.x_,
                                                   PointCoordinates.y_);
      if (pglterrain && pglterrain->GetTerrain())
         PointCoordinates = pglterrain->GetTerrain()->GetRasterPosition(coords);
      else
         return 0;
      return 1;
   }
   return 0;
}

/**
 * Metodo a implementar en la clase derivada (cuando corresponda) para transformar
 * las coordenadas en el formato de origen al formato de salida utilizando como
 * salida un vector de Coordinates.
 */
int Viewer3dTransformation::Transform(std::vector<Coordinates> &CoordinatesVector,
                                      bool Inverse) const {
   int count = 0;
   std::vector<Coordinates>::iterator it = CoordinatesVector.begin();
   for (; it != CoordinatesVector.end(); ++it) {
      count += Transform(*it, Inverse);
   }
   return count;
}

/**
 * Metodo a implementar en la clase derivada (cuando corresponda) para transformar
 * las coordenadas en el formato de origen al formato de salida utilizando como
 * salida un Subset.
 */
int Viewer3dTransformation::Transform(Subset &SubsetTransformed, int CalculationPoints,
                                      bool Inverse) const {
   if (IsIdentity())
        return CalculationPoints * CalculationPoints;

     if (CalculationPoints < 2)
        return 0;

     // Resultado
     int result = 0;

     // Pasos
     double dx = (SubsetTransformed.lr_.x_ - SubsetTransformed.ul_.x_) / (CalculationPoints - 1);
     double dy = (SubsetTransformed.lr_.y_ - SubsetTransformed.ul_.y_) / (CalculationPoints - 1);

     // Limites
     double minx = std::numeric_limits<double>::max();
     double miny = std::numeric_limits<double>::max();
     double maxx = -std::numeric_limits<double>::max();
     double maxy = -std::numeric_limits<double>::max();

     // Calcula los limites en el SR de salida
     for (int ix = 0; ix < CalculationPoints; ++ix) {
        for (int jx = 0; jx < CalculationPoints; ++jx) {
           Coordinates point(SubsetTransformed.ul_.x_ + ix * dx,
                             SubsetTransformed.ul_.y_ + jx * dy);
           if (Transform(point, Inverse) != 0) {
              if (point.x_ < minx)
                 minx = point.x_;

              if (point.y_ < miny)
                 miny = point.y_;

              if (point.x_ > maxx)
                 maxx = point.x_;

              if (point.y_ > maxy)
                 maxy = point.y_;

              ++result;
           }
        }
     }

     // Genera el subset de salida respetando los ejes originales
     if (result > 0) {
        if (dx < 0) {
           SubsetTransformed.ul_.x_ = maxx;
           SubsetTransformed.lr_.x_ = minx;
        } else {
           SubsetTransformed.ul_.x_ = minx;
           SubsetTransformed.lr_.x_ = maxx;
        }
        if (dy < 0) {
           SubsetTransformed.ul_.y_ = maxy;
           SubsetTransformed.lr_.y_ = miny;
        } else {
           SubsetTransformed.ul_.y_ = miny;
           SubsetTransformed.lr_.y_ = maxy;
        }
     }

     return result;
}

/**
 * Indica si es la transformacon identidad.
 */
bool Viewer3dTransformation::IsIdentity() const {
   return false;
}
/** Compara si dos transformaciones son iguales **/
bool Viewer3dTransformation::Equals(CoordinatesTransformation* pTransform) const {
   return false;
}
} /** namespace suri */
