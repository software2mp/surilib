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

// Includes standard

// Includes Suri
#include "BaseRasterRenderer.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/RasterSpatialModel.h"
#include "suri/CoordinatesTransformation.h"
#include "logmacros.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/**
 * Transforma entre coordenadas de raster y mundo.
 * @param[in] RasterModel wkt con transformacion para RasterSpatialModel
 * @param[in] ImageSpatialModel wkt con sistema de coordenadas de la imagen.
 * @param[in] WorldSpatialModel wkt con sistema de coordenadas del mundo
 * @param[in] Window Coordenadas en raster que se quieren obtener en
 * coordenadas de mundo
 * @param[out] Window Ventana de mundo que contiene exactamente a la parte
 * de la imagen de entrada
 */
bool BaseRasterRenderer::Transform(std::string RasterModel,
                                   std::string ImageSpatialModel,
                                   std::string WorldSpatialModel, Subset &Window) {
   RasterSpatialModel *prastermodel = RasterSpatialModel::Create(RasterModel);
   if (!prastermodel) {
      REPORT_DEBUG("D:No se pudo crear RasterSpatialModel");
      return false;
   }
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn, ImageSpatialModel);
   params.AddValue<std::string>(TransformationFactory::kParamWktOut, WorldSpatialModel);
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                     params);
   TransformationFactoryBuilder::Release(pfactory);
   if (!pct) {
      REPORT_DEBUG("D:No se pudo crear CoordinateTransform");
      return false;
   }

   // con reproyeccion, esto no deberia estar
   if (!pct->IsIdentity()) {
      REPORT_DEBUG("D:Referencia espacial del mundo != de la del raster");
      REPORT_DEBUG("D:AGREGAR REPROYECCION");
      delete pct;
      RasterSpatialModel::Destroy(prastermodel);
      return false;
   }

   // transforma a sistema de imagen
   prastermodel->Transform(Window.ul_);
   prastermodel->Transform(Window.lr_);

   // transforma coordenadas al sistema del mundo
   pct->Transform(Window.ul_);
   pct->Transform(Window.lr_);

   delete pct;
   return true;
}

/**
 * Transforma entre coordenadas de mundo y raster.
 * @param[in] RasterModel wkt con transformacion para RasterSpatialModel
 * @param[in] ImageSpatialModel wkt con sistema de coordenadas de la imagen.
 * @param[in] WorldSpatialModel wkt con sistema de coordenadas del mundo
 * @param[in] Window ventana que se quiere transformar a coordenadas de raster
 * @param[out] Window Coordenadas en pixel-linea que ocupa la ventana. El
 * pixel-linea puede no ser entero
 */
bool BaseRasterRenderer::InverseTransform(std::string RasterModel,
                                          std::string ImageSpatialModel,
                                          std::string WorldSpatialModel,
                                          Subset &Window) {
   RasterSpatialModel *prastermodel = RasterSpatialModel::Create(RasterModel);
   if (!prastermodel) {
      REPORT_DEBUG("D:No se pudo crear RasterSpatialModel");
      return false;
   }

   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                ImageSpatialModel);
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                WorldSpatialModel);
   TransformationFactory* pfactory =
         TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                            params);
   TransformationFactoryBuilder::Release(pfactory);
   if (!pct) {
      REPORT_DEBUG("D:No se pudo crear CoordinateTransform");
      return false;
   }

   // con reproyeccion, esto no deberia estar
   if (!pct->IsIdentity()) {
      REPORT_DEBUG("D:Referencia espacial del mundo != de la del raster");
      REPORT_DEBUG("D:AGREGAR REPROYECCION");
      delete pct;
      RasterSpatialModel::Destroy(prastermodel);
      return false;
   }

   // transforma coordenadas del sistema del mundo al sistema de la imagen
   pct->Transform(Window.ul_, true);
   pct->Transform(Window.lr_, true);

   // transforma a pixel linea
   prastermodel->InverseTransform(Window.ul_);
   prastermodel->InverseTransform(Window.lr_);

   return true;
}

/**
 * Transforma entre coordenadas de raster(en PL) y mundo.
 * @param[in] RasterModel wkt con transformacion para RasterSpatialModel
 * @param[in] ImageSpatialModel wkt con sistema de coordenadas de la imagen.
 * @param[in] WorldSpatialModel wkt con sistema de coordenadas del mundo
 * @param[in] Ulx Coordenadas de subset PL superior izquierda
 * @param[in] Uly Coordenadas de subset PL superior izquierda
 * @param[in] Lrx Coordenadas de subset PL inferior derecha
 * @param[in] Lry Coordenadas de subset PL inferior derecha
 * @param[out] Window Ventana de mundo que contiene exactamente a la parte
 * de la imagen de entrada
 */
bool BaseRasterRenderer::TransformImageSubset(std::string RasterModel,
                                              std::string ImageSpatialModel,
                                              std::string WorldSpatialModel,
                                              Subset &Window, int Ulx, int Uly, int Lrx,
                                              int Lry) {
   Subset window(Ulx, Uly, Lrx, Lry);
   if (!Transform(RasterModel, ImageSpatialModel, WorldSpatialModel, window)) {
      return false;
   }

   Window = window;
   return true;
}

/**
 * Transforma entre coordenadas de mundo y raster(en PL).
 * @param[in] RasterModel wkt con transformacion para RasterSpatialModel
 * @param[in] ImageSpatialModel wkt con sistema de coordenadas de la imagen.
 * @param[in] WorldSpatialModel wkt con sistema de coordenadas del mundo
 * @param[in] Window ventana que se quiere transformar a coordenadas de raster
 * @param[out] Ulx Coordenadas de subset PL superior izquierda
 * @param[out] Uly Coordenadas de subset PL superior izquierda
 * @param[out] Lrx Coordenadas de subset PL inferior derecha
 * @param[out] Lry Coordenadas de subset PL inferior derecha
 * \note Se redondea a las coordenadas de imagen que contengan el subset. Es
 * decir siempre se redondea hacia afuera
 */
bool BaseRasterRenderer::InverseTransformImageSubset(std::string RasterModel,
                                                     std::string ImageSpatialModel,
                                                     std::string WorldSpatialModel,
                                                     Subset Window, int &Ulx, int &Uly,
                                                     int &Lrx, int &Lry) {
   Subset window(Window);
   if (!InverseTransform(RasterModel, ImageSpatialModel, WorldSpatialModel, window)) {
      return false;
   }

   Ulx = SURI_TRUNC(int, window.ul_.x_);
   if (FLOAT_COMPARE(SURI_ROUND(int, window.ul_.x_), window.ul_.x_)) {
      Ulx = SURI_ROUND(int, window.ul_.x_);
   }

   Uly = SURI_TRUNC(int, window.ul_.y_);
   if (FLOAT_COMPARE(SURI_ROUND(int, window.ul_.y_), window.ul_.y_)) {
      Uly = SURI_ROUND(int, window.ul_.y_);
   }

   Lrx = SURI_TRUNC(int, SURI_CEIL(window.lr_.x_) );
   if (FLOAT_COMPARE(SURI_ROUND(int, window.lr_.x_), window.lr_.x_)) {
      Lrx = SURI_ROUND(int, window.lr_.x_);
   }

   Lry = SURI_TRUNC(int, SURI_CEIL(window.lr_.y_) );
   if (FLOAT_COMPARE(SURI_ROUND(int, window.lr_.y_), window.lr_.y_)) {
      Lry = SURI_ROUND(int, window.lr_.y_);
   }

   return true;
}
}

