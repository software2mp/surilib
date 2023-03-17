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

// Includes suri
#include "VectorDatasourceSelectionSource.h"
#include "GeometryTranslator.h"
#include "suri/Extent.h"
#include "suri/Vector.h"
#include "Filter.h"
#include "FilteredDatasource.h"
#include "suri/VectorStyle.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/Configuration.h"

// Includes gdal
#include "ogrsf_frmts.h"

namespace suri {

const std::string VectorDatasourceSelectionSource::FEATURE_ID_FIELD_NAME = "FID";

/** Ctor */
VectorDatasourceSelectionSource::VectorDatasourceSelectionSource(
                                             VectorDatasource *pDatasource,
                                             FilteredDatasource* pFiltredDatasource) {
   pDatasource_ = pDatasource;
   pVector_ = pDatasource_ ? pDatasource_->GetDatasourceAsVector() : NULL;
   pFiltredDatasource_ = pFiltredDatasource;
   if (pFiltredDatasource_)
      pFiltredDatasource_->SetDatasource(pDatasource_);
   pSelectedGeometries_ = NULL;
   pSelectedGeometryStyle_= VectorStyle::Create(Configuration::GetParameter("geo_selection_style", DEFAULT_GEO_SELECTION_STYLE));
}

/** Dtor */
VectorDatasourceSelectionSource::~VectorDatasourceSelectionSource() {
   Vector::Close(pVector_);
   VectorStyle::Destroy(pSelectedGeometryStyle_);
}

/**
 * Retorna el id del datasource donde se seleccionana features.
 * @return id del datasource donde se seleccionana features.
 */
SuriObject::UuidType VectorDatasourceSelectionSource::GetWorkingDatasourceId() {
   return pDatasource_->GetId();
}

/** Limpia la seleccion **/
void VectorDatasourceSelectionSource::ClearSelection() {
   FeatureSelectionSource::ClearSelection();
   UpdateDatasourceFilter();
}

/**
 * Agrega un feature a la seleccion y actualiza filtro
 * @param[in] DatasourceId del datasource al que pertenece el feature
 * @param[in] FeatureId id del feature dentro del datasource
 * @return bool que indica si se pudo seleccionar el feature
 */
bool VectorDatasourceSelectionSource::Select(FeatureIdType FeatureId,
                                       SuriObject::UuidType DatasourceId) {
   if (!FeatureSelectionSource::Select(FeatureId, DatasourceId))
      return false;
   return UpdateDatasourceFilter();
}

/**
 * Saca un feature de la seleccion y actualiza filtro
 * @param[in] DatasourceId del datasource al que pertenece el feature
 * @param[in] FeatureId id del feature dentro del datasource
 * @return bool que indica si se pudo deseleccionar el feature
 */
bool VectorDatasourceSelectionSource::Unselect(FeatureIdType FeatureId,
                                         SuriObject::UuidType DatasourceId) {
   if (!FeatureSelectionSource::Unselect(FeatureId, DatasourceId))
      return false;
   return UpdateDatasourceFilter();
}

/**
 * Selecciona los features cuya geometria intersecta el subset
 * @param[in] SubsetArea subset a intersecar
 * @param[in] SubsetSrs sistema de referencia en el que esta el subset
 * @return bool que indica si pudo intersecar.
 */
bool VectorDatasourceSelectionSource::Select(Subset &SubsetArea, const World* pWorld) {
   // Transformo subset a srs de vector
   std::string layersrs = pVector_->GetLayerSR(pDatasource_->GetActiveLayer());
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn, pWorld->GetSpatialReference());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut, layersrs);
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                     params);
   TransformationFactoryBuilder::Release(pfactory);
   if (!pct || pct->Transform(SubsetArea) < 1) {
      REPORT_DEBUG("D: Error al convertir a sistema de coordenadas de subset");
      if (pct)
         delete pct;
      return false;
   }
   delete pct;
   // Se configura filtro con subset
   OGRLayer* player = pVector_->GetLayer(pDatasource_->GetActiveLayer());
   Extent spatialfilter(SubsetArea);
   player->SetSpatialFilterRect(spatialfilter.min_.x_,
                                spatialfilter.min_.y_,
                                spatialfilter.max_.x_,
                                spatialfilter.max_.y_);

   // Actualizo ids de features seleccionadas.
   selectedFeatures_.clear();
   player->ResetReading();
   OGRFeature *pfeature = player->GetNextFeature();  // Apunta al primer feature
   while (pfeature) {
      FeatureSelectionSource::Select(pfeature->GetFID(), pDatasource_->GetId());
      OGRFeature::DestroyFeature(pfeature);
      pfeature = player->GetNextFeature();
   }
   return UpdateDatasourceFilter();
}

