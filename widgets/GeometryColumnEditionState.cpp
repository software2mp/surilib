/** \file GeometryEditionState.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
