/** \file RasterDnDriver.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "RasterDnDriver.h"
#include "suri/DataTypes.h"
#include "suri/World.h"

namespace suri {

const int RasterDnDriver::kPermissionKey = 0;
const std::string RasterDnDriver::kColumnName = "DN";

/**
 * Ctor.
 */
RasterDnDriver::RasterDnDriver(RasterDnInfo* pDnInfo) :
      pDnInfo_(pDnInfo) {
   LoadItems();
   LoadPermissions();
}

/**
 * Dtor.
 */
RasterDnDriver::~RasterDnDriver() {
   if (pDnInfo_ != NULL) {
      delete pDnInfo_;
      pDnInfo_ = NULL;
   }
}

/**
 * Leer un entero del driver.
 *
 * @param[in] Column Columna de la que se desea leer el valor.
 * @param[in] Row Fila de la que se desea leer el valor.
 * @param[out] Data Informacion leida
 * @return true se la operacion se realizo con exito.
 */
bool RasterDnDriver::ReadInt(int Column, int Row, int& Data) const {
   return false;
}

/**
 * Escribir un entero en el driver.
 *
 * OPERACION NO SOPORTADA
 */
bool RasterDnDriver::WriteInt(int Column, int Row, int Data) {
   return false;
}

/**
 * Leer un string del driver.
 *
 * OPERACION NO SOPORTADA
 */
bool RasterDnDriver::ReadVarchar(int Column, int Row, std::string& Data) const {
   if (Row < static_cast<int>(items_.size())) {
      PDRIVER_ITEM pitem = items_[Row];
      if (Column < static_cast<int>(pitem->columns_.size())) {
         PDRIVER_COLUMN pcolumn = pitem->columns_[Column];
         Data = pcolumn->value_;
         return true;
      }
   }

   return false;
}

/**
 * Escribir un string en el driver.
 *
 * OPERACION NO SOPORTADA
 */
bool RasterDnDriver::WriteVarchar(int Column, int Row, const std::string& Data) {
   return false;
}

/**
 * Leer un punto flotante del driver.
 *
 * OPERACION NO SOPORTADA
 */
bool RasterDnDriver::ReadFloat(int Column, int Row, float& Data) const {
   return false;
}

/**
 * Escribir un punto flotante en el driver.
 *
 * OPERACION NO SOPORTADA
 */
bool RasterDnDriver::WriteFloat(int Column, int Row, float Data) {
   return false;
}

/**
 * Devuelve la cantidad de columnas.
 *
 * @return Entero con la cantidad de columnas.
 */
int RasterDnDriver::GetColumns() const {
   if (items_.size() > 0)
      return items_[0]->columns_.size();

   return 0;
}

/**
 * Devuelve la cantidad de filas.
 *
 * @return Entero con la cantidad de filas.
 */
int RasterDnDriver::GetRows() const {
   return items_.size();
}

/**
 * Agrega una fila al final de la tabla.
 *
 * OPERACION NO SOPORTADA
 */
bool RasterDnDriver::AppendRow() {
   return false;
}

/**
 * Agrega una fila en la posicion especificada de la tabla.
 *
 * OPERACION NO SOPORTADA
 */
bool RasterDnDriver::InsertRow(int Row) {
   return false;
}

/**
 * Elimina una fila de la tabla (debe existir, sino devuelve false).
 *
 * OPERACION NO SOPORTADA
 */
bool RasterDnDriver::DeleteRow(int Row) {
   return false;
}

/**
 * Agrega una columna al final de la tabla.
 *
 * OPERACION NO SOPORTADA
 */
bool RasterDnDriver::AppendColumn(const std::string& ColumnType, const std::string& ColumnName,
                  int ColumnFlag) {
   return false;
}

/**
 * Agrega una columna en la posicion especificada de la tabla.
 *
 * OPERACION NO SOPORTADA
 */
bool RasterDnDriver::InsertColumn(int Column, const std::string& ColumnType,
                  const std::string& ColumnName, int ColumnFlag) {
   return false;
}

/**
 * Elimina una columna de la tabla (debe existir, sino devuelve false).
 *
 * OPERACION NO SOPORTADA
 */
bool RasterDnDriver::DeleteColumn(int Column) {
   return false;
}

