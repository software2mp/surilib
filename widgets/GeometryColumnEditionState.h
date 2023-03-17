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

#ifndef GEOMETRYEDITIONSTATE_H_
#define GEOMETRYEDITIONSTATE_H_

// Includes standard
#include <string>

// Includes suri
#include "suri/Table.h"
#include "VectorDataLayer.h"

namespace suri {

/**
 * Clase que administra la edicion de geometrias de una tabla. No se puede
 * editar mas de una geometria al mismo tiempo.
 * No tiene logica de commit, por lo que el cliente es el encargado
 * de persistir los cambios en la tabla.
 */
class GeometryColumnEditionState {
public:
   /** Ctor */
   GeometryColumnEditionState(Table* pTable, const std::string &LayerSrs,
                                       const std::string &GeometryColumnName);
   /** Dtor */
   ~GeometryColumnEditionState();

   /** Inicia la edicion de un feature */
   bool StartFeatureEdition(FeatureIdType FeatureId);
   /** Finaliza la edicion */
   bool EndFeatureEdition(bool SaveChanges);
   /** Retorna la geometria en el feature que se esta editando/creando */
   Geometry* GetEditedGeometry();
   /** Configura la geometria en el feature que se esta editando/creando */
   bool SetEditedGeometry(Geometry* pGeometry);

   /** Indica si se esta editando */
   bool IsEditing() const;
   /** Indica si la geometria editada es valida */
   bool IsValid() const;

private:
   /** DataLayer que permite obtener geometrias de capa */
   VectorDataLayer* pDataLayer_;
   /** Feature que se esta editando */
   VectorDataLayer::FeatureData* pEditedFeature_;
};

} /* namespace suri */
#endif /* GEOMETRYEDITIONSTATE_H_ */
