/*! \file FilteredVector.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
