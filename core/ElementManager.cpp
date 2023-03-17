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

// Includes standard
#include <list>

// Includes Suri
#include "suri/ElementManager.h"

// Includes Wx

// Includes OGR

// Defines

/** namespace suri */
namespace suri {

/**
 * Constructor
 */
ElementManagerClass::ElementManagerClass() :
      pAddFunc_(NULL), pDeleteFunc_(NULL), pDeletesFunc_(NULL) {
}

/**
 * Destructor
 */
ElementManagerClass::~ElementManagerClass() {}

/**
 * Llama a la funcion callback para agregar un elemento
 * @param[in] pElementNode Nodo del elemento
 * @return True en caso de exito
 * @return False si no esta registrada la funcion o error al agregar.
 */
bool ElementManagerClass::AddElement(wxXmlNode *pElementNode) {
   if (pAddFunc_) {
      return pAddFunc_(pDatasourceManager_, pElementNode);
   }
   REPORT_AND_FAIL_VALUE("D: No se resgistro ninguna funcion para agregar elementos.", false);
}

/**
 * Llama a la funcion callback para eliminar un elemento
 * @param[in] ElementUid Identificador del elemento
 * @return True en caso de exito
 * @return False si no esta registrada la funcion o error al agregar.
 */
bool ElementManagerClass::DeleteElement(const Element::UidType& ElementUid) {
   if (pDeleteFunc_) {
      return pDeleteFunc_(pDatasourceManager_, ElementUid);
   }
   REPORT_AND_FAIL_VALUE("D: No se resgistro ninguna funcion para eliminar elementos.", false);
}

/**
 * Llama a la funcion callback para eliminar un elemento
 * @param[in] ElementUids Lista con Identificadores de los elementos
 * @return True en caso de exito
 * @return False si no esta registrada la funcion o error al agregar.
 */
bool ElementManagerClass::DeleteElements(
      const std::list<suri::Element::UidType>& ElementUids) {
   if (pDeletesFunc_) {
      return pDeletesFunc_(pDatasourceManager_, ElementUids);
   }
   REPORT_AND_FAIL_VALUE("D: No se resgistro ninguna funcion para eliminar elementos.", false);
}

/**
 * Registra la funcion
 * @param[in] pFunction Funcion callback
 */
void ElementManagerClass::RegisterAddElement(
      DatasourceManagerInterface* DatasourceManager, AddElementFunction pFunction) {
   pDatasourceManager_ = DatasourceManager;
   pAddFunc_ = pFunction;
}

/**
 * Registra la funcion
 * @param[in] pFunction Funcion callback
 */
void ElementManagerClass::RegisterDeleteElement(
      DatasourceManagerInterface* DatasourceManager, DeleteElementFunction pFunction) {
   pDatasourceManager_ = DatasourceManager;
   pDeleteFunc_ = pFunction;
}

/**
 * Registra la funcion
 * @param[in] pFunction Funcion callback
 */
void ElementManagerClass::RegisterDeleteElements(
      DatasourceManagerInterface* DatasourceManager, DeleteElementsFunction pFunction) {
   pDatasourceManager_ = DatasourceManager;
   pDeletesFunc_ = pFunction;
}

/**
 * Obtiene la instancia. Singleton
 */
ElementManagerClass& ElementManager::Instance() {
   static ElementManagerClass object;
   return object;
}

}  // namespace suri

