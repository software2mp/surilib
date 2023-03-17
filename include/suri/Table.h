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

#ifndef Table_H_
#define Table_H_

// Includes standard
#include <map>
#include <string>
#include <set>

// Includes Suri
#include "PermissionList.h"
#include "DriverInterface.h"
#include "TransactionInterface.h"
#include "suri/Subject.h"
#include "suri/QueryInterpreterInterface.h"
#include "suri/Option.h"

namespace suri {

// forwards
class wxGenericTableBase;

/**
 * Definicion de clase base de modelo de tabla que son utilizadas por wxGrid
 * Las tablas utilizan un Driver para obtener el soporte a nivel mas bajo (memoria, disco, etc)
 * La interfaz de tabla es transparente ya que todos los valores de celdas son ingresados como
 * strings, pero almacenados segun sus correspondientes tipos
 * Las implementaciones de tablas pueden soportar un subgrupo de todos los tipos definidos en esta
 * interfaz
 * La interfaz de tabla admite el manejo de transacciones que depende de los drivers
 */
class Table : public TransactionInterface, public Subject {
public:
   /** ctor **/
   Table();
   /** Destructor */
   virtual ~Table();

   /** Enumerado con los tipos que pueden ser soportados */
   typedef enum {
      STRING, INT, DOUBLE, FLOAT, BOOL, DATE, COLOR_RGBA_HEXA, FILE, URL, FORMATTED_STRING,
      UNKNOWN, HOTLINK, CALCULATED, VSTYLE, QBUILDER
   } ColumnType;

   static const std::string NullString; /*! Valor para el nulo de los strings */
   static const int NullInt; /*! Valor para el nulo de los long */
   static const float NullFloat; /*! Valor para el nulo de los double */
   static const int DefaultBool; /*! Valor por defecto de los booleanos */
   static const long DefaultDate; /*! Valor por defecto de las fechas */
   static const std::string DefaultString;/*! Valor por defecto de los string */
   static const std::string DefaultColor; /*! Valor por defecto de los colores en hexadecimal */
   static const std::string DefaultFile; /*! Valor por defecto de los archivos */
   static const std::string DefaultUrl; /*! Valor por defecto de los url */
   /** Flags para propiedades de columna
    *  // TODO(Gabriel - TCK #2886): Ver como unificar este codigo con el de Driver.
    *  Por ahora esta duplicado */
   /** Propiedad columna virtual. Esta columna no se guarda en disco **/
   static const int  VirtualFlag = 0x01;
   /** Propiedad columna regular. Esta columna se guarda en disco **/
   static const int  RegularFlag = 0x02;
   static const std::string TokenSeparator; /*! Restricciones: Separa restricciones */
   static const std::string KeyValueSeparator; /*! Restricciones: Separa restric de su visualiz */

   /** Obtiene el contenido de la celda y lo interpreta segun el tipo asociado a la celda */
   virtual bool GetCellValue(int Column, int Row, std::string& CellValue) const=0;
   /** Modifica el contenido de la celda y lo interpreta segun el tipo asociado a la celda */
   virtual bool SetCellValue(int Column, int Row, std::string& CellValue)=0;
   /** Devuelve la cantidad de columnas en la tabla */
   virtual int GetColumns() const;
   /** Devuelve tipo de dato que se almacena/interpreta en la columna */
   virtual ColumnType GetColumnType(int Column) const;
   /** Devuelve el nombre de la columna */
   virtual std::string GetColumnName(int Column) const;
   /** Devuelve la posicion de la columna con el nombre indicado */
   virtual int GetColumnByName(const std::string &ColumnName) const;
   /** Devuelve la cantidad de filas en la tabla */
   virtual int GetRows() const;
   /** Agrega una fila al final de la tabla */
   virtual bool AppendRow();
   /** Inserta una fila en la tabla en la posicion indicada */
   virtual bool InsertRow(int Row);
   /** Elimina una fila de la tabla (debe existir, sino devuelve false) */
   virtual bool DeleteRow(int Row);
   /** Agrega una columna al final de la tabla */
   virtual bool AppendColumn(const std::string& ColumnName, ColumnType ColumnType,
                             int ColumnFlag = RegularFlag);
   /** Agrega una columna autocalculada con la formulada indicada por parametro*/
   virtual bool AppendCalculatedColumn(const std::string& ColumnName, ColumnType ColumnType,
                                       const std::string& Equation,
                                       int ColumnFlag = VirtualFlag);
   /** Inserta una columna en la tabla en la posicion indicada */
   virtual bool InsertColumn(int Column, const std::string& ColumnName,
                             ColumnType ColumnType, int ColumnFlag = VirtualFlag);
   /** Inserta una columna en la tabla en la posicion indicada */
   virtual bool InsertCalculatedColumn(int Column, const std::string& ColumnName,
                                       ColumnType ColumnType,
                                       const std::string& Equation,
                                       int ColumnFlag = VirtualFlag);
   /** Elimina una columna de la tabla (debe existir, sino devuelve false) */
   virtual bool DeleteColumn(int Column);
   /** Verifica si un tipo es soportado por una implementacion concreta de esta interfaz */
   virtual bool IsSupportedType(ColumnType ColumnType) const=0;
   /** Elimina todas las filas y su contenido de la tabla (no elimina columnas) */
   virtual bool Clear();
   /** Setter del Driver (Se apropia de la memoria) */
   virtual bool SetDriver(DriverInterface* pDriver);
   /** Cambia las restricciones de una columna */
   virtual bool ChangeRestrictions(int Column, const std::string& RestrictionString);
   /** Obtiene las restricciones de una columna */
   virtual std::map<std::string, std::string> GetRestrictions(int Column);
   /** Determina si una columna tiene restricciones */
   virtual bool HasRestrictions(int Column);
   /** Setea el nivel de permisos de la tabla */
   virtual bool SetPermission(int PermissionKey);
   /** Getea el nivel de permisos de la tabla */
   virtual int GetPermission() const;
   /** Getea la lista de permisos configurada para la tabla */
   virtual const PermissionList* GetPermissionList() const;
   /** Aplica los cambios y efectua la transaccion */
   virtual bool Commit();
   /** Descarta los cambios y no se efectua la transaccion */
   virtual bool Rollback();
#ifdef __SHOW_TABLE_LOG__
   /** Getter de un puntero const del Driver */
   virtual const DriverInterface* GetDriver() const=0;
   /** Obtiene las tabla en un string \note Metodo Debug */
   virtual std::string GetTable() const=0;
#endif
   /** Establece una columna como de solo lectura */
   virtual bool SetReadOnlyColumn(int Column, bool ReadOnly = true);
   /** Determina si una columna es de solo lectura */
   virtual bool IsReadOnlyColumn(int Column) const;
   /** Actualiza la vista de la tabla */
   virtual void UpdateGrid();
   /** Obtien el id asociado a una fila*/
   virtual long GetRowId(int Row) const;
   /** obtiene la fila asociada al id que se pasa por parametro*/
   virtual int GetRowById(long Id) const;
   /** Obtiene los ids de las filas que contiene la tabla **/
   virtual std::vector<long> GetRowIds() const;
   // TODO(Gabriel - TCK #2886): Ver como unificar este codigo con el de Driver.
   /** Definicion de puntero a funcion para comparar
    *  @param Value primer valor a comparar
    *  @param OtherValue segundo valor a comprar
    *  @return 0 si los valores son iguales
    *  @return menor que 0 caso OtherValue > Value
    *  @return mayor que 0 caso Value > OtherValue */

