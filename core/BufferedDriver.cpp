/*! \file BufferedDriver.cpp */
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
#include <string>
#include <sstream>
#include <algorithm>
#include <deque>
#include <limits>
#include <memory>
#ifdef __SHOW_TABLE_LOG__
#include <iostream>
#endif

// Includes Suri
#include "suri/BufferedDriver.h"
#include "TableOperation.h"
#include "PermissionList.h"
#include "suri/SuriObject.h"
#include "NewRowNotification.h"
#include "CanAppendRowNotification.h"
#include "suri/AuxiliaryFunctions.h"

#ifdef __SHOW_TABLE_LOG__
#include "suri/MemoryDriver.h"
#endif

#include "suri/ParameterCollection.h"
#include "DeleteRowNotification.h"


namespace suri {

const std::string BufferedDriver::DefaultData = "0"; /*! Valor por defecto de los strings */
const std::string BufferedDriver::DefaultDataStr = ""; /*! Valor por defecto de los strings */

/**
 * Constructor
 * @param pDriverInterface
 */
BufferedDriver::BufferedDriver(DriverInterface* pDriverInterface) :
      pDriver_(pDriverInterface), newColumnIndexSeed_(-1), newRowIndexSeed_(0) {
   Rollback();
}

/**
 * Destructor
 */
BufferedDriver::~BufferedDriver() {
   // libera el driver
   if (pDriver_ != NULL) {
      delete pDriver_;
      pDriver_ = NULL;
   }

   // libera los parametros
   for (int ix = 0, lenix = paramsGC_.size(); ix < lenix; ++ix) {
      delete paramsGC_[ix];
   }
   paramsGC_.clear();

   // libera columnas
   std::vector<COLUMN*>::iterator it;
   for (it = columns_.begin(); it != columns_.end(); ++it) {
      delete *it;
   }
   columns_.clear();
}

/**
 * Leer un entero del driver
 * La operacion es delegada al driver que se decora
 * @param Column Columna
 * @param Row Fila
 * @param Data Dato
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::ReadInt(int Column, int Row, int& Data) const {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(),
                                                PermissionList::GETVAL)) return false;

   if (!GetInt(Column, Row, Data)) {
      return pDriver_->ReadInt(columns_[Column]->Id, rowIndexHolder_[Row], Data);
   }
   return true;
}

/**
 * Escribir un entero en el driver
 * La operacion es delegada al driver que se decora
 * @param Column Columna
 * @param Row Fila
 * @param Data Dato
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::WriteInt(int Column, int Row, int Data) {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(),
                                                PermissionList::SETVAL))
      return false;

   // TODO(Sebastian - TCK# 2220): Revisar Tipo de Columna
   std::ostringstream osstream;
   osstream << Data;
   std::string strdata = osstream.str();
   return WriteVarchar(Column, Row, strdata);
}

/**
 * Leer un string del driver
 * La operacion es delegada al driver que se decora
 * @param Column Columna
 * @param Row Fila
 * @param Data Dato
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::ReadVarchar(int Column, int Row, std::string& Data) const {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(),
                                                PermissionList::GETVAL)) return false;

   if (!GetVarchar(Column, Row, Data)) {
      return pDriver_->ReadVarchar(columns_[Column]->Id, rowIndexHolder_[Row], Data);
   }

   return true;
}

/**
 * Escribir un string en el driver
 * La operacion es delegada al driver que se decora
 * @param Column Columna
 * @param Row Fila
 * @param Data Dato
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::WriteVarchar(int Column, int Row, const std::string& Data) {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(),
                                                PermissionList::SETVAL)) return false;

   int tablerow = Row;
   int dsrow = rowIndexHolder_[tablerow];

   TableOperation tableop(PermissionList::SETVAL, Column, columns_[Column]->Id,
                          tablerow, dsrow, Data);
   operations_.push_back(tableop);

   return true;
}

/**
 * Leer un punto flotante del driver
 * La operacion es delegada al driver que se decora
 * @param Column Columna
 * @param Row Fila
 * @param Data Dato
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::ReadFloat(int Column, int Row, float& Data) const {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(),
                                                PermissionList::GETVAL)) return false;

   if (!GetFloat(Column, Row, Data)) {
      return pDriver_->ReadFloat(columns_[Column]->Id, rowIndexHolder_[Row], Data);
   }

   return true;
}
/**
 * Leer un punto double del driver
 * La operacion es delegada al driver que se decora
 * @param Column Columna
 * @param Row Fila
 * @param Data Dato
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::ReadDouble(int Column, int Row, double& Data) const {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(),
                                                PermissionList::GETVAL)) return false;
  if (!GetDouble(Column, Row, Data)) {
     return pDriver_->ReadDouble(columns_[Column]->Id, rowIndexHolder_[Row], Data);
  }
  return true;
 }

/**
 * Escribir un double en el driver
 * La operacion es delegada al driver que se decora
 * @param Column Columna
 * @param Row Fila
 * @param Data Dato
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::WriteDouble(int Column, int Row, double Data) {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(),
                                                PermissionList::SETVAL)) return false;

   // TODO(Sebastian - TCK# 2220): Revisar Tipo de Columna
   return WriteVarchar(Column, Row, DoubleToStringEx(Data,15));
}

/**
 * Escribir un punto flotante en el driver
 * La operacion es delegada al driver que se decora
 * @param Column Columna
 * @param Row Fila
 * @param Data Dato
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::WriteFloat(int Column, int Row, float Data) {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(),
                                                PermissionList::SETVAL)) return false;

   // TODO(Sebastian - TCK# 2220): Revisar Tipo de Columna
   std::ostringstream osstream;
   osstream << Data;
   std::string strdata = osstream.str();
   return WriteVarchar(Column, Row, strdata);
}

/**
 * Devuelve la cantidad de columnas
 * @return Devuelve el valor de cantidad de columnas actualizado en el buffer
 */
int BufferedDriver::GetColumns() const {
   return columns_.size();
}

/**
 * Devuelve la cantidad de filas
 * @return Devuelve el valor de cantidad de filas actualizado en el buffer
 */
int BufferedDriver::GetRows() const {
   return rowIndexHolder_.size();
}

/**
 * Agrega una fila al final de la tabla
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::AppendRow() {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(),
                                                PermissionList::APPROW)) {
      return false;
   }

   int newdsrow = GetNewRowId();

   rowIndexHolder_.push_back(newdsrow);

   int newtablerow = rowIndexHolder_.size() - 1;

   TableOperation addrowop(PermissionList::APPROW, UNDEFINEDCOLUMN, UNDEFINEDCOLUMN,
                           newtablerow, newdsrow, "APPROW");
   operations_.push_back(addrowop);

   for (int ix = 0, lenix = GetColumns(); ix < lenix; ++ix) {
      TableOperation tableop(PermissionList::SETVAL, ix, columns_[ix]->Id, newtablerow,
                             newdsrow, "");
      operations_.push_back(tableop);
   }

   NewRowNotification newrownotif(newdsrow);
   Notify(&newrownotif);

   return true;
}

/**
 * Agrega una fila en la posicion especificada de la tabla
 * @param Row
 * @return
 */
bool BufferedDriver::InsertRow(int Row) {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(),
                                                PermissionList::INSROW)) {
      return false;
   }

   int newdsrow = GetNewRowId();
   rowIndexHolder_.insert(rowIndexHolder_.begin() + Row, newdsrow);

   int newtablerow = Row;

   TableOperation addrowop(PermissionList::INSROW, UNDEFINEDCOLUMN, UNDEFINEDCOLUMN,
                           newtablerow, newdsrow, "INSROW");
   operations_.push_back(addrowop);

   for (int ix = 0, lenix = GetColumns(); ix < lenix; ++ix) {
      TableOperation tableop(PermissionList::SETVAL, ix, columns_[ix]->Id, newtablerow,
                             newdsrow, "");
      operations_.push_back(tableop);
   }

   return true;
}

