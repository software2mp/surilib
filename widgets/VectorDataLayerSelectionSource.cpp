/** \file VectorDataLayerSelectionSource.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes suri
#include "VectorDataLayerSelectionSource.h"
#include "suri/VectorStyle.h"

namespace suri {

/**
 * Ctor
 * @pVectorDataLayer capa que se usa para seleccionar.
 * \attention La clase toma posecion del puntero. No eliminar fuera de la clase
 */
VectorDataLayerSelectionSource::VectorDataLayerSelectionSource(
                                    VectorDataLayer* pVectorDataLayer,
                                    SuriObject::UuidType DatasourceId) {
   pVectorDataLayer_ = pVectorDataLayer;
   datasourceId_ = DatasourceId;
}

/** Dtor */
VectorDataLayerSelectionSource::~VectorDataLayerSelectionSource() {
   delete pVectorDataLayer_;
}

/** 
 * Retorna el id del datasource donde se seleccionan features. 
 * @return id del datasource en edicion.
 */
SuriObject::UuidType VectorDataLayerSelectionSource::GetWorkingDatasourceId() {
	return datasourceId_;
}
   
/**
 * Selecciona los features cuya geometria intersecta el subset
 * @param[in] SubsetArea subset a intersecar
 * @param[in] SubsetSrs sistema de referencia en el que esta el subset
 * @return bool que indica si pudo intersecar.
 */
bool VectorDataLayerSelectionSource::Select(Subset &SubsetArea,
                                                const World* pWorld) {
   ClearSelectionCache();
   selectedFeatures_.clear();
   std::vector<FeatureIdType> featureids = pVectorDataLayer_->IntersectLayer(
                                     SubsetArea, pWorld->GetSpatialReference());
   std::vector<FeatureIdType>::iterator it = featureids.begin();
   for (; it != featureids.end(); ++it) {
      FeatureUuid featureuuid(datasourceId_, *it);
      selectedFeatures_.push_back(featureuuid);
   }
   return true;
}

/**
 * Retorna las geometrias seleccionadas
 * @return coleccion con las geometrias seleccionadas.
 * \attention la coleccion es responsablidad de esta clase, no eliminar.
 */
GeometryCollection* VectorDataLayerSelectionSource::GetSelectionCache(
                                                         const World* pWorld) {
   GeometryCollection* pselection;
   pselection = new GeometryCollection(pVectorDataLayer_->GetLayerSrs());
   pselection->SetStyle(pSelectedGeometryStyle_->Clone());
   std::vector<FeatureUuid>::iterator it = selectedFeatures_.begin();
   for (; it != selectedFeatures_.end(); ++it)
      pselection->AppendGeometry(pVectorDataLayer_->GetGeometry(it->featureId_));

   return pselection;
}

/**
 * Indica si un feature se puede seleccionar
 * @param[in] DatasourceId del datasource al que pertenece el feature
 * @param[in] FeatureId id del feature dentro del datasource
 * @return bool que indica si un feature se puede seleccionar
 */
bool VectorDataLayerSelectionSource::ValidateFeature(
                                    SuriObject::UuidType DatasourceId,
                                             FeatureIdType FeatureId) {
   if (!pVectorDataLayer_ || datasourceId_ != DatasourceId)
      return false;
   Table* ptable = pVectorDataLayer_->GetTable();
   return ptable && ptable->GetRowById(FeatureId) >= 0;
}

} /* namespace suri */
