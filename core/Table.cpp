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
#include <limits>
#include <algorithm>

// Includes Suri
#include "suri/Table.h"
#include "suri/AuxiliaryFunctions.h"
#include "wxGenericTableBase.h"
#include "suri/NotificationInterface.h"
#include "UpdateGridNotification.h"
#include "UpdateProviderNotification.h"
#include "suri/NewColumnNotification.h"
#include "suri/DeleteColumNotification.h"
#include "QueryInterpreter.h"
#include "NullQueryInterpreter.h"
#include "EquationParser.h"

// Includes Wx

// Defines

// forwards

namespace suri {

const int Table::PermissionReadOnly = 0; /*! Valor por defecto de permiso */
const int Table::PermissionFixedSize = 1; /*! Valor por defecto de permiso */
const int Table::PermissionAddRows = 2; /*! Valor por defecto de permiso */
const int Table::PermissionAddColumns = 3; /*! Valor por defecto de permiso */
const int Table::PermissionHideColumn = 4; /*! Valor por defecto de permiso */
const int Table::PermissionReadOnlyColumn = 5; /*! Valor por defecto de permiso */

const std::string Table::NullString = ""; /*! Valor para el nulo de los strings */
const int Table::NullInt = 0; /*! Valor para el nulo de los int */
const float Table::NullFloat = 0.0; /*! Valor para el nulo de los double */
const int Table::DefaultBool = 0; /*! Valor por defecto de los bool */
const long Table::DefaultDate = 0L; /*! Valor por defecto de las fechas */
const std::string Table::DefaultString = ""; /*! Valor defecto de cadenas */
const std::string Table::DefaultColor = "000000"; /*! Valor defecto de colores en hexa */
const std::string Table::DefaultFile = ""; /*! Valor por defecto de los archivos */
const std::string Table::DefaultUrl = ""; /*! Valor por defecto de los url */

const std::string Table::TokenSeparator = "|"; /*! Separa restricciones */
const std::string Table::KeyValueSeparator = ":"; /*! Separa restric de visual */

#define SQL_SELECT_STRING "SELECT FID, * FROM "

Table::Table() :
      pDriver_(NULL), permissionKey_(PermissionReadOnlyColumn),
      pQueryInterpreter_(new NullQueryInterpreter(pDriver_)) {
   LoadPermissions();
   SetReadOnlyState(false);
}

Table::~Table()  {
   delete pQueryInterpreter_;
}

/** Metodo auxiliar que carga las operaciones asociadas a cada nivel de permiso
 * Permisos:
 * 0) Solo lectura
 * 1) Solo modificacion de celdas
 * 2) Modificacion de celdas y Agregar/Eliminar registros (filas)
 * 3) Modificacion de celdas y Agregar/Eliminar registros (filas) y campos (columnas)
 * 4) Ocultar Columna
 * 5) Columna de solo lectura
 * 5 y 6 toman precedencia sobre los anteriores
 */
void Table::LoadPermissions() {
   permissions_ = new PermissionList();
   // Configuracion de PermissionReadOnly
   permissions_->InsertOperation(Table::PermissionReadOnly, PermissionList::GETVAL);
   // Configuracion de PermissionFixedSize
   permissions_->InsertOperation(Table::PermissionFixedSize, PermissionList::GETVAL);
   permissions_->InsertOperation(Table::PermissionFixedSize, PermissionList::SETVAL);
   // Configuracion de PermissionAddRows
   permissions_->InsertOperation(Table::PermissionAddRows, PermissionList::APPROW);
   permissions_->InsertOperation(Table::PermissionAddRows, PermissionList::INSROW);
   permissions_->InsertOperation(Table::PermissionAddRows, PermissionList::DELROW);
   permissions_->InsertOperation(Table::PermissionAddRows, PermissionList::GETVAL);
   permissions_->InsertOperation(Table::PermissionAddRows, PermissionList::SETVAL);
   // Configuracion de PermissionAddColumns
   permissions_->InsertOperation(Table::PermissionAddColumns, PermissionList::APPCOL);
   permissions_->InsertOperation(Table::PermissionAddColumns, PermissionList::INSCOL);
   permissions_->InsertOperation(Table::PermissionAddColumns, PermissionList::APPROW);
   permissions_->InsertOperation(Table::PermissionAddColumns, PermissionList::INSROW);
   permissions_->InsertOperation(Table::PermissionAddColumns, PermissionList::DELCOL);
   permissions_->InsertOperation(Table::PermissionAddColumns, PermissionList::DELROW);
   permissions_->InsertOperation(Table::PermissionAddColumns, PermissionList::GETVAL);
   permissions_->InsertOperation(Table::PermissionAddColumns, PermissionList::SETVAL);
   // Configuracion de PermissionHideColumn
   permissions_->InsertOperation(Table::PermissionHideColumn, PermissionList::APPCOL);
   permissions_->InsertOperation(Table::PermissionHideColumn, PermissionList::INSCOL);
   permissions_->InsertOperation(Table::PermissionHideColumn, PermissionList::APPROW);
   permissions_->InsertOperation(Table::PermissionHideColumn, PermissionList::INSROW);
   permissions_->InsertOperation(Table::PermissionHideColumn, PermissionList::DELCOL);
   permissions_->InsertOperation(Table::PermissionHideColumn, PermissionList::DELROW);
   permissions_->InsertOperation(Table::PermissionHideColumn, PermissionList::GETVAL);
   permissions_->InsertOperation(Table::PermissionHideColumn, PermissionList::SETVAL);
   permissions_->InsertOperation(Table::PermissionHideColumn, PermissionList::HIDECOL);
   // Configuracion de PermissionReadOnlyColumn
   permissions_->InsertOperation(Table::PermissionReadOnlyColumn,
                                 PermissionList::APPCOL);
   permissions_->InsertOperation(Table::PermissionReadOnlyColumn,
                                 PermissionList::INSCOL);
   permissions_->InsertOperation(Table::PermissionReadOnlyColumn,
                                 PermissionList::APPROW);
   permissions_->InsertOperation(Table::PermissionReadOnlyColumn,
                                 PermissionList::INSROW);
   permissions_->InsertOperation(Table::PermissionReadOnlyColumn,
                                 PermissionList::DELCOL);
   permissions_->InsertOperation(Table::PermissionReadOnlyColumn,
                                 PermissionList::DELROW);
   permissions_->InsertOperation(Table::PermissionReadOnlyColumn,
                                 PermissionList::GETVAL);
   permissions_->InsertOperation(Table::PermissionReadOnlyColumn,
                                 PermissionList::SETVAL);
   permissions_->InsertOperation(Table::PermissionReadOnlyColumn,
                                 PermissionList::ROCOL);
}

/**
 * Retorna la cantidad de columnas en la tabla
 * @return int con la cantidad de columnas
 */
int Table::GetColumns() const {
   return pDriver_ ? pDriver_->GetColumns() : -1;
}

/**
 * Retorna tipo de dato contenido en la columna.
 * @param[in] Column Posicion de la columna
 * @return std::string. Tipo de dato en la columna
 */
Table::ColumnType Table::GetColumnType(int Column) const {
   int columns = GetColumns();
   if (0 > Column || Column > columns)
      return Table::UNKNOWN;
   return columnTypes_.at(Column);
}

/**
 * Retorna el nombre de la columna.
 * @param[in] Column Posicion de la columna
 * @return Nombre de la columna. Si no tiene nombre o no existe devuelve un string vacio.
 */
std::string Table::GetColumnName(int Column) const {
   return pDriver_->GetColumnName(Column);
}

/**
 * Devuelve la posicion de la columna con el nombre indicado
 * @return posicion de la columna en la tabla
 * @return -1 si no lo encuentra.
 */
int Table::GetColumnByName(const std::string &ColumnName) const {
   int columncount = pDriver_->GetColumns();
   int i = 0;
   for (; i < columncount && pDriver_->GetColumnName(i) != ColumnName; i++)
      ;

   return (i == columncount) ? -1 : i;
}

/**
 * Retorna la cantidad de filas en la tabla
 * @return int con la cantidad de filas
 */
int Table::GetRows() const {
   return pQueryInterpreter_->GetRows();
}

/**
 * Setter del Driver (Se apropia de la memoria)
 * @param[in] pDriver Puntero al driver a ser utilizado
 * @return bool Si es true pudo liberar el driver anterior y setear el driver nuevo, sino es falso
 * Se destruye el driver que se utilizaba previamente y se apropia de la memoria de este driver
 */
bool Table::SetDriver(DriverInterface* pDriver) {
   if (pDriver == NULL) {
      return false;
   }
   delete pDriver_;
   pDriver_ = pDriver;
   delete pQueryInterpreter_;
   pQueryInterpreter_ = new NullQueryInterpreter(pDriver);
   int columns = pDriver_->GetColumns();
   for (int i = 0; i < columns; i++) {
      ColumnType meta = GetMetaTypeColumn(i);
      if (meta == Table::UNKNOWN) {
         if (pDriver_->IsIntColumn(i)) {
            columnTypes_.push_back(Table::INT);
         } else if (pDriver_->IsFloatColumn(i)) {
            columnTypes_.push_back(Table::FLOAT);
         } else if (pDriver_->IsDoubleColumn(i)) {
            columnTypes_.push_back(Table::DOUBLE);
         } else if (pDriver_->IsVarcharColumn(i)) {
            columnTypes_.push_back(Table::STRING);
         } else {
            columnTypes_.push_back(Table::UNKNOWN);
         }
      } else {
         columnTypes_.push_back(meta);
      }
   }
   return true;
}

/**
 * Elimina todas las filas y su contenido de la tabla
 */
bool Table::Clear() {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::DELROW))
      return false;
   if (!pDriver_->GetCapabilities()->IsOperationPermitted(pDriver_->GetPermission(),
                                                          PermissionList::DELROW))
      return false;
   return pDriver_->Clear();
}

