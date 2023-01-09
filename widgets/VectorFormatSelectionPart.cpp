/*! \file VectorFormatSelectionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "VectorFormatSelectionPart.h"
#include "resources.h"
#include "suri/FileVectorExporterProcess.h"

// Includes Wx

// Defines
#define ID_FORMAT_SELECTION_PART "ID_VECTOR_FORMAT_SELECT"

// forwards

namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(FormatSelectionEventHandler, VectorFormatSelectionPart)
   IMPLEMENT_EVENT_CALLBACK(OnUiUpdate, OnUiUpdate(event), wxUpdateUIEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonClicked, OnButtonClicked(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */


/** Constructor **/
VectorFormatSelectionPart::VectorFormatSelectionPart() : Part(true, false),
      NEW_EVENT_OBJECT(FormatSelectionEventHandler) {
   windowTitle_ = _(caption_OUTPUT_SELECTION);
}

/** Destructor **/
VectorFormatSelectionPart::~VectorFormatSelectionPart() {
   DELETE_EVENT_OBJECT;
}

/** Indica si el Part tiene cambios para salvar. */
bool VectorFormatSelectionPart::HasChanged() {
   return modified_;
}

/** Salva los cambios realizados en el Part. */
bool VectorFormatSelectionPart::CommitChanges() {
   fileName_ = USE_CONTROL(*pToolWindow_, wxT("ID_FILENAME_SELECT"),
                           wxTextCtrl, GetValue(), "");
   format_ = USE_CONTROL(*pToolWindow_, wxT("ID_FORMAT_SELECT"),
                         wxChoice, GetStringSelection(), "").c_str();
   if (pPal_) {
      SerializableString ss(fileName_);
      pPal_->AddSerializableAttribute<SerializableString>(
            ProcessAdaptLayer::OutputFileNameKeyAttr, ss);
      ss = SerializableString(format_);
      pPal_->AddSerializableAttribute<SerializableString>(
            ProcessAdaptLayer::OutputFileFormatKeyAttr, ss);
		bool exportsel = USE_CONTROL(*pToolWindow_, 
		                 wxT("ID_EXPORT_SEL_CHECK"), wxCheckBox, GetValue(), false);
		pPal_->AddAttribute<bool>(ProcessAdaptLayer::ExportSelectionKeyAttr, exportsel);
		bool exportmetadata = USE_CONTROL(*pToolWindow_, wxT("ID_EXPORT_METADATA_CHECKBOX"),
		                                  wxCheckBox, GetValue(), false);
		pPal_->AddAttribute<bool>(ProcessAdaptLayer::ExportMetadataKeyAttr, exportmetadata);
   }
   return true;
}

/** Restaura los valores originales del Part. */
bool VectorFormatSelectionPart::RollbackChanges() {
   GET_CONTROL(*pToolWindow_, wxT("ID_FORMAT_SELECT"), wxChoice)->Select(0);
   GET_CONTROL(*pToolWindow_, wxT("ID_FILENAME_SELECT"), wxTextCtrl)->SetValue(fileName_);
   return true;
}

/** Inicializa el part */
void VectorFormatSelectionPart::SetInitialValues() {
   format_ = name_SHAPEFILE_FILES;
}

/** Actualiza el estado de la parte */
void VectorFormatSelectionPart::Update() {
}

/** Setea el padre del Part y llama a CreateTool. */
bool VectorFormatSelectionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT(ID_FORMAT_SELECTION_PART));

   if (!pToolWindow_) {
      return false;
   }
   GET_CONTROL(*pToolWindow_, wxT("ID_FORMAT_SELECT"), wxChoice)->Insert(
            _(name_SHAPEFILE_FILES), 0);
   GET_CONTROL(*pToolWindow_, wxT("ID_FORMAT_SELECT"), wxChoice)->Select(0);
   GET_CONTROL(*pToolWindow_, wxT("ID_EXPORT_METADATA_CHOICE"), wxChoice)->Insert(wxT(name_XML), 0);
   GET_CONTROL(*pToolWindow_, wxT("ID_EXPORT_METADATA_CHOICE"), wxChoice)->Select(0);
   // Conecta el evento temporal
   pToolWindow_->Connect(
         wxEVT_UPDATE_UI,
         wxUpdateUIEventHandler(FormatSelectionEventHandler::OnUiUpdate), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_DIALOG_BUTTON"), wxButton)->Connect(
            wxEVT_COMMAND_BUTTON_CLICKED,
            wxCommandEventHandler(FormatSelectionEventHandler::OnButtonClicked), NULL,
            pEventHandler_);
   bool enablesel = true;
   if (!pPal_->GetAttribute(FileVectorExporterProcess::EnableSelectionExportAttr, enablesel))
      enablesel = false;
   GET_CONTROL(*pToolWindow_,
         wxT("ID_EXPORT_SEL_CHECK"), wxCheckBox)->Enable(enablesel);
   return true;
}

/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono de la ventana
 * @param[in] Y alto del icono de la ventana
 */
void VectorFormatSelectionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_IMAGE_FORMAT_SELECTION, ToolBitmap);
}

/** Obtiene el formato elegido para el archivo de salida **/
std::string VectorFormatSelectionPart::GetFormat() const {
   return format_;
}
/** Obtiene el nombre del archivo de salida **/
std::string VectorFormatSelectionPart::GetFileName() const {
   return fileName_;
}

/** Evento UIUpdate */
void VectorFormatSelectionPart::OnUiUpdate(wxUpdateUIEvent &Event) {
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
void VectorFormatSelectionPart::OnButtonClicked(wxCommandEvent &Event) {
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_FILENAME_SELECT"), wxTextCtrl);
   std::string filename = ptext->GetValue().c_str();
   wxFileDialog dlg(NULL, _(caption_SAVE_AS), wxT(""), filename, extension_SHAPEFILE,
   wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
   if (dlg.ShowModal() != wxID_OK)
      return;
   filename = dlg.GetPath();
   if (filename.find(".shp") == std::string::npos)
         filename += ".shp";
   ptext->SetValue(filename);
}
} /** namespace suri */
