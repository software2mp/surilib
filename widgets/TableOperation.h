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

#ifndef TABLEOPERATION_H_
#define TABLEOPERATION_H_

// Includes Estandar
#include <string>
#include <cstddef>

// Includes Suri
#include "PermissionList.h"
#include "suri/ParameterCollection.h"

namespace suri {

/**
 * Modela las operaciones de DriverInterface que son susceptibles de ser buffereadas
 * para asistir a la logica transaccional
 */
class TableOperation {
public:
   /**
    * Ctor.
    */
   explicit TableOperation(PermissionList::OperationType Operation, int TableColumn,
                           int DSColumn, int TableRow, int DSRow,
                           const std::string& value, BaseParameter* pTagP = NULL);
   /**
    * Dtor.
    */
   virtual ~TableOperation();

   /**
    * Devuelve el tipo de operacion.
    */
   PermissionList::OperationType GetOperationType() const;

   /**
    * Obtiene el numero de columna de la tabla.
    */
   int GetTableColumn() const;

   /**
    * Obtiene el numero de columna del origen de datos.
    */
   int GetDSColumn() const;

   /**
    * Obtiene el numero de fila de la tabla.
    */
   int GetTableRow() const;

   /**
    * Obtiene el numero de fila del origen de datos.
    */
   int GetDSRow() const;

   /**
    * Devuelve el valor asociado.
    */
   std::string GetValue() const;

   /**
    * Cambia el valor asociado.
    */
   void SetValue(const std::string& Value);

   /**
    * Devuelve la informacion extra asociada.
    */
   BaseParameter* GetTag() const;

private:
   PermissionList::OperationType operation_;  // Tipo de Operacion
   int tableColumn_;
   int dsColumn_;
   int tableRow_;  // Indice de la fila en la tabla
   int dsRow_;  // Indice de la fila en el origen de datos
   std::string value_;  // Valor
   BaseParameter* pTag_;  // Informacion extra
};

} /** namespace suri */

#endif /* TABLEOPERATION_H_ */
