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
// Includes Suri
#include "QueryInterpreter.h"
#include <ogrsf_frmts.h>
#include <ogr_feature.h>
// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
QueryInterpreter::QueryInterpreter(const std::string& TableUrl,
                                   const std::string& Query) :
      pFilteredVector_(NULL), tableUrl_(TableUrl), query_(Query), canExecute_(true) {
   pFilteredVector_ = new FilteredVector(Vector::Open(TableUrl), new Filter(Query));
   LoadRowsMap();
}

/** Destructor **/
QueryInterpreter::~QueryInterpreter() {
}

/**
 * Cuenta la cantidad de filas en la tabla filtrada
 * @return int con la cantidad de filas en la tabla filtrada
 */
int QueryInterpreter::GetRows() {
   int rows = -1;
   OGRLayer* player = pFilteredVector_->GetLayer(0);
   rows = player->GetFeatureCount();
   return rows;
}

/**
 * Busca una fila de la tabla sin filtrar en la tabla filtrada
 * @param[in] RowNumber numero de fila de la tabla sin filtrar
 * @return int correspondiente a la fila de la tabla filtrada
 */
int QueryInterpreter::GetRow(int RowNumber) {
   int row = -1;
   if(!rowsMap_.empty())
      row = rowsMap_.find(RowNumber)->second;
   return row;
}

/**
 * Cuenta la cantidad de columnas en la tabla filtrada
 * @return int con la cantidad de columnas en la tabla filtrada
 */
int QueryInterpreter::GetColumns() {
   int cols = -1;
   OGRLayer* player = pFilteredVector_->GetLayer(0);
   cols = player->GetNextFeature()->GetFieldCount();
   return cols;
}

/**
 * Busca una columna de la tabla sin filtrar en la tabla filtrada
 * @param[in] RowNumber numero de columna de la tabla sin filtrar
 * @return int correspondiente a la fila de la tabla filtrada
 */
int QueryInterpreter::GetColumn(int ColumnId) {
   int col = -1;
   return col;
}

/** Arma un mapa con numero de fila de fila y feature id */
void QueryInterpreter::LoadRowsMap() {
   OGRLayer* player = pFilteredVector_->GetLayer(0);
   if (!player) {
      canExecute_ = false;
      return;
   }
   player->ResetReading();
   OGRFeature* pfeature = player->GetNextFeature();
   for (int i = 0; pfeature != NULL; i++) {
      rowsMap_.insert(std::pair<int,int>(i, pfeature->GetFID()));
      OGRFeature::DestroyFeature(pfeature);
      pfeature = player->GetNextFeature();
   }
}

/** Retorna si es posible ejecutar la transaccion */
bool QueryInterpreter::CanExecute() {
   return canExecute_;
}
} /** namespace suri */
