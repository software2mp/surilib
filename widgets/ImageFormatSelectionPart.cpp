/*! \file ImageFormatSelectionPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "ImageFormatSelectionPart.h"

// Includes standard

// Includes Suri
#include "suri/Part.h"
#include "suri/Configuration.h"
#include "suri/AuxiliaryFunctions.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
#include "wx/combobox.h"
#include "wx/filename.h"

// Includes App
#include "suri/messages.h"
#include "resources.h"

// Defines
/** Formato default para los archivos */
#define DEFAULT_EXPORT_FORMAT "GTiff"

/** inicializa los recursos de la ventana */
void InitOutputSelectionXmlResource();

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(ImageFormatSelectionPartEvent, ImageFormatSelectionPart)
#ifdef __UNUSED_CODE__
/**
 * Implementa eventos que ya no son necesarios, solo el evento temporal
 * verifica si hubo cambios en la seleccion.
 */
   IMPLEMENT_EVENT_CALLBACK(OnFileSelection, OnFileSelection(event), wxFileDirPickerEvent)
   IMPLEMENT_EVENT_CALLBACK(OnFormatSelection, OnFormatSelection(event), wxCommandEvent)
#endif
   IMPLEMENT_EVENT_CALLBACK(OnUiUpdate, OnUiUpdate(event), wxUpdateUIEvent)
   IMPLEMENT_EVENT_CALLBACK(OnButtonClicked, OnButtonClicked(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Constructor
 */
ImageFormatSelectionPart::ImageFormatSelectionPart() :
      Part(false, false), NEW_EVENT_OBJECT(ImageFormatSelectionPartEvent) {
   windowTitle_ = _(caption_OUTPUT_SELECTION);
}
/**
 * Destructor
 */
ImageFormatSelectionPart::~ImageFormatSelectionPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Crea la ventana de la parte
 * @return false : si no pudo agregar el elemento XRC
 * @return true : si crea la ventana
 */
bool ImageFormatSelectionPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_FORMAT_SELECTION_PART"));

   if (!pToolWindow_) {
      return false;
   }

#ifdef __UNUSED_CODE__

   /**
    * Conecta eventos en el filepicker y en wxChoice. Ya no son necesarios,
    * ahora un evento temporal determina si hubo cambios en la seleccion.
    */

   // Conecta el evento de seleccion del nombre de archivo
   GET_CONTROL(*pToolWindow_, wxT("ID_FILENAME_SELECT"), wxFilePickerCtrl)->
   Connect(wxEVT_COMMAND_FILEPICKER_CHANGED,
         wxFileDirPickerEventHandler(ImageFormatSelectionPartEvent::OnFileSelection),
         NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, wxT("ID_FILENAME_SELECT"), wxFilePickerCtrl)->
   Connect(wxEVT_COMMAND_DIRPICKER_CHANGED,
         wxFileDirPickerEventHandler(ImageFormatSelectionPartEvent::OnFileSelection),
         NULL, pEventHandler_);

   // Conecta el evento de seleccion del formato de archivo
   GET_CONTROL(*pToolWindow_, wxT("ID_FORMAT_SELECT"), wxChoice)->
   Connect(wxEVT_COMMAND_COMBOBOX_SELECTED,
         wxCommandEventHandler(ImageFormatSelectionPartEvent::OnFormatSelection),
         NULL, pEventHandler_);
#endif

#ifdef __UNUSED_CODE__
#ifdef __LINUX__
   wxFilePickerCtrl* ppicker = XRCCTRL(*pToolWindow_, wxT("ID_FILENAME_SELECT"), wxFilePickerCtrl);
   wxTextCtrl* ptext = ppicker->GetTextCtrl();
   if (ptext != NULL)
      ptext->Disable();
#endif  // __LINUX__
#endif

   GET_CONTROL(*pToolWindow_, wxT("ID_DIALOG_BUTTON"), wxButton)->Connect(
         wxEVT_COMMAND_BUTTON_CLICKED,
         wxCommandEventHandler(ImageFormatSelectionPartEvent::OnButtonClicked), NULL,
         pEventHandler_);

   // Conecta el evento temporal
   pToolWindow_->Connect(
         wxEVT_UPDATE_UI,
         wxUpdateUIEventHandler(ImageFormatSelectionPartEvent::OnUiUpdate), NULL,
         pEventHandler_);

   // Carga los formatos disponibles para exportar
   ConfigureFormatList();
   return true;
}

/**
 * Obtiene la variable de configuracion con los formatos para exportar.
 * Si falla carga un formato por defecto. Guarda los formatos en la propiedad
 * suportedFormats_ para recuperarlos despues.
 */
void ImageFormatSelectionPart::ConfigureFormatList() {
   // Obtengo los formatos soportados para exportar y los cargo en la lista
   std::string formats = Configuration::GetParameter("lib_supported_export_formats",
                                                     wxT(DEFAULT_EXPORT_FORMAT));
   suportedFormats_ = tokenizer(formats, " ");
   std::vector<std::string>::iterator iterbegin = suportedFormats_.begin();
   std::vector<std::string>::iterator iterend = suportedFormats_.end();
   wxString format;
   int insertindex = 0;
   for (; iterbegin != iterend; iterbegin++) {
      format = *iterbegin;
      // Carga los formatos soportados de salida
      // El nombre del formato va dentro de la macro de traduccion
      GET_CONTROL(*pToolWindow_, wxT("ID_FORMAT_SELECT"), wxChoice)
         ->Insert(_(format), insertindex++);
      GET_CONTROL(*pToolWindow_, wxT("ID_FORMAT_SELECT"), wxChoice)->Select(0);
   }
   GET_CONTROL(*pToolWindow_, wxT("ID_EXPORT_METADATA_CHOICE"), wxChoice)->Insert(wxT(name_XML), 0);
   GET_CONTROL(*pToolWindow_, wxT("ID_EXPORT_METADATA_CHOICE"), wxChoice)->Select(0);
}

/**
 * Devuelve el estado de la parte
 * @return true cuando la parte esta habilitada
 * @return false cuado la parte esta desabilitada
 */
bool ImageFormatSelectionPart::IsEnabled() {
   return enabled_;
}

/**
 * Informa si la parte tiene cambios
 * @return true tiene cambios
 * @return false no tiene cambios
 */
bool ImageFormatSelectionPart::HasChanged() {
   return modified_;
}

/**
 * Si los datos de los controles wx son validos los guarda en las variables.
 * Si el archivo existe confirma sobreescritura.
 * Si no son validos muestra el mensaje de error que devuelve HasValidData.
 */
bool ImageFormatSelectionPart::CommitChanges() {
   if (!HasValidData()) {
      REPORT_ERROR(errorMessage_);
      return false;
   }
   if (modified_) {
      std::string filename = GetInternalFilename();
      if (wxFileName::IsFileWritable(filename)
            && ((SHOWQUERY(GetWindow(), question_OVERWRITE_FILE,
                  wxYES|wxNO|wxICON_QUESTION)) == wxID_NO)) {
         return false;
      }

      fileName_ = filename;
      format_ = GetInternalFormat();
      modified_ = false;
      if (pPal_) {
         SerializableString ss(fileName_);
         pPal_->AddSerializableAttribute<SerializableString>(
               ProcessAdaptLayer::OutputFileNameKeyAttr, ss);
         ss = SerializableString(format_);
         pPal_->AddSerializableAttribute<SerializableString>(
               ProcessAdaptLayer::OutputFileFormatKeyAttr, ss);
         bool exportmetadata = USE_CONTROL(*pToolWindow_, wxT("ID_EXPORT_METADATA_CHECKBOX"),
                                           wxCheckBox, GetValue(), false);
         pPal_->AddAttribute<bool>(ProcessAdaptLayer::ExportMetadataKeyAttr, exportmetadata);
      }
   }
   return true;
}

/**
 * Pone como el filename de archivo anterior.
 */
bool ImageFormatSelectionPart::RollbackChanges() {
   GET_CONTROL(*pToolWindow_, wxT("ID_FILENAME_SELECT"), wxTextCtrl)->SetValue(fileName_);
   GET_CONTROL(*pToolWindow_, wxT("ID_FORMAT_SELECT"), wxChoice)->Select(0);
   GET_CONTROL(*pToolWindow_, wxT("ID_EXPORT_METADATA_CHOICE"), wxChoice)->Select(0);
   return true;
}

/**
 * Pone en blanco el nombre el Filename seleccionado y hace un rollback.
 */
void ImageFormatSelectionPart::SetInitialValues() {
   fileName_.clear();
   format_ = suportedFormats_[0];
   RollbackChanges();
}

/**
 * Metodo que obtiene el nombre del archivo del PickerCtrl
 * @return nombre del archivo seleccionado
 */
std::string ImageFormatSelectionPart::GetFileName() {
   return fileName_;
}

/**
 * Metodo que obiente el formato de la imagen de salida del wxComboBox
 * @return formato de la imagen de salida.
 */
std::string ImageFormatSelectionPart::GetFormat() {
   return format_;
}

/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono de la ventana
 * @param[in] Y alto del icono de la ventana
 */
void ImageFormatSelectionPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_IMAGE_FORMAT_SELECTION, ToolBitmap);
}