   typedef int (*ComparatorFunction)(const std::string& Value,
                                     const std::string& OtherValue);
   /** Realiza un ordenamiento de la tabla por la columna indicada por
    * parametro utilizando el ordenamiento indicado */
   virtual bool OrderByColumn(int ColumnIndex, ComparatorFunction* pOrderFunc) {
      return true;
   }

   // TODO(Ramiro - TCK #2988) : Repensar logica de notificaciones para la tabla
   /** Retorna subject que notifica de cambios en los datos de la tabla */
   Subject* GetDataChangeNotifier();
   /** Configura subject que notifica de cambios en los datos de la tabla */
   void SetDataChangeNotifier(Subject* pDataChangeSubject);
   /** Obtiene el id asociado al indice de columna pasado por parametro **/
   long GetColumnId(int ColIndex) const;
   /** Obtiene el indice asociado al id de columna pasado por parametro **/
   int GetColumnIndexBy(long ColId) const;
   /** Ejecuta la consulta del SearchWidget */
   bool ExecuteQuery(const std::string& Query);
   /** Retorna el estado de la tabla */
   bool GetTableReadOnlyState();
   /** Setea el estado de la tabla */
   void SetReadOnlyState(bool State);
   /** Obtiene el indice de la columna que corresponde a una variable de una ecuacion */
   int GetVariableColumnIndex(const std::string& Variable) const;
   /** Verifica la validez de ecuacion */
   bool IsValidEquation(const std::string& Equation) const;

   /*! Valor por defecto de permiso */
   static const int PermissionReadOnly;
   static const int PermissionFixedSize;
   static const int PermissionAddRows;
   static const int PermissionAddColumns;
   static const int PermissionHideColumn;
   static const int PermissionReadOnlyColumn;
   std::string url_;
   std::string tableName_;
   void SetOption(const Option& Options) {
         options_.SetOption(Options.GetOption());
   }

   Option GetOptions() const {
      return options_;
   }

protected:
   /** Metodo que verifica que la columna posee definido un tipo logico de dato*/
   virtual ColumnType GetMetaTypeColumn(int Column) const;
   /** Devuelve tipo de dato que se almacena/interpreta en la columna */
   virtual std::string GetColumnTypeAsString(ColumnType ColumnType) const;

   virtual void UpdateProvider();
   DriverInterface* pDriver_; /*! Puntero a la interfaz del driver */
   std::vector<ColumnType> columnTypes_; /*! Tipos de las columnas ordenadas segun posicion en la tabla */
   std::map<int, std::map<std::string, std::string> > restrictions_; /*! Mapa de restricciones*/
   // wxGenericTableBase* pWxGenericTableBase_; /*! Puntero a wxGenericTableBase */
   int permissionKey_; /*! Permiso */
   PermissionList* permissions_; /*! Lista de Permisos */
   std::set<int> readOnlyColumns_; /*! Columnas de Solo Lectura */
   std::set<int> hiddenColumns_; /*! Columnas ocultas */
   bool isReadOnlyTable; /** Estado de la tabla */

   // TODO(Ramiro - TCK #2988) : Repensar logica de notificaciones para la tabla
   /** subject que notifica de cambios en los datos de la tabla */
   Subject* pDataChangeSubject_;
   /** Puntero a QueryInterpreter/NullQueryInterpreter */
   QueryInterpreterInterface* pQueryInterpreter_;
   Option options_;

private:
   /** Metodo auxiliar que carga las operaciones asociadas a cada nivel de permiso*/
   void LoadPermissions();
};
}

#endif /* Table_H_ */
