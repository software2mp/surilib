/*! \file PermissionList.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
