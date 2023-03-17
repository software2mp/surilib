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
#include <list>
#include <vector>

// Includes Suri
#include "DefaultDatasourceList.h"
#include "suri/LayerList.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
DefaultDatasourceList::DefaultDatasourceList() {
}

/**
 * Destructor
 */
DefaultDatasourceList::~DefaultDatasourceList() {
}

/**
 *  Verifica que no exista el Datasource en la lista
 *  @param[in] pDatasource fuente de datos a verificar
 */
bool DefaultDatasourceList::Exists(DatasourceInterface *pDatasource) {
   bool exists = false;
   DatasourceList::iterator it = datasourceList_.begin();
   while (!exists && it != datasourceList_.end()) {
      if (((*it)->Equals(*pDatasource))) {
         exists = true;
      }
      ++it;
   }
   return exists;
}

/**
 * Agrega una fuente de datos
 * @param[in] pDatasource Datasource a agregar
 *  @return true en caso de se quiera agregar el elemento y no exista, false en caso contrario
 */
bool DefaultDatasourceList::AddDatasource(DatasourceInterface* pDatasource) {
   if (!pDatasource) return false;
   if (!Exists(pDatasource)) {
      datasourceList_.push_back(pDatasource);
      return true;
   }
   return false;
}

/**
 * Quita una fuente de datos a partir de un id de datasource
 * @param[in] Id id de la fuente de datos a eliminar
 * @return true si puede eliminar la fuente de datos, false en caso contrario
 */
bool DefaultDatasourceList::RemoveDatasource(const SuriObject::UuidType& Id) {
   bool removed = false;
   DatasourceList::iterator it = datasourceList_.begin();
   while (!removed && it != datasourceList_.end()) {
      if (((*it)->GetId() == Id)) {
         removed = true;
         datasourceList_.erase(it);
      }
      ++it;
   }
   return removed;
}

/**
 * Obtiene una fuente de datos
 * @param[in] Id id de la fuente de datos a obtener
 *  @return puntero al datasource solicitado o NULL en caso de que no se encuentre
 */
DatasourceInterface* DefaultDatasourceList::GetDatasource(const SuriObject::UuidType& Id) const {
   bool found = false;
   DatasourceInterface *pdatasource = NULL;
   DatasourceList::const_iterator it = datasourceList_.begin();
   while (!found && it != datasourceList_.end()) {
      if (((*it)->GetId() == Id)) {
         found = true;
         pdatasource = *it;
      }
      ++it;
   }
   return pdatasource;
}

/**
 * Obtiene una fuente de datos a partir del Id del Element asociado
 * \deprecated
 * @param[in] ElementId Id del elemento
 * @return DatasourceInterface* Datasource asociada al Id del Element
 */
DatasourceInterface* DefaultDatasourceList::GetDatasourceByElementId(
      const SuriObject::UuidType& ElementId) const {
   bool found = false;
   DatasourceInterface *pdatasource = NULL;
   DatasourceList::const_iterator it = datasourceList_.begin();
   while (!found && it != datasourceList_.end()) {
      if (((*it)->GetElement()->GetUid() == ElementId)) {
         found = true;
         pdatasource = *it;
      }
      ++it;
   }
   return pdatasource;
}

/**
 * Retorna una lista de Ids de las fuentes de datos.
 * @return Lista de los Ids de las fuentes de datos.
 */
DatasourceContainerInterface::UuidListType DefaultDatasourceList::GetDatasourceList() const {
   DatasourceContainerInterface::UuidListType list;
   DatasourceList::const_iterator it = datasourceList_.begin();
   for (; it != datasourceList_.end(); ++it) {
      list.push_back((*it)->GetId());
   }
   return list;
}

/**
 * Cambia el orden de un elemento de la lista hacia la posicion siguiente al elemento destino
 * @param[in] Id Id del Datasource que se desea mover
 * @param[in] DestinationId Id del datasource que posee la posicion a insertar el Datasource
 * @return true en caso de poder cambiar el orden, false en caso contrario
 */
bool DefaultDatasourceList::ChangeOrder(const SuriObject::UuidType& Id,
                                        const SuriObject::UuidType& DestinationId) {
   if (Id == DestinationId)
      return true;
   DatasourceInterface *pdatasourceaux = NULL;
   DatasourceList::iterator it = datasourceList_.begin();
   bool found = false;
   for (; it != datasourceList_.end(); ++it) {
      if ((*it)->GetId() == Id) {
         pdatasourceaux = *it;
         datasourceList_.erase(it);
         found = true;
         break;
      }
   }
   if (!found) {
      return false;
   }
   bool moved = false;
   DatasourceList::iterator itTo = datasourceList_.begin();
   for (; itTo != datasourceList_.end(); ++itTo) {
      if ((*itTo)->GetId() == DestinationId) {
         datasourceList_.insert(itTo, pdatasourceaux);
         moved = true;
         break;
      }
   }
   return moved;
}

/**
 * Mueve un elemento al final de la lista
 * @param[in] pDatasource Elemento que se desea mover al final de la lista
 * @return true en caso de poder mover el elemento al final, false en caso contrario
 */
bool DefaultDatasourceList::MoveToEnd(DatasourceInterface* pDatasource) {
   DatasourceInterface* pdatasourceaux = NULL;
   DatasourceList::iterator it = datasourceList_.begin();
   bool moved = false;
   for (; it != datasourceList_.end(); ++it) {
      if ((*it)->GetId() == pDatasource->GetId()) {
         pdatasourceaux = *it;
         datasourceList_.erase(it);
         moved = true;
         break;
      }
   }
   if (moved) {
      datasourceList_.push_back(pdatasourceaux);
   }
   return moved;
}

/**
 * Obtiene los ids de las fuente de datos que contiene la lista
 * @return std::vector<SuriObject::UuidType> vector con ids
 */
std::vector<SuriObject::UuidType> DefaultDatasourceList::GetDatasourceIds() const {
   std::vector<SuriObject::UuidType> idvector;
   DatasourceList::const_iterator it = datasourceList_.begin();
   for (; it != datasourceList_.end() ; ++it) {
      idvector.push_back((*it)->GetId());
   }
   return idvector;
}

} /** namespace suri */
