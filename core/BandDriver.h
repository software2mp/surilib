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

#ifndef BANDDRIVER_H_
#define BANDDRIVER_H_

#include <string>

#include "PermissionList.h"
#include "suri/DriverInterface.h"
#include "suri/RasterElement.h"
#include "suri/SuriObject.h"

namespace suri {

/**
 * Clase que implementa la interfaz para drivers para usar en la
 * tabla de bandas de propiedades de elementos raster.
 */
class BandDriver : public suri::DriverInterface {
public:
   static const int kPermissionKey = 0;
   static const int kColumnName = 0;
   static const int kColumnValidity = 1;
   static const std::string kDefaultBandName;

   /**
    * Ctor.
    */
   explicit BandDriver(RasterElement* pRaster);

   /**
    * Dtor.
    */
   virtual ~BandDriver();
   /** Leer un double del driver */
   virtual bool ReadDouble(int Column, int Row, double& Data) const{return false;}
   /** Escribir un double en el driver */
   virtual bool WriteDouble(int Column, int Row, double Data){return false;}
   /** Leer un entero del driver */
   virtual bool ReadInt(int Column, int Row, int& Data) const;

   /** Escribir un entero en el driver */
   virtual bool WriteInt(int Column, int Row, int Data);

   /** Leer un string del driver */
   virtual bool ReadVarchar(int Column, int Row, std::string& Data) const;

   /** Escribir un string en el driver */
   virtual bool WriteVarchar(int Column, int Row, const std::string& Data);

   /** Leer un punto flotante del driver */
   virtual bool ReadFloat(int Column, int Row, float& Data) const;

   /** Escribir un punto flotante en el driver */
   virtual bool WriteFloat(int Column, int Row, float Data);

   /** Devuelve la cantidad de columnas */
   virtual int GetColumns() const;

   /** Devuelve la cantidad de filas */
   virtual int GetRows() const;

   /** Agrega una fila al final de la tabla */
   virtual bool AppendRow();

   /** Agrega una fila en la posicion especificada de la tabla */
   virtual bool InsertRow(int Row);

   /** Elimina una fila de la tabla (debe existir, sino devuelve false) */
   virtual bool DeleteRow(int Row);

   /** Agrega una columna al final de la tabla */
   virtual bool AppendColumn(const std::string& ColumnType,
                             const std::string& ColumnName,
                             int ColumnFlag = RegularFlag);

   /** Agrega una columna en la posicion especificada de la tabla */
   virtual bool InsertColumn(int Column, const std::string& ColumnType,
                             const std::string& ColumnName,
                             int ColumnFlag = RegularFlag);

   /** Elimina una columna de la tabla (debe existir, sino devuelve false) */
   virtual bool DeleteColumn(int Column);

   /** Obtiene el nombre de la columna */
   virtual std::string GetColumnName(int Column) const;

   /** Obtiene el tipo de la columna */
   virtual std::string GetColumnType(int Column) const;

   /** Elimina todas las filas y su contenido de la tabla (no elimina columnas) */
   virtual bool Clear();

   /** Getea la lista de permisos disponibles */
   virtual const PermissionList* GetCapabilities() const;

   /** Getea el permiso utilizado en GetCapabilities */
   virtual int GetPermission() const;

   /** Getea el Id del Datasource asociado */
   virtual SuriObject::UuidType GetAssociatedDatasourceId() const;

   /** Determina si se soporta el tipo */
   virtual bool IsSupportedType(const std::string& ColumnType);

   /** Determina si una celda es valida */
   virtual bool IsValidCell(int Column, int Row) const;

   /** Determina si la columna es entera */
   virtual bool IsIntColumn(int Column) const;

   /** Determina si la columna es de cadena de caracteres */
   virtual bool IsVarcharColumn(int Column) const;

   /** Determina si la columna es de punto flotante */
   virtual bool IsFloatColumn(int Column) const;

   /** Determina si la columna es de punto flotante (double) */
   virtual bool IsDoubleColumn(int Column) const;

   /** Obtiene un nuevo id */
   virtual long GetNewRowId();

private:
   RasterElement* pRaster_;  // Elemento raster para leer y guardar la info de bandas.
   PermissionList* pPermissionList_;  // lista de permisos sobre la tabla.
   std::vector<std::string> columnNames_;
   std::vector<std::string> columnTypes_;

   /**
    * Inicializa el driver con informacion de las columnas.
    */
   void LoadColumnsInfo();

   /**
    * Carga la estructura interna que contiene a los permisos.
    */
   void LoadPermissions();
};

}  // namespace suri

#endif  // BANDDRIVER_H_