/**
 * Elimina una fila de la tabla (debe existir, sino devuelve false)
 *
 * @param Row Fila
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::DeleteRow(int Row) {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(),
                                                PermissionList::DELROW)) {
      return false;
   }

   int tablerow = Row;
   int dsrow = rowIndexHolder_[tablerow];
   bool deleted = false;

   std::auto_ptr<GenericParameter<int> > fid(new GenericParameter<int>(dsrow));

   DeleteRowNotification deletenotif;
   deletenotif.SetTag(fid.get());
   Notify(&deletenotif);

   if (tablerow < static_cast<int>(rowIndexHolder_.size())) {
      rowIndexHolder_.erase(rowIndexHolder_.begin() + tablerow);
      deleted = true;
   }

   if (!deleted) return false;

   // Carga la operacion en la cola de operaciones
   TableOperation delrowop(PermissionList::DELROW, UNDEFINEDCOLUMN, UNDEFINEDCOLUMN,
                           tablerow, dsrow, "DELROW");
   operations_.push_back(delrowop);

   return true;
}

/**
 * Agrega una columna al final de la tabla
 * @param[in] ColumnType tipo de dato para la nueva columna
 * @param[in] ColumnName nombre de la columna
 * @param[in] ColumnFlag flag que indica el tipo de column
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::AppendColumn(const std::string& ColumnType,
                                  const std::string& ColumnName, int ColumnFlag) {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(),
                                                PermissionList::APPCOL)) {
      return false;
   }

   int newdscolumn = GetNewColumnId();

   COLUMN* pcol = new COLUMN();
   pcol->Id = newdscolumn;
   pcol->ColumnType = ColumnType;
   pcol->ColumnName = ColumnName;
   pcol->ColumnFlag = ColumnFlag;

   columns_.push_back(pcol);
   int newtablecolumn = columns_.size() - 1;

   GenericParameter<COLUMN*>* pdata = new GenericParameter<COLUMN*>(pcol);
   paramsGC_.push_back(pdata);

   TableOperation addcolop(PermissionList::APPCOL, newtablecolumn, newdscolumn,
                           UNDEFINEDROW, UNDEFINEDROW, "APPCOL", pdata);
   operations_.push_back(addcolop);

   for (int ix = 0, lenix = rowIndexHolder_.size(); ix < lenix; ++ix) {
      int tablerow = ix;
      int dsrow = rowIndexHolder_[tablerow];

      std::string dvalue =
            ColumnType.compare(DataInfo<unsigned char>::Name) == 0 ? DefaultDataStr :
                                                                     DefaultData;
      TableOperation setvalop(PermissionList::SETVAL, newtablecolumn, newdscolumn,
                              tablerow, dsrow, dvalue);
      operations_.push_back(setvalop);
   }
   return true;
}

/**
 * Agrega una columna en la posicion especificada de la tabla
 * @param Column
 * @param ColumnType
 * @param ColumnName
 * @return
 */
