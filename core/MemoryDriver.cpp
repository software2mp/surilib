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
#include <string>
#include <sstream>
#include <utility>
#include <algorithm>
#include <limits>

// Includes Suri
#include "suri/MemoryDriver.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
MemoryDriver::MemoryDriver() :
      permissionKey_(0), maxRowId_(-1) {
   // Por defecto se permite configurar libremente el MemoryDriver (permisos totales)
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
MemoryDriver::~MemoryDriver() {
   delete permissions_;
}

/**
 * Leer un entero del driver
 * @param[in] Column indice de la columna de la cual se leera el dato
 * @param[in] Row indice de la fila de la cual se leera el dato
 * @param[in] Data dato a ser leido
 * @return true si pudo leer exitosamente el dato, sino devuelve falso
 */
bool MemoryDriver::ReadInt(int Column, int Row, int& Data) const {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::GETVAL))
      return false;
   if (!IsValidCell(Column, Row) || !IsIntColumn(Column)) {
      return false;
   }

   std::string strdata = "";
   if (ReadVarchar(Column, Row, strdata)) {
      std::stringstream ss(strdata);
      ss >> Data;
      return true;
   }

   return false;
}

/**
 * Escribir un entero en el driver
 * @param[in] Column indice de la columna en la cual se escribira el dato
 * @param[in] Row indice de la fila en la cual se escribira el dato
 * @param[in] Data dato a ser escrito
 * @return true si pudo escribir exitosamente el dato, sino devuelve falso
 */
bool MemoryDriver::WriteInt(int Column, int Row, int Data) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::SETVAL))
      return false;
   if (!IsValidCell(Column, Row) || !IsIntColumn(Column)) {
      return false;
   }

   std::ostringstream osstream;
   osstream << Data;
   std::string strdata = osstream.str();
   return WriteVarchar(Column, Row, strdata);
}

/**
 * Leer un string del driver
 * @param[in] Column indice de la columna de la cual se leera el dato
 * @param[in] Row indice de la fila de la cual se leera el dato
 * @param[in] Data dato a ser leido
 * @return true si pudo leer exitosamente el dato, sino devuelve falso
 */
bool MemoryDriver::ReadVarchar(int Column, int Row, std::string& Data) const {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::GETVAL))
      return false;
   // No se valida que !IsVarcharColumn(Column) porque se utiliza para todas las columnas
   if (!IsValidCell(Column, Row)) {
      return false;
   }

   std::list<MemoryDriver::Column*>::const_iterator it = columns_.begin();
   for (int i = 0; i < Column; i++) {
      it++;
   }

   MemoryDriver::Column* pcolumn = (MemoryDriver::Column*) (*it);

   return pcolumn->ReadVarchar(Row, Data);
}

/**
 * Escribir un string en el driver
 * @param[in] Column indice de la columna en la cual se escribira el dato
 * @param[in] Row indice de la fila en la cual se escribira el dato
 * @param[in] Data dato a ser escrito
 * @return true si pudo escribir exitosamente el dato, sino devuelve falso
 */
bool MemoryDriver::WriteVarchar(int Column, int Row, const std::string& Data) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::SETVAL))
      return false;
   // No se valida que !IsVarcharColumn(Column) porque se utiliza para todas las columnas
   if (!IsValidCell(Column, Row)) {
      return false;
   }

   std::list<MemoryDriver::Column*>::iterator it = columns_.begin();
   for (int i = 0; i < Column; i++) {
      it++;
   }

   MemoryDriver::Column* pcolumn = (MemoryDriver::Column*) (*it);

   bool success = pcolumn->WriteVarchar(Row, Data);

   return success;
}

/**
 * Leer un punto flotante del driver
 * @param[in] Column indice de la columna de la cual se leera el dato
 * @param[in] Row indice de la fila de la cual se leera el dato
 * @param[in] Data dato a ser leido
 * @return true si pudo leer exitosamente el dato, sino devuelve falso
 */
