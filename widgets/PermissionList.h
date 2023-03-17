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

#ifndef PERMISSIONLIST_H_
#define PERMISSIONLIST_H_

// Includes Estandar
#include <map>
#include <vector>
#include <utility>
#include <cstddef>

// Includes Suri

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Provee la lista de permisos asociados a distintas operaciones que estos permiten a su poseedor
 * Principalmente fue realizado para dar soporte a las tablas y sus drivers
 * Se genera una clave entera y a la misma se le agregan o quitan operaciones
 * Los clientes de esta clase deberan preguntar si la clave entera tiene o no los permisos
 * asociados previo al momento de realizar la operacion asociada al permiso
 */
class PermissionList {
public:
   /** Enumerado con las operaciones soportadas */
   typedef enum {
      GETVAL, SETVAL, APPROW, INSROW, DELROW, APPCOL, INSCOL, DELCOL, ROCOL, HIDECOL
   } OperationType;

   /** Constructor */
   PermissionList();
   /** Destructor */
   virtual ~PermissionList();
   /** Autoriza una operacion a un permiso */
   void InsertOperation(int Permission, OperationType Operation);
   /** Determina si una operacion esta permitida para un permiso dado */
   bool IsOperationPermitted(int Permission, OperationType Operation) const;
   /** Cuenta la cantidad de operaciones permitidas para un permiso dado */
   size_t CountOperations(int Permission) const;
   /** Devuelve las operaciones permitidas para un permiso dado */
   std::vector<OperationType> GetPermittedOperations(int Permission) const;
   /** Quita una opeeracoin del permiso asociado*/
   void EraseOperation(int Permission, OperationType operation);
   /** Quita un permiso, su id y junto con sus operaciones permitidas */
   void ErasePermission(int Permission);
   /** Elimina el registro de todos los permisos y operaciones */
   void ClearPermissions();
   /** Determina si tiene el permiso */
   bool HasPermission(int Permission) const;

private:
   typedef std::multimap<int, OperationType> OperationsMultiMap;  /*! Alias para MultiMapa */
   typedef std::pair<int, OperationType> OperationsMultiMapPair;  /*! Alias para Par-MultiMapa */
   OperationsMultiMap operations_;                                /*! MultiMapa de Operaciones */
};

} /** namespace suri */

#endif /* PERMISSIONLIST_H_ */