bool BufferedDriver::InsertColumn(int Column, const std::string& ColumnType,
                                  const std::string& ColumnName, int ColumnFlag) {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(), PermissionList::INSCOL)) {
      return false;
   }

   int newdscolumn = GetNewColumnId();

   COLUMN* pcol = new COLUMN();
   pcol->Id = newdscolumn;
   pcol->ColumnType = ColumnType;
   pcol->ColumnName = ColumnName;
   pcol->ColumnFlag = ColumnFlag;

   columns_.insert(columns_.begin() + Column, pcol);

   int newtablecolumn = Column;

   GenericParameter<COLUMN*>* pinsdata = new GenericParameter<COLUMN*>(pcol);
   paramsGC_.push_back(pinsdata);

   TableOperation addcolop(PermissionList::INSCOL, newtablecolumn, newdscolumn,
                           UNDEFINEDROW, UNDEFINEDROW, "INSCOL", pinsdata);
   operations_.push_back(addcolop);

   for (int ix = 0, lenix = rowIndexHolder_.size(); ix < lenix; ++ix) {
      int tablerow = ix;
      int dsrow = rowIndexHolder_[tablerow];
      TableOperation setvalop(PermissionList::SETVAL, newtablecolumn, newdscolumn,
                              tablerow, dsrow, DefaultData);
      operations_.push_back(setvalop);
   }
   return true;
}

