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

#ifndef VECTORDATALAYERSELECTIONSOURCE_H_
#define VECTORDATALAYERSELECTIONSOURCE_H_

// Includes standard
#include <string>

// Includes suri
#include "FeatureSelectionCachedSource.h"
#include "VectorDataLayer.h"

namespace suri {

// fordwards
class VectorStyle;

/**
 * Permite seleccionar features dentro del vector en edicion.
 * Justificacion:
 *     Al editar, los cambios realizados al vector se guardan dentro de
 *     VectorDataLayerSelectionSource, por esta razon, no se puede usar el
 *     datasource directamente.
 */
class VectorDataLayerSelectionSource : public FeatureSelectionCachedSource {
public:
   /** Ctor */
   explicit VectorDataLayerSelectionSource(VectorDataLayer* pVectorDataLayer,
                                             SuriObject::UuidType DatasourceId);
   /** Dtor */
   virtual ~VectorDataLayerSelectionSource();

   /** Retorna el id del datasource donde se seleccionan features. */
   virtual SuriObject::UuidType GetWorkingDatasourceId();

   /** Selecciona los features cuya geometria intersecta el subset */
   virtual bool Select(Subset &SubsetArea, const World* pWorld);

protected:
   /** Retorna el cache asociado a un mundo */
   virtual GeometryCollection* GetSelectionCache(const World* pWorld);
   /** Indica si un feature se puede seleccionar */
   virtual bool ValidateFeature(SuriObject::UuidType DatasourceId,
                                                FeatureIdType FeatureId);

   /** VectorDataLayer que se usa como fuente de datos */
   VectorDataLayer* pVectorDataLayer_;
   /** Id del datasource editado */
   SuriObject::UuidType datasourceId_;
};

} /* namespace suri */
#endif /* VECTORDATALAYERSELECTIONSOURCE_H_ */
