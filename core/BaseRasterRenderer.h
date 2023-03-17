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

#ifndef BASERASTERRENDERER_H_
#define BASERASTERRENDERER_H_

// Includes standard
// Includes Suri
#include "suri/Renderer.h"
#include "suri/Subset.h"
// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Funciones comunes para el manejo de raster. */
/**
 * La idea es que todas los renderes que utilicen rasters usen esta clase
 * para transformar entre coordenadas de raster y mundo. Esto evita
 * que halla incongruencias entre ellos y que a partir de una coordenada
 * de mundo se obtenga el mismo raster en todos los renderers.
 * No se usa porque ConvolutionFilterRenderer al final no lo necesita. Las
 * clases que deberian heredar de esta son RasterRenderer, ZoomRenderer y
 * todas las que editen rasters.
 */
class BaseRasterRenderer : public Renderer {
public:
   /** Transforma entre coordenadas de raster y mundo. */
   bool Transform(std::string RasterModel, std::string ImageSpatialModel,
                  std::string WorldSpatialModel, Subset &Window);
   /** Transforma entre coordenadas de mundo y raster. */
   bool InverseTransform(std::string RasterModel, std::string ImageSpatialModel,
                         std::string WorldSpatialModel, Subset &Window);
   /** Transforma entre coordenadas de raster y mundo(usando pixel linea). */
   bool TransformImageSubset(std::string RasterModel, std::string ImageSpatialModel,
                             std::string WorldSpatialModel, Subset &Window, int Ulx,
                             int Uly, int Lrx, int Lry);
   /** Transforma entre coordenadas de mundo y raster(usando pixel linea). */
   bool InverseTransformImageSubset(std::string RasterModel,
                                    std::string ImageSpatialModel,
                                    std::string WorldSpatialModel, Subset Window,
                                    int &Ulx, int &Uly, int &Lrx, int &Lry);
};
}

#endif /* BASERASTERRENDERER_H_ */
