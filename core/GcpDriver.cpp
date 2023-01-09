/*! \file GcpDriver.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar

// Includes Suri
#include "suri/GcpDriver.h"
#include "NewRowNotification.h"
#include "suri/AuxiliaryFunctions.h"

// Defines

// forwards

namespace suri {

const std::string GcpDriver::XmodelColumnName =  "Modelo X";
const std::string GcpDriver::YmodelColumnName = "Modelo Y";
const std::string GcpDriver::XErrorColumnName =  "Error X";
const std::string GcpDriver::YErrorColumnName =  "Error Y";
const std::string GcpDriver::CuadraticErrorColumnName =  "Error cuadratico";

/** Constructor */
GcpDriver::GcpDriver() : rowAvailable_(true), currentValuesSetFlag_(0),
    includeEnableColumn_(false), lastGeneratedId_(-1), order_(1),
    enableIndex_(-1), pCoordinateTransform_(NULL) {}

/** destructor **/
GcpDriver::~GcpDriver() {
}

/** Leer un entero del driver */
bool GcpDriver::ReadInt(int Column, int Row, int& Data) const {
   return memDriver_.ReadInt(Column, Row, Data);
}

/** Escribir un entero en el driver */
bool GcpDriver::WriteInt(int Column, int Row, int Data) {
   if (!memDriver_.WriteInt(Column, Row, Data)) return false;
   UpdateAppendRowAvailability(Column, Row);
   UpdateGeometriesColumns(Column, Row, Data);
   return true;
}

/** Leer un string del driver */
bool GcpDriver::ReadVarchar(int Column, int Row, std::string& Data) const {
   return memDriver_.ReadVarchar(Column, Row, Data);
}

/** Escribir un string en el driver */
bool GcpDriver::WriteVarchar(int Column, int Row, const std::string& Data) {
   if (!memDriver_.WriteVarchar(Column, Row, Data))
      return false;
   UpdateAppendRowAvailability(Column, Row);
   UpdateCalculatedData(Row);
   UpdateGcpColumns(Column, Row, Data);
   return true;
}

/** Leer un punto flotante del driver */
bool GcpDriver::ReadFloat(int Column, int Row, float& Data) const {
   return memDriver_.ReadFloat(Column, Row, Data);
}

/** Escribir un punto flotante en el driver */
bool GcpDriver::WriteFloat(int Column, int Row, float Data) {
   if (!memDriver_.WriteFloat(Column, Row, Data))
      return false;
   UpdateAppendRowAvailability(Column, Row);
   UpdateGeometriesColumns(Column, Row, Data);
   return true;
}

/** Leer un double del driver*/
bool GcpDriver::ReadDouble(int Column, int Row, double& Data) const {
   return memDriver_.ReadDouble(Column, Row, Data);
}

/** Escribir un double en el driver*/
bool GcpDriver::WriteDouble(int Column, int Row, double Data) {
   if (!memDriver_.WriteDouble(Column, Row, Data))
      return false;
   UpdateAppendRowAvailability(Column, Row);
   UpdateGeometriesColumns(Column, Row, Data);
   return true;
}

/** Devuelve la cantidad de columnas */
int GcpDriver::GetColumns() const {
   return memDriver_.GetColumns();
}

/** Devuelve la cantidad de filas */
int GcpDriver::GetRows() const {
   return memDriver_.GetRows();
}

/** Agrega una fila al final de la tabla */
bool GcpDriver::AppendRow() {
   if (rowAvailable_ && memDriver_.AppendRow()) {
      if (lastGeneratedId_ < 0)
         lastGeneratedId_ = GetNewRowId();

      rowAvailable_ = false;
      currentValuesSetFlag_ = 0;
      int row = memDriver_.GetRows() - 1;
      UpdateCalculatedData(row);
      WriteInt(enableIndex_, row, 1);
      rowId_.insert(rowId_.begin() + row, lastGeneratedId_);
      lastGeneratedId_ = -1;

      // TODO(Ramiro - TCK #2988) : Se deberia pasar el id y no el nro de fila
      NewRowNotification newrownotif(memDriver_.GetRows() - 1);
      Notify(&newrownotif);

      return true;
   }
   return false;
}

