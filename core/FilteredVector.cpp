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

// Includes Estandar
#include <string>

// Includes Suri
#include "FilteredVector.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Constructor
 * @param pVector Vector sobre el cual se aplicara el filtro
 * @param pFilter filtro a aplicar, puede ser NULL.
 */
FilteredVector::FilteredVector(Vector* pVector, Filter* pFilter):
      pVector_(pVector), pFilter_(pFilter), pFilteredLayer_(NULL) {
   if (pFilter_)
      UpdateFilteredLayer();
}

/** dtor **/
FilteredVector::~FilteredVector() {
   if (pFilteredLayer_ && pVector_) {
      pVector_->DestroySqlLayer(pFilteredLayer_);
      Vector::Close(pVector_);
   }
}

/**
 * Acceso a la capa por indice
 * @param LayerIndex
 * @return capa filtrada
 * @return NULL en caso de que el filtro no coincida con ningun registro
 */
OGRLayer *FilteredVector::GetLayer(int LayerIndex) const {
      return pFilteredLayer_;
}

/**
 * Devuelve el indice a la capa por nombre (-1 si falla)
 * @param LayerName
 * @return
 */
int FilteredVector::GetLayerIndex(const std::string &LayerName) const {
   if (pFilter_)
      return 0;
   else
      return pVector_->GetLayerIndex(LayerName);
}

/**
 *  Configura el filtro a aplicar
 *  @param pFilter
 */
void FilteredVector::SetFilter(Filter* pFilter) {
   if (pFilter) {
      pFilter_ = pFilter;
      UpdateFilteredLayer();
   }
}

/** Metodo auxiliar que actualiza la capa filtrada en funcion del filtro*/
void FilteredVector::UpdateFilteredLayer() {
   if (pVector_ && pFilter_) {
      if (pFilteredLayer_)
         pVector_->DestroySqlLayer(pFilteredLayer_);
      std::string query = pFilter_->GenerateQueryAsString();
      pFilteredLayer_ = pVector_->CreateSqlLayer(query);
   }
}

/**
 * Interpreta el filtro pasado por parametro.
 * @param pFilter
 * @return
 */
bool FilteredVector::Interpret(Filter* pFilter) {
   SetFilter(pFilter);
   return true;
}

/** Devuelve el resultado de la interpretacion del filtro por un Vector
 * @return
 */
Vector* FilteredVector::GetInterpretationAsVector() {
   return this;
}

/** Devuelve el resultado de la interpretacion del filtro por un Raster
 *  @return
 */
Image* FilteredVector::GetInterpretationAsRaster() {
   return NULL;
}

/** Devuelve el resultado de la interpretacion del filtro representado por una Tabla
 *  @return
 */
Table* FilteredVector::GetInterpretationAsTable() {
   return NULL;
}

} /** namespace suri */
