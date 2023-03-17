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

#include "SetupGeneralPart.h"
#include "resources.h"
#include "suri/messages.h"
#include "suri/Configuration.h"
#include "suri/PartContainerWidget.h"

// Includes wx
#include "wx/xrc/xmlres.h"
#include "wx/wx.h"
#include "wx/choice.h"
#include "wx/spinctrl.h"
#include "wx/filepicker.h"

void InitSetupGeneralXmlResource();

#define IDIOMA_ "Espaniol"

namespace suri {

SetupGeneralPart::SetupGeneralPart(bool restrictedmode) :
      restrictedmode_(restrictedmode) {
   windowTitle_ = wxT(caption_GENERAL_PROPERTIES);
}

SetupGeneralPart::~SetupGeneralPart() {
}

/** Crea el part (el interior de la ventana)*/
bool SetupGeneralPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_, wxT("ID_GENERAL_SETUP"));

   if (!pToolWindow_)
      return false;

   modified_ = false;
   return true;
}
/** Asigna el icono para el part.  */
void SetupGeneralPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_GENERIC_PROPERTIES, ToolBitmap);
}
/** Inicializa los controles */
void SetupGeneralPart::SetInitialValues() {
   int selection;
   wxString aux;
   language_ = _(content_LANGUAGE_SPANISH);
   workingdirectory_ = Configuration::GetParameter("app_working_dir", "");
   visualizationdirectory_ = Configuration::GetParameter("app_visualization_dir", "");
   zoomfactor_ = Configuration::GetParameter(std::string("app_zoom_modifier"), 0);
   selection = USE_CONTROL(*pToolWindow_, "ID_CMB_GEO_COORD", wxChoice, GetSelection(),
                           0);
   aux = USE_CONTROL(*pToolWindow_, "ID_CMB_GEO_COORD", wxChoice, GetString(selection),
                     wxT(message_NOT_APPLICABLE));
   coordgeo_ = Configuration::GetParameter("app_format_coords_geo", aux);
   selection = USE_CONTROL(*pToolWindow_, "ID_CMB_MTS_COORD", wxChoice,
                           GetSelection(),  0);
   aux =  USE_CONTROL(*pToolWindow_, "ID_CMB_MTS_COORD", wxChoice,
                               GetString(selection),  wxT(message_NOT_APPLICABLE));
   coordmts_ = Configuration::GetParameter("app_format_coords_mts", aux);
   if( workingdirectory_.IsEmpty() )
      workingdirectory_ = wxT(message_NO_DEFINED);
   if( visualizationdirectory_.IsEmpty() )
      visualizationdirectory_ = wxT(message_NO_DEFINED);

   GET_CONTROL(*pToolWindow_,"ID_CMB_IDIOMA", wxChoice)->Append(_(content_LANGUAGE_SPANISH));
   GET_CONTROL(*pToolWindow_,"ID_CMB_IDIOMA", wxChoice)->SetSelection(0);
   GET_CONTROL(*pToolWindow_,
            "ID_WORKINGDIRPICKERCTRL", wxDirPickerCtrl)->SetPath(workingdirectory_);
   GET_CONTROL(*pToolWindow_,
            "ID_VISUALDIRPICKERCTRL", wxDirPickerCtrl)->SetPath(visualizationdirectory_);
   GET_CONTROL(*pToolWindow_, "ID_SPINCTRL_ZOOM", wxSpinCtrl)->SetValue(zoomfactor_);

   if ( restrictedmode_)
      GET_CONTROL(*pToolWindow_,
                 "ID_NAVIGATION_PANEL", wxPanel)->Show(false);

   modified_ = false;
}
/** Indica si el Part tiene cambios para salvar. */
bool SetupGeneralPart::HasChanged() {
   wxString value;
   // Chequeo si cambio el lenguaje.
   int tmpvalue = USE_CONTROL(*pToolWindow_, "ID_CMB_IDIOMA", wxChoice, GetSelection(),
         -1);
   if (tmpvalue >= 0) {
      value = USE_CONTROL(*pToolWindow_, "ID_CMB_IDIOMA", wxChoice, GetString(tmpvalue),
            wxT(message_NOT_APPLICABLE));
      if (value.Cmp(language_) != 0) return true;
   }
   // Chequeo si cambi0 directorio de trabajo.
   value = USE_CONTROL(*pToolWindow_, "ID_WORKINGDIRPICKERCTRL",
         wxDirPickerCtrl, GetPath(),  wxT(message_NOT_APPLICABLE));
   if (value.Cmp(workingdirectory_) != 0) return true;

   // Chequeo si cambio el directorio de salvado de las visualizaciones.
   value = USE_CONTROL(*pToolWindow_, "ID_VISUALDIRPICKERCTRL", wxDirPickerCtrl,
         GetPath(), wxT(message_NOT_APPLICABLE));
   if (value.Cmp(visualizationdirectory_) != 0) return true;

   // Chequeo si cambio el ZOOM.
   long lvalue = USE_CONTROL(*pToolWindow_, "ID_SPINCTRL_ZOOM", wxSpinCtrl,
         GetValue(), 0);
   if ((int) lvalue != zoomfactor_) return true;

   // Chequeo si cambio el formato de las coordenadas en mts.
   tmpvalue = USE_CONTROL(*pToolWindow_, "ID_CMB_MTS_COORD", wxChoice,
         GetSelection(),  0);
   value = USE_CONTROL(*pToolWindow_, "ID_CMB_MTS_COORD", wxChoice,
         GetString(tmpvalue), wxT(message_NOT_APPLICABLE));
   if (value.Cmp(coordmts_) != 0) return true;

   // Chequeo si cambio el formato de las coordenadas geo.
   tmpvalue = USE_CONTROL(*pToolWindow_, "ID_CMB_GEO_COORD", wxChoice, GetSelection(),
                          0);
   value = USE_CONTROL(*pToolWindow_, "ID_CMB_GEO_COORD", wxChoice, GetString(tmpvalue),
                       wxT(message_NOT_APPLICABLE));
   if (value.Cmp(coordgeo_) != 0) return true;

   return false;
}
/** Salva los cambios realizados en el Part. */
bool SetupGeneralPart::CommitChanges() {

   int tmpvalue = USE_CONTROL(*pToolWindow_, "ID_CMB_IDIOMA", wxChoice, GetSelection(), -1);
   wxString value = USE_CONTROL(*pToolWindow_, "ID_CMB_IDIOMA", wxChoice, GetString(tmpvalue), "");
   Configuration::SetParameter("app_language", value.c_str());

   value = USE_CONTROL(*pToolWindow_, "ID_WORKINGDIRPICKERCTRL", wxDirPickerCtrl, GetPath(), "");
   Configuration::SetParameter("app_working_dir", value.c_str());
   workingdirectory_ = value.c_str();

   value = USE_CONTROL(*pToolWindow_, "ID_VISUALDIRPICKERCTRL", wxDirPickerCtrl, GetPath(), "");
   Configuration::SetParameter("app_visualization_dir", value.c_str());
   visualizationdirectory_ = value.c_str();

   long lvalue = USE_CONTROL(*pToolWindow_, "ID_SPINCTRL_ZOOM", wxSpinCtrl, GetValue(), -1);
   Configuration::SetParameter("app_zoom_modifier", lvalue);
   zoomfactor_ = lvalue;

   tmpvalue = USE_CONTROL(*pToolWindow_, "ID_CMB_GEO_COORD", wxChoice, GetSelection(), -1);
   value = USE_CONTROL(*pToolWindow_, "ID_CMB_GEO_COORD", wxChoice, GetString(tmpvalue), "");
   Configuration::SetParameter("app_geo_coords", value.c_str());
   coordgeo_ = value.c_str();

   tmpvalue = USE_CONTROL(*pToolWindow_, "ID_CMB_MTS_COORD", wxChoice, GetSelection(), -1);
   value = USE_CONTROL(*pToolWindow_, "ID_CMB_MTS_COORD", wxChoice, GetString(tmpvalue), "");
   Configuration::SetParameter("app_mts_coords", value.c_str());
   coordmts_ = value.c_str();

   modified_ = false;
   return true;
}
/** Restaura los valores originales del Part. */
bool SetupGeneralPart::RollbackChanges() {
   SetInitialValues();
   return true;
}
/** Actualiza el estado de la parte */
void SetupGeneralPart::Update() {

}
} /* namespace suri */
