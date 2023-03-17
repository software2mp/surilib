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
