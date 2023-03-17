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
#include "wxHotLinkEditor.h"
#include "wxGenericTableBase.h"
#include "wxmacros.h"
#include "suri/Table.h"
#include "suri/VectorStyle.h"
#include "wxGridColorCellEditor.h"
#include "wxGridColAttrProvider.h"
#include "wxGridColorCellRenderer.h"
// Includes Wx
// Defines

namespace suri {

wxHotLinkEditor::wxHotLinkEditor(wxGenericTableBase* pTable) : pTable_(pTable) {
}

wxHotLinkEditor::~wxHotLinkEditor() {
}

/**
 * Lo que se efectua cuando comienza la edicion de la celda
 * @param[in] Row numero de la fila de la celda a editar
 * @param[in] Column numero de la columna de la celda a editar
 * @param[in] pGrid puntero a la grilla en edicion
 */
void wxHotLinkEditor::BeginEdit(int Row, int Column, wxGrid* pGrid) {
   wxGridCellTextEditor::BeginEdit(Row, Column, pGrid);
}
/**
 * Metodo invocado al finalizar la edicion de una celda
 */
bool wxHotLinkEditor::EndEdit(int Row, int Column, wxGrid* pGrid) {

   wxGridCellAttr* pattr = new wxGridCellAttr(wxColour(0,0,255), wxColour(255,255,255),
                                              wxFont(10,wxFONTFAMILY_DEFAULT,
                                                     wxFONTSTYLE_NORMAL ,
                                                     wxFONTWEIGHT_LIGHT),
                                              0, 0);
   if (pattr && pTable_->GetAttrProvider()) {
      pTable_->GetAttrProvider()->SetAttr(pattr, Row, Column);
   }
   wxGridCellTextEditor::EndEdit(Row, Column, pGrid);
   return true;
}

} /** namespace suri */