/** Agrega una fila en la posicion especificada de la tabla */
bool GcpDriver::InsertRow(int Row) {
   if (rowAvailable_ && memDriver_.InsertRow(Row)) {
      if (lastGeneratedId_ < 0)
         lastGeneratedId_ = GetNewRowId();
      rowId_.insert(rowId_.begin() + Row, lastGeneratedId_);
      rowAvailable_ = false;
      currentValuesSetFlag_ = 0;
      UpdateCalculatedData(Row);
      lastGeneratedId_ = -1;
      return true;
   }
   return false;
}

/** Elimina una fila de la tabla (debe existir, sino devuelve false) */
bool GcpDriver::DeleteRow(int Row) {
   if (Row == GetRows() - 1)
      rowAvailable_ = true;
   return memDriver_.DeleteRow(Row);
}

/** Agrega una columna al final de la tabla
 * @param[in] ColumnType tipo de dato para la nueva columna
 * @param[in] ColumnName nombre de la columna
 * @param[in] ColumnFlag flag que indica el tipo de column
 * */
bool GcpDriver::AppendColumn(const std::string& ColumnType,
                             const std::string& ColumnName, int ColumnFlag) {
   if (ColumnFlag & this->RegularFlag) {
      regularColumns_.insert(memDriver_.GetColumns());
   }
   return memDriver_.AppendColumn(ColumnType, ColumnName, ColumnFlag);
}

/** Agrega una columna en la posicion especificada de la tabla */
bool GcpDriver::InsertColumn(int Column, const std::string& ColumnType,
                             const std::string& ColumnName, int ColumnFlag) {
   if (ColumnFlag & this->RegularFlag) {
      regularColumns_.insert(Column);
   }
   return memDriver_.InsertColumn(Column, ColumnType, ColumnName, ColumnFlag);
}

/** Elimina una columna de la tabla (debe existir, sino devuelve false) */
bool GcpDriver::DeleteColumn(int Column) {
   regularColumns_.erase(Column);
   autoColumns_.erase(Column);
   return memDriver_.DeleteColumn(Column);
}

/** Obtiene el nombre de la columna */
std::string GcpDriver::GetColumnName(int Column) const {
   return memDriver_.GetColumnName(Column);
}

/** Obtiene el tipo de la columna */
std::string GcpDriver::GetColumnType(int Column) const {
   return memDriver_.GetColumnType(Column);
}

/** Elimina todas las filas y su contenido de la tabla (no elimina columnas) */
bool GcpDriver::Clear() {
   return memDriver_.Clear();
}

/** Getea la lista de permisos disponibles */
const PermissionList* GcpDriver::GetCapabilities() const {
   return memDriver_.GetCapabilities();
}

/** Getea el permiso utilizado en GetCapabilities */
int GcpDriver::GetPermission() const {
   return memDriver_.GetPermission();
}

/** Getea el Id del Datasource asociado */
SuriObject::UuidType GcpDriver::GetAssociatedDatasourceId() const {
   return memDriver_.GetAssociatedDatasourceId();
}

/** Determina si se soporta el tipo */
bool GcpDriver::IsSupportedType(const std::string& ColumnType) {
   return memDriver_.IsSupportedType(ColumnType);
}

/** Determina si una celda es valida */
bool GcpDriver::IsValidCell(int Column, int Row) const {
   return memDriver_.IsValidCell(Column, Row);
}

/** Determina si la columna es entera */
bool GcpDriver::IsIntColumn(int Column) const {
   return memDriver_.IsIntColumn(Column);
}

/** Determina si la columna es de cadena de caracteres */
bool GcpDriver::IsVarcharColumn(int Column) const {
   return memDriver_.IsVarcharColumn(Column);
}

/** Determina si la columna es de punto flotante */
bool GcpDriver::IsFloatColumn(int Column) const {
   return memDriver_.IsFloatColumn(Column);
}

/** Determina si la columna es de punto flotante (double) */
bool GcpDriver::IsDoubleColumn(int Column) const {
   return memDriver_.IsDoubleColumn(Column);
}

