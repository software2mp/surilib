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
#include "wxWidgetCellEditor.h"

// Includes Wx
// Defines
// forwards

namespace suri {

wxWidgetCellEditor::wxWidgetCellEditor(wxGenericTableBase* pTable,
                                       wxCellEditorWidgetInterface* pEditor) :
      pTable_(pTable), pEditor_(pEditor), endedit_(false) {
}

wxWidgetCellEditor::~wxWidgetCellEditor() {

}

/** Lo que se efectua cuando comienza la edicion de la celda */
void wxWidgetCellEditor::BeginEdit(int Row, int Column, wxGrid* pGrid) {
   oldValue_ = pTable_->GetValue(Row, Column);
   pEditor_->SetValue(oldValue_.c_str());
   if (pEditor_->BeginEdition() && !pEditor_->GetValue().empty()) {
      /** obtengo el valor resultante de la edicion **/
      newValue_ = pEditor_->GetValue();
      pGrid->SaveEditControlValue();
      endedit_ = false;
   }
}

/** Creacion del objeto actual (es requerido por wxwidgets para inicializar el objeto) */
void wxWidgetCellEditor::Create(wxWindow* pParent, wxWindowID Id,
                                wxEvtHandler* pEventHandler) {
   m_control = new wxControl(pParent, Id);
   wxGridCellEditor::Create(pParent, Id, pEventHandler);
}

/** Lo que se efectua cuando finaliza la edicion de la celda */
bool wxWidgetCellEditor::EndEdit(int Row, int Column, wxGrid* pGrid) {
   if (!newValue_.empty() && oldValue_.CompareTo(newValue_) != 0) {
      pTable_->SetValue(Row, Column, newValue_.c_str());
      if (!endedit_ && pGrid->IsCellEditControlEnabled()) {
         /** es necesario este bool para forzar el refresh de la grilla en
          *  algunos casos **/
         endedit_ = true;
         pGrid->DisableCellEditControl();
      }
      pGrid->ForceRefresh();
   } else if (newValue_.empty()) {
      return false;
   }
   return true;
}

/** Reseteo del valor original de la celda */
void wxWidgetCellEditor::Reset() {
   oldValue_ = newValue_;
}

/** Clonacion del objeto actual */
wxGridCellEditor* wxWidgetCellEditor::Clone() const {
   return new wxWidgetCellEditor(pTable_, pEditor_);
}

/** Obtencion de valor de la celda */
wxString wxWidgetCellEditor::GetValue() const {
   return newValue_;
}

} /** namespace suri */
