/*! \file BufferedDriver.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef BUFFEREDDRIVER_H_
#define BUFFEREDDRIVER_H_

// Includes Estandar
#include <deque>
#include <vector>
#include <string>
#include <limits>

// Includes Suri
#include "TableOperation.h"
#include "DriverInterface.h"
#include "suri/Subject.h"

#define UNDEFINEDCOLUMN    std::numeric_limits<int>::min()
#define UNDEFINEDROW       std::numeric_limits<int>::min()

// typedefs
typedef struct _column_t {
   int Id;
   std::string ColumnType;
   std::string ColumnName;
   int ColumnFlag;

   bool operator==(const struct _column_t& col) const {
      return Id == col.Id;
   }
   int GetId() const {
      return Id;
   }
} COLUMN;

namespace suri {

/**
 * Es un DriverInterface que implementa Transaction Interface para un Driver concreto
 *
 * Esta clase agrega las funcionalidades de Commit y Rollback a cualquier Driver.
 * Este agregado de funcionalidad se efectua mediante un Decorador.
 *
 * El uso es el siguiente:
 *    DriverInterface buffmemdriver = new BufferedDriver(new MemoryDriver())
 *    DriverInterface buffvecdriver = new BufferedDriver(new VectorEditorDriver())
 *
 * Todas las operaciones de lectura, escritura, etc son delegadas al driver base salvo aquellas
 * modificaciones no commiteadas que son almacenadas por los objetos de esta clase
 */
class BufferedDriver : public suri::DriverInterface, public Subject {
public:
   /** Constructor */
   explicit BufferedDriver(DriverInterface* pDriverInterface);
   /** Destructor */
   virtual ~BufferedDriver();
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
   /** Leer un double del driver */
   virtual bool ReadDouble(int Column, int Row, double& Data) const;
   /** Escribir un double en el driver */
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
   /** Verifica que se puede agrega una columna **/
   virtual bool CanAppendRow();
   /** Aplica los cambios y efectua la transaccion */
   virtual bool Commit();
   /** Descarta los cambios y no se efectua la transaccion */
   virtual bool Rollback();
   /** Devuelve el id asociado a la columna, -1 en caso contrario **/
   virtual long GetRowId(int Row) const;
   /** obtiene la fila asociada al id que se pasa por parametro*/
   virtual int GetRowById(long RowId) const;
#ifdef __SHOW_TABLE_LOG__
   /** Getter del string del subdriver */
   std::string GetSubDriverString() const;
   /** Obtiene las operaciones en un string \note Metodo Debug */
   std::string GetOperations() const;
   /** Obtiene los indices de columnas en un string \note Metodo Debug */
   std::string GetColumnIndex() const;
   /** Obtiene los indices de filas en un string \note Metodo Debug */
   std::string GetRowIndex() const;
#endif
   static const int PermissionReadOnly; /*! Valor por defecto de permiso */
   static const int PermissionFixedSize; /*! Valor por defecto de permiso */
   static const int PermissionAllAccess; /*! Valor por defecto de permiso */
   /** Obtiene un nuevo id para una fila */
   virtual long GetNewRowId();
   /** Obtiene un nuevo id para una columna */
   virtual long GetNewColumnId();
   /** Obtiene los ids de las filas que contiene la tabla **/
   virtual std::vector<long> GetRowIds() const;
   /** Obtiene el id asociado al indice de columna pasado por parametro
    *  @return id de la columna
    *  @return int max en caso de que el indice sea invalido **/
   virtual long GetColumnId(int ColIndex) const;
   /** Obtiene el indice asociado al id de columna pasado por parametro
    *  @return indice de columna asociado al id
    *  @return -1 en caso contrario**/
   virtual int GetColumnIndexBy(long ColId) const;

private:
   static const std::string DefaultData; /*! Valor por defecto de los numeros */
   static const std::string DefaultDataStr; /*! Valor por defecto de los strings */

   DriverInterface* pDriver_; /*! Puntero al driver a bufferear */
   std::deque<TableOperation> operations_; /*! Cola de operaciones */
   std::vector<COLUMN*> columns_; /** Vector de columnas */
   int newColumnIndexSeed_; /** base para la generacion de IDs para columnas */
   std::vector<BaseParameter*> paramsGC_; /** guarda los punteros a los parametros creados */
   std::vector<long> rowIndexHolder_;
   long newRowIndexSeed_;

   /** Busca el valor entero en la cola */
   bool GetInt(int Column, int Row, int& Data) const;
   /** Busca el valor varchar en la cola */
   bool GetVarchar(int Column, int Row, std::string& Data) const;
   /** Busca el valor flotante en la cola */
   bool GetFloat(int Column, int Row, float& Data) const;
   /** Busca el valor double en la cola */
   bool GetDouble(int Column, int Row, double& Data) const;
   /** Cuenta el valor neto de agregado y quitado de columnas en la cola de operaciones */
   int CountColumns() const;
   /** Cuenta el valor neto de agregado y quitado de filas en la cola de operaciones */
   int CountRows() const;
   /** Inicializa los indices de columnas con los del driver */
   void InitializeColumns();
   /** Inicializa los indices de filas con los del driver */
   void InitializeRows();
};

} /** namespace suri */

#endif /* BUFFEREDDRIVER_H_ */
