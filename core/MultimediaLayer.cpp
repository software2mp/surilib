/*! \file MultimediaLayer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "MultimediaLayer.h"
#include "suri/ViewcontextInterface.h"

namespace suri {

/**
 * Constructor
 */
MultimediaLayer::MultimediaLayer(DatasourceInterface *pDatasource):
      pDatasource_(pDatasource), pElement_(NULL), hidden_(false), pViewcontext_(NULL) {
}

/**
 * Constructor
 * \deprecated
 */
MultimediaLayer::MultimediaLayer(DatasourceInterface *pDatasource, Element *pElement) :
      pDatasource_(pDatasource), pElement_(pElement), hidden_(false),
      pViewcontext_(NULL) {
}

MultimediaLayer::~MultimediaLayer() {
	// TODO Auto-generated destructor stub
}

/**
 * Obtiene la Datasource asociada a la Layer.
 * Trazabilidad entre Datasource Layer
 * @return Id asociados
 */
SuriObject::UuidType MultimediaLayer::GetAssociatedDatasourceId() const {
   return pDatasource_->GetId();
}

/**
 * Devuelve el estilo de renderizacion del layer
 */
void MultimediaLayer::GetRenderizationStyle(const SuriObject::UuidType &LayerId) const {
   // TODO(Sebastian - TCK #852): Implementar
}

/**
 * Devuelve el elemento
 * \deprecated
 */
Element* MultimediaLayer::GetElement() {
   return pElement_;
}

/**
 * Compara un Datasource contra un Layer
 */
bool MultimediaLayer::Equals(DatasourceInterface &Datasource) {
   return (Datasource.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara dos tipos diferentes de Layers
 */
bool MultimediaLayer::Equals(LayerInterface &Layer) {
   return (Layer.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara un Layer contra un Element
 * \deprecated
 */
bool MultimediaLayer::Equals(Element &Element) {
   return (Element.GetUid() == GetElement()->GetUid());
}

/**
 * Oculta
 */
void MultimediaLayer::Hide(bool Hide) {
   hidden_ = Hide;
}

/** Verifica si esta oculto */
bool MultimediaLayer::IsHidden() const {
   return hidden_;
}

/** Setea el nombre de la capa **/
void MultimediaLayer::SetName(const std::string& Name) {
   name_ = Name;
   pElement_->SetName(Name);
}

/** Setea el titulo de la capa **/
void MultimediaLayer::SetTitle(const std::string& Title) {
   title_ = Title;
}

/** Configura el viewcontext al cual pertenece
 * @param[in] pViewcontext referencia al viewcontext contenedor
 **/
void MultimediaLayer::SetViewcontext(ViewcontextInterface* pViewcontext) {
   pViewcontext_ = pViewcontext;
}

/** Obtiene la instancia de fuente de datos asociada a la capa
  * @return DatasourceInterface* referencia a la instancia de la fuente de datos
 * asociada a la capa
 * @return NULL en caso de que no exista la fuente de datos
 **/
DatasourceInterface* MultimediaLayer::FetchAssociatedDatasource() {
   if (!pViewcontext_)
      return NULL;
   return pViewcontext_->GetAssociatedDatasource(GetId());
}

} /* namespace suri */
