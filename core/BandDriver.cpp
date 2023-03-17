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

#include "BandDriver.h"

#include "PermissionList.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DriverInterface.h"
#include "suri/RasterElement.h"
#include "suri/SuriObject.h"

namespace suri {

const std::string BandDriver::kDefaultBandName = "DEFAULT BAND NAME";

/**
 * Ctor.
 */
BandDriver::BandDriver(RasterElement* pRaster) :
      pRaster_(pRaster), pPermissionList_(NULL) {
   LoadColumnsInfo();
   LoadPermissions();
}

/**
 * Dtor.
 */
BandDriver::~BandDriver() {
   if (pPermissionList_) {
      delete pPermissionList_;
      pPermissionList_ = NULL;
   }
}

/** Leer un entero del driver */
bool BandDriver::ReadInt(int Column, int Row, int& Data) const {
   if (Row < pRaster_->GetBandCount()) {
      if (Column < static_cast<int>(columnNames_.size())) {
         if (Column == kColumnValidity) {
            pRaster_->GetBandValidity(Data, Row);
            return true;
         }
      }
   }

   return false;
}

/** Escribir un entero en el driver */
bool BandDriver::WriteInt(int Column, int Row, int Data) {
   if (Column == kColumnValidity) {
      pRaster_->SetBandValidity(Data, Row);
      return true;
   }
   return false;
}

/** Leer un string del driver */
bool BandDriver::ReadVarchar(int Column, int Row, std::string& Data) const {
   if (Row < pRaster_->GetBandCount()) {
      if (Column < static_cast<int>(columnNames_.size())) {
         if (Column == kColumnName) {
            pRaster_->GetBandName(Data, Row);
            return true;
         }
      }
   }

   return false;
}

/** Escribir un string en el driver */
bool BandDriver::WriteVarchar(int Column, int Row, const std::string& Data) {
   if (Column == kColumnName) {
      pRaster_->SetBandName(Data, Row);
      return true;
   }
   return false;
}

/** Leer un punto flotante del driver */
bool BandDriver::ReadFloat(int Column, int Row, float& Data) const {
   return false;
}

/** Escribir un punto flotante en el driver */
bool BandDriver::WriteFloat(int Column, int Row, float Data) {
   return false;
}

/** Devuelve la cantidad de columnas */
int BandDriver::GetColumns() const {
   return columnNames_.size();
}

/** Devuelve la cantidad de filas */
int BandDriver::GetRows() const {
   return pRaster_->GetBandCount();
}

/** Agrega una fila al final de la tabla */
bool BandDriver::AppendRow() {
   pRaster_->SetBandName(kDefaultBandName, pRaster_->GetBandCount());
   pRaster_->SetBandCountEx(pRaster_->GetBandCount() + 1);
   return true;
}

/** Agrega una fila en la posicion especificada de la tabla */
bool BandDriver::InsertRow(int Row) {
   return false;
}

/** Elimina una fila de la tabla (debe existir, sino devuelve false) */
bool BandDriver::DeleteRow(int Row) {
   pRaster_->DeleteBand(Row);
   pRaster_->SetBandCountEx(pRaster_->GetBandCount() - 1);
   return true;
}

/** Agrega una columna al final de la tabla */
bool BandDriver::AppendColumn(const std::string& ColumnType,
      const std::string& ColumnName,
      int ColumnFlag) {
   return false;
}

/** Agrega una columna en la posicion especificada de la tabla */
bool BandDriver::InsertColumn(int Column, const std::string& ColumnType,
      const std::string& ColumnName,
      int ColumnFlag) {
   return false;
}

/** Elimina una columna de la tabla (debe existir, sino devuelve false) */
bool BandDriver::DeleteColumn(int Column) {
   return false;
}

/** Obtiene el nombre de la columna */
std::string BandDriver::GetColumnName(int Column) const {
   return columnNames_[Column];
}

/** Obtiene el tipo de la columna */
std::string BandDriver::GetColumnType(int Column) const {
   return columnTypes_[Column];
}

/** Elimina todas las filas y su contenido de la tabla (no elimina columnas) */
bool BandDriver::Clear() {
   return false;
}

/** Getea la lista de permisos disponibles */
const PermissionList* BandDriver::GetCapabilities() const {
   return pPermissionList_;
}

/** Getea el permiso utilizado en GetCapabilities */
int BandDriver::GetPermission() const {
   return kPermissionKey;
}

/** Getea el Id del Datasource asociado */
SuriObject::UuidType BandDriver::GetAssociatedDatasourceId() const {
   return 0;
}

/** Determina si se soporta el tipo */
bool BandDriver::IsSupportedType(const std::string& ColumnType) {
   for (int ix = 0, lenix = columnTypes_.size(); ix < lenix; ++ix) {
      if (ColumnType.compare(columnTypes_[ix]) == 0) {
         return true;
      }
   }
   return false;
}

/** Determina si una celda es valida */
bool BandDriver::IsValidCell(int Column, int Row) const {
   if (Row < pRaster_->GetBandCount()) {
      if (Column < static_cast<int>(columnNames_.size())) {
         return true;
      }
   }
   return false;
}

/** Determina si la columna es entera */
bool BandDriver::IsIntColumn(int Column) const {
   return (Column == kColumnValidity);
}

/** Determina si la columna es de cadena de caracteres */
bool BandDriver::IsVarcharColumn(int Column) const {
   return (Column == kColumnName || Column == 1);
}

/** Determina si la columna es de punto flotante */
bool BandDriver::IsFloatColumn(int Column) const {
   return false;
}

/** Determina si la columna es de punto flotante (double) */
bool BandDriver::IsDoubleColumn(int Column) const {
   return false;
}

/** Obtiene un nuevo id */
long BandDriver::GetNewRowId() {
   return -1;
}

/**
 * Inicializa el driver con informacion de las columnas.
 */
void BandDriver::LoadColumnsInfo() {
   columnNames_.push_back("Nombre");
   columnTypes_.push_back(DataInfo<unsigned char>::Name);

   columnNames_.push_back("Validez");
   columnTypes_.push_back(DataInfo<int>::Name);
}

/**
 * Carga la estructura interna que contiene a los permisos.
 */
void BandDriver::LoadPermissions() {
   pPermissionList_ = new PermissionList();
   pPermissionList_->InsertOperation(kPermissionKey, PermissionList::APPCOL);
   pPermissionList_->InsertOperation(kPermissionKey, PermissionList::APPROW);
   pPermissionList_->InsertOperation(kPermissionKey, PermissionList::GETVAL);
   pPermissionList_->InsertOperation(kPermissionKey, PermissionList::SETVAL);
   pPermissionList_->InsertOperation(kPermissionKey, PermissionList::DELROW);
   pPermissionList_->InsertOperation(kPermissionKey, PermissionList::INSROW);
}

}  // namespace suri