bool MemoryDriver::ReadFloat(int Column, int Row, float& Data) const {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::GETVAL))
      return false;
   if (!IsValidCell(Column, Row) || !IsFloatColumn(Column)) {
      return false;
   }
   std::string strdata = "";
   if (ReadVarchar(Column, Row, strdata)) {
      std::stringstream ss(strdata);
      ss >> std::setprecision(std::numeric_limits<float>::digits10) >> Data;
      return true;
   }

   return false;
}

/**
 * Escribir un punto flotante en el driver
 * @param[in] Column indice de la columna en la cual se escribira el dato
 * @param[in] Row indice de la fila en la cual se escribira el dato
 * @param[in] Data dato a ser escrito
 * @return true si pudo escribir exitosamente el dato, sino devuelve falso
 */
bool MemoryDriver::WriteFloat(int Column, int Row, float Data) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::SETVAL))
      return false;
   if (!IsValidCell(Column, Row) || !IsFloatColumn(Column)) {
      return false;
   }
   std::ostringstream osstream;
   osstream.precision(std::numeric_limits<float>::digits10);
   osstream << Data;
   std::string strdata = osstream.str();
   return WriteVarchar(Column, Row, strdata);
}

/**
 * Leer un punto flotante de doble precision del driver
 * @param[in] Column indice de la columna de la cual se leera el dato
 * @param[in] Row indice de la fila de la cual se leera el dato
 * @param[in] Data dato a ser leido
 * @return true si pudo leer exitosamente el dato, sino devuelve falso
 */
bool MemoryDriver::ReadDouble(int Column, int Row, double& Data) const {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::SETVAL))
      return false;
   if (!IsValidCell(Column, Row) || !IsFloatColumn(Column)) {
      return false;
   }
   std::string strdata = "";
   if (ReadVarchar(Column, Row, strdata)) {
      std::stringstream ss(strdata);
      ss >> std::setprecision(std::numeric_limits<double>::digits10) >> Data;
      return true;
   }
   return false;
}

/**
 * Escribir un punto flotante de precision doble en el driver
 * @param[in] Column indice de la columna en la cual se escribira el dato
 * @param[in] Row indice de la fila en la cual se escribira el dato
 * @param[in] Data dato a ser escrito
 * @return true si pudo escribir exitosamente el dato, sino devuelve falso
 */
bool MemoryDriver::WriteDouble(int Column, int Row, double Data) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::SETVAL))
         return false;
      if (!IsValidCell(Column, Row) || !IsFloatColumn(Column)) {
         return false;
      }
      std::ostringstream osstream;
      osstream.precision(std::numeric_limits<double>::digits10);
      osstream << Data;
      std::string strdata = osstream.str();
      return WriteVarchar(Column, Row, strdata);
}

/**
 * Devuelve la cantidad de columnas
 * @return la cantidad de columnas del driver
 */
int MemoryDriver::GetColumns() const {
   return columns_.size();
}

/**
 * Devuelve la cantidad de filas
 * @return la cantidad de filas del driver
 */
int MemoryDriver::GetRows() const {
   // La cantidad de filas para todas las columnas es la misma
   return (GetColumns() == 0) ? 0 : (*(columns_.begin()))->GetRows();
}

/**
 * Agrega una fila al final de la tabla
 * @return devuelve true si la operacion tuvo exito, sino devuelve false
 */
bool MemoryDriver::AppendRow() {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::APPROW))
      return false;

   std::list<MemoryDriver::Column*>::iterator it;
   for (it = columns_.begin(); it != columns_.end(); it++) {
      // Aca podria haber problemas si se cambian permisos
      (*it)->AppendRow();
   }

   int rowssize = GetRows();
   if (maxRowId_ < rowssize)
      maxRowId_ = rowssize;
   return true;
}

/**
 * Agrega una fila en la posicion especificada de la tabla
 * @param Row
 * @return
 */
bool MemoryDriver::InsertRow(int Row) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::INSROW))
      return false;
   if (0 > Row || Row > GetRows())
      return false;

   std::list<MemoryDriver::Column*>::iterator it;
   for (it = columns_.begin(); it != columns_.end(); it++) {
      (*it)->InsertRow(Row);
   }
   return true;
}

