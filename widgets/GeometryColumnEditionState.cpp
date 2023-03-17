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

// Includes suri
#include "GeometryColumnEditionState.h"

namespace suri {

/** Ctor */
GeometryColumnEditionState::GeometryColumnEditionState(Table* pTable,
                                    const std::string &LayerSrs,
                                    const std::string &GeometryColumnName) {
   pDataLayer_ = new VectorDataLayer(pTable, LayerSrs, GeometryColumnName);
   pEditedFeature_ = NULL;
}

/** Dtor */
GeometryColumnEditionState::~GeometryColumnEditionState() {
   delete pEditedFeature_;
   delete pDataLayer_;
}

/**
 * Inicia la edicion de un feature
 * @param[in] FeatureId id del feature que quiero editar
 * @return bool que indica si pudo iniciar la edicion
 */
bool GeometryColumnEditionState::StartFeatureEdition(FeatureIdType FeatureId) {
   if (IsEditing())
      return false;
   pEditedFeature_ = pDataLayer_->GetFeature(FeatureId);
   return pEditedFeature_ != NULL;
}

/**
 * Finaliza la edicion
 * @param[in] SaveChanges bool que indica si se deben salvar los cambios.
 * @return bool que indica si pudo guardar los cambios
 */
bool GeometryColumnEditionState::EndFeatureEdition(bool SaveChanges) {
   if (!IsEditing())
      return true;

   bool returnvalue = false;
   if (SaveChanges && pEditedFeature_->pGeometry_ &&
                                     pEditedFeature_->pGeometry_->IsValid())
      returnvalue = pDataLayer_->UpdateGeometry(pEditedFeature_->featureId_,
                                                   pEditedFeature_->pGeometry_);

   if (returnvalue || (!SaveChanges && pEditedFeature_)) {
      delete pEditedFeature_;
      pEditedFeature_ = NULL;
   }

   return returnvalue;
}

/** Retorna el feature que se esta editando/creando */
Geometry* GeometryColumnEditionState::GetEditedGeometry() {
   return IsEditing() ? pEditedFeature_->pGeometry_ : NULL;
}

/** Configura la geometria en el feature que se esta editando/creando */
bool GeometryColumnEditionState::SetEditedGeometry(Geometry* pGeometry) {
   if (!IsEditing())
      return false;

   delete pEditedFeature_->pGeometry_;
   pEditedFeature_->pGeometry_ = pGeometry;
   return true;
}

/**
 * Indica si se esta editando un feature
 * @return bool que indica si se esta editando una geometria.
 */
bool GeometryColumnEditionState::IsEditing() const {
   return pEditedFeature_ != NULL;
}

/** Indica si la geometria editada es valida */
bool GeometryColumnEditionState::IsValid() const {
   return IsEditing() && pEditedFeature_->pGeometry_ &&
                        pEditedFeature_->pGeometry_->IsValid();
}

} /* namespace suri */
