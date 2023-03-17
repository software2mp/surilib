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
