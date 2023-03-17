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

#ifndef DEFAULTTABLE_H_
#define DEFAULTTABLE_H_

// Includes standard
#include <map>
#include <set>
#include <vector>
#include <string>

// Includes suri
#include "suri/Table.h"
#include "suri/DriverInterface.h"
#include "wxGenericTableBase.h"

// Defines

// Forwards

namespace suri {

/**
 * Define la implementacion por defecto de TableInterface que provee SuriLib
 *
 * Por defecto se crea con un MemoryDriver, pero se puede cambiar con SetDriver.
 * La tabla se hace cargo de la memoria del driver, por lo que se le entrega un puntero con new
 * y la tabla misma se encarga de liberar la memoria cuando corresponda.
 *
 * Posee una dependencia ciclica con wxGenericTableBase para hacer las actualizaciones que
 * correspondan
 *
 * Se tienen distintos valores de Permisos para ser utilizados:
 *
 * DefaultTable::PermissionReadOnly
 * DefaultTable::PermissionFixedSize
 * DefaultTable::PermissionAddRows
 * DefaultTable::PermissionAddColumns
 * DefaultTable::PermissionHideColumn
 * DefaultTable::PermissionReadOnlyColumn
 *
 * Por defecto se utiliza DefaultTable::PermissionReadOnlyColumn
 *
 * Las operaciones permitidas por los permisos se consultan antes de la ejecucion de un metodo.
 * Tambien es necesario preguntar al driver por los permisos porque si no se permite en el driver
 * una operacion, tampoco sera permitida en la tabla.
 *
 * Las transacciones de Commit y Rollback se ejecutan solo en el caso de que el driver lo soporte
 *
 * Por el momento se soportan los siguientes tipos: INT, COLOR_RGBA_HEXA, STRING; FLOAT
 *
 */
class DefaultTable : public Table {
public:
   /** Constructor */
   DefaultTable();
   /** Destructor */
   virtual ~DefaultTable();
   /** Obtiene el contenido de la celda y lo interpreta el tipo asociado a la columna de celda */
   virtual bool GetCellValue(int Column, int Row, std::string& CellValue) const;
   /** Modifica el contenido de la celda y lo interpreta el tipo asociado a la columna de celda */
   virtual bool SetCellValue(int Column, int Row, std::string& CellValue);
   /** Verifica si un tipo es soportado por esta implementacion */
   virtual bool IsSupportedType(ColumnType ColumnType) const;
#ifdef __SHOW_TABLE_LOG___
   /** Getter de un puntero const del Driver */
   virtual const DriverInterface* GetDriver() const;
   /** Obtiene las tabla en un string \note Metodo Debug */
   virtual std::string GetTable() const;
#endif
};

} /** namespace suri */

#endif /* DEFAULTTABLE_H_ */