/**
 * Obtiene el nombre de la columna.
 *
 * @param[in] Column Columna de la que se desea obtener el nombre.
 * @return Nombre de la columna.
 */
std::string RasterDnDriver::GetColumnName(int Column) const {
   if (Column == 0)
      return kColumnName;
   return "";
}

/**
 * Obtiene el tipo de la columna.
 *
 * @param[in] Column Columna de la que se desea obtener el tipo.
 * @return Nombre de la columna.
 */
std::string RasterDnDriver::GetColumnType(int Column) const {
   if (Column == 0)
      return DataInfo<unsigned char>::Name;
   return DataInfo<void>::Name;
}

/**
 * Elimina todas las filas y su contenido de la tabla (no elimina columnas)
 *
 * OPERACION NO SOPORTADA
 */
bool RasterDnDriver::Clear() {
   return false;
}

/**
 * Getea la lista de permisos disponibles.
 *
 * @return Lista con los permisos habilitados.
 */
const PermissionList* RasterDnDriver::GetCapabilities() const {
   return pPermissionList_;
}

/**
 * Getea el permiso utilizado en GetCapabilities.
 */
int RasterDnDriver::GetPermission() const {
   return kPermissionKey;
}

/**
 * Getea el Id del Datasource asociado.
 *
 * OPERACION NO SOPORTADA
 */
SuriObject::UuidType RasterDnDriver::GetAssociatedDatasourceId() const {
   return 0;
}

/**
 * Determina si se soporta el tipo.
 *
 * @param[in] ColumnType Tipo de dato a verificar.
 * @return true si el tipo de dato esta soportado.
 */
bool RasterDnDriver::IsSupportedType(const std::string& ColumnType) {
   return ColumnType.compare(DataInfo<unsigned char>::Name) == 0;
}

/**
 * Determina si una celda es valida.
 *
 * @param[in] Column Columna a verificar.
 * @param[in] Row Fila a verificar.
 */
bool RasterDnDriver::IsValidCell(int Column, int Row) const {
   if (Row < static_cast<int>(items_.size())) {
      if (Column < static_cast<int>(items_[Row]->columns_.size())) {
         return true;
      }
   }
   return false;
}

/**
 * Determina si la columna es entera.
 *
 * OPERACION NO SOPORTADA
 */
bool RasterDnDriver::IsIntColumn(int Column) const {
   return false;
}

/**
 * Determina si la columna es de cadena de caracteres.
 *
 * @param[in] Column Columna a verficar.
 */
bool RasterDnDriver::IsVarcharColumn(int Column) const {
   return (Column == 0);
}

/**
 * Determina si la columna es de punto flotante.
 *
 * OPERACION NO SOPORTADA
 */
bool RasterDnDriver::IsFloatColumn(int Column) const {
   return false;
}

/** Determina si la columna es de punto flotante (double) */
bool RasterDnDriver::IsDoubleColumn(int Column) const {
   return false;
}

/**
 * Obtiene un nuevo id.
 *
 * OPERACION NO SOPORTADA
 */
long RasterDnDriver::GetNewRowId() {
   return -1;
}

/**
 * Carga la estructura interna que contiene a los items para
 * mostrar en la tabla.
 */
void RasterDnDriver::LoadItems() {
   std::vector<std::string> dns = pDnInfo_->GetBandsDnInfo();

   for (int ix = 0, lenix = dns.size(); ix < lenix; ++ix) {
      PDRIVER_COLUMN pcolumn = new DRIVER_COLUMN;
      pcolumn->value_ = dns[ix];

      PDRIVER_ITEM pitem = new DRIVER_ITEM;
      pitem->columns_.push_back(pcolumn);

      items_.push_back(pitem);
   }
}

/**
 * Carga la estructura interna que contiene a los permisos.
 */
void RasterDnDriver::LoadPermissions() {
   pPermissionList_ = new PermissionList();
   pPermissionList_->InsertOperation(kPermissionKey, PermissionList::APPCOL);
   pPermissionList_->InsertOperation(kPermissionKey, PermissionList::APPROW);
   pPermissionList_->InsertOperation(kPermissionKey, PermissionList::GETVAL);
}

} // namespace suri
