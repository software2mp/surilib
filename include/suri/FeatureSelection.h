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

#ifndef FEATURESELECTION_H_
#define FEATURESELECTION_H_

// Includes Estandar
#include <set>

// Includes Suri
#include "suri/Subject.h"
#include "suri/FeatureSelectionSource.h"
#include "GeometryCollection.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 * Administra la seleccion de features en la aplicacion.
 * Existen dos modos de seleccion:
 * i. Para seleccionar un feature hay que pasar el id de la fuente de datos
 * y el id del feature dentro de la fuente de datos.
 * i. Para seleccionar un feature solo hay que pasar el FeatureId, el
 * FeatureSelectionSource sabe en que datasource buscar.
 * 
 * Cada FeatureSelectionSource pueden soportar uno o los dos modos.
 */
class FeatureSelection : public suri::Subject {
public:
   /** ctor **/
   FeatureSelection();
   /** dtor **/
   virtual ~FeatureSelection();

   /** Configura el GeometrySelectionSource que maneja la seleccion */
   bool Configure(FeatureSelectionSource* pSelectionSource);
   /** Retorna el id del datasource donde se seleccionana features */
   SuriObject::UuidType GetWorkingDatasourceId();

   /** Limpia la seleccion **/
   virtual void ClearSelection();
   /** Actualiza la seleccion existente notificando a los interesados **/
   virtual void UpdateSelection(const std::set<FeatureIdType>& FeaturesSelected,
                  const SuriObject::UuidType &DatasourceId = SuriObject::NullUuid);
   /** Selecciona un feature */
   virtual bool Select(FeatureIdType FeatureId,
                  const SuriObject::UuidType &DatasourceId = SuriObject::NullUuid);
   /** Deselecciona un feature */
   virtual bool Unselect(FeatureIdType FeatureId,
                  const SuriObject::UuidType &DatasourceId = SuriObject::NullUuid);
   /** Retorna vector con el id de los features seleccionados de DatasourceId */
   virtual std::vector<FeatureIdType> GetSelectedFeatures(
                  const SuriObject::UuidType &DatasourceId = SuriObject::NullUuid);

   /** Selecciona todos los features cuya geometria intersectan el subset */
   virtual bool Select(Subset &SubsetArea, const World* pWorld);
   /** Retorna las geometrias seleccionadas */
   virtual GeometryCollection* GetSelectedGeometries(const World* pWorld);

private:
   /** Clase que administra la seleccion, patron strategy */
   FeatureSelectionSource* pSelectionSource_;
};

} /** namespace suri */

#endif /* FEATURESELECTION_H_ */
