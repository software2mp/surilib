/** \file GeoreferenceSelectionSource.cpp
 *
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <vector>

#include "GeoreferenceSelectionSource.h"
#include "suri/GeoreferenceTask.h"
#include "suri/Subset.h"
#include "suri/World.h"
#include "VectorDataLayer.h"
#include "suri/SuriObject.h"
#include "suri/Table.h"
#include "GeometryCollection.h"
#include "suri/RasterSpatialModel.h"
#include "suri/VectorStyle.h"

namespace suri {

/**
 * Ctor.
 */
GeoreferenceSelectionSource::GeoreferenceSelectionSource(GeoreferenceTask* pTask) {
   pTask_ = pTask;
   gcpTableId_ = SuriObject::CreateId();
}

/**
 * Dtor.
 */
GeoreferenceSelectionSource::~GeoreferenceSelectionSource() {

}

/**
 * Retorna el id del datasource donde se seleccionan features.
 */
SuriObject::UuidType GeoreferenceSelectionSource::GetWorkingDatasourceId() {
   return gcpTableId_;
}

/**
 * Selecciona los features cuya geometria intersecta el subset.
 */
bool GeoreferenceSelectionSource::Select(Subset& SubsetArea, const World* pWorld) {
   Subset area = SubsetArea;
   pTask_->GetSpatialModel()->InverseTransform(area.lr_);
   pTask_->GetSpatialModel()->InverseTransform(area.ul_);

   VectorDataLayer datalayer(pTask_->GetTable(), pTask_->GetGcpColumnName());

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

/**
 * Indica si un feature se puede seleccionar.
 */
bool GeoreferenceSelectionSource::ValidateFeature(SuriObject::UuidType DatasourceId,
                                                  FeatureIdType FeatureId) {
   if (!pTask_ || gcpTableId_ != DatasourceId) return false;

   Table* ptable = pTask_->GetTable();

   return ptable && ptable->GetRowById(FeatureId) >= 0;
}

/**
 * Retorna el cache asociado a un mundo.
 */
GeometryCollection* GeoreferenceSelectionSource::GetSelectionCache(const World* pWorld) {
   VectorDataLayer datalayer(pTask_->GetTable(), pTask_->GetGcpColumnName());

   GeometryCollection* pselection;
   pselection = new GeometryCollection(pWorld->GetSpatialReference());
   pselection->SetStyle(pSelectedGeometryStyle_->Clone());
   std::vector<FeatureUuid>::iterator it = selectedFeatures_.begin();
   for (; it != selectedFeatures_.end(); ++it) {
      Geometry* pgeom = datalayer.GetGeometry(it->featureId_);
      pTask_->AddSpatialReference(pgeom);
      pselection->AppendGeometry(pgeom);
   }

   return pselection;
}

}  // namespace suri