/** Carga los valores a partir del filename configurado*/
void GcpDriver::LoadValues() {
   if (gcpFileName_.empty())
      return;
   gcpList_.Clear();
   loader_.Load(gcpFileName_, gcpList_);
   UpdateModel(false);
   std::vector<GroundControlPointExtended> gcps = gcpList_.GetAsExtendedGcps();
   std::vector<GroundControlPointExtended>::iterator it = gcps.begin();
   for (int row = 0; it != gcps.end(); ++it, ++row) {
      AddGcpToTable(*it, row);
      int id = memDriver_.GetRowId(row);
      rowId_.push_back(id);
   }
}

/** Aplica los cambios y efectua la transaccion */
bool GcpDriver::Commit() {
   UpdateGcpList();
   if (!gcpFileName_.empty())
      return loader_.Save(gcpFileName_, gcpList_);
   return true;
}

/** Descarta los cambios y no se efectua la transaccion */
bool GcpDriver::Rollback() {
   LoadValues();
   return true;
}

/** Se implementa el metodo para actualizar el estado del observer
 *  @param[in] Notificacion emitida por el cambio en el widget
*/

void GcpDriver::Update(NotificationInterface* pNotification) {
   pNotification->ApplyNotification(this);
}

/** Agrega una columna autocalculable en la posicion especificada de la tabla
 *  * Condicion de ecuacion valida:
 *  - No contener entre sus variables alguna columna autocalculable
 *  - El formato de las variables debe ser del estilo : variable_indice (ej c_0)
 *  - El indice de la variable tiene que hacer referencia al indice de columna sobre
 *  el cual se tomara el valor de la variable
 * @param[in] Column indice de columna
 * @param[in] ColumnName Nombre de la columna
 * @param[in] ColumnType Tipo de dato de la columna
 * @param[in] Equation ecuacion que se utiliza para calcular el valor
 * @param[in] ColumnFlag flag que indica si la columna es virtual o fisica
 * @return true en caso de agregar la columna
 * @return false en caso contrario (ej. ecuacion invalida)
 */
bool GcpDriver::InsertCalculatedColumn(int Column, const std::string& ColumnType,
                                       const std::string& ColumnName,
                                       const std::string& Equation, int ColumnFlag) {
   return false;
}

/**
 * Agrega una columna autocalculada con la formulada indicada por parametro
 * Condicion de ecuacion valida:
 *  - No contener entre sus variables alguna columna autocalculable
 *  - El formato de las variables debe ser del estilo : variable_indice (ej c_0)
 *  - El indice de la variable tiene que hacer referencia al indice de columna sobre
 *  el cual se tomara el valor de la variable
 * @param[in] ColumnName Nombre de la columna
 * @param[in] ColumnType Tipo de dato de la columna
 * @param[in] Equation ecuacion que se utiliza para calcular el valor
 * @param[in] ColumnFlag flag que indica si la columna es virtual o fisica
 * @return true en caso de agregar la columna
 * @return false en caso contrario (ej. ecuacion invalida)
 */
bool GcpDriver::AppendCalculatedColumn(const std::string& ColumnType,
                                       const std::string& ColumnName,
                                       const std::string& Equation, int ColumnFlag) {
   if (!IsValidEquation(Equation))
      return false;
   int column = memDriver_.GetColumns();
   if (ColumnFlag & this->RegularFlag) {
      regularColumns_.insert(column);
   }
   autoColumns_.insert(std::make_pair(column, Equation));
   return memDriver_.AppendColumn(ColumnType, ColumnName, ColumnFlag);
}

/**
 * Actualiza los valores de las celdas de las columnas autocalculables.
 */
void GcpDriver::UpdateCalculatedColumnsValue() {
   for (int i = 0; i < GetRows(); ++i) {
      UpdateCalculatedData(i);
   }
}

/**
 * Verifica la validez de ecuacion
 * @param[in] Equation
 * @return true en caso una ecuacion valida
 * @return false en caso de ecuacion invalida
 */
bool GcpDriver::IsValidEquation(const std::string& Equation) const {
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
   return true;
}

/**
 * Actualiza el orden del modelo de transformacion para el calculo de error
 * @param Order orden del polinomio que se quiere generar
 */
void GcpDriver::SetOrderModel(int Order) {
   if (Order != order_) {
      order_ = Order;
      UpdateModel();
   }
}

