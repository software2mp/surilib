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

#ifndef GCPDRIVER_H_
#define GCPDRIVER_H_

// Includes Estandar
#include <string>
#include <map>
#include <set>
#include <vector>

// Includes Suri
#include "DriverInterface.h"
#include "GcpLoaderInterface.h"
#include "MemoryDriver.h"
#include "EquationParser.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/Subject.h"
#include "ObserverInterface.h"
#include "suri/PolynomialCoordinatesTransformation.h"
#include "TextFileGcpLoader.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase base que representa un driver para puntos de control (Gcp).
 *
 */
class GcpDriver : public suri::DriverInterface,
                  public suri::Subject,
                  public suri::ObserverInterface {
   /** Constructor copia */
   GcpDriver(const GcpDriver&);

public:
   /** Definicion de columnas para los drivers gcps **/
   static const std::string XmodelColumnName;
   static const std::string YmodelColumnName;
   static const std::string XErrorColumnName;
   static const std::string YErrorColumnName;
   static const std::string CuadraticErrorColumnName;
   /** destructor **/
   virtual ~GcpDriver();
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
   /** Obtiene el nombre de la columna */
   virtual std::string GetColumnName(int Column) const;
   /** Obtiene el tipo de la columna */
   virtual std::string GetColumnType(int Column) const;
   /** Agrega una columna en la posicion especificada de la tabla */
   virtual bool InsertColumn(int Column, const std::string& ColumnType,
                             const std::string& ColumnName,
                             int ColumnFlag = RegularFlag);
   /** Elimina una columna de la tabla (debe existir, sino devuelve false) */
   virtual bool DeleteColumn(int Column);
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
   /** Elimina todas las filas y su contenido de la tabla (no elimina columnas) */
   virtual bool Clear();
   /** Getea la lista de permisos disponibles */
   virtual const PermissionList* GetCapabilities() const;
   /** Getea el permiso utilizado en GetCapabilities */
   virtual int GetPermission() const;
   /** Getea el Id del Datasource asociado */
   virtual SuriObject::UuidType GetAssociatedDatasourceId() const;
   /** Aplica los cambios y efectua la transaccion */
   virtual bool Commit();
   /** Descarta los cambios y no se efectua la transaccion */
   virtual bool Rollback();
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
   /** Actualiza el orden del modelo de transformacion para el calculo de error */
   virtual void SetOrderModel(int Order);
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
   /** Agrega una columna autocalculable en la posicion especificada de la tabla
    *  * Condicion de ecuacion valida:
    *  - No contener entre sus variables alguna columna autocalculable
    *  - El formato de las variables debe ser del estilo : variable_indice (ej c_0)
    *  - El indice de la variable tiene que hacer referencia al indice de columna sobre
    *  el cual se tomara el valor de la variable
    * */
   virtual bool InsertCalculatedColumn(int Column, const std::string& ColumnType,
                                       const std::string& ColumnName,
                                       const std::string& Equation, int ColumnFlag =
                                             VirtualFlag);
   /** Indica el modelo de transformacion a utilizar */
   virtual void SetTransformationModel(CoordinatesTransformation* pCoordinateTransform);
   /** Metodo Update de la interfaz ObserverInterface */
   virtual void Update(NotificationInterface* pNotification);
   /** Devuelve el id asociado a la columna, -1 en caso contrario **/
   virtual long GetRowId(int Row) const;
   /** obtiene la fila asociada al id que se pasa por parametro*/
   virtual int GetRowById(long RowId) const;
   /** Obtiene los ids de las filas que contiene la tabla **/
   virtual std::vector<long> GetRowIds() const;

   /** Metodo que devuelve el polinomio de aproximacion. */
   virtual double GetRmse();
   /** Obtiene un nuevo id */
   virtual long GetNewRowId();
   /** Inicializa el objeto */
   virtual void Init(bool LoadFile);
   /**
    * Ejecuta tareas extra de inicializacion.
    * Se usa para terminar algunas tareas despues de haber attachado un observer.
    */
   virtual void LazyInit();
   /**
    * Actualiza el modelo de gcps
    */
    virtual void Update();

protected:
   /** Constructor **/
   GcpDriver();
   
   MemoryDriver memDriver_;
   TextFileGcpLoader loader_;
   /** Lista de puntos de control **/
   GcpList gcpList_;
   bool rowAvailable_;
   /** Orden del polinomio utilizado para la transformacion */
   /** filename del cual cargar o guardar los puntos de control **/
   std::string gcpFileName_;
   /** flag que indica que valores fueron seteados de la fila en edicion */
   int currentValuesSetFlag_;
   /** boolean que indica si se debe incluir la columna de activacion de puntos **/
   bool includeEnableColumn_;
   /** ultimo id generado. Se utiliza para las nuevas columnas*/
   int lastGeneratedId_;
   /** bool que indica si se puede agregar una fila **/
   int order_;
   /** conjunto de columnas virtuales **/
   std::set<int> regularColumns_;
   /** definicion de mapa que asocia numero de fila a id generado*/
   typedef std::vector<int> RowIdAssociation;
   RowIdAssociation rowId_;
   /** indices de columnas **/
   int enableIndex_;
   /** typedef para la definicion del mapa de columnas autocalculables**/
   typedef std::map<int, std::string> AutoColumnsMap;
   /** conjunto de columnas autocalculables. Se guarda la asociacion columna ecuacion **/
   AutoColumnsMap autoColumns_;
   
   CoordinatesTransformation* pCoordinateTransform_;
   /**
    * Verifica si se configuraron valores en las columnas para permitir el
    * agregado de una nueva fila en funcion de la celda en el cual se configuro
    * el valor.
    */
   virtual void UpdateAppendRowAvailability(int Column, int Row)=0;

   /**
    * Actualiza las celdas de geometrias en funcion de la actualizacion de celdas
    * correspondientes a alguna coordenada del punto de control.
    */
   virtual void UpdateGeometriesColumns(int Column, int Row, float Data)=0;

   /**
    * Inicializa la tabla (columnas y atributos necesarios)
    */
   virtual void InitializeColumns()=0;

   /**
    * Metodo auxiliar que dado un punto de control se agrega en la tabla.
    */
   virtual void AddGcpToTable(const GroundControlPointExtended& pGcp, int Row)=0;
   
   /**
    * Devuelve la representacion de la coordenada en un wkt valido
    */
   virtual std::string GetWktRepresentation(const Coordinates& pCoord) const;

   /**
    * Actualiza los valores de las celdas de las columnas autocalculables.
    */
   virtual void UpdateCalculatedColumnsValue();
   /**
    * Metodo auxiliar que verifica que para la columna autocalculada
    *  se encuentren configurados los valores necesarios para realizar
    *  el calculo de su valor
    */
   virtual bool HasValidCalculationData(int ColumnIndex, int Row)=0;
   /** Verifica la validez de ecuacion */
   virtual bool IsValidEquation(const std::string& Equation) const;
   /** Carga los valores a partir del filename configurado*/
   virtual void LoadValues();

   /** Actualiza el modelo para la conversion de puntos */
   virtual void UpdateModel(bool UpdateValues = true)=0;

   /**  Metodo auxiliar que actualiza el valor de las celdas de las columnas autocalculables
    *  en funcion de los valores actualizados de las demas columnas*/
   virtual bool UpdateCalculatedData(int Row)=0;
   /** Lee la informacion que posee la celda en funcion del tipo de dato de la columna*/
   virtual bool ReadData(int Column, int Row, std::string& Data)=0;
   /** Obtiene el indice de la columna que corresponde a una variable de una ecuacion */
   virtual int GetVariableColumnIndex(const std::string& Variable) const=0;
   /** Actualiza la celda de la columna calculada en funcion del parser que se pasa por parametro*/
   virtual bool UpdateCalculatedCellData(int Column, int Row, EquationParser& Parser)=0;
   /** Convierte las filas de la tabla en una lista valida de puntos de control */
   virtual void UpdateGcpList()=0;
   /** Actualiza las celdas de puntos de control en funcion de la
    *  actualizacion de celdas correspondientes a la descripcion de la geometria del punto*/
   virtual void UpdateGcpColumns(int Column, int Row, const std::string& Data)=0;
   /** Obtiene la coordenada correspondiente al punto definido en formato wkt **/
   virtual void GetCoordinateFromWkt(const std::string& Data, Coordinates& Coordinate) const=0;
   /** Sacar los permisos para operaciones no soportadas. */
   void RemovePermissions();
};

} /** namespace suri */

#endif /* GCPDRIVER_H_ */
