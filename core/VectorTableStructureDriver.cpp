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
#include "suri/VectorTableStructureDriver.h"
#include "suri/DataTypes.h"

// Includes Wx
// Defines
#define FIELD_NAME_COLUMN "Campo"
#define FIELD_TYPE_COLUMN "Tipo"

// forwards

namespace suri {

/** numero de columna que corresponde al nombre del campo **/
const int VectorTableStructureDriver::FieldNameColumn = 0;
/** numero de columna que corresponde al tipo del campo **/
const int VectorTableStructureDriver::FieldTypeColumn = 1;

/** Constructor **/
VectorTableStructureDriver::VectorTableStructureDriver(
      DatasourceInterface* pDatasource) {
   pVectorDatasource_ = dynamic_cast<VectorDatasource*>(pDatasource);
   if (!pVectorDatasource_)
      return;
   id_ = pDatasource->GetId();
   InitializeTable();
}

/** Destructor **/
VectorTableStructureDriver::~VectorTableStructureDriver() {
}

/** Metodo auxiliar que inicializa la tabla a partir de la fuente de datos**/
void VectorTableStructureDriver::InitializeTable() {
   std::string file = pVectorDatasource_->GetUrl();
   vectorEditor_.OpenVector(file);
   if (!vectorEditor_.OpenLayer(pVectorDatasource_->GetActiveLayer()))
      return;
   int fields = vectorEditor_.GetFieldCount();
   AppendColumn(DataInfo<unsigned char>::Name, _(FIELD_NAME_COLUMN));
   AppendColumn(DataInfo<unsigned char>::Name, _(FIELD_TYPE_COLUMN));
   for (int i = 0; i < fields; ++i) {
      AppendRow();
      std::string fieldname = vectorEditor_.GetFieldName(i);
      WriteVarchar(FieldNameColumn, GetRows() - 1, fieldname);
      std::string fieldtype = vectorEditor_.GetFieldType(i);
      WriteVarchar(FieldTypeColumn, GetRows() - 1, fieldtype);
   }
   vectorEditor_.CloseVector();
}

} /** namespace suri */
