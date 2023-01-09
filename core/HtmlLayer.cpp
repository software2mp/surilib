/*! \file HtmlLayer.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
// Includes Suri
#include "HtmlLayer.h"
#include "suri/ViewcontextInterface.h"

// Includes Wx

// Defines

// forwards


namespace suri {

/**
 * Constructor
 */
HtmlLayer::HtmlLayer(DatasourceInterface *pDatasource):
      pDatasource_(pDatasource), pElement_(NULL), hidden_(false), pViewcontext_(NULL) {
}

/**
 * Constructor
 * \deprecated
 */
HtmlLayer::HtmlLayer(DatasourceInterface *pDatasource, Element *pElement) :
      pDatasource_(pDatasource), pElement_(pElement), hidden_(false),
      pViewcontext_(NULL) {
}

HtmlLayer::~HtmlLayer() {
	// TODO Auto-generated destructor stub
}

/**
 * Obtiene la Datasource asociada a la Layer.
 * Trazabilidad entre Datasource Layer
 * @return Id asociados
 */
SuriObject::UuidType HtmlLayer::GetAssociatedDatasourceId() const {
   return pDatasource_->GetId();
}

/**
 * Devuelve el estilo de renderizacion del layer
 */
void HtmlLayer::GetRenderizationStyle(const SuriObject::UuidType &LayerId) const {
   // TODO(Sebastian - TCK #852): Implementar
}

/**
 * Devuelve el elemento
 * \deprecated
 */
Element* HtmlLayer::GetElement() {
   return pElement_;
}

/**
 * Compara un Datasource contra un Layer
 */
bool HtmlLayer::Equals(DatasourceInterface &Datasource) {
   return (Datasource.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara dos tipos diferentes de Layers
 */
bool HtmlLayer::Equals(LayerInterface &Layer) {
   return (Layer.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara un Layer contra un Element
 * \deprecated
 */
bool HtmlLayer::Equals(Element &Element) {
   return (Element.GetUid() == GetElement()->GetUid());
}

/**
 * Oculta
 */
void HtmlLayer::Hide(bool Hide) {
   hidden_ = Hide;
}

/** Verifica si esta oculto */
bool HtmlLayer::IsHidden() const {
   return hidden_;
}

/** Setea el nombre de la capa **/
void HtmlLayer::SetName(const std::string& Name) {
   name_ = Name;
   pElement_->SetName(Name);
}

/** Setea el titulo de la capa **/
void HtmlLayer::SetTitle(const std::string& Title) {
   title_ = Title;
}

/** Configura el viewcontext al cual pertenece
 * @param[in] pViewcontext referencia al viewcontext contenedor
 **/
void HtmlLayer::SetViewcontext(ViewcontextInterface* pViewcontext) {
   pViewcontext_ = pViewcontext;
}

/** Obtiene la instancia de fuente de datos asociada a la capa
  * @return DatasourceInterface* referencia a la instancia de la fuente de datos
 * asociada a la capa
 * @return NULL en caso de que no exista la fuente de datos
 **/
DatasourceInterface* HtmlLayer::FetchAssociatedDatasource() {
   if (!pViewcontext_)
      return NULL;
   return pViewcontext_->GetAssociatedDatasource(GetId());
}
} /* namespace suri */