/** Obtiene un nuevo id */
long GcpDriver::GetNewRowId() {
   lastGeneratedId_ = SuriObject::CreateIntId(SuriObject::CreateId());
   return lastGeneratedId_;
}

/**
 * Devuelve la representacion de la coordenada en un wkt valido
 * @param[in] Coord coordenada a obtener el wkt
 * @return wkt asociado
 */
std::string GcpDriver::GetWktRepresentation(const Coordinates& pCoord) const {
   std::string wktcoord = "POINT(";
   wktcoord.append(NumberToString<double>(pCoord.x_));
   wktcoord.append(" ");
   wktcoord.append(NumberToString<double>(pCoord.y_));
   wktcoord.append(")");
   return wktcoord;
}

/**
 * Metodo que devuelve el polinomio de aproximacion.
 * @return retorna el polinomio de GcpDriver
 * @return retorna NULL en caso de que GcpDriver este asociado al metodo Exacto
 */
double GcpDriver::GetRmse() {
   double rmse = std::numeric_limits<double>::min();
   PolynomialCoordinatesTransformation *ptransform =
         dynamic_cast<PolynomialCoordinatesTransformation*>(pCoordinateTransform_);
   if (ptransform) {
      rmse = ptransform->GetRmse();
   }
   return rmse;
}

/**
 * Inicializa el objeto.
 *
 * @param[in] LoadFile Indica si la inicializacion debe cargar desde un archivo.
 */
void GcpDriver::Init(bool LoadFile) {
   InitializeColumns();

   // Saco los permisos que no necesito
   RemovePermissions();

   if (LoadFile)
      LoadValues();
}

/** Indica el modelo de transformacion a utilizar
 * @param pCoordinateTransform modelo de transformacion que se utiliza para el calculo
 * de error de los puntos de control
 */
void GcpDriver::SetTransformationModel(
      CoordinatesTransformation* pCoordinateTransform) {
   if (pCoordinateTransform_)
      delete pCoordinateTransform_;
   pCoordinateTransform_ = pCoordinateTransform;
}

/** Devuelve el id asociado a la fila, -1 en caso contrario
 *  @param[in] Row fila a consultar
 *  @return id de la fila
 *  @return -1 en caso de que la fila no se encuentre en la tabla
 *  */
long GcpDriver::GetRowId(int Row) const {
   return rowId_.at(Row);
}

/** Obtiene los ids de las filas que contiene la tabla **/
std::vector<long> GcpDriver::GetRowIds() const {
   std::vector<long> rowids;
   RowIdAssociation::const_iterator it = rowId_.begin();
   for (; it != rowId_.end(); ++it) {
      rowids.push_back(*it);
   }
   return rowids;
}

/** obtiene la fila asociada al id que se pasa por parametro
 *  @param[in] Id id de la fila a consultar
 *  @return id de la fila
 *  @return -1 en caso de que la fila no se encuentre en la tabla*/
int GcpDriver::GetRowById(long Id) const {
   int row = -1;
   RowIdAssociation::const_iterator it = rowId_.begin();
   int i = 0;
   for (; it != rowId_.end() && (*it) != Id; ++it) {
      i++;
   }
   if (it != rowId_.end())
      row = i;
   else
      row = memDriver_.GetRowById(Id);
   return row;
}

/**
 * Sacar los permisos para operaciones no soportadas.
 */
void GcpDriver::RemovePermissions() {
   PermissionList* ppermissions =
         const_cast<PermissionList*>(memDriver_.GetCapabilities());
   int permissionkey = memDriver_.GetPermission();

   ppermissions->EraseOperation(permissionkey, PermissionList::APPCOL);
   ppermissions->EraseOperation(permissionkey, PermissionList::INSCOL);
   ppermissions->EraseOperation(permissionkey, PermissionList::DELCOL);
   ppermissions->EraseOperation(permissionkey, PermissionList::INSROW);
}

/**
 * Ejecuta tareas extra de inicializacion.
 * Se usa para terminar algunas tareas despues de haber attachado un observer.
 */
void GcpDriver::LazyInit() {

}

void GcpDriver::Update() {
   UpdateGcpList();
   UpdateModel();
}


} /** namespace suri */
