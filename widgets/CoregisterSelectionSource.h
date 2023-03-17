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

#ifndef COREGISTERSELECTIONSOURCE_H_
#define COREGISTERSELECTIONSOURCE_H_

// Includes suri
#include "VectorDataLayerSelectionSource.h"
#include "suri/CoregisterTask.h"

namespace suri {

/**
 * Administra la seleccion mientras este activa la tarea de coregistro.
 */
class CoregisterSelectionSource : public FeatureSelectionCachedSource {
public:
   /** Ctor */
   explicit CoregisterSelectionSource(CoregisterTask* pTask);
   /** Dtor */
   virtual ~CoregisterSelectionSource();

   /** Retorna el id del datasource donde se seleccionan features. */
   virtual SuriObject::UuidType GetWorkingDatasourceId();

   /** Selecciona los features cuya geometria intersecta el subset */
   virtual bool Select(Subset &SubsetArea, const World* pWorld);

protected:
   /** Indica si un feature se puede seleccionar */
   virtual bool ValidateFeature(SuriObject::UuidType DatasourceId,
                                                FeatureIdType FeatureId);

   /** Retorna el cache asociado a un mundo */
   virtual GeometryCollection* GetSelectionCache(const World* pWorld);
   /** Tarea de coregistro */
   CoregisterTask* pTask_;
   /** Id con el que se identifica a la tabla de gcps */
   SuriObject::UuidType gcpTableId_;
};

} /* namespace suri */
#endif /* COREGISTERSELECTIONSOURCE_H_ */
