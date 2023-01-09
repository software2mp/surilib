/** \file FeatureSelectionCachedSource.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "FeatureSelectionCachedSource.h"

namespace suri {

FeatureSelectionCachedSource::FeatureSelectionCachedSource() {
}

FeatureSelectionCachedSource::~FeatureSelectionCachedSource() {
}

/** Limpia la seleccion **/
void FeatureSelectionCachedSource::ClearSelection() {
   ClearSelectionCache();
   FeatureSelectionSource::ClearSelection();
}

/**
 * Selecciona un feature
 * @param[in] DatasourceId del datasource al que pertenece el feature
 * @param[in] FeatureId id del feature dentro del datasource
 * @return bool que indica si se pudo seleccionar el feature
 */
bool FeatureSelectionCachedSource::Select(FeatureIdType FeatureId,
                                         SuriObject::UuidType DatasourceId) {
   ClearSelectionCache();
   return FeatureSelectionSource::Select(FeatureId, DatasourceId);
}

/**
 * Deselecciona un feature
 * @param[in] DatasourceId del datasource al que pertenece el feature
 * @param[in] FeatureId id del feature dentro del datasource
 * @return bool que indica si se pudo deseleccionar el feature
 */
bool FeatureSelectionCachedSource::Unselect(FeatureIdType FeatureId,
                                           SuriObject::UuidType DatasourceId) {
   ClearSelectionCache();
   return FeatureSelectionSource::Unselect(FeatureId, DatasourceId);
}

/** 
 * Retorna las geometrias seleccionadas 
 * @param[in] pWorld mundo en donde hay que buscar
 * @return coleccion con geometrias seleccionadas en mundo
 */
GeometryCollection* FeatureSelectionCachedSource::GetSelectedGeometries(const World* pWorld) {
   GeometryCollection* pcollection = NULL;
   std::map<const World*, GeometryCollection*>::iterator it = cache_.find(pWorld);
   if (it == cache_.end()) {
      pcollection = GetSelectionCache(pWorld);
      if (pcollection)
         cache_.insert(std::pair<const World*, GeometryCollection*>(pWorld, pcollection));
   } else {
      pcollection = it->second;
   }
   return pcollection;
}

/** Limpia el cache para todos los world */
void FeatureSelectionCachedSource::ClearSelectionCache() {
   std::map<const World*, GeometryCollection*>::iterator it = cache_.begin();
   for (; it != cache_.end(); ++it)
      delete it->second;
   cache_.clear();
}

} /* namespace suri */
