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

#include <vector>
#include <cstdlib>
#include <sstream>
#include <string>

#include "RasterDnInfo.h"
#include "suri/DatasourceInterface.h"
#include "CacheRenderer.h"
#include "MemoryCanvas.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/RasterElement.h"
#include "suri/TransformationFactory.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/RasterSpatialModel.h"
#include "suri/Dimension.h"
#include "suri/DataTypes.h"
#include "suri/World.h"

namespace suri {

template<typename T>
void FormatDataDn(std::string &Data, void* pSrc, size_t Position) {
   T* psrc = static_cast<T*>(pSrc);
   std::stringstream ssnumber;
   ssnumber << std::fixed << std::setprecision(6) << std::setw(0) << psrc[Position];
   Data = ssnumber.str();
}

template<>
void FormatDataDn<unsigned char>(std::string &Data, void* pSrc, size_t Position) {
   unsigned char* psrc = static_cast<unsigned char*>(pSrc);
   std::stringstream ssnumber;
   ssnumber << std::fixed << std::setprecision(6) << std::setw(0)
         << static_cast<int>(psrc[Position]);
   Data = ssnumber.str();
}

/** definicion del tipo de funcion que realiza la conversion de datos */
typedef void (*FormatDataDnFunctionType)(std::string&, void*, size_t);

/** Inicializa mapa de tipos de datos. */
INITIALIZE_DATATYPE_MAP(FormatDataDnFunctionType, FormatDataDn);

/**
 * Ctor.
 */
RasterDnInfo::RasterDnInfo() :
      pixel_(0), line_(0), datasource_(NULL), pWorld_(NULL), pCanvas_(NULL),
      pElement_(NULL), pCt_(NULL) {

}

/**
 * Dtor.
 */
RasterDnInfo::~RasterDnInfo() {
   delete pCt_;

}

/**
 * Configura el pixel (de la combinacion pixel/linea).
 */
void RasterDnInfo::SetPixel(double Pixel) {
   pixel_ = Pixel;
}

/**
 * Configura la linea (de la combinacion pixel/linea).
 */
void RasterDnInfo::SetLine(double Line) {
   line_ = Line;
}

/**
 * Configura el origen de datos para para el cual se van a calcular los DN.
 */
void RasterDnInfo::SetDatasource(DatasourceInterface* Datasource) {
   datasource_ = Datasource;
}

/**
 * Configura el mundo a ser utilizado.
 */
void RasterDnInfo::SetWorld(World* pWorld) {
   pWorld_ = pWorld;
}

/**
 * Configura el canvas a ser utilizado.
 */
void RasterDnInfo::SetCanvas(MemoryCanvas* pCanvas) {
   pCanvas_ = pCanvas;
}

/**
 * Configura el elemento a ser utilizado.
 */
void RasterDnInfo::SetElement(Element* pElement) {
   pElement_ = pElement;
}

/**
 * Devuelve un vecto que contiene para cada banda cual es el DN para el pixel/linea.
 *
 * @return Devuelve un vector con los valores DN de cada banda.
 */
std::vector<std::string> RasterDnInfo::GetBandsDnInfo() {
   MemoryCanvas* pcanvas = GetCanvas();
   Element* pelement = GetElement();
   std::vector<std::string> retvalues;

   // Muestra los datos de imagen siempre que haya un Canvas obtenido del elemento
   if (pcanvas && pelement) {
      std::vector<void*> data;
      std::vector<int> idx;
      for (int ix = 0, lenix = pcanvas->GetBandCount(); ix < lenix; ++ix)
         idx.push_back(ix);
      pcanvas->GetInternalData(idx, data);
      int sx = 0, sy = 0;
      pcanvas->GetSize(sx, sy);
      int vpx, vpy;
      pWorld_->GetViewport(vpx, vpy);
      UpdateTransform();
      if (pCt_) {
         RasterSpatialModel *prastermodel = RasterSpatialModel::Create(
               dynamic_cast<RasterElement*>(pelement)->GetRasterModel());

         if (!prastermodel) {
            return retvalues;
         }

         Coordinates pos, ul;
         pWorld_->Transform(Coordinates(vpx, vpy), pos);
         pWorld_->Transform(Coordinates(), ul);
         pCt_->Transform(pos, true);
         pCt_->Transform(ul, true);
         prastermodel->InverseTransform(pos);
         prastermodel->InverseTransform(ul);
         RasterSpatialModel::Destroy(prastermodel);
         Dimension dim(ul, pos);
         double factorx = dim.GetWidth() / vpx, factory = dim.GetHeight() / vpy;
         // Para que en zoom out de bien (la matriz y viewport tienen el mismo
         // tamano)
         factorx = factorx > 1 ? 1 : factorx;
         factory = factory > 1 ? 1 : factory;
         // offset en coordenadas del viewport desde las coordenadas de
         // cache
         double offsetx = ul.x_ - SURI_TRUNC(int, ul.x_), offsety = ul.y_
               - SURI_TRUNC(int, ul.y_);
         int cachex = SURI_TRUNC(int, (line_) * factorx + offsetx), cachey =
               SURI_TRUNC(int, (pixel_) * factory + offsety);
         int position = cachex + cachey * sx;
         if (position < sx * sy && position >= 0) {
            for (size_t i = 0; i < data.size(); i++) {
               std::string datastr;
               std::string datatype = pcanvas->GetDataType();
               FormatDataDnTypeMap[datatype](datastr, data[i], position);
               retvalues.push_back(datastr);
            }
         }
      }
   }

   return retvalues;
}

/**
 * Obtiene un canvas para el elemento raster.
 *
 * @return MemoryCanvas listo para ser utilizado.
 */
MemoryCanvas* RasterDnInfo::GetCanvas() {
   if (pCanvas_ != NULL)
      return pCanvas_;

   if (GetElement() == NULL)
      return NULL;

   MemoryCanvas* pcanvas = NULL;

   CacheRenderer::CachedElementsMap cache = CacheRenderer::GetCacheForElement(
         GetElement());

   CacheRenderer::CachedElementsMap::iterator it = cache.begin();

   // toma el primer cache
   if (it != cache.end())
      pcanvas = it->second;

   return pcanvas;
}

/**
 * Obtiene el elemento raster.
 */
Element* RasterDnInfo::GetElement() {
   if (pElement_ != NULL)
      return pElement_;

   if (datasource_ != NULL)
      return datasource_->GetElement();

   return NULL;
}
/** Metodo auxiliar que actualiza la transformacion **/
void RasterDnInfo::UpdateTransform() {
   Element* pelement = GetElement();
   if (pCt_) {
      ExactCoordinatesTransformation* ptransform =
            dynamic_cast<ExactCoordinatesTransformation*>(pCt_);
      std::string wktin = pWorld_->GetSpatialReference();
      std::string wktout = pWorld_->GetSpatialReference();
      // Si no es exacta o los sr son diferentes se elimina para regenerase
      if (!ptransform
            || (ptransform
                  && (ptransform->GetWktIn().compare(wktin)
                        || ptransform->GetWktOut().compare(wktout)))) {
         delete pCt_;
         pCt_ = NULL;
      }
   }
   if (!pCt_) {
      ParameterCollection params;
      params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                   pelement->GetSpatialReference().c_str());
      params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                   pWorld_->GetSpatialReference());
      TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
      pCt_ = pfactory->Create(TransformationFactory::kExact,
                                                        params);
      TransformationFactoryBuilder::Release(pfactory);
   }
}
}  // namespace suri
