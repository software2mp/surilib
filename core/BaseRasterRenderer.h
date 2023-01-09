/*! \file BaseRasterRenderer.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
