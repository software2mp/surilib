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

#ifndef VECTOREDITIONTABLE_H_
#define VECTOREDITIONTABLE_H_


// Includes Estandar
#include <string>

// Includes Suri
#include "suri/Table.h"

// Includes Wx
// Defines
// forwards

namespace suri {

// forwards
class DatasourceInterface;

/*!
 *   Clase que extiende el comportamiento de BufferedTable
 *   para la incorporacion del concepto de registro fantasma.
 *
 */
class VectorEditionTable : public Table {
   /** ctor copia. **/
   VectorEditionTable(const VectorEditionTable&);

public:
   explicit VectorEditionTable(DatasourceInterface* pDatasource);
   virtual ~VectorEditionTable();
   /** Verifica si un tipo es soportado por una implementacion concreta de esta interfaz */
   virtual bool IsSupportedType(ColumnType ColumnType) const;

   /** Obtiene el contenido de la celda y lo interpreta segun el tipo asociado a la celda */
   virtual bool GetCellValue(int Column, int Row, std::string& CellValue) const;
   /** Modifica el contenido de la celda y lo interpreta segun el tipo asociado a la celda */
   virtual bool SetCellValue(int Column, int Row, std::string& CellValue);

private:
   /** Metodo que verifica que la columna posee definido un tipo logico de dato*/
   virtual ColumnType GetMetaTypeColumn(int Column) const;
   /** fuente de datos que se utiliza para la tabla **/
   DatasourceInterface* pDatasource_;
};
}  /** namespace suri */

#endif /* VECTOREDITIONTABLE_H_ */
