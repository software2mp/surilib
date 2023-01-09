/*! \file FeatureSelection.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes Estandar

// Includes Suri
#include "suri/FeatureSelection.h"
#include "FeatureSelectionUpdatedNotification.h"
#include "Filter.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Patron Null Object Pattern
 */
class NullSelectionSource : public FeatureSelectionSource {
   SuriObject::UuidType GetWorkingDatasourceId() {return SuriObject::NullUuid;}
   bool Select(Subset &SubsetArea, const World* pWorld) {return false;}
   bool ValidateFeature(SuriObject::UuidType DatasourceId,
                                                   FeatureIdType FeatureId) {
      return false;
   }
   GeometryCollection* GetSelectedGeometries(const World* pWorld) {return NULL;}
};

/** Constructor  */
FeatureSelection::FeatureSelection() :
      pSelectionSource_(new NullSelectionSource()) {
}

/** Destructor */
FeatureSelection::~FeatureSelection() {
   delete pSelectionSource_;
}

/**
 * Configura el GeometrySelectionSource que administra la seleccion.
 * @param[in] pSelectionSource instancia que administra la seleccion.
 * \attention la instancia pasa a ser responsabilidad de esta clase no eliminar.
 * @return siempre true
 */
bool FeatureSelection::Configure(FeatureSelectionSource* pSelectionSource) {
   delete pSelectionSource_;
   pSelectionSource_ = (pSelectionSource != NULL) ? pSelectionSource :
                                                new NullSelectionSource();
   return true;
}

/**
 * Retorna el id del datasource donde se seleccionana features. El resultado
 * depende de FeatureSelectionSource.
 * @return id del datasource donde se seleccionana features.
 * @return SuriObject::NullUuid si FeatureSelectionSource no soporta
 * tabajar con un datasource default.
 */
SuriObject::UuidType FeatureSelection::GetWorkingDatasourceId() {
   return pSelectionSource_->GetWorkingDatasourceId();
}

/** Limpia la seleccion **/
void FeatureSelection::ClearSelection() {
   pSelectionSource_->ClearSelection();
}

/**
 * Actualiza la seleccion existente notificando a los interesados
 * @param[in] FeaturesSelected vector con features a seleccionar.
 **/
void FeatureSelection::UpdateSelection(
         const std::set<FeatureIdType>& FeaturesSelected,
         const SuriObject::UuidType &DatasourceId) {
   SuriObject::UuidType datasourceid = DatasourceId != SuriObject::NullUuid ?
                                       DatasourceId : GetWorkingDatasourceId();

   pSelectionSource_->ClearSelection();
   std::set<FeatureIdType>::const_iterator it = FeaturesSelected.begin();
   for (; it != FeaturesSelected.end(); ++it)
      pSelectionSource_->Select(*it, datasourceid);

   FeatureSelectionUpdatedNotification featureselection;
   Notify(&featureselection);
}

/**
 * Selecciona un feature
 * @param[in] FeatureId id de feature a seleccionar.
 * @return bool que indica si pudo seleccionar fueatures
 */
bool FeatureSelection::Select(FeatureIdType FeatureId,
                      const SuriObject::UuidType &DatasourceId) {
   SuriObject::UuidType datasourceid = DatasourceId != SuriObject::NullUuid ?
                                       DatasourceId : GetWorkingDatasourceId();

   if (!pSelectionSource_->Select(FeatureId, datasourceid))
      return false;

   FeatureSelectionUpdatedNotification featureselection;
   Notify(&featureselection);
   return true;
}

/**
 * Deselecciona un feature
 * @param[in] FeatureId id de feature a deseleccionar.
 * @return bool que indica si pudo deseleccionar fueatures
 */
bool FeatureSelection::Unselect(FeatureIdType FeatureId,
                     const SuriObject::UuidType &DatasourceId) {
   SuriObject::UuidType datasourceid = DatasourceId != SuriObject::NullUuid ?
                                       DatasourceId : GetWorkingDatasourceId();

   if (!pSelectionSource_->Unselect(FeatureId, datasourceid))
      return false;

   FeatureSelectionUpdatedNotification featureselection;
   Notify(&featureselection);
   return true;
}

/**
 * Selecciona los features cuya geometria intersecta el subset.
 * @param[in] SubsetArea subset a intersecar. Los puntos estan en srs de mundo
 * @param[in] pWorld mundo en que se cargo el subset.
 * @return bool que indica si pudo intersecar.
 */
bool FeatureSelection::Select(Subset &SubsetArea, const World* pWorld) {
   if (!pSelectionSource_->Select(SubsetArea, pWorld))
      return false;

   FeatureSelectionUpdatedNotification featureselection;
   Notify(&featureselection);
   return true;
}

/** Retorna vector con los features seleccionados */
std::vector<FeatureIdType> FeatureSelection::GetSelectedFeatures(
                                    const SuriObject::UuidType &DatasourceId) {
   SuriObject::UuidType datasourceid = DatasourceId != SuriObject::NullUuid ?
                                       DatasourceId : GetWorkingDatasourceId();

   return pSelectionSource_->GetSelectedFeatures(datasourceid);
}

/**
 * Retorna las geometrias seleccionadas
 * @param[in] pWorld mundo en que se mostraran las geometrias.
 * @return Coleccion con las geometrias seleccionadas.
 */
GeometryCollection* FeatureSelection::GetSelectedGeometries(const World* pWorld) {
   return pSelectionSource_->GetSelectedGeometries(pWorld);
}

} /** namespace suri */