/**
 * Agrega restricciones
 * @param[in] Column Numero de la columna sobre la cual se agregan restricciones
 * @param[in] RestrictionString Las restricciones se empaquetaran en un string de la forma siguiente
 * opcion1=representacion1|opcion2=representacion2|...
 * donde los separadores de restricciones y opcion/representacion son DefaultTable::TokenSeparator
 * y DefaultTable::KeyValueSeparator respectivamente
 * @return bool indica si pudo cambiar las restricciones con exito con true, sino devuelve false
 */
bool Table::ChangeRestrictions(int Column, const std::string& RestrictionString) {
   if (Column < 0 || Column >= GetColumns()) {
      return false;
   }
   restrictions_[Column] = StringToMap(RestrictionString, Table::TokenSeparator,
                                       Table::KeyValueSeparator, false);
   UpdateProvider();
   return true;
}

/**
 * Obtiene las restricciones
 * @param[in] Column Numero de la columna sobre la cual se consultan las restricciones
 * @return std::map devuelve un mapa con los pares <restriccion, representacion>
 * No hay restricciones si este mapa se encuentra vacio (es el mismo resultado de HasRestrictions)
 */
std::map<std::string, std::string> Table::GetRestrictions(int Column) {
   if (!HasRestrictions(Column) || Column < 0 || Column >= GetColumns()) {
      return std::map<std::string, std::string>();
   }
   return restrictions_[Column];
}

