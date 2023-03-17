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

#include "wxQueryBuilderCellEditor.h"

// Includes Estandar
// Includes Suri
#include "QueryBuilderPart.h"
#include "suri/Table.h"
#include "DefaultTable.h"
#include "suri/DatasourceInterface.h"
#include "suri/VectorEditorDriver.h"
#include "VectorDatasource.h"

// Includes Wx
// Defines
// forwards

namespace suri {

wxQueryBuilderCellEditor::wxQueryBuilderCellEditor(const std::string& VectorUrl) :
      pTable_(NULL), pQueryEditor_(NULL) {
   pDatasource_ = DatasourceInterface::Create(
         "VectorDatasource", VectorUrl);
   if (pDatasource_) {
      VectorEditorDriver* pdriver = new VectorEditorDriver(pDatasource_);
      pTable_ = new DefaultTable();
      pTable_->SetDriver(pdriver);
   }
}

wxQueryBuilderCellEditor::~wxQueryBuilderCellEditor() {
   delete pTable_;
   delete pDatasource_;
}

/** Metodo virtual que inicia la edicion del atributo de la celda **/
bool wxQueryBuilderCellEditor::BeginEdition() {
   /** Por ahora no se soporte utilizar la biblioteca.
    *  Necsito generar una instancia aca porque partcontainer la destruye **/

   pQueryEditor_ = new QueryBuilderPart(pTable_, &currentValue_);
   if (pQueryEditor_->CreateTool() && pQueryEditor_->ShowModal(true) == wxID_OK) {
      return true;
   }
    return false;
}
/** Obtiene el valor resultante de la edicion de la grilla */
std::string wxQueryBuilderCellEditor::GetValue() {
   return currentValue_;
}
/** Configura el valor a editar **/
void wxQueryBuilderCellEditor::SetValue(const std::string& Value) {
   currentValue_ = Value;
}
} /** namespace suri */
