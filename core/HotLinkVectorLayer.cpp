/*! \file HotLinkVectorLayer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar

// Includes Suri
#include "HotLinkVectorLayer.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

HotLinkVectorLayer::HotLinkVectorLayer(DatasourceInterface *pDatasource) :
      VectorLayer(pDatasource), pDatasource_(pDatasource), pElement_(NULL) {
}

HotLinkVectorLayer::HotLinkVectorLayer(DatasourceInterface *pDatasource, Element *pElement) :
      VectorLayer(pDatasource), pDatasource_(pDatasource), pElement_(pElement) {
}

HotLinkVectorLayer::~HotLinkVectorLayer() {
}

SuriObject::UuidType HotLinkVectorLayer::GetAssociatedDatasourceId() const {
   return pDatasource_->GetId();
}

/**
 * Devuelve el estilo de renderizacion del layer
 */
void HotLinkVectorLayer::GetRenderizationStyle(const SuriObject::UuidType &LayerId) const {
}

/**
 * Devuelve el elemento
 * \deprecated
 */
Element* HotLinkVectorLayer::GetElement() {
   return pElement_;
}

/**
 * Compara un Datasource contra un Layer
 */
bool HotLinkVectorLayer::Equals(DatasourceInterface &Datasource) {
   return (Datasource.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara dos tipos diferentes de Layers
 */
bool HotLinkVectorLayer::Equals(LayerInterface &Layer) {
   return (Layer.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara un Layer contra un Element
 */
bool HotLinkVectorLayer::Equals(Element &Element) {
   return (Element.GetUid() == GetElement()->GetUid());
}

} /** namespace suri */
