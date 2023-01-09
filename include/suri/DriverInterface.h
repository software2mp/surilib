/*! \file DriverInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DRIVERINTERFACE_H_
#define DRIVERINTERFACE_H_

// Includes Estandar
#include <string>
#include <list>
#include <limits>

// Includes Suri
#include "PermissionList.h"
#include "suri/DataTypes.h"
#include "suri/SuriObject.h"
#include "TransactionInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {


typedef struct _drivercolumn_t {
   std::string value_;
} DRIVER_COLUMN, *PDRIVER_COLUMN;

typedef struct _driveritem_t {
   std::vector<PDRIVER_COLUMN> columns_;
} DRIVER_ITEM, *PDRIVER_ITEM;

/**
 * Interfaz uniforme para escribir datos en un driver
 *
 * Los drivers son entidades concretas a traves de las cuales las TableInterface pueden escribir
 * datos en un dispositivo (memoria, en disco de forma vectorial, etc).
 *
 * Los drivers pueden o no tener un buffer, por lo que deberan utilizar a la clase decoradora:
 * BufferedDriver
 *
 * Los drivers solo soportan tipos basicos dispuestos en forma de matriz bidimensional.
 * Los tipos soportados son: enteros, flotantes y cadenas de caracteres.
 * Pueden existir limitaciones a los mismos para distintos tipos de drivers.
 *
 * Los Drivers poseen permisos que son chequeados contra el dispositivos o pueden configurarse
 * manualmente si asi se desea (cuya utilidad es de agregar mayor restriccion si asi se desea)
 */
