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
#include <string>
#include <vector>

// Includes Suri
#include "FileExporterProcessPartProvider.h"
#include "RasterizationProcessPartProvider.h"
#include "RasterizationPart.h"
#include "ImageFormatSelectionPart.h"
#include "ReprojectionProcessPartProvider.h"
#include "RasterOutputSizePart.h"
#include "VectorDatasource.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
RasterizationProcessPartProvider::RasterizationProcessPartProvider(ProcessAdaptLayer* pPal) :
      FileExporterProcessPartProvider(pPal) {
}

/** Destructor **/
RasterizationProcessPartProvider::~RasterizationProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool RasterizationProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   GetRasterizationParts(ProcessParts);
   GetRasterizationOutputSizeParts(ProcessParts);
   if (!FileExporterProcessPartProvider::GetParts(ProcessParts))
      return false;
   return true;
}

void RasterizationProcessPartProvider::GetRasterizationParts(std::vector<Part*> &ProcessParts) {
   std::vector<Element*> elementList;
   pPal_->GetAttribute<std::vector<Element*> >(
         ProcessAdaptLayer::InputRasterElementsAttr, elementList);
   RasterizationPart* pmosaic = new RasterizationPart(elementList);
   pmosaic->SetEnabled(true);
   pmosaic->SetProcessLayer(pPal_);
   ProcessParts.push_back(pmosaic);
   bool showespectral = false;
   bool showmask = false;
   bool showspatial = false;
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr, showmask);
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, showespectral);
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr, showspatial);

}
void RasterizationProcessPartProvider::GetRasterizationOutputSizeParts(std::vector<Part*> &ProcessParts) {
   VectorDatasource* pVectorDatasource = NULL;
   pPal_->GetAttribute<VectorDatasource*>(
                  ProcessAdaptLayer::RasterizationVectorDatasourceAttr, pVectorDatasource);

   ReprojectionProcess::ReprojectionParameters *pReprojectionParameters =
                                    new ReprojectionProcess::ReprojectionParameters();
   /* Sisetema de referencia*/
   pReprojectionParameters->srWktIn_ = pVectorDatasource->GetSpatialReference();
   pReprojectionParameters->srWktOut_ = pVectorDatasource->GetSpatialReference();

   /* algoritmo exacto */
   pReprojectionParameters->algorithm_ = suri::TransformationFactory::kExact;

   /* Alto y ancho default */
   pReprojectionParameters->width_ = 3000;
   pReprojectionParameters->height_ = 3000;

   /* Creo la transformacion */
   suri::ParameterCollection params;
   params.AddValue<std::string>(suri::TransformationFactory::kParamWktIn,
                                pReprojectionParameters->srWktIn_);
   params.AddValue<std::string>(suri::TransformationFactory::kParamWktOut,
                                pReprojectionParameters->srWktOut_);
   suri::TransformationFactory* pfactory = suri::TransformationFactoryBuilder::Build();
   pReprojectionParameters->pTranform_ = pfactory->Create(
         suri::TransformationFactory::kExact, params);

   /* Creo el raster spatial model default */
   VectorElement* pVectorElement =
         dynamic_cast<VectorElement*>(pVectorDatasource->GetElement());
   Subset sub;
   pVectorElement->GetElementExtent(sub);
   pPal_->AddAttribute<Subset>(ProcessAdaptLayer::OutputSubsetWindowKeyAttr, sub);
   Dimension dim(sub);
   double adfGeoTransform[6] = { sub.ul_.x_, dim.GetWidth() / pReprojectionParameters->width_,
                                 0, sub.ul_.y_, 0,
                                 -dim.GetHeight() / pReprojectionParameters->height_};
   RasterSpatialModel* pRasterModel = NULL;
   RasterSpatialModel::Parameters paramRasterModel;
   for (int x = 0; x < 6; x++) {
      paramRasterModel.matrixModel_[x] = adfGeoTransform[x];
   }
   std::string wkt = RasterSpatialModel::ParametersToWkt(paramRasterModel);
   pRasterModel = RasterSpatialModel::Create(wkt);
   pReprojectionParameters->pRasterSpatialModelIn_ = RasterSpatialModel::Create(wkt);
   pReprojectionParameters->pRasterSpatialModelOut_ = RasterSpatialModel::Create(wkt);
   pReprojectionParameters->rasterModelIn_ = wkt;

   World* pworld = NULL;
   pPal_->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr, pworld);
   pReprojectionParameters->pWorldIn_ = pworld;
   RasterOutputSizePart* pPart = new RasterOutputSizePart(pReprojectionParameters, pworld, pworld);
   pPart->SetEnabled(true);
   pPart->SetProcessLayer(pPal_);
   ProcessParts.push_back(pPart);
}
/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* RasterizationProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   RasterizationProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("RasterizationProcess") == 0 )
      pprovider = new RasterizationProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */


