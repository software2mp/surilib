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

#ifndef VECTORDATASOURCESELECTIONSOURCE_H_
#define VECTORDATASOURCESELECTIONSOURCE_H_

// Includes standard
#include <string>

// Includes suri
#include "suri/FeatureSelectionSource.h"
#include "FilteredDatasource.h"

namespace suri {

// fordwards
class Vector;
class VectorStyle;

/**
 * Permite seleccionar features de una fuente de datos vectorial.
 * Si pFiltredDatasource que recibe en constructor != NULL, cada vez
 * que se ocurre un cambio, actualiza el filtro para que coincida 
 * con la seleccion.
 */
class VectorDatasourceSelectionSource : public FeatureSelectionSource {
public:
   /** Ctor */
   VectorDatasourceSelectionSource(VectorDatasource *pDatasource,
                                    FilteredDatasource* pFiltredDatasource);
   /** Dtor */
   virtual ~VectorDatasourceSelectionSource();

   /** Retorna el id del datasource donde se seleccionana features. */
   virtual SuriObject::UuidType GetWorkingDatasourceId();

   /** Limpia la seleccion **/
   virtual void ClearSelection();
   /** Selecciona un feature */
   virtual bool Select(FeatureIdType FeatureId,
                  SuriObject::UuidType DatasourceId = SuriObject::NullUuid);
   /** Deselecciona un feature */
   virtual bool Unselect(FeatureIdType FeatureId,
                  SuriObject::UuidType DatasourceId = SuriObject::NullUuid);

   /** Selecciona los features cuya geometria intersecta el subset */
   virtual bool Select(Subset &SubsetArea, const World* pWorld);
   /** Retorna las geometrias seleccionadas */
   virtual GeometryCollection* GetSelectedGeometries(const World* pWorld);

protected:
   /** Indica si un feature se puede seleccionar */
   virtual bool ValidateFeature(SuriObject::UuidType DatasourceId,
                                                FeatureIdType FeatureId);
   /** Actualiza la seleccion en datasource */
   bool UpdateDatasourceFilter();

   /** Fuente de datos que posee el filtro de seleccion **/
   FilteredDatasource* pFiltredDatasource_;
   /** Fuente de datos vectorial sobre el que se estan seleccionando features */
   VectorDatasource *pDatasource_;
   /** Vector asociado a fuente de datos */
   Vector* pVector_;

   /** Cache con la seleccion, evita buscar en vector si no se actualizo seleccion. */
   GeometryCollection* pSelectedGeometries_;

   /** Estilo de geometrias seleccionada */
   VectorStyle* pSelectedGeometryStyle_;

   /** Valor que se le pasa a filtro para filtrar todas las capas */
   static const int INVALID_FEATUREID = -1;
   /** nombre del campo que contiene los features ids  */
   static const std::string FEATURE_ID_FIELD_NAME;
};

} /* namespace suri */
#endif /* VECTORDATASOURCESELECTIONSOURCE_H_ */