/**
 * Elimina una columna de la tabla (debe existir, sino devuelve false)
 * @param Column Columna
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::DeleteColumn(int Column) {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(),
                                                PermissionList::DELCOL)) {
      return false;
   }

   int tablecolumn = Column;
   int dscolumn = columns_[tablecolumn]->Id;

   bool deleted = false;
   if (tablecolumn < static_cast<int>(columns_.size())) {
      columns_.erase(columns_.begin() + tablecolumn);
      deleted = true;
   }

   if (!deleted) return false;

   TableOperation delcolop(PermissionList::DELCOL, tablecolumn, dscolumn, UNDEFINEDROW,
                           UNDEFINEDROW, "DELCOL");
   operations_.push_back(delcolop);
   return true;
}

/**
 * Elimina todas las filas y su contenido de la tabla (no elimina columnas)
 * La operacion es delegada al driver que se decora
 * Esta operacion no es reversible
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::Clear() {
   if (!GetCapabilities()->IsOperationPermitted(GetPermission(),
                                                PermissionList::DELROW))
      return false;

   if (Rollback())
      return pDriver_->Clear();
   return false;
}

/**
 * Getea la lista de permisos configurada para la tabla (los permisos son los del driver subyacente)
 * @return Devuelve la lista de permisos del Driver. Devuelve NULL si es invalida.
 */
const PermissionList* BufferedDriver::GetCapabilities() const {
   return pDriver_->GetCapabilities();
}

/**
 * Getea el permiso utilizado en GetCapabilities
 * @return
 */
int BufferedDriver::GetPermission() const {
   return pDriver_->GetPermission();
}

/**
 * Aplica los cambios realizados mediante el uso del buffer y efectua la transaccion transmitiendo
 * los cambios al driver subyacente
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::Commit() {
   while (!operations_.empty()) {
#ifdef __SHOW_TABLE_LOG__
      std::cout << std::endl;
      std::cout << GetSubDriverString() << std::endl;
      std::cout << GetOperations() << std::endl;
      std::cout << GetColumnIndex() << std::endl;
      std::cout << GetRowIndex() << std::endl;
      std::cout << std::endl;
#endif
      bool success = false;
      TableOperation operation = operations_.front();
      PermissionList::OperationType type = operation.GetOperationType();
      int storedcol = operation.GetTableColumn();
      int storedrow = operation.GetTableRow();
      std::string data = operation.GetValue();
      int intdata;
      float floatdata;
      std::stringstream ssi(data);
      ssi >> intdata;
      std::stringstream ssf(data);
      ssf >> floatdata;
      switch (type) {
         case PermissionList::INSCOL:
            {
               GenericParameter<COLUMN*>* pgp =
                  dynamic_cast<GenericParameter<COLUMN*>*>(operation.GetTag());
               COLUMN* pcol = pgp->GetValue();
               if (pcol->ColumnFlag & DriverInterface::RegularFlag) {
                  success = pDriver_->InsertColumn(storedcol, pcol->ColumnType, pcol->ColumnName);
               } else {
                  success = true;
               }
            }
            break;
         case PermissionList::APPCOL:
            {
               GenericParameter<COLUMN*>* pgp =
                  dynamic_cast<GenericParameter<COLUMN*>*>(operation.GetTag());
               COLUMN* pcol = pgp->GetValue();
               if (pcol->ColumnFlag & DriverInterface::RegularFlag) {
                  success = pDriver_->AppendColumn(pcol->ColumnType, pcol->ColumnName);
               } else {
                  success = true;
               }
            }
            break;
         case PermissionList::INSROW:
            {
               success = pDriver_->InsertRow(storedrow);
            }
            break;
         case PermissionList::APPROW:
            success = pDriver_->AppendRow();
            break;
         case PermissionList::DELCOL:
            success = pDriver_->DeleteColumn(storedcol);
            break;
         case PermissionList::DELROW:
            {
               success = pDriver_->DeleteRow(storedrow);
            }
            break;
         case PermissionList::SETVAL:
            {
               if (Type2Id(pDriver_->GetColumnType(storedcol))
                     == DataInfo<unsigned char>::Id) {
                  success = pDriver_->WriteVarchar(storedcol, storedrow, data);
               } else if (Type2Id(pDriver_->GetColumnType(storedcol))
                     == DataInfo<int>::Id) {
                  success = pDriver_->WriteInt(storedcol, storedrow, intdata);
               } else if (Type2Id(pDriver_->GetColumnType(storedcol))
                     == DataInfo<double>::Id) {
                  success = pDriver_->WriteDouble(storedcol, storedrow, floatdata);
               }
            }
            break;
         case PermissionList::GETVAL:
         case PermissionList::HIDECOL:
         case PermissionList::ROCOL:
            // Estas operaciones no estan soportadas
            break;
         default:
            // Estas operaciones no estan soportadas
            break;
      }
      if (success) {
         operations_.pop_front();
      } else {
         return false;
      }
   }
   return Rollback();
}

/**
 * Descarta los cambios y no se efectua la transaccion
 * Limpia todas las operaciones en el buffer y resetea los indices de filas y columnas
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::Rollback() {
   operations_.clear();
   InitializeColumns();
   InitializeRows();
   return true;
}

/**
 * Busca el valor entero en la cola
 * @param Column Columna
 * @param Row Fila
 * @param Data Dato
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::GetInt(int Column, int Row, int& Data) const {
   std::string strdata = BufferedDriver::DefaultData;
   if (GetVarchar(Column, Row, strdata)) {
      std::stringstream ss(strdata);
      ss >> Data;
      return true;
   }
   return false;
}

/**
 * Busca el valor varchar en la cola
 * @param Column Columna
 * @param Row Fila
 * @param Data Dato
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::GetVarchar(int Column, int Row, std::string& Data) const {
   int tablerow = Row;
   int dsrow = rowIndexHolder_[tablerow];

   std::deque<TableOperation>::const_reverse_iterator it;
   for (it = operations_.rbegin(); it != operations_.rend(); it++) {
      if (it->GetOperationType() == PermissionList::SETVAL && it->GetDSRow() == dsrow
            && it->GetDSColumn() == columns_[Column]->Id) {
         Data.clear();
         Data.append(it->GetValue());
         return true;
      }
   }

   return false;
}

/**
 * Busca el valor flotante en la cola
 * @param Column Columna
 * @param Row Fila
 * @param Data Dato
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::GetFloat(int Column, int Row, float& Data) const {
   std::string strdata = BufferedDriver::DefaultData;
   if (GetVarchar(Column, Row, strdata)) {
      std::stringstream ss(strdata);
      ss >> Data;
      return true;
   }
   return false;
}
/**
 * Busca el valor double en la cola
 * @param Column Columna
 * @param Row Fila
 * @param Data Dato
 * @return Devuelve verdadero si la operacion fue exitosa, sino devuelve falso
 */
