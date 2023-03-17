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
#include "QueryBuilderPart.h"
#include "DefaultTable.h"
#include "SearchWidget.h"
#include "TablePart.h"
#include "suri/messages.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(QueryBuilderPartEvent, QueryBuilderPart)
   IMPLEMENT_EVENT_CALLBACK(OnButtonGreaterOperation, OnButtonGreaterOperation(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonLessOperation, OnButtonLessOperation(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonEqualOperation, OnButtonEqualOperation(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonAndOperation, OnButtonAndOperation(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonOrOperation, OnButtonOrOperation(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonNotEqualOperation, OnButtonNotEqualOperation(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonOk, OnButtonOk(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonAppendColumnName, OnButtonAppendColumnName(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnCellDoubleClick, OnCellDoubleClick(event), wxGridEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Constructor */
QueryBuilderPart::QueryBuilderPart(TablePart* pTablePart, SearchWidget* pSearchWidget,
                                   std::string LastCondition) :
      PartContainerWidget(_(tooltip_QUERY_BUILDER),
                          SUR_BTN_OK | SUR_BTN_CANCEL,
                          SUR_BTN_OK | SUR_BTN_CANCEL),
      NEW_EVENT_OBJECT(QueryBuilderPartEvent),
      pTablePart_(pTablePart), pSearchWidget_(pSearchWidget), lastCondition_(LastCondition),
      pCondition_(NULL), pTable_(NULL){
}

/** Constructor */
QueryBuilderPart::QueryBuilderPart(Table* pTable,
                                   std::string* pCondition) :
      PartContainerWidget(_(tooltip_QUERY_BUILDER),
      SUR_BTN_OK | SUR_BTN_CANCEL,
                          SUR_BTN_OK | SUR_BTN_CANCEL),
      NEW_EVENT_OBJECT(QueryBuilderPartEvent),
      pTablePart_(NULL), pSearchWidget_(NULL),
      pCondition_(pCondition), pTable_(pTable) {

}
/** Destructor */
QueryBuilderPart::~QueryBuilderPart() {
   DELETE_EVENT_OBJECT;
}

/** Creacion de Ventana  */
bool QueryBuilderPart::CreateToolWindow() {
   if (!PartContainerWidget::CreateToolWindow())
      return false;

   wxWindow *pcontainer = XRCCTRL(*pToolWindow_, wxT("ID_CONTAINER_WIN"), wxWindow);

   wxWindow* pwin = wxXmlResource::Get()->LoadPanel(
         pcontainer, "ID_QUERY_BUILDER_PANEL");

   AddMainControl(pwin);

   // Conecto los eventos a los controles.
   GET_CONTROL(*pToolWindow_, "ID_GREATER_BUTTON", wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(QueryBuilderPartEvent::OnButtonGreaterOperation),
         NULL, pEventHandler_);

   GET_CONTROL(*pToolWindow_, "ID_LESS_BUTTON", wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(QueryBuilderPartEvent::OnButtonLessOperation),
         NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_EQUAL_BUTTON", wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(QueryBuilderPartEvent::OnButtonEqualOperation),
         NULL, pEventHandler_);

   GET_CONTROL(*pToolWindow_, "ID_AND_BUTTON", wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(QueryBuilderPartEvent::OnButtonAndOperation),
         NULL, pEventHandler_);

   GET_CONTROL(*pToolWindow_, "ID_OR_BUTTON", wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(QueryBuilderPartEvent::OnButtonOrOperation),
         NULL, pEventHandler_);

   GET_CONTROL(*pToolWindow_, "ID_NOT_EQUAL_BUTTON", wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(QueryBuilderPartEvent::OnButtonNotEqualOperation),
         NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_APPEND_COLUMN_BUTTON", wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(QueryBuilderPartEvent::OnButtonAppendColumnName),
         NULL, pEventHandler_);
   wxGrid* pgrid = XRCCTRL(*pToolWindow_, wxT("ID_QUERY_BUILDER_GRID"), wxGrid);


   int colCount =
         pTablePart_ ? pTablePart_->GetTable()->GetColumns() : pTable_->GetColumns();
   int rowNumber = 0;
   pgrid->CreateGrid(0, 1);
   for (int ix = 0; ix < colCount; ++ix) {
      if (!pTablePart_ || !pTablePart_->IsColumnHidden(ix)) {
         Table* ptable = pTablePart_ ? pTablePart_->GetTable() : pTable_;
         std::string col = ptable->GetColumnName(ix).c_str();
         if (col!=VectorEditorDriver::GeometryColumnName){
            pgrid->AppendRows();
            pgrid->SetCellValue(rowNumber, 0, ptable->GetColumnName(ix));
            pgrid->SetReadOnly(rowNumber, 0);
            rowNumber++;
         }
      }
   }

   pgrid->Connect(wxEVT_GRID_CELL_LEFT_DCLICK,
         wxGridEventHandler(QueryBuilderPartEvent::OnCellDoubleClick), NULL, pEventHandler_);
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_QUERY_BUILDER_TEXTCTRL"), wxTextCtrl);

   if (pCondition_)
      lastCondition_ = *pCondition_;
   ptext->SetValue(lastCondition_);
   return true;
}

/** Captura el evento de click sobre el boton con el operador > */
void QueryBuilderPart::OnButtonGreaterOperation(wxCommandEvent &Event) {
   AppendString(Greater);
}

/** Captura el evento de click sobre el boton con el operador < */
void QueryBuilderPart::OnButtonLessOperation(wxCommandEvent &Event) {
   AppendString(Less);
}

/** Captura el evento de click sobre el boton con el operador = */
void QueryBuilderPart::OnButtonEqualOperation(wxCommandEvent &Event) {
   AppendString(Equal);
}

/** Captura el evento de click sobre el boton con el operador AND */
void QueryBuilderPart::OnButtonAndOperation(wxCommandEvent &Event) {
   AppendString(And);
}

/** Captura el evento de click sobre el boton con el operador OR */
void QueryBuilderPart::OnButtonOrOperation(wxCommandEvent &Event) {
   AppendString(Or);
}

/** Captura el evento de click sobre el boton con el operador != */
void QueryBuilderPart::OnButtonNotEqualOperation(wxCommandEvent &Event) {
   AppendString(NotEqual);
}

/** Inicializa la ejecucion de la consulta */
void QueryBuilderPart::OnButtonOk(wxCommandEvent &Event) {
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_QUERY_BUILDER_TEXTCTRL"), wxTextCtrl);
   std::string condition = ptext->GetValue().c_str();

   if (pSearchWidget_)
      pSearchWidget_->SetValue(condition);
   if (pCondition_)
      *pCondition_ = condition;
}

/** Captura el evento de click sobre el boton Agregar columna */
void QueryBuilderPart::OnButtonAppendColumnName(wxCommandEvent &Event) {
   wxGrid* pgrid = XRCCTRL(*pToolWindow_, wxT("ID_QUERY_BUILDER_GRID"), wxGrid);
   AppendColumnName(pgrid->GetCellValue(pgrid->GetGridCursorRow(), pgrid->GetGridCursorCol()).c_str());
}

/** Captura el evento de doble click sobre la grilla */
void QueryBuilderPart::OnCellDoubleClick(wxGridEvent &Event) {
   wxGrid* pgrid = XRCCTRL(*pToolWindow_, wxT("ID_QUERY_BUILDER_GRID"), wxGrid);
   AppendColumnName(pgrid->GetCellValue(Event.GetRow(), Event.GetCol()).c_str());
}

/** Agrega un string al final del TextCtrl */
void QueryBuilderPart::AppendColumnName(std::string ColumnName) {
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_QUERY_BUILDER_TEXTCTRL"), wxTextCtrl);
   std::string Condition = ptext->GetValue().c_str();
   Condition += "'";
   Condition += ColumnName.c_str();
   Condition += "'";
   ptext->AppendText(Condition);
}

/** Agrega un string al final del TextCtrl */
void QueryBuilderPart::AppendString(OperationType Operation) {
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_QUERY_BUILDER_TEXTCTRL"), wxTextCtrl);
   std::string Condition = ptext->GetValue().c_str();
   switch (Operation) {
      case Greater:
         Condition =+ " > ";
         break;
      case Less:
         Condition =+ " < ";
         break;
      case Equal:
         Condition =+ " = ";
         break;
      case And:
         Condition =+ " AND ";
         break;
      case Or:
         Condition =+ " OR ";
         break;
      case NotEqual:
         Condition =+ " != ";
         break;
      default:
         break;
   }
   ptext->AppendText(Condition);
}

} /** namespace suri */