/**
 * Determina si una columna tiene restricciones
 * @param[in] Column Numero de la columna sobre la cual se consulta si hay las restricciones
 * @return bool indica si la columna tiene restricciones con true, sino las tiene devuelve false
 */
bool Table::HasRestrictions(int Column) {
   return restrictions_.find(Column) != restrictions_.end();
}

/**
 * Setea el nivel de permisos de la tabla
 * @param PermissionKey
 * @return
 */
bool Table::SetPermission(int PermissionKey) {
   if (!permissions_->HasPermission(PermissionKey))
      return false;
   permissionKey_ = PermissionKey;
   return true;
}

/**
 * Getea el nivel de permisos de la tabla
 * @return
 */
int Table::GetPermission() const {
   // Se transparentan los permisos del driver (no incluye columnas de solo lectura o escondidas)
   return pDriver_->GetPermission();
}

/**
 * Getea la lista de permisos configurada para la tabla
 * @return
 */
const PermissionList* Table::GetPermissionList() const {
   // Se transparentan los permisos del driver (no incluye columnas de solo lectura o escondidas)
   return pDriver_->GetCapabilities();
}

/**
 * Determina si una columna es de solo lectura
 * @param Column
 * @return
 */
bool Table::IsReadOnlyColumn(int Column) const {
   return readOnlyColumns_.find(Column) != readOnlyColumns_.end();
}

/**
 * Establece una columna como de solo lectura
 * @param Column
 * @param ReadOnly
 * @return
 */
