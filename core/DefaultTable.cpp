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
#include <map>
#include <string>
#include <vector>
#include <utility>

// Includes suri
#include "DefaultTable.h"
#include "suri/MemoryDriver.h"
#include "suri/BufferedDriver.h"
#include "wxGenericTableBase.h"
#include "suri/AuxiliaryFunctions.h"

// Defines

// Forwards

namespace suri {

/**
 * Constructor
 */
DefaultTable::DefaultTable() {
   SetDriver(new BufferedDriver(new MemoryDriver()));
   permissions_ = new PermissionList();
   permissions_->InsertOperation(permissionKey_, PermissionList::APPCOL);
   permissions_->InsertOperation(permissionKey_, PermissionList::INSCOL);
   permissions_->InsertOperation(permissionKey_, PermissionList::APPROW);
   permissions_->InsertOperation(permissionKey_, PermissionList::INSROW);
   permissions_->InsertOperation(permissionKey_, PermissionList::DELCOL);
   permissions_->InsertOperation(permissionKey_, PermissionList::DELROW);
   permissions_->InsertOperation(permissionKey_, PermissionList::GETVAL);
   permissions_->InsertOperation(permissionKey_, PermissionList::SETVAL);
   permissions_->InsertOperation(permissionKey_, PermissionList::HIDECOL);
   permissions_->InsertOperation(permissionKey_, PermissionList::ROCOL);
}

/**
 * Destructor
 */
DefaultTable::~DefaultTable() {
   delete pDriver_;
   delete permissions_;
}

#ifdef __UNUSED_CODE__
/**
 * Setter del puntero a wxGenericTableBase
 * @param[in] pWxGenericTableBase Puntero a la visualizacion de tabla sobre la cual se hace update
 */
void DefaultTable::SetWxGenericTableBase(wxGenericTableBase* pWxGenericTableBase) {
   pWxGenericTableBase_= pWxGenericTableBase;
}

/**
 * Getter del puntero a wxGenericTableBase
 * @return
 */
const wxGenericTableBase* DefaultTable::GetWxGenericTableBase() const {
   return pWxGenericTableBase_;
}
#endif

/**
 * Obtiene el contenido de la celda especificada
 * @param[in] Column Posicion de la columna
 * @param[in] Row Posicion de la fila
 * @param[out] CellValue El contenido de la celda se almacena en este string
 * @return bool Si es true se coloca en CellValue el contenido de la celda, en caso de false nada
 */
bool DefaultTable::GetCellValue(int Column, int Row, std::string& CellValue) const {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::GETVAL))
      return false;
   if (!pDriver_->GetCapabilities()->IsOperationPermitted(pDriver_->GetPermission(),
                                                          PermissionList::GETVAL))
      return false;

   bool success = false;
   ColumnType type = GetColumnType(Column);

   if (type == Table::STRING) {
      success = pDriver_->ReadVarchar(Column, Row, CellValue);
   } else if (type == Table::INT) {
      int number = 0;
      success = pDriver_->ReadInt(Column, Row, number);
      if (success) {
         CellValue.clear();
         CellValue.append(NumberToString(number));
      }
   } else if (type == Table::COLOR_RGBA_HEXA) {
      // TODO(Sebastian - TCK #2084): Agregar validacion de Color
      success = pDriver_->ReadVarchar(Column, Row, CellValue);
   } else if (type == Table::FLOAT) {
      /*TCK #10759 : Hago esta lectura usando ReadVarchar en lugar de ReadFloat,
       * ya que se pierde precision en las conversiones usando stringstreams
       * (a partir del 7mo decimal) que el usuario pidio especficiamente
       * que no se pierda*/
      success = pDriver_->ReadVarchar(Column, Row, CellValue);
   }
   else if (type == Table::HOTLINK || type == Table::VSTYLE
         || type == Table::QBUILDER) {
      success = pDriver_->ReadVarchar(Column, Row, CellValue);
   }
   return success;
}

/**
 * Actualiza el contenido de la celda especificada
 * @param[in] Column Posicion de la columna
 * @param[in] Row Posicion de la fila
 * @param[in] CellValue El contenido de la celda se actualizara con el contenido de este string
 * @return bool Si es true se coloca el contenido de CellValue en la celda, en caso de false nada
 */
bool DefaultTable::SetCellValue(int Column, int Row, std::string& CellValue) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::SETVAL))
      return false;
   if (IsReadOnlyColumn(Column))
      return false;
   if (!pDriver_->GetCapabilities()->IsOperationPermitted(pDriver_->GetPermission(),
                                                          PermissionList::SETVAL))
      return false;

   bool success = false;
   ColumnType type = GetColumnType(Column);

   if (type == Table::STRING) {
      success = pDriver_->WriteVarchar(Column, Row, CellValue);
   } else if (type == Table::INT) {
      int value = 0;
      std::stringstream sstr(CellValue.c_str());
      sstr >> value;
      success = pDriver_->WriteInt(Column, Row, value);
   } else if (type == Table::COLOR_RGBA_HEXA) {
      // TODO(Sebastian - TCK #2084): Agregar validacion de Color
      success = pDriver_->WriteVarchar(Column, Row, CellValue);
   } else if (type == Table::FLOAT) {
      success = pDriver_->WriteVarchar(Column, Row, CellValue);
   } else if (type == Table::HOTLINK) {
      success = pDriver_->WriteVarchar(Column, Row, CellValue);
   } else if (type == Table::VSTYLE || type == Table::QBUILDER) {
      success = pDriver_->WriteVarchar(Column, Row, CellValue);
   }

   pDriver_->Update();
   UpdateGrid();

   return success;
}

/**
 * Verifica si un tipo es soportado por esta implementacion
 * @param[in] Type Tipo a ser validado
 * @return bool indica si el tipo esta soportado devolviendo true, sino es asi devuelve false
 */
bool DefaultTable::IsSupportedType(ColumnType ColumnType) const {
   switch (ColumnType) {
      case Table::STRING:
      case Table::COLOR_RGBA_HEXA:
      case Table::INT:
      case Table::FLOAT:
      case Table::HOTLINK:
      case Table::VSTYLE:
      case Table::QBUILDER:
         return true;
      default:
         return false;
   }
}

#ifdef __SHOW_TABLE_LOG__
/**
 * Getter de un puntero const del Driver
 * @return
 */
const DriverInterface* DefaultTable::GetDriver() const {
   return pDriver_;
}

/**
 * Obtiene las tabla en un string
 * \note Metodo Debug
 * @return
 */
std::string DefaultTable::GetTable() const {
   std::string table = "";
   int columns = GetColumns();
   int rows = GetRows();
   for (int j = 0; j < rows; j++) {
      table.append("\t[\t");
      for (int i = 0; i < columns; i++) {
            std::string value = "";
            GetCellValue(i, j, value);
            table.append(value);
            table.append("\t");
      }
      table.append("]\n");
   }
   return table;
}
#endif
} /** namespace suri */
