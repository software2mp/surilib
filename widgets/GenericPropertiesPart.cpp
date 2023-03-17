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

// Includes estandar
#include <string>

// Includes suri
#include "GenericPropertiesPart.h"
#include "suri/messages.h"
#include "suri/xmlnames.h"
#include "resources.h"
#include "suri/DatasourceManagerInterface.h"
#include "suri/DatasourceInterface.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/filedlg.h"

// Defines

#ifndef ICON_MAX_SIZE
/** Size maximo de archivo permitido */
#  define ICON_MAX_SIZE    10485760
#endif

#ifndef ICON_WIDTH
/** Ancho de los iconos (valor al cual se escalan) */
#  define ICON_WIDTH       32
#endif

#ifndef ICON_HEIGHT
/** Altura de los iconos (valor al cual se escalan) */
#  define ICON_HEIGHT      32
#endif

/** Inicializa los recursos de */
void InitGenericPropertiesXmlResource();

/** namespace suri */
namespace suri {

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(GenericPropertiesPartEvent, GenericPropertiesPart)
   IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
   IMPLEMENT_EVENT_CALLBACK(OnChangeIconButtonPressed, OnChangeIconButtonPressed(), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/**
 * Ctor. default.
 * @param[in] pElement elemento del que se quieren conocer las propiedades
 */
GenericPropertiesPart::GenericPropertiesPart(Element *pElement) :
      pElement_(pElement), NEW_EVENT_OBJECT(GenericPropertiesPartEvent), pDatasourceManager_(NULL),
      pDatasource_(NULL) {
   windowTitle_ = _(caption_GENERAL_PROPERTIES);
}

/**
 * Ctor
 *
 */
GenericPropertiesPart::GenericPropertiesPart(DatasourceManagerInterface* pDatasourceManager,
                                             DatasourceInterface* pDatasource) :
                                             pElement_(NULL),
                                             NEW_EVENT_OBJECT(GenericPropertiesPartEvent),
                                             pDatasourceManager_(pDatasourceManager),
      pDatasource_(pDatasource) {
   windowTitle_ = _(caption_GENERAL_PROPERTIES);
}

/**
 * Dtor.
 */
GenericPropertiesPart::~GenericPropertiesPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Se crea la ventana de herramienta en las clases derivadas
 * @return true siempre
 */
bool GenericPropertiesPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_PANEL_GENPROPERTIES"));
   GET_CONTROL(*pToolWindow_, "ID_TXT_NAME", wxTextCtrl)
      ->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(
            GenericPropertiesPartEvent::
            OnUIUpdate),
                NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_TXT_COPY", wxTextCtrl)
      ->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(
            GenericPropertiesPartEvent::
            OnUIUpdate),
                NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_TXT_DESC", wxTextCtrl)
      ->Connect(wxEVT_UPDATE_UI, wxUpdateUIEventHandler(
            GenericPropertiesPartEvent::
            OnUIUpdate),
                NULL, pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_BITMAPBUTTON", wxBitmapButton)
      ->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(
            GenericPropertiesPartEvent::
            OnChangeIconButtonPressed),
                NULL, pEventHandler_);
   modified_ = false;
   return true;
}

/**
 * Se crean los controles para las propiedades del elemento
 */
void GenericPropertiesPart::SetInitialValues() {
   Element* pelement = (pDatasource_)?pDatasource_->GetElement():pElement_;
   GET_CONTROL(*pToolWindow_, "ID_TXT_NAME", wxTextCtrl)
      ->SetValue(pelement->GetName());
   GET_CONTROL(*pToolWindow_, "ID_TXT_COPY", wxTextCtrl)
      ->SetValue(pelement->GetCopyRight());
   GET_CONTROL(*pToolWindow_, "ID_LBL_URL", wxStaticText)
      ->SetLabel(pelement->GetUrl());
   GET_CONTROL(*pToolWindow_, "ID_LBL_URL", wxStaticText)->Wrap(320);
   /* convierto la 1er letra a mayuscula porque hay elementos que devuelven el
    * tipo sin reimplementar el metodo XmlElement::GetTypeAsString */
   wxString type = pelement->GetFormatDescription();
   type = type.Mid(0, 1).Upper() + type.Mid(1);
   GET_CONTROL(*pToolWindow_, "ID_LBL_TYPE", wxStaticText)->SetLabel(type);
   GET_CONTROL(*pToolWindow_, "ID_LBL_SIZE", wxStaticText)
      ->SetLabel(pelement->GetSizeAsString());
   GET_CONTROL(*pToolWindow_, "ID_TXT_DESC", wxTextCtrl)->
                                       SetValue(pelement->GetDescription());
   /* Logica del Icono */
   selectedIcon_ = pelement->GetIcon();
   wxBitmap bitmap;
   GET_BITMAP_RESOURCE(selectedIcon_.c_str(), bitmap);
   ESCALE_BITMAP(bitmap, ICON_WIDTH, ICON_HEIGHT);
   GET_CONTROL(*pToolWindow_, "ID_BITMAPBUTTON", wxBitmapButton)
      ->SetBitmapLabel(bitmap);
   modified_ = false;
}

/**
 * Indica si el Part tiene cambios para salvar.
 * @return true si hay cambios, false en otro caso
 */
bool GenericPropertiesPart::HasChanged() {
   wxString value = wxEmptyString;
   Element* pelement = (pDatasource_)?pDatasource_->GetElement():pElement_;
   value = USE_CONTROL(*pToolWindow_, "ID_TXT_NAME", wxTextCtrl, GetValue(), "");
   if (value.Cmp(pelement->GetName().c_str()) != 0 && modified_) {
      return true;
   }
   value = USE_CONTROL(*pToolWindow_, "ID_TXT_COPY", wxTextCtrl, GetValue(), "");
   if (value.Cmp(pelement->GetCopyRight().c_str()) != 0 && modified_) {
      return true;
   }
   value = USE_CONTROL(*pToolWindow_, "ID_TXT_DESC", wxTextCtrl, GetValue(), "");
   if (value.Cmp(pelement->GetDescription().c_str()) != 0 && modified_) {
      return true;
   }
   if (selectedIcon_.CmpNoCase(pelement->GetIcon()) != 0 && modified_) {
      return true;
   }
   return false;
}

/**
 * Salva los cambios realizados en el Part.
 * @return true siempre
 */
bool GenericPropertiesPart::CommitChanges() {
   wxString value = wxEmptyString;
   value = USE_CONTROL(*pToolWindow_, "ID_TXT_NAME", wxTextCtrl, GetValue(), "");
   if (pElement_) {
      pElement_->SetName(value);
   } else {
      const std::string datasourceId = pDatasource_->GetId();
      std::string stringvalue = WS2S(value);
      pDatasourceManager_->ChangeDatasourceName(datasourceId, stringvalue);
   }
   value = USE_CONTROL(*pToolWindow_, "ID_TXT_COPY", wxTextCtrl, GetValue(), "");
   if (pElement_) {
      pElement_->SetCopyRight(value);
   } else {
      const std::string datasourceId = pDatasource_->GetId();
      std::string stringvalue = WS2S(value);
      pDatasourceManager_->ChangeDatasourceCopyRight(datasourceId, stringvalue);
   }
   value = USE_CONTROL(*pToolWindow_, "ID_TXT_DESC", wxTextCtrl, GetValue(), "");
   if (pElement_) {
      pElement_->SetDescription(value);
   } else {
      const std::string datasourceId = pDatasource_->GetId();
      std::string stringvalue = WS2S(value);
      pDatasourceManager_->ChangeDatasourceDescription(datasourceId, stringvalue);
   }

   // El control de tipo static text es solo de lectura, al formatear el texto del url para que
   // entre en la ventana, se agregaban \n, por lo que el URL era modificado.
#ifdef __UNUSED_CODE__
   value = USE_CONTROL(*pToolWindow_, "ID_LBL_URL", wxStaticText, GetLabel(), "");
   pelement->SetUrl(value, pelement->GetNode(wxT("")));
#endif   // __UNUSED_CODE__
   if (pElement_) {
      pElement_->SetIcon(selectedIcon_);
   } else {
      const std::string datasourceId = pDatasource_->GetId();
      std::string stringvalue = WS2S(selectedIcon_);
      pDatasourceManager_->ChangeDatasourceIcon(datasourceId, stringvalue);
   }
   Element* pelement = (pDatasource_)? pDatasource_->GetElement(): pElement_;
   pelement->SetChanged();
   pelement->SendViewerUpdate();
   GET_CONTROL(*pToolWindow_, "ID_TXT_NAME", wxTextCtrl)->DiscardEdits();
   GET_CONTROL(*pToolWindow_, "ID_TXT_COPY", wxTextCtrl)->DiscardEdits();
   GET_CONTROL(*pToolWindow_, "ID_TXT_DESC", wxTextCtrl)->DiscardEdits();
   modified_ = false;
   return true;
}

/**
 * Restaura los valores originales del Part.
 * @return true siempre
 */
bool GenericPropertiesPart::RollbackChanges() {
   SetInitialValues();
   return true;
}

/**
 * Objetivo: Se implementa este Evento para que al modificar algun dato que se
 * muestra en este Part, desde otro Part, el Boton de Aplicar no quede
 * habilitado aun despues de haber sido presionado.(Ver HasChanged)
 * NOTA: debe implemetarse de la misma forma en todos los Part que compartan
 * informacion y la expongan mediante algun control.
 * @param[in] Event: Evento.
 */
void GenericPropertiesPart::OnUIUpdate(wxUpdateUIEvent &Event) {
   bool modname =
         USE_CONTROL(*pToolWindow_, "ID_TXT_NAME", wxTextCtrl, IsModified(), false);
   bool modcopy =
         USE_CONTROL(*pToolWindow_, "ID_TXT_COPY", wxTextCtrl, IsModified(), false);
   bool moddesc =
         USE_CONTROL(*pToolWindow_, "ID_TXT_DESC", wxTextCtrl, IsModified(), false);
   Element* pelement = (pDatasource_)?pDatasource_->GetElement():pElement_;
   bool modbitm = (selectedIcon_.CmpNoCase(pelement->GetIcon()) != 0);
   modified_ = modname || modcopy || moddesc || modbitm;
   Event.SetUpdateInterval(100);
   Event.Skip();
}

/**
 * Evento que se dispara al presionar el boton para el cambio de icono.
 * Despliega dialogo de apertura de archivos para insertar Elementos en los
 * Contextos.
 */
void GenericPropertiesPart::OnChangeIconButtonPressed() {
   wxFileDialog filedialog(GetFrame(),
                           _(caption_SELECT_IMAGE), wxEmptyString, wxEmptyString,
                           BUILD_FILTER(name_COMMON_IMAGE_FILES,
                                        filter_extension_COMMON_IMAGE),
                           wxFD_OPEN);

   if (filedialog.ShowModal() == wxID_OK) {
      wxString filepath = filedialog.GetPath();
      wxULongLong size = wxFileName::GetSize(filepath);
      if (size != wxInvalidSize && 0 < size && size < ICON_MAX_SIZE) {
         wxBitmap bitmap;
         GET_BITMAP_RESOURCE(filepath.c_str(), bitmap);
         ESCALE_BITMAP(bitmap, ICON_WIDTH, ICON_HEIGHT);
         GET_CONTROL(*pToolWindow_, "ID_BITMAPBUTTON", wxBitmapButton)
            ->SetBitmapLabel(bitmap);
         selectedIcon_ = filepath;
         modified_ = true;
      } else {
         SHOW_WARNING(message_IMAGE_TOO_BIG);
         modified_ = false;
      }
   } else {
      modified_ = false;
   }
}

/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 * @param[out] ToolBitmap icono de la ventana
 * @param[in] X ancho del icono de la ventana
 * @param[in] Y alto del icono de la ventana
 */
void GenericPropertiesPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_GENERIC_PROPERTIES, ToolBitmap);
}

}  // namespace suri

// ------------------------------------------------------------------------------
// ----------------------------Fin GenericPropertiesPart-----------------------------
// ------------------------------------------------------------------------------
