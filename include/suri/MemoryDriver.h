/*! \file MemoryDriver.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MEMORYDRIVER_H_
#define MEMORYDRIVER_H_

// Includes Estandar
#include <list>
#include <string>
#include <iomanip>

// Includes Suri
#include "DriverInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Implementa la interfaz de driver para arrays en memoria
 * Almacena todos los datos en memoria y por defecto se tienen permisos totales para modificaciones
 */
class MemoryDriver : public suri::DriverInterface {
public:
   /** Constructor */
   MemoryDriver();
   /** Destructor */
   virtual ~MemoryDriver();
   /** Escribir un double en el driver */
   //virtual bool WriteDouble(int Column, int Row, double Data){return false;}
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
   /** Leer un double del driver*/
   virtual bool ReadDouble(int Column, int Row, double& Data) const;
   /** Escribir un double en el driver*/
   virtual bool WriteDouble(int Column, int Row, double Data);
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
                             const std::string& ColumnName, int ColumnFlag = RegularFlag);
   /** Elimina una columna de la tabla (debe existir, sino devuelve false) */
   virtual bool DeleteColumn(int Column);
   /** Obtiene el nombre de la columna */
   virtual std::string GetColumnName(int Column) const;
   /** Obtiene el tipo de la columna */
   virtual std::string GetColumnType(int Column) const;
   /** Elimina todas las filas y su contenido de la tabla (no elimina columnas) */
   virtual bool Clear();
   /** Getea la lista de permisos configurada para la tabla */
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
#ifdef __SHOW_TABLE_LOG__
   /** Devuelve la representacion del Driver en un string */
   virtual std::string ToString() const;
#endif
   /** Obtiene un nuevo id */
   virtual long GetNewRowId();

protected:
   std::list<MemoryDriver::Column*> columns_; /*! Lista de columnas */
   int permissionKey_; /*! Permiso */
   PermissionList* permissions_; /*! Lista de permisos */
   int maxRowId_; /*! */
};

} /** namespace suri */

#endif /* MEMORYDRIVER_H_ */
