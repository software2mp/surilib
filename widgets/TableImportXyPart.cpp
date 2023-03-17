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
#include "TableImportXyPart.h"
#include "suri/messages.h"
#include "resources.h"
#include "suri/LibraryManagerFactory.h"
#include "GdalSrsItemOrigin.h"
#include "CsvVectorCreator.h"

// Includes Wx
#include "wx/event.h"
#include "wx/filepicker.h"
#include "wx/spinctrl.h"
// Defines
#define TAB "\t"
#define COMA ","
#define SEMICOLON ";"
// forwards

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(TableImportXyPartEvent, TableImportXyPart)
   IMPLEMENT_EVENT_CALLBACK(OnButtonOk, OnButtonOk(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonCancel, OnButtonCancel(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRadioButtonTab, OnRadioButtonTab(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRadioButtonComa, OnRadioButtonComa(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRadioButtonSpace, OnRadioButtonSpace(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnRadioButtonOther, OnRadioButtonOther(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Constructor */
TableImportXyPart::TableImportXyPart(LibraryManager* pManager, DatasourceManagerInterface* dm) :
NEW_EVENT_OBJECT(TableImportXyPartEvent), isenabled_(false), dm_(dm) {
   windowTitle_ = _(tooltip_LAYER_ADD_CSV_BUTTON);
   token_ = TAB;
   pClient_ = pManager ? new LibraryClient(pManager, LibraryManager::READONLY) : NULL;
   const Library* plib =
         pClient_->GetLibraryByCode(LibraryManagerFactory::SpatialReferenceLibraryCode);
   pItemSelector_ = new BasicLibraryItemSelector(this,
                                       plib->GetId(), pManager, "ID_LIBRARY_SELECTION_PANEL",
                                       "ID_LIBRARY_SELECTION_PANEL", true,
                                       UniversalGraphicalComponentPartInterface::ExtendedROWithFF);
   if (plib) {
      const LibraryItem* pitem = plib->GetItemByPrincipal("EPSG:4326");
      if (pitem)
         pItemSelector_->SetActiveItem(pitem);
   }
}

/** Destructor */
TableImportXyPart::~TableImportXyPart() {
   DELETE_EVENT_OBJECT;
}

/** Creacion de Ventana  */
bool TableImportXyPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_XYTABLEIMPORT_PANEL"));
   if (pToolWindow_ == NULL) return false;
   // Conecto los eventos a los controles.
   GET_CONTROL(*pToolWindow_, "ID_BUTTON_OK", wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(TableImportXyPartEvent::OnButtonOk), NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_BUTTON_CANCEL", wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(TableImportXyPartEvent::OnButtonCancel), NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_RADIOBUTTON_TAB", wxRadioButton)->Connect(
         wxEVT_COMMAND_RADIOBUTTON_SELECTED,
         wxCommandEventHandler(TableImportXyPartEvent::OnRadioButtonTab), NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_RADIOBUTTON_COMA", wxRadioButton)->Connect(
         wxEVT_COMMAND_RADIOBUTTON_SELECTED,
         wxCommandEventHandler(TableImportXyPartEvent::OnRadioButtonComa), NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_RADIOBUTTON_SPACE", wxRadioButton)->Connect(
         wxEVT_COMMAND_RADIOBUTTON_SELECTED,
         wxCommandEventHandler(TableImportXyPartEvent::OnRadioButtonSpace), NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_RADIOBUTTON_OTHER", wxRadioButton)->Connect(
         wxEVT_COMMAND_RADIOBUTTON_SELECTED,
         wxCommandEventHandler(TableImportXyPartEvent::OnRadioButtonOther), NULL, pEventHandler_);
   if (pItemSelector_) {
      AddControl(pItemSelector_->GetWidget(), "ID_SR_SELECTOR_PANEL");
   }
   return true;
}

/**
 * Evento click sobre Boton Aceptar
 * @param[in] Event evento
 */
void TableImportXyPart::OnButtonOk(wxCommandEvent &Event) {
   // Si el token es personalizado lo asigna
   if (isenabled_) {
      wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_TEXTCTRL_SEPARATOR"), wxTextCtrl);
      token_ = ptext->GetValue().c_str();
   }
   // Verifica el token seleccionado
   if (token_.empty()) {
      SHOW_ERROR(message_TOKEN_ERROR);
      return;
   }
   // Obtiene los valores configurados para importar el archivo
   int startfromrow = USE_CONTROL(*pToolWindow_, "ID_SPINCTRL_START_ROW", wxSpinCtrl, GetValue(), 0);
   int xcolumn = USE_CONTROL(*pToolWindow_, "ID_SPINCTRL_COLUMN_X", wxSpinCtrl, GetValue(), 0);
   xcolumn--;
   int ycolumn = USE_CONTROL(*pToolWindow_, "ID_SPINCTRL_COLUMN_Y", wxSpinCtrl, GetValue(), 0);
   ycolumn--;
   wxFilePickerCtrl* pfilepicker = XRCCTRL(*pToolWindow_, "ID_FILECTRL", wxFilePickerCtrl);
   std::string path = pfilepicker->GetPath().c_str();
   wxCheckBox* chkheader = XRCCTRL(*pToolWindow_, wxT("ID_CHECKBOX_HEADER"), wxCheckBox);
   // Verifica que se haya seleccionado un archivo
   if (path.empty()) {
      SHOW_ERROR(message_EMPTY_FILE_ERROR);
      return;
   }
   CsvVectorCreator importer(path, xcolumn, ycolumn, startfromrow, GetSpatialReference(), token_,
                             isenabled_, chkheader->GetValue());

   if (dm_->AddDatasource(importer.Create())) EndModal(wxID_OK);
   else {
      SHOW_ERROR(message_REPEAT_COLUMN_ERROR);
            return;
       }

}

/**
 * Evento click sobre Boton Cancelar
 * @param[in] Event evento
 */
void TableImportXyPart::OnButtonCancel(wxCommandEvent &Event) {
   EndModal(wxID_OK);
}

/**
 * Evento click sobre RadioButton Tabulacion
 * @param[in] Event evento
 */
void TableImportXyPart::OnRadioButtonTab(wxCommandEvent &Event) {
   SetState(false);
   token_ = TAB;
}

/**
 * Evento click sobre RadioButton Coma
 * @param[in] Event evento
 */
void TableImportXyPart::OnRadioButtonComa(wxCommandEvent &Event) {
   SetState(false);
   token_ = COMA;
}

/**
 * Evento click sobre RadioButton Espacio
 * @param[in] Event evento
 */
void TableImportXyPart::OnRadioButtonSpace(wxCommandEvent &Event) {
   SetState(false);
   token_ = SEMICOLON;
}

/**
 * Evento click sobre RadioButton Otro
 * @param[in] Event evento
 */
void TableImportXyPart::OnRadioButtonOther(wxCommandEvent &Event) {
   SetState(true);
}

/**
 * Maneja el estado del TextCtrl
 * @param[in] isEnabled true habilita el TextCtrl
 * @param[in] isEnabled false deshabilita el TextCtrl
 */
void TableImportXyPart::SetState(bool IsEnabled) {
   isenabled_ = IsEnabled;
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_TEXTCTRL_SEPARATOR"), wxTextCtrl);
   if (IsEnabled)
      ptext->Enable();
   else
      ptext->Disable();
}

/* Obtiene el icono para la ventana de propiedades especificas de la anotacion.
 * @param[in]  X: sin utilizar
 * @param[in]  Y: sin utilizar
 * @param[out] ToolBitmap: bitmap con el icono de la ventana.
 */
void TableImportXyPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(tool_LAYER_ADD, ToolBitmap);
}

void TableImportXyPart::SetActiveItem(const LibraryItem* pItem) {
}

/** Retorna el atributo WKT_ATTRIBUTE_NAME del espacio de referencia seleccionado */
std::string TableImportXyPart::GetSpatialReference() {
   const LibraryItem* pItem = pItemSelector_->GetActiveItem();
   std::string spatialreference;
   if (pItem)
      spatialreference = pItem->GetAttribute(GdalSrsItemOrigin::WKT_ATTRIBUTE_NAME)->GetValue();
   return spatialreference;
}

} /** namespace suri */
