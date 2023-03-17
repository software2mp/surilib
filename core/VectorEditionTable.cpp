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
#include "suri/VectorEditionTable.h"
#include "PermissionList.h"
#include "suri/AuxiliaryFunctions.h"
#include "RegisterEditionStartedNotification.h"
#include "suri/VectorElement.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Defines

// forwards
namespace suri {

/** ctor que recibe una fuente de datos**/
VectorEditionTable::VectorEditionTable(DatasourceInterface* pDatasource) : pDatasource_(pDatasource) {
   url_ = pDatasource_->GetUrl();
   tableName_ = pDatasource_->GetName();
}
/** Destructor */
VectorEditionTable::~VectorEditionTable() {
   if (pDriver_)
    delete pDriver_;
}

/** Verifica si un tipo es soportado por una implementacion concreta de esta interfaz */
bool VectorEditionTable::IsSupportedType(ColumnType ColumnType) const {
   return true;
}

/**
 * Obtiene el contenido de la celda especificada
 * @param[in] Column Posicion de la columna
 * @param[in] Row Posicion de la fila
 * @param[out] CellValue El contenido de la celda se almacena en este string
 * @return bool Si es true se coloca en CellValue el contenido de la celda, en caso de false nada
 */
bool VectorEditionTable::GetCellValue(int Column, int Row, std::string& CellValue) const {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::GETVAL))
      return false;
   if (!pDriver_->GetCapabilities()->IsOperationPermitted(pDriver_->GetPermission(),
                                                          PermissionList::GETVAL))
      return false;
   // Si pQueryInterpreter_ es NullQueryInterpreter Row viene del Driver

   int row = pQueryInterpreter_->GetRow(Row);
   row = row >= 0 ? row : Row;
   bool success = false;
   ColumnType type = GetColumnType(Column);

   if (type == Table::STRING) {
      success = pDriver_->ReadVarchar(Column, row, CellValue);
   } else if (type == Table::INT) {
      int number = 0;
      success = pDriver_->ReadInt(Column, row, number);
      if (success) {
         CellValue.clear();
         CellValue.append(NumberToString(number));
      }
   } else if (type == Table::COLOR_RGBA_HEXA) {
      // TODO(Sebastian - TCK #2084): Agregar validacion de Color
      success = pDriver_->ReadVarchar(Column, row, CellValue);
   } else if (type == Table::FLOAT) {
      float number = 0;
      success = pDriver_->ReadFloat(Column, row, number);
      if (success) {
         CellValue.clear();
         CellValue.append(DoubleToStringEx(number,8));
      }
   } else if (type == Table::DOUBLE) {
      double number = 0;
      success = pDriver_->ReadDouble(Column, row, number);
      if (success) {
         CellValue.clear();
         CellValue.append(DoubleToStringEx(number,8));
      }
   } else if (type == Table::HOTLINK) {
      success = pDriver_->ReadVarchar(Column, row, CellValue);
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
bool VectorEditionTable::SetCellValue(int Column, int Row, std::string& CellValue) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::SETVAL))
      return false;
   if (IsReadOnlyColumn(Column))
      return false;
   if (!pDriver_->GetCapabilities()->IsOperationPermitted(pDriver_->GetPermission(),
                                                          PermissionList::SETVAL))
      return false;
   // Si pQueryInterpreter_ es NullQueryInterpreter Row viene del Driver
   int row = pQueryInterpreter_->GetRow(Row);
   row = row >= 0 ? row : Row;
   bool success = false;

   ColumnType type = GetColumnType(Column);

   if (type == Table::STRING) {
      success = pDriver_->WriteVarchar(Column, row, CellValue);
   } else if (type == Table::INT) {
      int value = 0;
      std::stringstream sstr(CellValue.c_str());
      sstr >> value;
      success = pDriver_->WriteInt(Column, row, value);
   } else if (type == Table::COLOR_RGBA_HEXA) {
      // TODO(Sebastian - TCK #2084): Agregar validacion de Color
      success = pDriver_->WriteVarchar(Column, row, CellValue);
   } else if (type == Table::FLOAT) {
      float value = 0;
      std::stringstream sstr(CellValue.c_str());
      sstr >> value;
      success = pDriver_->WriteFloat(Column, row, value);
   } else if (type == Table::DOUBLE) {
      double value = 0;
      std::stringstream sstr(CellValue.c_str());
      sstr >> value;
      success = pDriver_->WriteDouble(Column, row, value);
   } else if (type == Table::HOTLINK) {
      success = pDriver_->WriteVarchar(Column, row, CellValue);
   }

   UpdateGrid();

   return success;
}
/** Metodo que verifica que la columna posee definido un tipo logico de dato*/
Table::ColumnType VectorEditionTable::GetMetaTypeColumn(int Column) const {
   if (!pDatasource_)
      return Table::UNKNOWN;
   bool hasmetatype = false;
   // Por ahora se verifica solamente tipo hotlink
   // TODO(Gabriel - TCK #3476): migrar metodo a VectorDatasource
   VectorElement* pelement = dynamic_cast<VectorElement*>(pDatasource_->GetElement());
   if (pelement) {
      std::string columname = GetColumnName(Column);
      std::list<std::string> hllist;
      pelement->GetHotlinkNodesList(hllist);
      std::list<std::string>::iterator it = hllist.begin();
      for (; !hasmetatype && it != hllist.end(); ++it) {
         hasmetatype = columname.compare(*it) == 0;
      }
   }
   if (hasmetatype)
      return Table::HOTLINK;
   return Table::UNKNOWN;
}
}  /** namespace suri */