#ifdef __UNUSED_CODE__
/**
 * Este codigo se ejecutaba ante eventos en la lista de formatos o en la de
 * seleccion de filename. Ya no es necesario.
 */

/**
 * Metodo que se ejecuta cuando se modifica la ruta o nombre de archivo
 */
void ImageFormatSelectionPart::OnFileSelection(wxFileDirPickerEvent &Event) {
   REPORT_DEBUG("E: Cambio el filename");
}

/**
 * Metodo que se ejecuta cuando se selecciona un formato para el archivo salida
 */
void ImageFormatSelectionPart::OnFormatSelection(wxCommandEvent &Event) {
   REPORT_DEBUG("E: Cambio el formato");
}
#endif

/**
 * Compara los valores de los controles wx con los valores de las propiedades,
 * y modifica el valor de la propiedad modified_
 * @param[in] Event evento UIUpdate
 */
void ImageFormatSelectionPart::OnUiUpdate(wxUpdateUIEvent &Event) {
   modified_ = false;

   if (fileName_.compare(GetInternalFilename()) != 0) {
      modified_ = true;
   }
   if (format_.compare(GetInternalFormat()) != 0) {
      modified_ = true;
   }

   Event.Skip();
}

/**
 * Abre un file dialog para configurar nombre y salida del archivo
 * @param[in] Event evento click sobre el boton
 */