class DriverInterface : TransactionInterface {
public:
   static const int NullInt; /*! Valor para el nulo de los int */
   static const std::string NullVarchar; /*! Valor para el nulo de los strings */
   static const float NullFloat; /*! Valor para el nulo de los punto flotante */
   /** Flags para propiedades de columna **/
   // TODO(Gabriel - TCK #2886): Ver como unificar este codigo con el de Tabel.
   /** Propiedad columna virtual. Esta columna no se guarda en disco **/
   static const int VirtualFlag = 0x01;
   /** Propiedad columna regular. Esta columna se guarda en disco **/
   static const int RegularFlag = 0x02;
   /** Constructor */
   DriverInterface();
   /** Destructor */
   virtual ~DriverInterface();
   /** Leer un entero del driver */
   virtual bool ReadInt(int Column, int Row, int& Data) const=0;
   /** Escribir un entero en el driver */
   virtual bool WriteInt(int Column, int Row, int Data)=0;
   /** Leer un string del driver */
   virtual bool ReadVarchar(int Column, int Row, std::string& Data) const=0;
   /** Escribir un string en el driver */
   virtual bool WriteVarchar(int Column, int Row, const std::string& Data)=0;
   /** Leer un punto flotante del driver */
   virtual bool ReadFloat(int Column, int Row, float& Data) const=0;
   /** Escribir un punto flotante en el driver */
   virtual bool WriteFloat(int Column, int Row, float Data)=0;
   /** Leer un double del driver */
   virtual bool ReadDouble(int Column, int Row, double& Data)const=0;
   /** Escribir un punto flotante en el driver */
   virtual bool WriteDouble(int Column, int Row, double Data)=0;
   /** Devuelve la cantidad de columnas */
   virtual int GetColumns() const=0;
   /** Devuelve la cantidad de filas */
   virtual int GetRows() const=0;
   /** Agrega una fila al final de la tabla */
   virtual bool AppendRow()=0;
   /** Agrega una fila en la posicion especificada de la tabla */
   virtual bool InsertRow(int Row)=0;
   /** Elimina una fila de la tabla (debe existir, sino devuelve false) */
   virtual bool DeleteRow(int Row)=0;
   /** Agrega una columna al final de la tabla */
   virtual bool AppendColumn(const std::string& ColumnType,
                             const std::string& ColumnName,
                             int ColumnFlag = RegularFlag)=0;
   /** Agrega una columna autocalculada con la formulada indicada por parametro
    *  * Condicion de ecuacion valida:
    *  - No contener entre sus variables alguna columna autocalculable
    *  - El formato de las variables debe ser del estilo : variable_indice (ej c_0)
    *  - El indice de la variable tiene que hacer referencia al indice de columna sobre
    *  el cual se tomara el valor de la variable
    * */
   virtual bool AppendCalculatedColumn(const std::string& ColumnType,
                                       const std::string& ColumnName,
                                       const std::string& Equation, int ColumnFlag =
                                             VirtualFlag);
   /** Agrega una columna en la posicion especificada de la tabla */
   virtual bool InsertColumn(int Column, const std::string& ColumnType,
                             const std::string& ColumnName,
                             int ColumnFlag = RegularFlag)=0;
   /** Agrega una columna autocalculable en la posicion especificada de la tabla
    *  * Condicion de ecuacion valida:
    *  - No contener entre sus variables alguna columna autocalculable
    *  - El formato de las variables debe ser del estilo : variable_indice (ej c_0)
    *  - El indice de la variable tiene que hacer referencia al indice de columna sobre
    *  el cual se tomara el valor de la variable
    * */
   virtual bool InsertCalculatedColumn(int Column, const std::string& ColumnName,
                                       const std::string& ColumnType,
                                       const std::string& Equation, int ColumnFlag =
                                             VirtualFlag);
   /** Elimina una columna de la tabla (debe existir, sino devuelve false) */
   virtual bool DeleteColumn(int Column)=0;
   /** Obtiene el nombre de la columna */
   virtual std::string GetColumnName(int Column) const=0;
   /** Obtiene el tipo de la columna */
   virtual std::string GetColumnType(int Column) const=0;
   /** Obtiene los atributos de la columna */
   virtual int GetColumnFlags(int Column) const;
   /** Elimina todas las filas y su contenido de la tabla (no elimina columnas) */
   virtual bool Clear()=0;
   /** Getea la lista de permisos disponibles */
   virtual const PermissionList* GetCapabilities() const=0;
   /** Getea el permiso utilizado en GetCapabilities */
   virtual int GetPermission() const=0;
   /** Getea el Id del Datasource asociado */
   virtual SuriObject::UuidType GetAssociatedDatasourceId() const=0;
   /** Determina si se soporta el tipo */
   virtual bool IsSupportedType(const std::string& ColumnType)=0;
   /** Determina si una celda es valida */
   virtual bool IsValidCell(int Column, int Row) const=0;
   /** Determina si la columna es entera */
   virtual bool IsIntColumn(int Column) const=0;
   /** Determina si la columna es de cadena de caracteres */
   virtual bool IsVarcharColumn(int Column) const=0;
   /** Determina si la columna es de punto flotante (float) */
   virtual bool IsFloatColumn(int Column) const=0;
   /** Determina si la columna es de punto flotante (double) */
   virtual bool IsDoubleColumn(int Column) const=0;
   /** Aplica los cambios y efectua la transaccion */
   virtual bool Commit();
   /** Descarta los cambios y no se efectua la transaccion */
   virtual bool Rollback();
   /** Verifica que se puede agrega una columna **/
   virtual bool CanAppendRow();
   /** Devuelve el id asociado a la columna, -1 en caso contrario **/
   virtual long GetRowId(int Row) const;
   /** obtiene la fila asociada al id que se pasa por parametro*/
   virtual int GetRowById(long RowId) const;
   /** Obtiene los ids de las filas que contiene la tabla **/
   virtual std::vector<long> GetRowIds() const;
   // TODO(Gabriel - TCK #2886): Ver como unificar este codigo con el de Table.
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
   virtual bool OrderByColumn(int ColumnIndex, ComparatorFunction* pOrderFunc);
   /** Definicion de mapa para la indexacion key = columna origen, value = columna destino
    *  ej. entrada (1,2) indica que la columna 1 pasa a ser la columna 2 **/
   typedef std::map<int, int> IndexMap;
   /**
    * Realiza una reindexacion de las columnas indicadas por el mapa de asociacion
    * de indices en cuanto la reindexacion sea consistente
    * @param Map mapa con la indexacion solicitada
    * @return true en caso de realizar la reindexacion de forma correcta
    * @return false en caso contrario (ej. dos columnas al mismo indice)
    */
   virtual bool ReIndexColumn(const IndexMap& Map);
   /** Obtiene un nuevo id */
   virtual long GetNewRowId()=0;
   /** Obtiene el id asociado al indice de columna pasado por parametro
    *  @return id de la columna
    *  @return int max en caso de que el indice sea invalido **/
   virtual long GetColumnId(int ColIndex) const {
      if (ColIndex > GetColumns() - 1)
         return std::numeric_limits<int>::max();
      return ColIndex;
   }
   /** Obtiene el indice asociado al id de columna pasado por parametro
    *  @return indice de la columna asociado
    *  @return std::numeric_limits<int>::max() en caso contrario **/
   virtual int GetColumnIndexBy(long ColId) const {
      if (ColId > GetColumns() - 1)
         return std::numeric_limits<int>::max();
      return ColId;
   }
   /**
    * Actualiza el modelo sobre el que opera el driver
    * TODO (matiaslafroce - TCK #10085): se implementa para GcpDriver
    * revisar si se puede utilizar a otro driver
    */
   virtual void Update() {};

protected:
   /**
    * Modela las columnas de una tabla en memoria para una granularidad mas fina de la complejidad
    */
   class Column {
   public:
      /** Constructor */
      Column(const std::string& ColumnType, const std::string& ColumnName,
             int DefaultRows = 0);
      /** Destructor */
      virtual ~Column();
      /** Leer un string del driver */
      virtual bool ReadVarchar(int Row, std::string& Data) const;
      /** Escribir un string en el driver */
      virtual bool WriteVarchar(int Row, const std::string& Data);
      /** Devuelve la cantidad de filas */
      virtual int GetRows() const;
      /** Agrega una fila al final de la tabla */
      virtual bool AppendRow();
      /** Agrega una fila en la posicion especificada de la tabla */
      virtual bool InsertRow(int Row);
      /** Elimina una fila de la tabla (debe existir, sino devuelve false) */
      virtual bool DeleteRow(int Row);
      /** Elimina todas las filas y su contenido */
      virtual bool Clear();
      /** Obtiene el nombre de la columna */
      virtual std::string GetColumnName() const;
      /** Obtiene el tipo de la columna */
      virtual std::string GetColumnType() const;

   private:
      std::string columnType_; /*! Tipos de la columna */
      std::string columnName_; /*! Nombre de la columna */
      std::list<std::string> rows_; /*! Filas de varchars */
   };
};
} /** namespace suri */

#endif /* DRIVERINTERFACE_H_ */
