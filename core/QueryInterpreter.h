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

#ifndef QUERYINTERPRETER_H_
#define QUERYINTERPRETER_H_

// Includes Estandar
// Includes Suri
#include "suri/QueryInterpreterInterface.h"
#include "FilteredVector.h"
// Includes Wx
// Defines
// forwards

namespace suri {

class QueryInterpreter : public QueryInterpreterInterface {
public:
   /** Constructor */
   QueryInterpreter(const std::string& TableUrl, const std::string& Query);
   /** Destructor */
   virtual ~QueryInterpreter();
   /** Retorna la fila del interpreter, es decir, de la tabla filtrada */
   virtual int GetRow(int RowId);
   /** Retorna la cantidad total de filas en la tabla filtrada */
   virtual int GetRows();
   /** Retorna la columna del interpreter, es decir, de la tabla filtrada */
   virtual int GetColumn(int ColId);
   /** Retorna la cantidad total de columnas en la tabla filtrada */
   virtual int GetColumns();

   bool CanExecute();
private:
   /** Arma un mapa con numero de fila de fila y feature id */
   void LoadRowsMap();
   /** vector con el contenido filtrado */
   FilteredVector* pFilteredVector_;
   /** string con Url de la tabla */
   std::string tableUrl_;
   /** string con la consulta */
   std::string query_;
   bool canExecute_;
};

} /** namespace suri */

#endif /* QUERYINTERPRETER_H_ */