void ImageFormatSelectionPart::OnButtonClicked(wxCommandEvent &Event) {
   wxTextCtrl* ptext = XRCCTRL(*pToolWindow_, wxT("ID_FILENAME_SELECT"), wxTextCtrl);
   std::string filename = ptext->GetValue().c_str();
   wxFileDialog dlg(NULL, _(caption_SAVE_AS), wxT(""), filename, extension_TIF,
   wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
   if (dlg.ShowModal() != wxID_OK)
      return;
   filename = dlg.GetPath();
   if (filename.find(".tif") == std::string::npos)
         filename += ".tif";
   ptext->SetValue(filename);
}

/**
 * Obtiene la el nombre y ruta de archivo seleccionado en el control
 * @return filename seleccionado en el control wx
 */
std::string ImageFormatSelectionPart::GetInternalFilename() {
   std::string filename;
   wxString selectpath = USE_CONTROL(*pToolWindow_, wxT("ID_FILENAME_SELECT"),
                                     wxTextCtrl, GetValue(), "");
   filename = selectpath.c_str();
   return filename;
}

/**
 * Obtiene el indice del formato seleccionado en el control
 * y obtiene el driver del vector de formatos
 * @return format el formato seleccionado o vacio.
 */
std::string ImageFormatSelectionPart::GetInternalFormat() {
   std::string format;

   int selectformat = USE_CONTROL(*pToolWindow_, "ID_FORMAT_SELECT", wxChoice,
         GetSelection(), wxNOT_FOUND);
   if (selectformat > wxNOT_FOUND) {
      format = suportedFormats_[selectformat];
   }

   return format;
}

/**
 * Verifica para el nombre, que no este vacio, que el directorio exista , y
 * verifica si el archivo existe y pregunta si sobreescribir.
 * Tambien verifica que haya algun formato de salida seleccionado.
 * @return true : filename y formato validos
 * @return false : filename o formato invalidos o en blanco
 */
bool ImageFormatSelectionPart::HasValidData() {
   bool result = true;

   wxFileName filename(GetInternalFilename());

   // Verifica que la ruta este completa y que el directorio exista
   if (!filename.IsAbsolute() || !wxFileName::DirExists(filename.GetPath())) {
      errorMessage_ = message_INVALID_DIRECTORY;
      return false;
   }
   // Verifica que el nombre de archivo no este vacio ni sea invalido
   if (filename.GetName().empty() || !IsValidFileName(filename.GetFullName())) {
      errorMessage_ = message_INVALID_FILENAME;
      return false;
   }

   // Si el archivo existe y tiene permisos de escritura, confirma sobreescritura
   if (filename.FileExists() && !filename.IsFileWritable()) {
      errorMessage_ = message_FILENAME_WITHOUT_PERMISSION;
      result = false;
   }
   // Si el archivo no existe verifica permisos de escritura sobre el directorio
   else if (!filename.IsDirWritable()) {
      errorMessage_ = message_DIRECTORY_WITHOUT_PERMISSION;
      result = false;
   }
   // Si no esta seleccionado el formato devuelve falso
   if (GetInternalFormat().empty()) {
      result = false;
   }

   return result;
}

/**
 * Obtiene los caracteres no validos para archivo y deirectorio
 * y los busca en el nombre de archivo.
 * @param[in] Filename: nombre de archivo.
 * @return true : si en el nombre no hay caracteres invalidos
 * @return false : si en el nombre hay caracteres invalidos
 */
bool ImageFormatSelectionPart::IsValidFileName(const wxString &Filename) {
   wxString invalidchars = wxFileName::GetForbiddenChars();
   for (size_t i = 0; i < invalidchars.length(); i++) {
      if (Filename.Contains(invalidchars.GetChar(i))) {
         return false;
      }
   }
   return true;
}
}

