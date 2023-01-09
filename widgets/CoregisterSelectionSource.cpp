/** \file CoregisterSelectionSource.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
