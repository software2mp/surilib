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

#ifndef FEATURESELECTIONSOURCE_H_
#define FEATURESELECTIONSOURCE_H_

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "GeometryCollection.h"
#include "VectorDataLayer.h"
#include "suri/Subset.h"
#include "suri/World.h"

namespace suri {

// fordwards
class VectorStyle;

/**
 * Permite definir una estrategia de seleccion. Lo usa FeatureSelection
 * (patron strategy).
 */
class FeatureSelectionSource {
public:
   /** Ctor */
   FeatureSelectionSource();
   /** Dtor */
   virtual ~FeatureSelectionSource();

   /**
    * Retorna el id del datasource donde se seleccionan features.
    * NULL si el FeatureSelectionSource concreto no tiene un datasource default.
    */
   virtual SuriObject::UuidType GetWorkingDatasourceId() = 0;

   /** Limpia la seleccion **/
   virtual void ClearSelection();
   /** Selecciona un feature */
   virtual bool Select(FeatureIdType FeatureId,
                           SuriObject::UuidType DatasourceId);
   /** Deselecciona un feature */
   virtual bool Unselect(FeatureIdType FeatureId,
                           SuriObject::UuidType DatasourceId);
   /** Retorna vector con el id de los features seleccionados */
   std::vector<FeatureIdType> GetSelectedFeatures(
                        const SuriObject::UuidType &DatasourceId);

   /** Selecciona los features cuya geometria intersecta el subset */
   virtual bool Select(Subset &SubsetArea, const World* pWorld) = 0;
   /** Retorna las geometrias seleccionadas */
   virtual GeometryCollection* GetSelectedGeometries(const World* pWorld) = 0;

protected:
   struct FeatureUuid {
      FeatureUuid(SuriObject::UuidType DatasourceId, FeatureIdType FeatureId) {
         datasourceId_ = DatasourceId;
         featureId_ = FeatureId;
      }
      SuriObject::UuidType datasourceId_;
      FeatureIdType featureId_;
   };

   /** Indica si un feature se puede seleccionar */
   virtual bool ValidateFeature(SuriObject::UuidType DatasourceId,
                                                FeatureIdType FeatureId) = 0;
   /** Retorna iterador correspondiente a feature */
   std::vector<FeatureUuid>::iterator FindFeature(SuriObject::UuidType DatasourceId,
                                                              FeatureIdType FeatureId);

   /** Vector con el id de los features seleccionados */
   std::vector<FeatureUuid> selectedFeatures_;
   /** Estilo de geometrias seleccionada */
   VectorStyle* pSelectedGeometryStyle_;
};

} /* namespace suri */
#endif /* FEATURESELECTIONSOURCE_H_ */
