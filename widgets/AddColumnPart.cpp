/*! \file AddColumnPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <ctype.h>
// Includes Suri
#include "AddColumnPart.h"
#include "TablePart.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/messages.h"
#include "resources.h"
// Includes Wx
#include "wx/choice.h"
#include "wx/event.h"
// Defines
// forwards

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(AddColumnPartEvent, AddColumnPart)
   IMPLEMENT_EVENT_CALLBACK(OnButtonVerifyEquation, OnButtonVerifyEquation(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnChoiceSelection, OnChoiceSelection(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonOk, OnButtonOk(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonCancel, OnButtonCancel(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Constructor */
AddColumnPart::AddColumnPart(TableTool* pTableTool, TablePart* pTablePart) :
   NEW_EVENT_OBJECT(AddColumnPartEvent),
   pTableTool_(pTableTool), pTablePart_(pTablePart) {

   // Valor por default
   columnname_ = _(content_DEFAULT_COLUMN_NAME);

   // Variables parametro y de control para interactuar con la tabla
   type_ = Table::UNKNOWN;
   validname_ = true;
   windowTitle_ = _(caption_ADD_COLUMN);
}

/** Destructor */
AddColumnPart::~AddColumnPart() {
   DELETE_EVENT_OBJECT;
}

/** Creacion de Ventana  */
bool AddColumnPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                     wxT("ID_ADD_COLUMN_PANEL"));
   if (pToolWindow_ == NULL) return false;

   SetChoiceDataTypes();

   // Conecto los eventos a los controles.
   GET_CONTROL(*pToolWindow_, "ID_VERIFY_EQ_BITMAPBUTTON", wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(AddColumnPartEvent::OnButtonVerifyEquation), NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_BUTTON_OK", wxButton)->Connect(
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(AddColumnPartEvent::OnButtonOk), NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_BUTTON_CANCEL", wxButton)->Connect(
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(AddColumnPartEvent::OnButtonCancel), NULL, pEventHandler_);

   wxChoice* ptypechoice = XRCCTRL(*pToolWindow_, wxT("ID_COLUMN_TYPE_CHOICE"), wxChoice);
   ptypechoice->Connect(wxEVT_COMMAND_CHOICE_SELECTED,
                        wxCommandEventHandler(AddColumnPartEvent::OnChoiceSelection),
                        NULL, pEventHandler_);

   wxTextCtrl* ptxtctrl = XRCCTRL(*pToolWindow_, wxT("ID_COLUMN_NAME_TEXTCTRL"), wxTextCtrl);
   int fieldnamesize = 10;
   ptxtctrl->SetMaxLength(fieldnamesize);
   ptxtctrl->SetValidator(wxTextValidator(wxFILTER_NONE, &columnname_));
   ptxtctrl->SelectAll();
   ptxtctrl->SetFocusFromKbd();
   ptxtctrl->SetValue(columnname_);
   ptypechoice->Select(0);
   type_ = Table::INT;

   return true;
}

/** Accion de verificar la ecuacion ingresada en el TextCtrl */
void AddColumnPart::OnButtonVerifyEquation(wxCommandEvent &Event) {
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_EQUATION_TEXTCTRL"), wxTextCtrl);
   const std::string Equation = ptext->GetValue().c_str();
   bool success = pTablePart_->GetTable()->IsValidEquation(Equation);
   ShowStatusMessage(success);
}

/** Accion de seleccionar una opcion en el Choice */
void AddColumnPart::OnChoiceSelection(wxCommandEvent &Event) {
   wxBitmapButton* pbutton = XRCCTRL(*pToolWindow_, wxT("ID_VERIFY_EQ_BITMAPBUTTON"), wxBitmapButton);
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_EQUATION_TEXTCTRL"), wxTextCtrl);
   wxChoice* ptypechoice = XRCCTRL(*pToolWindow_, wxT("ID_COLUMN_TYPE_CHOICE"), wxChoice);

   // Obtengo la columna
   wxString controltype = ptypechoice->GetStringSelection();

   bool enableElements = false;

   if (controltype.IsSameAs(wxT(label_INT_DATA_TYPE))) {
      type_ = Table::INT;
   }
   if (controltype.IsSameAs(wxT(label_DOUBLE_DATA_TYPE))) {
      type_ = Table::DOUBLE;
   }
   if (controltype.IsSameAs(wxT(label_UCHAR_DATA_TYPE))) {
      type_ = Table::STRING;
   }
   if (controltype.IsSameAs(wxT(label_CALCULATED_DATA_TYPE))) {
      type_ = Table::CALCULATED;
      enableElements = true;
   }

   if (enableElements) {
      pbutton->Enable();
      ptext->Enable();
   } else {
      pbutton->Disable();
      ptext->Disable();
   }
}

