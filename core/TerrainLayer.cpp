/*! \file TerrainLayer.cpp */
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
#include "TerrainLayer.h"
#include "suri/Element.h"
#include "suri/DatasourceInterface.h"
#include "suri/ViewcontextInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
TerrainLayer::TerrainLayer(DatasourceInterface *pDatasource):
      pDatasource_(pDatasource), pElement_(NULL), hidden_(false), pViewcontext_(NULL) {
}

/**
 * Constructor
 */
TerrainLayer::TerrainLayer(DatasourceInterface *pDatasource, Element *pElement):
      pDatasource_(pDatasource), pElement_(pElement), hidden_(false),
      pViewcontext_(NULL) {
}

/**
 * Destructor
 */
TerrainLayer::~TerrainLayer() {
}

/**
 * Devuelve el Id de Datasource asociado al layer
 */
SuriObject::UuidType TerrainLayer::GetAssociatedDatasourceId() const {
   return pDatasource_->GetId();
}

/**
 * Devuelve el estilo de renderizacion del layer
 */
void TerrainLayer::GetRenderizationStyle(const SuriObject::UuidType &LayerId) const {
}

/**
 * Devuelve el elemento
 * \deprecated
 */
Element* TerrainLayer::GetElement() {
   return pElement_;
}

/**
 * Compara un Datasource contra un Layer
 */
bool TerrainLayer::Equals(DatasourceInterface &Datasource) {
   return (Datasource.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara dos tipos diferentes de Layers
 */
bool TerrainLayer::Equals(LayerInterface &Layer) {
   return (Layer.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara un Layer contra un Element
 * \deprecated
 */
bool TerrainLayer::Equals(Element &Element) {
   return (Element.GetUid() == GetElement()->GetUid());
}

/**
 * Oculta
 */
void TerrainLayer::Hide(bool Hide) {
   hidden_ = Hide;
}

/**
 * Verifica si esta oculto
 */
bool TerrainLayer::IsHidden() const {
   return hidden_;
}

/** Setea el nombre de la capa **/
void TerrainLayer::SetName(const std::string& Name) {
   name_ = Name;
   pElement_->SetName(Name);
}

/** Setea el titulo de la capa **/
void TerrainLayer::SetTitle(const std::string& Title) {
   title_ = Title;
}

/** Configura el viewcontext al cual pertenece
 * @param[in] pViewcontext referencia al viewcontext contenedor
 **/
void TerrainLayer::SetViewcontext(ViewcontextInterface* pViewcontext) {
   pViewcontext_ = pViewcontext;
}

/** Obtiene la instancia de fuente de datos asociada a la capa
  * @return DatasourceInterface* referencia a la instancia de la fuente de datos
 * asociada a la capa
 * @return NULL en caso de que no exista la fuente de datos
 **/
DatasourceInterface* TerrainLayer::FetchAssociatedDatasource() {
   if (!pViewcontext_)
      return NULL;
   return pViewcontext_->GetAssociatedDatasource(GetId());
}

} /** namespace suri */