bool BufferedDriver::GetDouble(int Column, int Row, double& Data) const {
   std::string strdata = BufferedDriver::DefaultData;
   if (GetVarchar(Column, Row, strdata)) {
      std::stringstream ss(strdata);
      ss >> Data;
      return true;
   }
   return false;
}
/**
 * Cuenta el valor neto de agregado y quitado de columnas en la cola de operaciones
 * @return Devuelve el valor neto de agregado y quitado de columnas en la cola de operaciones
 */
int BufferedDriver::CountColumns() const {
   int count = 0;
   std::deque<TableOperation>::const_iterator it;
   for (it = operations_.begin(); it != operations_.end(); it++) {
      if (it->GetOperationType() == PermissionList::APPCOL
            || it->GetOperationType() == PermissionList::INSCOL) {
         count++;
      } else if (it->GetOperationType() == PermissionList::DELCOL) {
         count--;
      }
   }
   return count;
}

/**
 * Cuenta el valor neto de agregado y quitado de filas en la cola de operaciones
 * @return Devuelve el valor neto de agregado y quitado de columnas en la cola de operaciones
 */
int BufferedDriver::CountRows() const {
   int count = 0;
   std::deque<TableOperation>::const_iterator it;
   for (it = operations_.begin(); it != operations_.end(); it++) {
      if (it->GetOperationType() == PermissionList::APPROW
            || it->GetOperationType() == PermissionList::INSROW) {
         count++;
      } else if (it->GetOperationType() == PermissionList::DELROW) {
         count--;
      }
   }
   return count;
}

/**
 * Inicializa los indices de columnas con los del driver
 */
void BufferedDriver::InitializeColumns() {
   // inicializo y/o limpio
   std::vector<COLUMN*>::iterator it;
   for (it = columns_.begin(); it != columns_.end(); ++it) {
      delete *it;
   }
   columns_.clear();

   // cargo
   for (int ix = 0, lenix = pDriver_->GetColumns(); ix < lenix; ++ix) {
      COLUMN* pcol = new COLUMN();
      pcol->Id = ix;
      pcol->ColumnType = pDriver_->GetColumnType(ix);
      pcol->ColumnName = pDriver_->GetColumnName(ix);
      pcol->ColumnFlag = pDriver_->GetColumnFlags(ix);
      columns_.push_back(pcol);
   }

   // Actualiza el seed para la generacion de indices nuevos
   newColumnIndexSeed_ = -1;
}