/**
 * Retorna las geometrias seleccionadas
 * @return Coleccion con las geometrias seleccionadas.
 */
GeometryCollection* VectorDatasourceSelectionSource::GetSelectedGeometries(
                                                         const World* pWorld) {
   if (!pSelectedGeometries_) {
      std::string layersrs = pVector_->GetLayerSR(pDatasource_->GetActiveLayer());
      OGRLayer* player = pVector_->GetLayer(pDatasource_->GetActiveLayer());
      pSelectedGeometries_ = new GeometryCollection(layersrs);
      pSelectedGeometries_->SetStyle(pSelectedGeometryStyle_->Clone());
      std::vector<FeatureUuid>::iterator it = selectedFeatures_.begin();
      for (; it != selectedFeatures_.end(); ++it) {
         OGRFeature* pfeature = player->GetFeature(it->featureId_);
         Geometry* pgeom = GeometryTranslator::ImportFromWkt(
                             GeometryTranslator::ExportToWkt(pfeature->
                                              GetGeometryRef()), layersrs);
         pSelectedGeometries_->AppendGeometry(pgeom);
         OGRFeature::DestroyFeature(pfeature);
      }
   }
   return pSelectedGeometries_;
}

/**
 * Indica si un feature se puede seleccionar
 * @param[in] DatasourceId del datasource al que pertenece el feature
 * @param[in] FeatureId id del feature dentro del datasource
 * @return bool que indica si un feature se puede seleccionar
 */
bool VectorDatasourceSelectionSource::ValidateFeature(
                                    SuriObject::UuidType DatasourceId,
                                             FeatureIdType FeatureId) {
   if (pDatasource_->GetId() != DatasourceId)
      return false;
   OGRLayer* player = pVector_->GetLayer(pDatasource_->GetActiveLayer());
   OGRFeature* pfeature = player->GetFeature(FeatureId);
   if(!pfeature)
      return false;
   OGRFeature::DestroyFeature(pfeature);
   return true;
}

/**
 * Actualiza la seleccion en filtro y la aplica a pFiltredDatasource_
 * @return bool que indica si pudo actualizar filtro
 */
bool VectorDatasourceSelectionSource::UpdateDatasourceFilter() {
   delete pSelectedGeometries_;
   pSelectedGeometries_ = NULL;
   if (!pFiltredDatasource_)
      return true;
   pFiltredDatasource_->SetFilter(NULL);
   Filter* pfilter = new Filter();
   pfilter->SetFilteredName(pDatasource_->GetName());
   pfilter->SetIdFieldName(FEATURE_ID_FIELD_NAME);

   if (!selectedFeatures_.empty()) {
      std::vector<FeatureUuid>::iterator it = selectedFeatures_.begin();
      for (; it != selectedFeatures_.end(); ++it)
         pfilter->AddEqualConstraint(it->featureId_);
   } else {
      pfilter->AddEqualConstraint(INVALID_FEATUREID);
   }

   pFiltredDatasource_->SetFilter(pfilter);
   return true;
}

} /* namespace suri */
