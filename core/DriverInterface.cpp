/*! \file DriverInterface.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/DriverInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

const int DriverInterface::NullInt = 0; /*! Valor para el nulo de los long */
const std::string DriverInterface::NullVarchar = ""; /*! Valor para el nulo de los strings */
const float DriverInterface::NullFloat = 0.0; /*! Valor para el nulo de los double */

/**
 * Constructor
 */
DriverInterface::DriverInterface() {
}

/**
 * Destructor
 */
DriverInterface::~DriverInterface() {
}

/** Obtiene los atributos de la columna */
int DriverInterface::GetColumnFlags(int Column) const {
   return RegularFlag;
}

bool DriverInterface::Commit() {
   return true;
}
/** Descarta los cambios y no se efectua la transaccion */
bool DriverInterface::Rollback() {
   return false;
}

/** Verifica que se puede agrega una columna
 *  @return true en caso de poder agregar una columna
 *  @return false en caso contrario
 */
bool DriverInterface::CanAppendRow() {
   return true;
}

/** Devuelve el id asociado a la columna, -1 en caso contrario
 *  @param[in] Row fila a consultar
 *  @return id de la fila
 *  @return -1 en caso de que la fila no se encuentre en la tabla
 *  */
long DriverInterface::GetRowId(int Row) const {
   return IsValidCell(0, Row) ? Row : -1;
}

/** obtiene la fila asociada al id que se pasa por parametro
 *  @param[in] Id id de la fila a consultar
 *  @return id de la fila
 *  @return -1 en caso de que la fila no se encuentre en la tabla*/
int DriverInterface::GetRowById(long RowId) const {
   return IsValidCell(0, RowId) ? RowId : -1;
}

/** Obtiene los ids de las filas que contiene la tabla **/
std::vector<long> DriverInterface::GetRowIds() const {
   std::vector<long> rowids;
   for (int row = 0; row < GetRows(); ++row) {
      rowids.push_back(row);
   }
   return rowids;
}

/** Realiza un ordenamiento de la tabla por la columna indicada por
 * parametro utilizando el ordenamiento indicado */
bool DriverInterface::OrderByColumn(int ColumnIndex, ComparatorFunction* pOrderFunc) {
   return true;
}

/**
 * Realiza una reindexacion de las columnas indicadas por el mapa de asociacion
 * de indices en cuanto la reindexacion sea consistente
 * @param Map mapa con la indexacion solicitada
 * @return true en caso de realizar la reindexacion de forma correcta
 * @return false en caso contrario (ej. dos columnas al mismo indice)
 */
bool DriverInterface::ReIndexColumn(const IndexMap& Map) {
   return true;
}

/** Agrega una columna autocalculada con la formulada indicada por parametro
 * @param[in] ColumnName Nombre de la columna
 * @param[in] Equation ecuacion que se aplicara a la columna
 * @param[in] ColumnFlag flag que indica el tipo de columna
 */
bool DriverInterface::AppendCalculatedColumn(const std::string& ColumnName,
                                             const std::string& ColumnType,
                                             const std::string& Equation,
                                             int ColumnFlag) {
   return true;
}

/** Agrega una columna autocalculable en la posicion especificada de la tabla */
bool DriverInterface::InsertCalculatedColumn(int Column, const std::string& ColumnName,
                                             const std::string& ColumnType,
                                             const std::string& Equation,
                                             int ColumnFlag) {
   return false;
}

} /** namespace suri */