/**
 * Inicializa los indices de filas con los del driver
 */
void BufferedDriver::InitializeRows() {
   rowIndexHolder_.clear();
   int ix = 0;
   for (int lenix = pDriver_->GetRows(); ix < lenix; ++ix) {
      rowIndexHolder_.push_back(ix);
   }

   // Actualiza el seed para la generacion de indices nuevos
   newRowIndexSeed_ = ix;
}

/**
 * Obtiene el nombre de la columna
 * @param Column Columna
 * @return Obtiene el nombre de la columna, si no existe devuelve vacio
 */
std::string BufferedDriver::GetColumnName(int Column) const {
   return columns_[Column]->ColumnName;
}

/**
 * Obtiene el tipo de la columna
 * @param Column Columna
 * @return Obtiene el nombre de la columna, si no existe devuelve vacio
 */
std::string BufferedDriver::GetColumnType(int Column) const {
   return columns_[Column]->ColumnType;
}

#ifdef __SHOW_TABLE_LOG__
/**
 * Getter del string del subdriver
 * @return
 */
std::string BufferedDriver::GetSubDriverString() const {
   MemoryDriver* pmem = dynamic_cast<MemoryDriver*>(pDriver_);
   if (pmem == NULL)
   return "";
   return pmem->ToString();
}

/**
 * Obtiene las operaciones en un string
 * \note Metodo Debug
 * @return
 */
std::string BufferedDriver::GetOperations() const {
   std::string opstr = "[ ";
   OperationQueue::const_iterator it;
   for (it = operations_.begin(); it != operations_.end(); it++) {
      opstr.append("( ");

      std::ostringstream osot;
      osot << it->GetOperationType();
      opstr.append(osot.str());
      opstr.append(", ");

      std::ostringstream oscol;
      oscol << it->GetColumn();
      opstr.append(oscol.str());
      opstr.append(", ");

      std::ostringstream osrow;
      osrow << it->GetRow();
      opstr.append(osrow.str());
      opstr.append(", ");

      opstr.append(it->GetValue());
      opstr.append(" )");

      if (it + 1 != operations_.end()) {
         opstr.append(";");
      }

      opstr.append(" ");
   }
   opstr.append("]");
   return opstr;
}

/**
 * Obtiene los indices de columnas en un string
 * \note Metodo Debug
 * @return
 */
std::string BufferedDriver::GetColumnIndex() const {
   std::string cistr = "[ ";
   int size = columnIndex_.size();
   for (int i = 0; i < size; i++) {
      cistr.append("( ");

      std::ostringstream newcol;
      newcol << i;
      cistr.append(newcol.str());
      cistr.append(", ");

      std::ostringstream oldcol;
      oldcol << columnIndex_[i];
      cistr.append(oldcol.str());
      cistr.append(" )");

      if (i + 1 != size) {
         cistr.append(";");
      }

      cistr.append(" ");
   }
   cistr.append("]");
   return cistr;
}

/**
 * Obtiene los indices de filas en un string
 * \note Metodo Debug
 * @return
 */
std::string BufferedDriver::GetRowIndex() const {
   std::string ristr = "[ ";
   int size = rowIndex_.size();
   for (int i = 0; i < size; i++) {
      ristr.append("( ");

      std::ostringstream newrow;
      newrow << i;
      ristr.append(newrow.str());
      ristr.append(", ");

      std::ostringstream oldrow;
      oldrow << rowIndex_[i];
      ristr.append(oldrow.str());
      ristr.append(" )");

      if (i + 1 != size) {
         ristr.append(";");
      }

      ristr.append(" ");
   }
   ristr.append("]");
   return ristr;
}
#endif

/**
 * Getea el Id del Datasource asociado
 * @return
 */
SuriObject::UuidType BufferedDriver::GetAssociatedDatasourceId() const {
   return pDriver_->GetAssociatedDatasourceId();
}

/**
 * Determina si se soporta el tipo
 * @param ColumnType Tipo de dato
 * @return devuelve true si se soporta el tipo
 */