/**
 * Elimina una fila de la tabla (debe existir, sino devuelve false)
 * @param[in] Row indice de la fila a ser eliminada
 * @return devuelve true si la operacion tuvo exito, sino devuelve false
 */
bool MemoryDriver::DeleteRow(int Row) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::DELROW))
      return false;
   if (0 > Row || Row >= GetRows()) {
      return false;
   }

   std::list<MemoryDriver::Column*>::iterator it;
   for (it = columns_.begin(); it != columns_.end(); it++) {
      (*it)->DeleteRow(Row);
   }

   return true;
}

/**
 * Agrega una columna al final de la tabla
 * @param[in] ColumnType tipo de dato para la nueva columna
 * @param[in] ColumnName nombre de la columna
 * @param[in] ColumnFlag flag que indica el tipo de column
 * @return devuelve true si la operacion tuvo exito, sino devuelve false
 */
bool MemoryDriver::AppendColumn(const std::string& ColumnType,
                                const std::string& ColumnName, int ColumnFlag) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::APPCOL))
      return false;
   if (!IsSupportedType(ColumnType)) {
      return false;
   }

   int column = GetColumns();
   std::list<MemoryDriver::Column*>::iterator it = columns_.begin();
   for (int i = 0; i < column; i++) {
      it++;
   }
   MemoryDriver::Column* pcolumn = new MemoryDriver::Column(ColumnType, ColumnName,
                                                            GetRows());
   columns_.insert(it, pcolumn);
   return true;
}

/**
 * Agrega una columna en la posicion especificada de la tabla
 * @param Column
 * @param ColumnType
 * @param ColumnName
 * @return
 */
bool MemoryDriver::InsertColumn(int Column, const std::string& ColumnType,
                                const std::string& ColumnName, int ColumnFlag) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::INSCOL))
      return false;
   if (!IsSupportedType(ColumnType)) {
      return false;
   }
   if (0 > Column || Column > GetColumns()) {
      return false;
   }

   std::list<MemoryDriver::Column*>::iterator it = columns_.begin();
   for (int i = 0; i < Column; i++) {
      it++;
   }
   MemoryDriver::Column* pcolumn = new MemoryDriver::Column(ColumnType, ColumnName,
                                                            GetRows());
   columns_.insert(it, pcolumn);
   return true;
}

/**
 * Elimina una columna de la tabla (debe existir, sino devuelve false)
 * @param[in] Column indice de la columna a ser eliminada
 * @return devuelve true si la operacion tuvo exito, sino devuelve false
 */
bool MemoryDriver::DeleteColumn(int Column) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::DELCOL))
      return false;
   if (0 > Column || Column >= GetColumns()) {
      return false;
   }

   std::list<MemoryDriver::Column*>::iterator it = columns_.begin();
   for (int i = 0; i < Column; i++) {
      it++;
   }

   MemoryDriver::Column* pcolumn = dynamic_cast<MemoryDriver::Column*>(*it);
   delete pcolumn;

   columns_.erase(it);
   return true;
}

/**
 * Elimina todas las filas y su contenido de la tabla
 * @return devuelve true si la operacion tuvo exito, sino devuelve false
 */
bool MemoryDriver::Clear() {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::DELROW))
      return false;

   for (int row = GetRows(); row >= 0; row--) {
      DeleteRow(row);
   }
   return true;
}

/**
 * Determina si se soporta el tipo
 * @param ColumnType Tipo de dato
 * @return devuelve true si se soporta el tipo
 */
bool MemoryDriver::IsSupportedType(const std::string& ColumnType) {
   bool idint = Type2Id(ColumnType) == DataInfo<int>::Id;
   bool idstring = Type2Id(ColumnType) == DataInfo<unsigned char>::Id;
   bool idfloat = Type2Id(ColumnType) == DataInfo<double>::Id
         || Type2Id(ColumnType) == DataInfo<float>::Id;
   return idint || idstring || idfloat;
}