/** Inicializa los tipos de dato de wxChoice */
void AddColumnPart::SetChoiceDataTypes() {
   wxArrayString datatypes;
   datatypes.Add(wxT(label_INT_DATA_TYPE));
   datatypes.Add(wxT(label_DOUBLE_DATA_TYPE));
   datatypes.Add(wxT(label_UCHAR_DATA_TYPE));
   datatypes.Add(wxT(label_CALCULATED_DATA_TYPE));
   wxChoice* ptypechoice = XRCCTRL(*pToolWindow_, wxT("ID_COLUMN_TYPE_CHOICE"), wxChoice);
   ptypechoice->Append(datatypes);
}

/** Inserta la nueva columna */
void AddColumnPart::OnButtonOk(wxCommandEvent &Event) {
   if (pTablePart_ == NULL) {
      return;
   }

   append_ = pTablePart_->GetTable()->GetPermissionList()->IsOperationPermitted(
         pTablePart_->GetTable()->GetPermission(), PermissionList::APPCOL);

   insert_ = pTablePart_->GetTable()->GetPermissionList()->IsOperationPermitted(
         pTablePart_->GetTable()->GetPermission(), PermissionList::INSCOL);

   if (!append_ && !insert_) {
      return;
   }

   wxTextCtrl* ptxtctrl = XRCCTRL(*pToolWindow_, wxT("ID_COLUMN_NAME_TEXTCTRL"), wxTextCtrl);
   columnname_ = ptxtctrl->GetValue();

   // Verifico el string
   if (!columnname_.IsAscii()) {
      SHOW_ERROR(message_NON_ASCII_FIELD_NAME_ERROR);
      validname_ = false;
   }

   if (!validname_ || type_ == Table::UNKNOWN)
      return;

   int columns = pTablePart_->GetTable()->GetColumns();
   std::string newcolumnname = ToLower(columnname_.c_str());
   for (int i = 0; i < columns; i++) {
      std::string columnname = ToLower(pTablePart_->GetTable()->GetColumnName(i));
      if (newcolumnname.compare(columnname)==0) {
         SHOW_ERROR(message_FIELD_NAME_DUPLICATED);
         return;
      }
   }

   if (type_ != Table::CALCULATED)
      AddColumn();
   else
      AddCalculatedColumn();
}

/** Agrega una columna a la tabla */
void AddColumnPart::AddColumn() {
   std::string name = WS2S(columnname_);
   std::vector<int> columns = pTableTool_->GetSelectedColumns();
   if (insert_) {
      int size = columns.size();
      if (size == 0) {
         pTableTool_->GetTable()->AppendColumn(name, type_);
      } else if (size == 1) {
         int column = columns[0];
         pTableTool_->GetTable()->InsertColumn(column, name, type_);
      }
   } else {
      pTableTool_->GetTable()->AppendColumn(name, type_);
   }
   EndModal(wxID_OK);
}

/** Agrega una columna calculada a la tabla */
void AddColumnPart::AddCalculatedColumn() {
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_EQUATION_TEXTCTRL"), wxTextCtrl);
   const std::string Equation = ptext->GetValue().c_str();
   bool success = pTablePart_->GetTable()->IsValidEquation(Equation);
   if (success) {
      pTablePart_->GetTable()->AppendCalculatedColumn(
            columnname_.c_str(), Table::CALCULATED, Equation, Table::RegularFlag);
      EndModal(wxID_OK);
   }
   else
      ShowStatusMessage(success);
}

/* Muestra un mensaje informando el estado del parseo de la ecuacion
 * @param[in] success estado del parseo de la ecuacion
 */
void AddColumnPart::ShowStatusMessage(bool success) {
   if (success)
      SHOW_WARNING(message_SINTAX_VALID);
   else
      SHOW_ERROR(message_SINTAX_ERROR);
}

/**
 * Evento click sobre Boton Cancel
 * @param[in] Event evento
 */
void AddColumnPart::OnButtonCancel(wxCommandEvent &Event) {
   EndModal(wxID_OK);
}

/**
 * Obtiene el icono para la ventana de propiedades especificas de la anotacion.
 * @param[in]  X: sin utilizar
 * @param[in]  Y: sin utilizar
 * @param[out] ToolBitmap: bitmap con el icono de la ventana.
 */
void AddColumnPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(tool_TABLE_ADD_COL, ToolBitmap);
}
} /** namespace suri */
