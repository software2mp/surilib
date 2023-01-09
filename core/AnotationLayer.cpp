/*! \file AnotationLayer.cpp */
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
#include "AnotationLayer.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
AnotationLayer::AnotationLayer(DatasourceInterface *pDatasource) :
      VectorLayer(pDatasource), pDatasource_(pDatasource), pElement_(NULL) {
}

/**
 * Constructor
 */
AnotationLayer::AnotationLayer(DatasourceInterface *pDatasource, Element *pElement) :
      VectorLayer(pDatasource), pDatasource_(pDatasource), pElement_(pElement) {
}

/**
 * Destructor
 */
AnotationLayer::~AnotationLayer() {
}

/**
 * Obtiene el id del datasource asociado
 */
SuriObject::UuidType AnotationLayer::GetAssociatedDatasourceId() const {
   return pDatasource_->GetId();
}

/**
 * Devuelve el estilo de renderizacion del layer
 */
void AnotationLayer::GetRenderizationStyle(const SuriObject::UuidType &LayerId) const {
}

/**
 * Devuelve el elemento
 * \deprecated
 */
Element* AnotationLayer::GetElement() {
   return pElement_;
}

/**
 * Compara un Datasource contra un Layer
 */
bool AnotationLayer::Equals(DatasourceInterface &Datasource) {
   return (Datasource.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara dos tipos diferentes de Layers
 */
bool AnotationLayer::Equals(LayerInterface &Layer) {
   return (Layer.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara un Layer contra un Element
 * \deprecated
 */
bool AnotationLayer::Equals(Element &Element) {
   return (Element.GetUid() == GetElement()->GetUid());
}

} /** namespace suri */