/**
 * Determina si una celda es valida
 * @param[in] Column indice de la columna a ser eliminada
 * @return devuelve true si la operacion tuvo exito, sino devuelve false
 */
bool MemoryDriver::IsValidCell(int Column, int Row) const {
   if (Column >= GetColumns() || Row >= GetRows() || Column < 0 || Row < 0) {
      return false;
   }
   return true;
}

/**
 * Determina si la columna es entera
 * @param[in] Column indice de la columna
 * @return devuelve true si la columna es entera
 */
bool MemoryDriver::IsIntColumn(int Column) const {
   return Type2Id(GetColumnType(Column)) == DataInfo<int>::Id;
}

/**
 * Determina si la columna es de cadena de caracteres
 * @param[in] Column indice de la columna
 * @return devuelve true si la columna es varchar
 */
bool MemoryDriver::IsVarcharColumn(int Column) const {
   // El tipo unsigned char se usa para representar del string porque DataInfo no lo contempla
   return Type2Id(GetColumnType(Column)) == DataInfo<unsigned char>::Id;
}

/**
 * Determina si la columna es de punto flotante
 * @param[in] Column indice de la columna
 * @return devuelve true si la columna es flotante
 */
bool MemoryDriver::IsFloatColumn(int Column) const {
   return Type2Id(GetColumnType(Column)) == DataInfo<float>::Id;
}

/** Determina si la columna es de punto flotante (double) */
bool MemoryDriver::IsDoubleColumn(int Column) const {
   return Type2Id(GetColumnType(Column)) == DataInfo<double>::Id;
}

/**
 * Getea la lista de permisos configurada para la tabla
 * @return
 */
const PermissionList* MemoryDriver::GetCapabilities() const {
   return permissions_;
}

/**
 * Getea el permiso utilizado en GetCapabilities
 * @return
 */
int MemoryDriver::GetPermission() const {
   return permissionKey_;
}

/**
 * Devuelve el tipo de la columna
 * @param[in] Column indice de la columna
 * @return devuelve el tipo de columna, y si la columna no existe devuelve un string vacio
 */
std::string MemoryDriver::GetColumnType(int Column) const {
   // Seria conveniento uniformizar los valores devueltos de tipos para una manejo mas simple
   // por parte de los usuarios de la clase
   if (0 > Column || Column >= GetColumns())
      return "";

   std::list<MemoryDriver::Column*>::const_iterator it = columns_.begin();
   for (int i = 0; i < Column; i++) {
      it++;
   }

   MemoryDriver::Column* pcolumn = (MemoryDriver::Column*) (*it);

   return pcolumn->GetColumnType();
}

/**
 * Obtiene el nombre de la columna
 * @return
 */
std::string MemoryDriver::GetColumnName(int Column) const {
   if (Column < 0 || Column >= GetColumns())
      return "";

   std::list<MemoryDriver::Column*>::const_iterator it = columns_.begin();
   for (int i = 0; i < Column; i++) {
      it++;
   }

   MemoryDriver::Column* pcolumn = (MemoryDriver::Column*) (*it);

   return pcolumn->GetColumnName();
}

/*------------------------------------------------------------------------------------------------*/

/**
 * Constructor
 * @param ColumnType Tipo de la columna (utilizado en MemoryDriver)
 * @param ColumnName Nombre de la columna
 * @param DefaultRows Cantidad de filas que se agregaran con valores por defecto (solo si es > 0)
 */
MemoryDriver::Column::Column(const std::string& ColumnType,
                             const std::string& ColumnName, int DefaultRows) :
      columnType_(ColumnType), columnName_(ColumnName) {
   for (int i = 0; i < DefaultRows; i++) {
      AppendRow();
   }
}

/**
 * Destructor
 */
MemoryDriver::Column::~Column() {
   Clear();
}

/**
 * Leer un string del driver
 * @param Row Fila (desde "0" hasta "GetRows - 1")
 * @param Data Valor de la celda si existe Row
 * @return
 */
