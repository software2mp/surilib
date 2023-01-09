/*! \file wxWidgetCellEditor.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