bool Table::SetReadOnlyColumn(int Column, bool ReadOnly) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::ROCOL))
      return false;

   bool isreadonly = IsReadOnlyColumn(Column);

   if (isreadonly && !ReadOnly) {
      readOnlyColumns_.erase(Column);
   } else if (!isreadonly && ReadOnly) {
      readOnlyColumns_.insert(Column);
   }
   return true;
}

/**
 * Aplica los cambios y efectua la transaccion
 * @return Devuelve true si pudo aplicar los cambios, sino devuelve falso
 */
bool Table::Commit() {
   bool success = pDriver_->Commit();
   if (success)
      UpdateGrid();
   return success;
}

/**
 * Descarta los cambios y no se efectua la transaccion
 * @return Devuelve true si pudo deshacer los cambios, sino devuelve falso
 */
bool Table::Rollback() {
   bool success = pDriver_->Rollback();
   if (success)
      UpdateGrid();
   return success;
}

/**
 * Actualiza la vista de la tabla
 */
void Table::UpdateGrid() {
   UpdateGridNotification notification;
   Notify(&notification);
}

/**
 * Actualiza el provider
 */
void Table::UpdateProvider() {
   UpdateProviderNotification notification;
   Notify(&notification);
}

/**
 * Elimina una fila de la tabla
 * @param[in] Row Nro de fila que quiero eliminar
 * @return bool que indica si tuvo exito
 */
bool Table::DeleteRow(int Row) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::DELROW))
      return false;
   if (!pDriver_->GetCapabilities()->IsOperationPermitted(pDriver_->GetPermission(),
                                                          PermissionList::DELROW))
      return false;

   bool success = pDriver_->DeleteRow(Row);
   if (success)
      UpdateGrid();
   return success;
}

/**
 * Agrega una fila al final de la tabla
 * @return bool indica si se pudo agregar la fila
 */
bool Table::AppendRow() {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::APPROW))
      return false;

   if (!pDriver_->GetCapabilities()->IsOperationPermitted(pDriver_->GetPermission(),
                                                          PermissionList::APPROW)) {
      return false;
   }

   bool success = false;
   // TODO(Gabriel - TCK #2741): ver que pasa cuando se da false (caso reg. fantasma)
   if (pDriver_->CanAppendRow()) {
      success = pDriver_->AppendRow();
      if (success) {
         UpdateGrid();
      }
   }

   return success;
}

/**
 * Inserta una fila en la tabla en la posicion indicada
 * @param Row
 * @return
 */
bool Table::InsertRow(int Row) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::INSROW))
      return false;
   if (!pDriver_->GetCapabilities()->IsOperationPermitted(pDriver_->GetPermission(),
                                                          PermissionList::INSROW))
      return false;
   // TODO(Gabriel - TCK #2741): ver que pasa cuando se da false (caso reg. fantasma)
   bool success = false;
   if (pDriver_->CanAppendRow()) {
      success = pDriver_->InsertRow(Row);
      if (success) {
         UpdateGrid();
      }
   }
   return success;
}

/**
 * Elimina una fila de la tabla (debe existir, sino devuelve false)
 * @param[in] Column nro de columna a eliminar
 * @return bool que indica si se pudo eliminar la columna
 */
bool Table::DeleteColumn(int Column) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::DELCOL))
      return false;
   if (!pDriver_->GetCapabilities()->IsOperationPermitted(pDriver_->GetPermission(),
                                                          PermissionList::DELCOL))
      return false;

   if (0 > Column || Column >= GetColumns()) {
      return false;
   }
   std::string columnname = GetColumnName(Column);
   long columnid = GetColumnId(Column);
   columnTypes_.erase(columnTypes_.begin() + Column);
   bool success = pDriver_->DeleteColumn(Column);
   if (success) {
      DeleteColumNotification notif(columnid, columnname);
      Notify(&notif);
   }

   return success;
}

/**
 * Agrega una columna a la tabla (debe iniciar en 0 e ir incrementandose de a 1)
 * @param[in] ColumnName Nombre de la columna
 * @param[in] ColumnType tipo de la columna
 * @param[in] ColumnFlag flag que indica el tipo de columna
 * @return bool indica si se pudo agregar la columna
 */