bool MemoryDriver::Column::ReadVarchar(int Row, std::string& Data) const {
   if (Row < 0 || Row >= GetRows())
      return false;

   std::list<std::string>::const_iterator it = rows_.begin();
   for (int i = 0; i < Row; i++) {
      it++;
   }

   Data.clear();
   Data.append(*it);

   return true;
}

/**
 * Escribir un string en el driver
 * @param Row
 * @param Data
 * @return
 */
bool MemoryDriver::Column::WriteVarchar(int Row, const std::string& Data) {
   if (Row < 0 || Row >= GetRows())
      return false;

   std::list<std::string>::iterator it = rows_.begin();
   for (int i = 0; i < Row; i++) {
      it++;
   }

   it->clear();
   it->append(Data);

   return true;
}

/**
 * Devuelve la cantidad de filas
 * @return
 */
int MemoryDriver::Column::GetRows() const {
   return rows_.size();
}

/**
 * Agrega una fila al final de la tabla
 * @return
 */
bool MemoryDriver::Column::AppendRow() {
   if (Type2Id(columnType_) == DataInfo<int>::Id) {
      rows_.push_back("0");
   } else if (Type2Id(columnType_) == DataInfo<float>::Id) {
      rows_.push_back("0.0");
   } else {
      rows_.push_back("");
   }
   return true;
}

/**
 * Agrega una fila en la posicion especificada de la tabla
 * @param Row
 * @return
 */
bool MemoryDriver::Column::InsertRow(int Row) {
   if (Row < 0 || Row > GetRows())
      return false;

   std::list<std::string>::iterator it = rows_.begin();
   for (int i = 0; i < Row; i++) {
      it++;
   }

   if (Type2Id(columnType_) == DataInfo<int>::Id) {
      rows_.insert(it, "0");
   } else if (Type2Id(columnType_) == DataInfo<float>::Id) {
      rows_.insert(it, "0.0");
   } else {
      rows_.insert(it, "");
   }

   return true;
}

/**
 * Elimina una fila de la tabla (debe existir, sino devuelve false)
 * @param Row
 * @return
 */
bool MemoryDriver::Column::DeleteRow(int Row) {
   if (Row < 0 || Row > GetRows())
      return false;

   std::list<std::string>::iterator it = rows_.begin();
   for (int i = 0; i < Row; i++) {
      it++;
   }

   if (Type2Id(columnType_) == DataInfo<int>::Id) {
      rows_.erase(it);
   } else if (Type2Id(columnType_) == DataInfo<float>::Id) {
      rows_.erase(it);
   } else {
      rows_.erase(it);
   }

   return true;
}

/**
 * Elimina todas las filas y su contenido
 * @return
 */
bool MemoryDriver::Column::Clear() {
   rows_.clear();
   return true;
}

/**
 * Obtiene el nombre de la columna
 * @return
 */
std::string MemoryDriver::Column::GetColumnName() const {
   return columnName_;
}

/**
 * Obtiene el tipo de la columna
 * @return
 */
std::string MemoryDriver::Column::GetColumnType() const {
   return columnType_;
}

#ifdef __SHOW_TABLE_LOG__
/**
 * Obtiene las tabla en un string
 * \note Metodo Debug
 * @return
 */
std::string MemoryDriver::ToString() const {
   std::string table = "";
   int columns = GetColumns();
   int rows = GetRows();
   for (int j = 0; j < rows; j++) {
      table.append("\t[\t");
      for (int i = 0; i < columns; i++) {
         std::string value = "";
         this->ReadVarchar(i, j, value);
         table.append(value);
         table.append("\t");
      }
      table.append("]\n");
   }
   return table;
}
#endif

/**
 * Getea el Id del Datasource asociado
 * \attention No hay soporte para fuentes de datos en los drivers de memoria
 * @return
 */
SuriObject::UuidType MemoryDriver::GetAssociatedDatasourceId() const {
   // No hay soporte para fuentes de datos en los drivers de memoria
   return "";
}

/**
 *
 */
long MemoryDriver::GetNewRowId() {
   return maxRowId_ <0 ? GetRows()+1 : maxRowId_;
}

} /** namespace suri */
