/*! \file PermissionList.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
