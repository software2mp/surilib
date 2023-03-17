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