bool Table::AppendColumn(const std::string& ColumnName, ColumnType ColumnType,
                         int ColumnFlag) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::APPCOL))
      return false;
   if (!IsSupportedType(ColumnType))
      return false;
   if (!pDriver_->GetCapabilities()->IsOperationPermitted(pDriver_->GetPermission(),
                                                          PermissionList::APPCOL))
      return false;
   std::string type = GetColumnTypeAsString(ColumnType);
   bool success = pDriver_->AppendColumn(type, ColumnName, ColumnFlag);
   if (success) {
      columnTypes_.push_back(ColumnType);
      long colid = pDriver_->GetColumnId(pDriver_->GetColumns()-1);
      NewColumnNotification newcolnotif(colid);
      Notify(&newcolnotif);
      UpdateGrid();
   }
   return success;
}

/**
 * Inserta una columna en la tabla en la posicion indicada
 * @param Column
 * @param ColumnName
 * @param ColumnType
 * @return
 */
bool Table::InsertColumn(int Column, const std::string& ColumnName,
                         ColumnType ColumnType, int ColumnFlag) {
   if (!permissions_->IsOperationPermitted(permissionKey_, PermissionList::INSCOL))
      return false;
   if (!IsSupportedType(ColumnType))
      return false;
   if (!pDriver_->GetCapabilities()->IsOperationPermitted(pDriver_->GetPermission(),
                                                          PermissionList::INSCOL))
      return false;
   std::string type = GetColumnTypeAsString(ColumnType);
   bool success = pDriver_->InsertColumn(Column, type, ColumnName);
   if (success) {
      columnTypes_.insert(columnTypes_.begin() + Column, ColumnType);
      long colid = pDriver_->GetColumnId(Column);
      NewColumnNotification newcolnotif(colid);
      Notify(&newcolnotif);
      UpdateGrid();
   }
   return success;
}

/**
 * Retorna subject que notifica de cambios en los datos de la tabla
 * @return subject que notifica los cambios
 */
Subject* Table::GetDataChangeNotifier() {
   return pDataChangeSubject_;
}

/**
 * Configura subject que notifica de cambios en los datos de la tabla
 * @param[in] pDataChangeSubject notifica los cambios
 */
void Table::SetDataChangeNotifier(Subject* pDataChangeSubject) {
   pDataChangeSubject_ = pDataChangeSubject;
}

/**
 * Devuelve el nombre asociado al tipo de columna
 * @param Type
 * @return
 */
std::string Table::GetColumnTypeAsString(ColumnType ColumnType) const {
   std::string type;
   switch (ColumnType) {
      case STRING:
      case COLOR_RGBA_HEXA:
      case FILE:
      case URL:
      case VSTYLE:
      case QBUILDER:
         type = DataInfo<unsigned char>::Name;
         break;
      case INT:
      case BOOL:
      case DATE:
         type = DataInfo<int>::Name;
         break;
      case FLOAT:
         type = DataInfo<float>::Name;
         break;
      case DOUBLE:
         type= DataInfo<double>::Name;
         break;
      default:
         type = "";
   }
   return type;
}

/** Obtien el id asociado a una fila
 *  @param[in] Row fila a consultar
 *  @return id de la fila
 *  @return -1 en caso de que la fila no se encuentre en la tabla*/
long Table::GetRowId(int Row) const {
   // Obtiene el fid al que pertenece la fila
   return pQueryInterpreter_->GetRow(Row) < 0 ?
         pDriver_->GetRowId(Row) : pQueryInterpreter_->GetRow(Row);
}

/** obtiene la fila asociada al id que se pasa por parametro
 *  @param[in] Id id de la fila a consultar
 *  @return id de la fila
 *  @return -1 en caso de que la fila no se encuentre en la tabla*/
int Table::GetRowById(long Id) const {
   return pDriver_->GetRowById(Id);
}

/** Obtiene los ids de las filas que contiene la tabla **/
std::vector<long> Table::GetRowIds() const {
   return pDriver_->GetRowIds();
}

/** Agrega una columna autocalculada con la formulada indicada por parametro
 * @param[in] ColumnName Nombre de la columna
 * @param[in] Equation ecuacion que se aplicara a la columna
 * @param[in] ColumnFlag flag que indica el tipo de columna
 */
