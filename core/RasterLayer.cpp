/*! \file RasterLayer.cpp */
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
#include "RasterLayer.h"
#include "suri/Element.h"
#include "suri/DatasourceInterface.h"
#include "suri/ViewcontextInterface.h"
#include "suri/RasterElement.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
RasterLayer::RasterLayer(DatasourceInterface *pDatasource):
      pDatasource_(pDatasource), pElement_(NULL), hidden_(false), pViewcontext_(NULL) {
}

/**
 * Constructor
 * \deprecated
 */
RasterLayer::RasterLayer(DatasourceInterface *pDatasource, Element *pElement) :
      pDatasource_(pDatasource), pElement_(pElement), hidden_(false),
      pViewcontext_(NULL) {
}

/**
 * Destructor
 */
RasterLayer::~RasterLayer() {}

/**
 * Obtiene la Datasource asociada a la Layer.
 * Trazabilidad entre Datasource Layer
 * @return Id asociados
 */
SuriObject::UuidType RasterLayer::GetAssociatedDatasourceId() const {
   return pDatasource_->GetId();
}

/**
 * Devuelve el estilo de renderizacion del layer
 */
void RasterLayer::GetRenderizationStyle(const SuriObject::UuidType &LayerId) const {
   // TODO(Sebastian - TCK #852): Implementar
}

/**
 * Devuelve el elemento
 * \deprecated
 */
Element* RasterLayer::GetElement() {
   return pElement_;
}

/**
 * Compara un Datasource contra un Layer
 */
bool RasterLayer::Equals(DatasourceInterface &Datasource) {
   return (Datasource.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara dos tipos diferentes de Layers
 */
bool RasterLayer::Equals(LayerInterface &Layer) {
   return (Layer.GetElement()->GetUid() == GetElement()->GetUid());
}

/**
 * Compara un Layer contra un Element
 * \deprecated
 */
bool RasterLayer::Equals(Element &Element) {
   return (Element.GetUid() == GetElement()->GetUid());
}

/**
 * Oculta
 */
void RasterLayer::Hide(bool Hide) {
   hidden_ = Hide;
}

/** Verifica si esta oculto */
bool RasterLayer::IsHidden() const {
   return hidden_;
}

/** Setea el nombre de la capa **/
void RasterLayer::SetName(const std::string& Name) {
   name_ = Name;
   pElement_->SetName(Name);
}

/** Setea el titulo de la capa **/
void RasterLayer::SetTitle(const std::string& Title) {
   title_ = Title;
}

/** Configura el viewcontext al cual pertenece
 * @param[in] pViewcontext referencia al viewcontext contenedor
 **/
void RasterLayer::SetViewcontext(ViewcontextInterface* pViewcontext) {
   pViewcontext_ = pViewcontext;
}

/** Obtiene la instancia de fuente de datos asociada a la capa
  * @return DatasourceInterface* referencia a la instancia de la fuente de datos
 * asociada a la capa
 * @return NULL en caso de que no exista la fuente de datos
 **/
DatasourceInterface* RasterLayer::FetchAssociatedDatasource() {
   if (!pViewcontext_)
      return NULL;
   return pViewcontext_->GetAssociatedDatasource(GetId());
}

/** Devuelve el viewcontext */
ViewcontextInterface* RasterLayer::GetViewcontext() {
   return pViewcontext_;
}

/** Devuelve la cantidad de bandas del raster
 *  @return entero con la cantidad de bandas del raster
 *  @return -1 en caso de no poder obtener la cantidad de bandas
 */
int RasterLayer::GetBandCount() const {
   int bandcount = -1;
   RasterElement* praster = dynamic_cast<RasterElement*>(pElement_);
   if (praster) {
      bandcount = praster->GetBandCount();
   }
   return bandcount;
}

/** Devuelve el nombre de una banda */
bool RasterLayer::GetBandName(std::string &BandName, int BandNumber) const {
   RasterElement* praster = dynamic_cast<RasterElement*>(pElement_);
   if (!praster)
      return false;
   return praster->GetBandName(BandName, BandNumber);
}
} /** namespace suri */
