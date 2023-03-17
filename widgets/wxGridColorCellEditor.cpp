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
#include "wxGridColorCellEditor.h"
#include "wxGenericTableBase.h"
#include "wxmacros.h"
#include "suri/Table.h"
#include "suri/VectorStyle.h"

// Includes Wx
#include "wx/colordlg.h"

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
wxGridColorCellEditor::wxGridColorCellEditor(wxGenericTableBase* pTable) :
      pTable_(pTable), oldValue_(), newValue_() {
}

/**
 * Destructor
 */
wxGridColorCellEditor::~wxGridColorCellEditor() {
}

/**
 * Lo que se efectua cuando comienza la edicion de la celda
 * @param[in] Row numero de la fila de la celda a editar
 * @param[in] Column numero de la columna de la celda a editar
 * @param[in] pGrid puntero a la grilla en edicion
 */
void wxGridColorCellEditor::BeginEdit(int Row, int Column, wxGrid* pGrid) {
   wxString value = pTable_->GetValue(Row, Column);
   oldValue_ = "#" + value;
   newValue_ = oldValue_;
   wxColourData oldcolor;
   VectorStyle::Color vscolor(WS2S(value));
   oldcolor.SetColour(wxColour(vscolor.red_, vscolor.green_, vscolor.blue_, vscolor.alpha_));
   wxColourDialog colourdialog(m_control, &oldcolor);
   m_control->Hide();
   if (colourdialog.ShowModal() == wxID_OK) {
      wxColourData colordata = colourdialog.GetColourData();
      wxColour colour = colordata.GetColour();
      newValue_ = colour.GetAsString(wxC2S_HTML_SYNTAX);
      pGrid->SaveEditControlValue();
   }
}

/**
 * Creacion del objeto actual (es requerido por wxwidgets para inicializar el objeto)
 * @param[in] Parent puntero a la ventana padre
 * @param[in] Id id de la ventana
 * @param[in] pEventHandler puntero al controlador de eventos
 */
void wxGridColorCellEditor::Create(wxWindow* pParent, wxWindowID Id, wxEvtHandler* pEventHandler) {
   m_control = new wxControl(pParent, Id);
   wxGridCellEditor::Create(pParent, Id, pEventHandler);
}

/**
 * Lo que se efectua cuando finaliza la edicion de la celda
 * @param[in] Row numero de la fila de la celda a editar
 * @param[in] Column numero de la columna de la celda a editar
 * @param[in] pGrid puntero a la grilla en edicion
 * @return bool Devuelve true si tuvo exito, false en caso contrario
 */
bool wxGridColorCellEditor::EndEdit(int Row, int Column, wxGrid* pGrid) {
   if (oldValue_.CompareTo(newValue_) != 0) {
      std::string auxstring = WS2S(newValue_);
      auxstring.replace(0, 1, "");
      pTable_->SetValue(Row, Column, auxstring);
   }
   return true;
}

/**
 * Reseteo del valor de la celda al valor original (anterior a la edicion)
 */
void wxGridColorCellEditor::Reset() {
   oldValue_ = newValue_;
}

/**
 * Clonacion del objeto actual
 * @return wxGridColorCellEditor nuevo objeto con una copia profunda del actual
 */
wxGridCellEditor* wxGridColorCellEditor::Clone() const {
   // Se hace un casteo dinamico porque necesito pasarle los parametros
   return dynamic_cast<wxGridCellEditor*>(new wxGridColorCellEditor(pTable_));
}

/**
 * Obtencion de valor de la celda
 * @return wxString valor de la celda
 */
wxString wxGridColorCellEditor::GetValue() const {
   return newValue_;
}

} /** namespace suri */