bool BufferedDriver::IsSupportedType(const std::string& ColumnType) {
   bool idint = Type2Id(ColumnType) == DataInfo<int>::Id;
   bool idstring = Type2Id(ColumnType) == DataInfo<unsigned char>::Id;
   bool idfloat = Type2Id(ColumnType) == DataInfo<double>::Id;
   return idint || idstring || idfloat;
}

/**
 * Determina si una celda es valida
 * @param[in] Column indice de la columna a ser eliminada
 * @return devuelve true si la operacion tuvo exito, sino devuelve false
 */
bool BufferedDriver::IsValidCell(int Column, int Row) const {
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
bool BufferedDriver::IsIntColumn(int Column) const {
   return Type2Id(GetColumnType(Column)) == DataInfo<int>::Id;
}

/**
 * Determina si la columna es de cadena de caracteres
 * @param[in] Column indice de la columna
 * @return devuelve true si la columna es varchar
 */
bool BufferedDriver::IsVarcharColumn(int Column) const {
   // El tipo unsigned char se usa para representar del string porque DataInfo no lo contempla
   return Type2Id(GetColumnType(Column)) == DataInfo<unsigned char>::Id;
}

/**
 * Determina si la columna es de punto flotante
 * @param[in] Column indice de la columna
 * @return devuelve true si la columna es flotante
 */
bool BufferedDriver::IsFloatColumn(int Column) const {
   return Type2Id(GetColumnType(Column)) == DataInfo<float>::Id;
}

/** Determina si la columna es de punto flotante (double) */
bool BufferedDriver::IsDoubleColumn(int Column) const {
   return Type2Id(GetColumnType(Column)) == DataInfo<double>::Id;
}

/**
 * Verifica que se puede agrega una columna.
 */
bool BufferedDriver::CanAppendRow() {
   CanAppendRowNotification carnotif;
   Notify(&carnotif);
   bool canappend = carnotif.CanAppendRow() && pDriver_->CanAppendRow();
   return canappend;
}

/** Devuelve el id asociado a la fila, -1 en caso contrario
 *  @param[in] Row fila a consultar
 *  @return id de la fila
 *  @return -1 en caso de que la fila no se encuentre en la tabla
 *  */
long BufferedDriver::GetRowId(int Row) const {
   if (Row < static_cast<int>(rowIndexHolder_.size()))
      return rowIndexHolder_[Row];

   return -1;
}

/** obtiene la fila asociada al id que se pasa por parametro
 *  @param[in] Id id de la fila a consultar
 *  @return id de la fila
 *  @return -1 en caso de que la fila no se encuentre en la tabla*/
int BufferedDriver::GetRowById(long Id) const {
   int row = std::numeric_limits<int>::min();
   bool found = false;

   for (int ix = 0, lenix = rowIndexHolder_.size(); ix < lenix && !found; ++ix) {
      if (Id == rowIndexHolder_[ix]) {
         row = ix;
         found = true;
      }
   }
   return row;
}

/** Obtiene un nuevo id para una fila */
long BufferedDriver::GetNewRowId() {
   return ++newRowIndexSeed_;
}

/** Obtiene un nuevo id para una columna */
long BufferedDriver::GetNewColumnId() {
   return newColumnIndexSeed_--;
}

/** Obtiene los ids de las filas que contiene la tabla **/
std::vector<long> BufferedDriver::GetRowIds() const {
   return pDriver_->GetRowIds();
}

/** Obtiene el id asociado al indice de columna pasado por parametro **/
long BufferedDriver::GetColumnId(int ColIndex) const {
   int columnssize = columns_.size() - 1;
   if (ColIndex > columnssize)
      return std::numeric_limits<int>::max();
   return columns_[ColIndex]->Id;
}
/** Obtiene el indice asociado al id de columna pasado por parametro
 *  @return indice de columna asociado al id
 *  @return std::numeric_limits<int>::max() en caso contrario**/
int BufferedDriver::GetColumnIndexBy(long ColId) const {
   std::vector<COLUMN*>::const_iterator it = columns_.begin();
   int index = std::numeric_limits<int>::max();
   for (int i = 0; it != columns_.end(); ++it, ++i) {
      if ((*it)->GetId() == ColId) {
         index = i;
      }
   }
   return index;
}
} /** namespace suri */
