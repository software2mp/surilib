/** \file FeatureSelectionSource.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <string>
#include <vector>

// Includes suri
#include "suri/FeatureSelectionSource.h"
#include "suri/VectorStyle.h"
#include "suri/Configuration.h"

namespace suri {

/** Ctor */
FeatureSelectionSource::FeatureSelectionSource() {
   std::string vectorStyle = Configuration::GetParameter("geo_edition_style",
                                                         DEFAULT_GEO_EDITION_STYLE);

   if (!vectorStyle.empty())
      pSelectedGeometryStyle_ = VectorStyle::Create(vectorStyle.c_str());
   else
      // Si no fue seteado ningun estilo previamente, ponemos este por defecto
      pSelectedGeometryStyle_ = VectorStyle::Create();
}

/** Dtor */
FeatureSelectionSource::~FeatureSelectionSource () {
   VectorStyle::Destroy(pSelectedGeometryStyle_);
}

/** Limpia la seleccion **/
void FeatureSelectionSource::ClearSelection() {
   selectedFeatures_.clear();
}

/**
 * Selecciona un feature
 * @param[in] DatasourceId del datasource al que pertenece el feature
 * @param[in] FeatureId id del feature dentro del datasource
 * @return bool que indica si pudo seleccionar feature
 */
bool FeatureSelectionSource::Select(FeatureIdType FeatureId,
                                                SuriObject::UuidType DatasourceId) {
   if (!ValidateFeature(DatasourceId, FeatureId) ||
                  FindFeature(DatasourceId, FeatureId) != selectedFeatures_.end())
      return false;

   FeatureUuid featureuid(DatasourceId, FeatureId);
   selectedFeatures_.push_back(featureuid);
   return true;
}

/**
 * Deselecciona un feature
 * @param[in] DatasourceId del datasource al que pertenece el feature
 * @param[in] FeatureId id del feature dentro del datasource
 * @return bool que indica si pudo deseleccionar feature
 */
bool FeatureSelectionSource::Unselect(FeatureIdType FeatureId,
                                           SuriObject::UuidType DatasourceId) {
   std::vector<FeatureUuid>::iterator it = FindFeature(DatasourceId, FeatureId);
   if (it == selectedFeatures_.end())
      return false;
   selectedFeatures_.erase(it);
   return true;
}

/**
 * Retorna iterador correspondiente a feature en selectedFeatures_
 * @param[in] DatasourceId del datasource al que pertenece el feature
 * @param[in] FeatureId id del feature dentro del datasource
 * @return iterador correspondiente a feature en selectedFeatures_
 * @return selectedFeatures_.end() si no lo encontro
 */
std::vector<FeatureSelectionSource::FeatureUuid>::iterator FeatureSelectionSource::FindFeature(
                                          SuriObject::UuidType DatasourceId,
                                                     FeatureIdType FeatureId) {
   std::vector<FeatureUuid>::iterator it = selectedFeatures_.begin();
   for (; it != selectedFeatures_.end() &&
                        (it->datasourceId_ != DatasourceId ||
                                 it->featureId_ != FeatureId); ++it);

   return it;
}

/** Retorna vector con el id de los features seleccionados */
std::vector<FeatureIdType> FeatureSelectionSource::GetSelectedFeatures(
                                    const SuriObject::UuidType &DatasourceId) {
   std::vector<FeatureIdType> selectedfeatures;
   std::vector<FeatureUuid>::iterator it = selectedFeatures_.begin();
   for (; it != selectedFeatures_.end(); ++it) {
      if (it->datasourceId_ == DatasourceId)
         selectedfeatures.push_back(it->featureId_);
   }
   return selectedfeatures;
}

} /* namespace suri */
