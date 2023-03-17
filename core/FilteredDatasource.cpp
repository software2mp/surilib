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
#include "FilteredDatasource.h"
#include "VectorDatasource.h"
#include "FilteredVector.h"

// Includes Wx

// Defines

// forwards

namespace suri {
/**
 * Macro para registrar el tipo de elemento
 */
AUTO_REGISTER_CLASS(DatasourceInterface, FilteredDatasource, 0)

/**  Constructor  */
FilteredDatasource::FilteredDatasource():
      DatasourceInterface(TYPE_PROPERTY_VALUE_RELATIVE),
      pDatasource_(NULL), pFilter_(NULL), pInterpreter_(NULL) {
}

/** Destructor **/
FilteredDatasource::~FilteredDatasource() {
   if (pInterpreter_ != NULL)
      delete pInterpreter_;
   if (pFilter_ != NULL)
      delete pFilter_;
}

/** Devuelve el elemento de la fuente de datos \deprecated */
Element* FilteredDatasource::GetElement() {
   return pDatasource_ ? pDatasource_->GetElement() : NULL;
}
/** Devuelve la interpretacion de la fuente de datos como una imagen */
Image* FilteredDatasource::GetDatasourceAsImage() {
   return NULL;
}

/** Devuelve la interpretacion de la fuente de datos como un vector */
Vector* FilteredDatasource::GetDatasourceAsVector() {
   if (!pInterpreter_ && !pDatasource_)
      return NULL;
   return pInterpreter_? pInterpreter_->GetInterpretationAsVector() :
         pDatasource_->GetDatasourceAsVector();
}

/** Setea el siguiente Datasource si es que al crearse se detecta multiplicidad */
bool FilteredDatasource::FilteredDatasource::SetNextDatasource(DatasourceInterface* Datasource) {
   return false;
}

/** Devuelve el siguiente Datasource si es que al crearse se detecta multiplicidad */
DatasourceInterface* FilteredDatasource::GetNextDatasource() {
   return NULL;
}

/** Compara dos tipos diferentes de Datasource */
bool FilteredDatasource::FilteredDatasource::Equals(DatasourceInterface &Datasource) {
   return pDatasource_? pDatasource_->Equals(Datasource) : false;
}

/** Compara un Datasource contra un Layer */
bool FilteredDatasource::FilteredDatasource::FilteredDatasource::Equals(LayerInterface &Layer) {
   return pDatasource_? pDatasource_->Equals(Layer) : false;
}

/** Compara un Datasource contra un Element \deprecated */
 bool FilteredDatasource::FilteredDatasource::Equals(Element &Element) {
    return pDatasource_? pDatasource_->Equals(Element) : false;
 }

/** Setea el nombre de la fuente de datos */
 bool FilteredDatasource::FilteredDatasource::SetName(const std::string &Name) {
    return false;
 }
/** Obtiene el nombre de la fuente de datos. Devuelve el string vacio si no tiene nombre */
 std::string FilteredDatasource::GetName() const {
    std::string name = "SELECCION ";
    name += pDatasource_? pDatasource_->GetName() : "";
    return name;
 }

/** Cambia el nombre de la fuente de datos */
bool FilteredDatasource::SetCopyRight(const std::string &CopyRight) {
   return false;
}

/** Cambia la descripcion de una fuente de datos */
bool FilteredDatasource::SetDescription(const std::string &Description) {
   return false;
}
/** Cambia el url de la fuente de datos */
bool FilteredDatasource::SetUrl(const std::string &Url) {
   return false;
}

/** Cambia el icono asociado a la fuente de datos \deprecated */
bool FilteredDatasource::SetIcon(const std::string &Icon) {
   return false;
}

/** Cambia el nombre de la fuente de datos */
const std::string FilteredDatasource::GetCopyRight() const {
   return wxT("");
}

/** Cambia la descripcion de una fuente de datos */
const std::string FilteredDatasource::GetDescription() const {
   return pDatasource_? pDatasource_->GetDescription() : "";
}

/** Cambia el url de la fuente de datos */
const std::string FilteredDatasource::GetUrl() const {
   return pDatasource_? pDatasource_->GetUrl() : "";
}
/** Cambia el icono asociado a la fuente de datos \deprecated */
const std::string FilteredDatasource::GetIcon() const {
   return pDatasource_? pDatasource_->GetIcon() : "";
}

/** Devuelve una instancia del validador asociado a la fuente de datos**/
DatasourceValidatorInterface* FilteredDatasource::GetAssociatedValidator()const {
   return pDatasource_->GetAssociatedValidator();
}

/** Configura la fuente de datos a filtrar **/
void FilteredDatasource::SetDatasource(DatasourceInterface* pDatasource) {
   pDatasource_ = pDatasource;
   UpdateFilterInterpreter();
}

/** Configura el filtro sobre la fuente de datos. */
void FilteredDatasource::SetFilter(Filter* pNewFilter) {
   if (pFilter_ != NULL)
      delete pFilter_;
   pFilter_ = pNewFilter;
   UpdateFilterInterpreter();
}

/** Metodo encargado de actualizar el interpretador de filtro, en funcion
 *  de la fuente de datos y/o filtro indicado */
void FilteredDatasource::UpdateFilterInterpreter() {
   // TODO(Gabriel - TCK #2826): cambiar por una mejor forma de decidir
   // el interpretador de filtro (una puede ser un chain of responsability
   if (pInterpreter_ != NULL)
      delete pInterpreter_;
   VectorDatasource* pvector = dynamic_cast<VectorDatasource*>(pDatasource_);
   if (pvector) {
      pInterpreter_ = new FilteredVector(pvector->GetDatasourceAsVector(), pFilter_);
   }
}

/** Obtiene el sistema de referencia que posee la fuente de datos*/
std::string FilteredDatasource::GetSpatialReference() const {
   std::string spatialreference;
   if (pDatasource_)
      spatialreference = pDatasource_->GetSpatialReference().c_str();
   return spatialreference;
}

/** Funcion estatica para crear un FilteredDatasource
 * Metodo que crea un FilteredDatasource. Requerido para implementar Abstract Factory
 * @param[in]  FileName
 * @return NULL
 */
FilteredDatasource *FilteredDatasource::Create(const std::string &FileName) {
   return NULL;
}

/**
 * Create a partir del nodo de xml
 */
FilteredDatasource *FilteredDatasource::Create(const wxXmlNode* pRootNode) {
   return NULL;
}

/**
 * Create a partir del elemento
 */
FilteredDatasource *FilteredDatasource::Create(Element *pElement) {
   return NULL;
}

/** Funcion estatica para crear un DatasourceInterface */
FilteredDatasource* FilteredDatasource::Create(DatasourceInterface *pDatasource) {
   if (!pDatasource)
      return NULL;
   FilteredDatasource *pfilterds = new FilteredDatasource;
   if (pfilterds->Initialize(pDatasource) != pfilterds) {
      delete pfilterds;
      return NULL;
   }
   return pfilterds;
}

/** Inicializa los atributos de Datasource a partir de un nombre de archivo */
FilteredDatasource* FilteredDatasource::Initialize(const std::string &FileName) {
   return NULL;
}
/** Inicializa los atributos de Datasource a partir de un nodo xml */
FilteredDatasource* FilteredDatasource::Initialize(const wxXmlNode* pRootNode) {
   return NULL;
}
/** Inicializa los atributos de Datasource a partir de un Element \deprecated */
FilteredDatasource* FilteredDatasource::Initialize(Element *pElement) {
   return NULL;
}

/** Inicializa los atributos de Datasource a partir de un DatasourceInterface */
FilteredDatasource* FilteredDatasource::Initialize(DatasourceInterface *pDatasource) {
   FilteredDatasource* pdatasource = dynamic_cast<FilteredDatasource*>(pDatasource);
   if (pdatasource) {
      if (!pdatasource->Copy(this)) {
         return NULL;
      }
   } else
      return NULL;
   return this;
}

/** Metodo que copia la configuracion del filtro en otra instancia */
bool FilteredDatasource::Copy(FilteredDatasource* pFilterDatasource) {
   if (!pFilterDatasource)
      return false;
   pFilterDatasource->SetDatasource(pDatasource_);
   Filter* pfilter = new Filter(*pFilter_);
   pFilterDatasource->SetFilter(pfilter);
   return true;
}

/** Devuelve true si un elemento esta listo para ser utilizado **/
bool FilteredDatasource::IsValid() const {
   return true;
}

} /** namespace suri */
