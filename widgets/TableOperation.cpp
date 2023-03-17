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

#include <string>
#include <cstddef>

#include "TableOperation.h"
#include "suri/ParameterCollection.h"

namespace suri {

/**
 * Ctor.
 */
TableOperation::TableOperation(PermissionList::OperationType Operation, int TableColumn,
                               int DSColumn, int TableRow, int DSRow,
                               const std::string& value, BaseParameter* pTagP) :
      operation_(Operation), tableColumn_(TableColumn), dsColumn_(DSColumn),
      tableRow_(TableRow), dsRow_(DSRow), value_(value), pTag_(pTagP) {

}

/**
 * Dtor.
 */
TableOperation::~TableOperation() {
}

/**
 * Devuelve el numero de columna
 * @return Devuelve el numero de columna
 */
PermissionList::OperationType TableOperation::GetOperationType() const {
   return operation_;
}

/**
 * Obtiene el numero de columna de la tabla.
 */
int TableOperation::GetTableColumn() const {
   return tableColumn_;
}

/**
 * Obtiene el numero de columna del origen de datos.
 */
int TableOperation::GetDSColumn() const {
   return dsColumn_;
}

/**
 * Obtiene el numero de fila de la tabla.
 */
int TableOperation::GetTableRow() const {
   return tableRow_;
}

/**
 * Obtiene el numero de fila del origen de datos.
 */
int TableOperation::GetDSRow() const {
   return dsRow_;
}


/**
 * Devuelve el valor asociado
 * @return Devuelve el valor asociado
 */
std::string TableOperation::GetValue() const {
   return value_;
}

/**
 * Cambia el valor asociado
 * @param Value Valor a ser seteado
 */
void TableOperation::SetValue(const std::string& Value) {
   value_ = Value;
}

/**
 * Devuelve la informacion extra asociada.
 *
 * @return Informacion extra asociada al objeto.
 */
BaseParameter* TableOperation::GetTag() const {
   return pTag_;
}

} /** namespace suri */
