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
#include "CoregisterSelectionSource.h"
#include "suri/VectorStyle.h"

namespace suri {

CoregisterSelectionSource::CoregisterSelectionSource(CoregisterTask* pTask) {
   pTask_ = pTask;
   gcpTableId_ = SuriObject::CreateId();
}

CoregisterSelectionSource::~CoregisterSelectionSource() {
}

/** Retorna el id del datasource donde se seleccionan features. */
SuriObject::UuidType CoregisterSelectionSource::GetWorkingDatasourceId() {
   return gcpTableId_;
}

/** Selecciona los features cuya geometria intersecta el subset */
bool CoregisterSelectionSource::Select(Subset &SubsetArea, const World* pWorld) {
   CoregisterTask::WorkingImageType imagetype;
   if (!pTask_->GetImageType(pWorld, imagetype))
      return false;

   Subset area = SubsetArea;
   pTask_->GetSpatialModel(imagetype)->InverseTransform(area.lr_);
   pTask_->GetSpatialModel(imagetype)->InverseTransform(area.ul_);
   
   VectorDataLayer datalayer(pTask_->GetTable(),
                        pTask_->GetGcpColumnName(imagetype));

   ClearSelectionCache();
   selectedFeatures_.clear();
   std::vector<FeatureIdType> featureids = datalayer.IntersectLayer(
                                              area, datalayer.GetLayerSrs());
   std::vector<FeatureIdType>::iterator it = featureids.begin();
   for (; it != featureids.end(); ++it) {
      FeatureUuid featureuuid(gcpTableId_, *it);
      selectedFeatures_.push_back(featureuuid);
   }
   return true;
}

/** Indica si un feature se puede seleccionar */
bool CoregisterSelectionSource::ValidateFeature(SuriObject::UuidType DatasourceId,
                                                            FeatureIdType FeatureId) {
   if (!pTask_ || gcpTableId_ != DatasourceId)
      return false;
   Table* ptable = pTask_->GetTable();
   return ptable && ptable->GetRowById(FeatureId) >= 0;
}

/** Retorna el datalayer asociado al mundo */
GeometryCollection* CoregisterSelectionSource::GetSelectionCache(const World* pWorld) {
   CoregisterTask::WorkingImageType imagetype;
   if (!pTask_->GetImageType(pWorld, imagetype))
      return NULL;

   VectorDataLayer datalayer(pTask_->GetTable(),
                        pTask_->GetGcpColumnName(imagetype));

   GeometryCollection* pselection;
   pselection = new GeometryCollection(pWorld->GetSpatialReference());
   pselection->SetStyle(pSelectedGeometryStyle_->Clone());
   std::vector<FeatureUuid>::iterator it = selectedFeatures_.begin();
   for (; it != selectedFeatures_.end(); ++it) {
      Geometry* pgeom = datalayer.GetGeometry(it->featureId_);
      pTask_->AddSpatialReference(imagetype, pgeom);
      pselection->AppendGeometry(pgeom);
   }

   return pselection;
}

} /* namespace suri */
