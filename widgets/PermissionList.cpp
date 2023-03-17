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
#include <vector>
#include <utility>

// Includes Suri
#include "PermissionList.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
PermissionList::PermissionList() {
}

/**
 * Destructor
 */
PermissionList::~PermissionList() {
}

/**
 * Autoriza una operacion a un permiso (si el permiso no existe, lo crea)
 * @param Permission Permiso a ser insertado
 * @param Operation Operacion a ser insertada en el permiso
 */
void PermissionList::InsertOperation(int Permission, OperationType Operation) {
   operations_.insert(OperationsMultiMapPair(Permission, Operation));
}

/**
 * Determina si una operacion esta permitida para un permiso dado
 * @param Permission Permiso a ser consultado
 * @param Operation Operacion a ser consultada
 * @return Devuelve true si permite la operacion para el permiso, false en caso contrario
 */
bool PermissionList::IsOperationPermitted(int Permission, OperationType Operation) const {
   if (!HasPermission(Permission))
      return false;
   for (OperationsMultiMap::const_iterator it = operations_.equal_range(Permission).first;
         it != operations_.equal_range(Permission).second; it++) {
      if ((*it).second == Operation) {
         return true;
      }
   }
   return false;
}

/**
 * Cuenta la cantidad de operaciones permitidas para un permiso dado
 * @param Permission Permiso a ser consultado
 * @return Devuelve la cantidad de operaciones permitidas por el nivel de permiso dado
 */
size_t PermissionList::CountOperations(int Permission) const {
   return operations_.count(Permission);
}

/**
 * Devuelve las operaciones permitidas para un permiso dado
 * @param Permission Permiso a ser consultado
 * @return Devuelve una lista con las operaciones permitidas, si no hay permiso la lista es vacia
 */
std::vector<PermissionList::OperationType> PermissionList::GetPermittedOperations(
      int Permission) const {
   OperationsMultiMap::const_iterator it = operations_.find(Permission);
   std::vector<OperationType> operations;
   for (it = operations_.equal_range(Permission).first;
         it != operations_.equal_range(Permission).second; it++) {
      operations.push_back((*it).second);
   }
   return operations;
}


/** Quita una opeeracoin del permiso asociado*/
void PermissionList::EraseOperation(int Permission, OperationType operation) {
   std::pair < OperationsMultiMap::iterator, OperationsMultiMap::iterator > findit =
         operations_.equal_range(Permission);
   for (OperationsMultiMap::iterator it = findit.first; it != findit.second; it++) {
      if (it->second == operation) {
            operations_.erase(it);
            break;
      }
   }
}

/**
 * Quita un permiso, su id y junto con sus operaciones permitidas
 * @param Permission Permiso a ser removido
 */
void PermissionList::ErasePermission(int Permission) {
   operations_.erase(Permission);
}

/**
 * Elimina el registro de todos los permisos y operaciones
 */
void PermissionList::ClearPermissions() {
   operations_.clear();
}

/**
 * Determina si tiene el permiso
 * @param Permission
 * @return
 */
bool PermissionList::HasPermission(int Permission) const {
   if (operations_.size() == 0) return false;
   OperationsMultiMap::const_iterator it = operations_.find(Permission);
   return (it != operations_.end());
}

} /** namespace suri */
