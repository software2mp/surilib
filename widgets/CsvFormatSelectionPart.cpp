/*! \file CsvFormatSelectionPart.cpp */
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
#include "CsvFormatSelectionPart.h"
#include "resources.h"
// Includes Wx
#include "wx/filepicker.h"
// Defines
#define ID_FORMAT_CSV_SELECTION_PART "ID_VECTOR_FORMAT_SELECT"
// forwards

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(CsvFormatSelectionEventHandler, CsvFormatSelectionPart)
   IMPLEMENT_EVENT_CALLBACK(OnUiUpdate, OnUiUpdate(event), wxUpdateUIEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonClicked, OnButtonClicked(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Constructor **/
CsvFormatSelectionPart::CsvFormatSelectionPart() : Part(true, false),
      NEW_EVENT_OBJECT(CsvFormatSelectionEventHandler) {
   windowTitle_ = _(caption_OUTPUT_SELECTION);
}

CsvFormatSelectionPart::~CsvFormatSelectionPart() {
   DELETE_EVENT_OBJECT;
}

/** Indica si el Part tiene cambios para salvar. */
bool CsvFormatSelectionPart::HasChanged() {
   return modified_;
}

/** Salva los cambios realizados en el Part. */
bool CsvFormatSelectionPart::CommitChanges() {
   fileName_ = USE_CONTROL(*pToolWindow_, "ID_FILENAME_SELECT",
                           wxTextCtrl, GetValue(), "").c_str();
   format_ = USE_CONTROL(*pToolWindow_, wxT("ID_FORMAT_SELECT"),
                         wxChoice, GetStringSelection(), "").c_str();
   if (pPal_) {
      SerializableString ss(fileName_);
      pPal_->AddSerializableAttribute<SerializableString>(
                     ProcessAdaptLayer::OutputFileNameKeyAttr, ss);
      ss = SerializableString(format_);
      pPal_->AddSerializableAttribute<SerializableString>(
                     ProcessAdaptLayer::OutputFileFormatKeyAttr, ss);
   }
   return true;
}

/** Restaura los valores originales del Part. */
bool CsvFormatSelectionPart::RollbackChanges() {
   GET_CONTROL(*pToolWindow_, wxT("ID_FORMAT_SELECT"), wxChoice)->Select(0);
   GET_CONTROL(*pToolWindow_, wxT("ID_FILENAME_SELECT"),
               wxTextCtrl)->SetValue(fileName_);
   return true;
}

/** Inicializa el part */
void CsvFormatSelectionPart::SetInitialValues() {
}

/** Actualiza el estado de la parte */
void CsvFormatSelectionPart::Update() {
}

/** Setea el padre del Part y llama a CreateTool. */
bool CsvFormatSelectionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT(ID_FORMAT_CSV_SELECTION_PART));

   if (!pToolWindow_) {
      return false;
   }
   GET_CONTROL(*pToolWindow_, wxT("ID_FORMAT_SELECT"), wxChoice)->Insert(
            _(name_TXT_FILES), 0);
   GET_CONTROL(*pToolWindow_, wxT("ID_FORMAT_SELECT"), wxChoice)->Select(0);
   // Conecta el evento temporal
   pToolWindow_->Connect(
         wxEVT_UPDATE_UI,
         wxUpdateUIEventHandler(CsvFormatSelectionEventHandler::OnUiUpdate), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_DIALOG_BUTTON"), wxButton)->
               Connect(wxEVT_COMMAND_BUTTON_CLICKED,
               wxCommandEventHandler(CsvFormatSelectionEventHandler::OnButtonClicked), 
               NULL, pEventHandler_); 
   return true;
}

/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono de la ventana
 * @param[in] Y alto del icono de la ventana
 */
void CsvFormatSelectionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_IMAGE_FORMAT_SELECTION, ToolBitmap);
}

/** Obtiene el formato elegido para el archivo de salida **/
std::string CsvFormatSelectionPart::GetFormat() const {
   return format_;
}
/** Obtiene el nombre del archivo de salida **/
std::string CsvFormatSelectionPart::GetFileName() const {
   return fileName_;
}

/** Evento UIUpdate */
void CsvFormatSelectionPart::OnUiUpdate(wxUpdateUIEvent &Event) {
   modified_ = false;
   std::string filename = USE_CONTROL(*pToolWindow_, wxT("ID_FILENAME_SELECT"),
                                      wxTextCtrl, GetValue(), "").c_str();
   std::string format = USE_CONTROL(*pToolWindow_, wxT("ID_FORMAT_SELECT"),
                           wxChoice, GetStringSelection(), "").c_str();
   if (fileName_.compare(filename) != 0) {
      modified_ = true;
   }
   if (format_.compare(format) != 0) {
      modified_ = true;
   }
   Event.Skip();
}

/**
 * Abre un file dialog para configurar nombre y salida del archivo
 * @param[in] Event evento click sobre el boton
 */
void CsvFormatSelectionPart::OnButtonClicked(wxCommandEvent &Event) {
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_FILENAME_SELECT"), wxTextCtrl);
   std::string filename = ptext->GetValue().c_str();
   wxFileDialog dlg(NULL, _(caption_SAVE_AS), wxT(""), filename, "*.csv",
   wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
   if (dlg.ShowModal() != wxID_OK)
      return;
   filename = dlg.GetPath();
   if (filename.find(".csv") == std::string::npos)
         filename += ".csv";
   ptext->SetValue(filename);
}
} /** namespace suri */