bool Table::AppendCalculatedColumn(const std::string& ColumnName, ColumnType ColumnType,
                                   const std::string& Equation, int ColumnFlag) {
   // se podria validar la ecuacion
   // Agrego la columna como si fuese una de tipo float
   AppendColumn(ColumnName, Table::FLOAT, ColumnFlag);
   // aplico la formula a cada valor de la nueva columna
   int colindex = GetColumns() -1;
   EquationParser parser;
   if (!parser.SetEquation(Equation))
         return false;
   std::vector < std::string > variablenames;
   parser.GetVariableNames(variablenames);
   for (int row = 0; row < GetRows(); ++row) {
      std::vector < std::string >::iterator it = variablenames.begin();
      for ( ; it != variablenames.end(); ++it) {
         int cvarindex = GetVariableColumnIndex(*it);
         double* pvalue = NULL;
         parser.GetVariableValuePointer(*it, pvalue);
         std::string cellvalue;
         GetCellValue(cvarindex, row, cellvalue);
         *pvalue = StringToNumber<double>(cellvalue);
      }
      std::string value = DoubleToString(parser.EvaluateEquation());
      SetCellValue(colindex, row, value);
   }
   return true;
}

/** Inserta una columna en la tabla en la posicion indicada */
bool Table::InsertCalculatedColumn(int Column, const std::string& ColumnName,
                                   ColumnType ColumnType, const std::string& Equation,
                                   int ColumnFlag) {
   return false;
}

/** Metodo que verifica que la columna posee definido un tipo logico de dato*/
Table::ColumnType Table::GetMetaTypeColumn(int Column) const {
   return Table::UNKNOWN;
}

/** Obtiene el id asociado al indice de columna pasado por parametro **/
long Table::GetColumnId(int ColIndex) const {
   return pDriver_ ? pDriver_->GetColumnId(ColIndex) : std::numeric_limits<int>::max();
}
/** Obtiene el indice asociado al id de columna pasado por parametro **/
int Table::GetColumnIndexBy(long ColId) const {
   return pDriver_ ? pDriver_->GetColumnIndexBy(ColId) : std::numeric_limits<int>::max();
}

/** Ejecuta la consulta del SearchWidget
 * @param[in] Query condiciones que deben agregarse a la seleccion
 * @return true si la consulta se ejecuto correctamente
 */
bool Table::ExecuteQuery(const std::string& Query) {
   if (pQueryInterpreter_)
      delete pQueryInterpreter_;
   bool canExecute = true;
   if (!url_.empty() && !tableName_.empty() && !Query.empty()) {
      std::string auxQuery = SQL_SELECT_STRING;
      auxQuery += "'";
      auxQuery += tableName_;
      auxQuery += "'";
      auxQuery += " " + Query;
      QueryInterpreter* pquery = new QueryInterpreter(url_, auxQuery);
      pQueryInterpreter_ = pquery;
      canExecute = pquery->CanExecute();
   } else {
      pQueryInterpreter_ = new NullQueryInterpreter(pDriver_);
   }
   if (canExecute)
      UpdateGrid();
   return canExecute;
}

/** Retorna el estado de la tabla */
bool Table::GetTableReadOnlyState() {
   return isReadOnlyTable;
}

/** Setea el estado de la tabla */
void Table::SetReadOnlyState(bool State) {
   isReadOnlyTable = State;
}

/**
 * Obtiene el indice de la columna que corresponde a una variable de una ecuacion
 * @param Variable variable sobre la cual se consulta
 * @return indice de la columna
 * @return -1 en caso de no encontrar un indice valido de la columna
 */
int Table::GetVariableColumnIndex(const std::string& Variable) const {
   size_t found = Variable.find_last_of("_");
   int columnindex = -1;
   if (found != std::string::npos) {
      std::string columnindexstr = Variable.substr(found + 1);
      columnindex = StringToNumber<int>(columnindexstr);
      columnindex = columnindex > GetColumns() ? -1 : columnindex;
   }
   return columnindex;
}

/** Validacion de ecuacion */
bool Table::IsValidEquation(const std::string& Equation) const {
   EquationParser parser;
   if (!parser.SetEquation(Equation))
         return false;
   std::vector < std::string > variablenames;
   parser.GetVariableNames(variablenames);
   std::vector<std::string>::iterator it = variablenames.begin();
   bool valideq = true;
   for (; it != variablenames.end() && valideq; ++it) {
      int columnindex = GetVariableColumnIndex(*it);
      if (columnindex < 0)
         valideq = false;
   }
   return valideq;
}
} /** namespace suri **/
