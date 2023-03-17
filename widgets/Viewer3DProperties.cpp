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

// Includes standard

// Includes Suri
#include "Viewer3DProperties.h"
#include "suri/messages.h"
#include "suri/DynamicViewerWidget.h"
#include "suri/Viewer3D.h"
#include "suri/DataViewManager.h"
#include "suri/ToolSupport.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Configuration.h"

// Includes Wx
#include "wx/choice.h"
#include "wx/clrpicker.h"
#include "wx/colour.h"

// Includes App

// Defines
/** indice de la columna de Terreno en tabla de Calidad */
#define TERRAIN_COLUMN 0
/** indice de la columna de Textura en tabla de Calidad */
#define TEXTURE_COLUMN 1

namespace suri {

/** Tabla de conversion de valores de Calidad en Factor de Terreno y Textura */
int qualityTable[20][2] = { { 11, 11 }, { 11, 10 }, { 11, 9 }, { 11, 8 }, { 10, 8 }, { 9, 8 },
                            {  8,  8 }, {  7,  7 }, {  7, 6 }, {  7, 5 }, {  7, 4 }, { 6, 4 },
                            {  5,  4 }, {  4,  4 }, {  4, 3 }, {  4, 2 }, {  4, 1 }, { 3, 1 },
                            {  2,  1 }, {  1,  1 } };

/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(Viewer3DPropertiesEvent, Viewer3DProperties)
   IMPLEMENT_EVENT_CALLBACK(OnDemChange, OnDemChange(event), wxCommandEvent)
   IMPLEMENT_EVENT_CALLBACK(OnSliderScroll, OnSliderScroll(event), wxScrollEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */

/** Ctor */
Viewer3DProperties::Viewer3DProperties(ViewerWidget *pViewer, DataViewManager* pDataViewManager) :
      PartCollection(wxT("ID_VIEWER3D_PROPERTIES_PANEL"), caption_VIEWPORT_PROPERTIES),
      NEW_EVENT_OBJECT(Viewer3DPropertiesEvent),
      pViewer_(pViewer), pDataViewManager_(pDataViewManager) {
   windowTitle_ = _(caption_VIEWPORT_PROPERTIES);
}

/** Dtor */
Viewer3DProperties::~Viewer3DProperties() {
   DELETE_EVENT_OBJECT;
}

/** Indica si el Part tiene cambios para salvar. */
bool Viewer3DProperties::HasChanged() {
   return true;
}

/** Salva los cambios realizados en el Part. */
bool Viewer3DProperties::CommitChanges() {
   DynamicViewerWidget* p3d = dynamic_cast<DynamicViewerWidget*>(pViewer_);
   if (p3d && !p3d->GetDemId().empty()) {
      p3d->DeactivateViewerElements();
      SetParameters();
      p3d->ChangeVisualizationMode(DynamicViewerWidget::Z);
   } else {
      SHOW_ERROR(message_MUST_SELECT_TEXTURE);
      return false;
   }
   return PartCollection::CommitChanges();
}

/** Restaura los valores originales del Part. */
bool Viewer3DProperties::RollbackChanges() {
   return PartCollection::RollbackChanges();
}

/** Inicializa el part */
void Viewer3DProperties::SetInitialValues() {
}

bool Viewer3DProperties::ConfigureWidget() {
   PartCollection::ConfigureWidget();
   wxWindow* pwidgetwindow = GetWidget()->GetWindow();
   DynamicViewerWidget* p3d = dynamic_cast<DynamicViewerWidget*>(pViewer_);

   int value = Configuration::GetParameter("v3d_quality", 1);
   GET_CONTROL(*pwidgetwindow, "ID_SLIDER_QUALITY", wxSlider)->SetValue(value);
   GET_CONTROL(*pwidgetwindow, "ID_QUAL_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(value).c_str());

   value = Configuration::GetParameter("v3d_exageration", 1);
   GET_CONTROL(*pwidgetwindow, "ID_SLIDER_EXAGERATION", wxSlider)->SetValue(value);
   GET_CONTROL(*pwidgetwindow, "ID_EXAG_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(value).c_str());

   value = Configuration::GetParameter("v3d_camera", 1);
   GET_CONTROL(*pwidgetwindow, "ID_SLIDER_CAMERA", wxSlider)->SetValue(value);
   GET_CONTROL(*pwidgetwindow, "ID_VEL_CAM_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(value).c_str());

   GET_CONTROL(*pwidgetwindow, "ID_SLIDER_QUALITY", wxSlider)->Connect(wxEVT_SCROLL_CHANGED,
         wxScrollEventHandler(Viewer3DPropertiesEvent::OnSliderScroll), NULL, pEventHandler_);
   GET_CONTROL(*pwidgetwindow, "ID_SLIDER_EXAGERATION", wxSlider)->Connect(wxEVT_SCROLL_CHANGED,
         wxScrollEventHandler(Viewer3DPropertiesEvent::OnSliderScroll), NULL, pEventHandler_);
   GET_CONTROL(*pwidgetwindow, "ID_SLIDER_CAMERA", wxSlider)->Connect(wxEVT_SCROLL_CHANGED,
         wxScrollEventHandler(Viewer3DPropertiesEvent::OnSliderScroll), NULL, pEventHandler_);

   GET_CONTROL(*pwidgetwindow, wxT("ID_MDE_CHOICE"), wxChoice)->Connect(
            wxEVT_COMMAND_CHOICE_SELECTED,
            wxCommandEventHandler(Viewer3DPropertiesEvent::OnDemChange), NULL,
            pEventHandler_);

   wxColour colour(Configuration::GetParameterEx("v3d_color", "#FFFFFF"));
   XRCCTRL(*pwidgetwindow, "ID_BACKGROUND_COLOURCTRL", wxColourPickerCtrl)->SetColour(colour);

   dems_.clear();
   wxChoice* mdechoice = XRCCTRL(*pwidgetwindow, wxT("ID_MDE_CHOICE"), wxChoice);
   LayerList* playerlist = ToolSupport::GetLayerList(pDataViewManager_->GetViewcontextManager());

   int index = -1;
   for (int i = 0, x = 0; i < playerlist->GetElementCount(); ++i) {
      RasterElement* pelement = dynamic_cast<RasterElement*>(playerlist->GetElement(i));
      if (pelement && pelement->GetBandCount() == 1) {
         if (pelement->GetUid() == p3d->GetDemId())
            index = x;
         dems_.push_back(pelement);
         mdechoice->Append(pelement->GetName());
         ++x;
      }
   }

   if (!mdechoice->IsEmpty()) {
      if (p3d->GetDemId().empty()) {
         mdechoice->Select(0);
         p3d->ChangeDem(dems_[0]->GetUid());
      }
      else
         mdechoice->Select(index);
   }

   return true;
}

void Viewer3DProperties::OnDemChange(wxCommandEvent &Event) {
   wxWindow* pwidgetwindow = GetWidget()->GetWindow();
   wxChoice* mdechoice = XRCCTRL(*pwidgetwindow, wxT("ID_MDE_CHOICE"), wxChoice);
   int index = mdechoice->GetSelection();
   DynamicViewerWidget* p3d = dynamic_cast<DynamicViewerWidget*>(pViewer_);
   if (p3d)
      p3d->ChangeDem(dems_[index]->GetUid());
}

/** Actualiza el estado de la parte */
void Viewer3DProperties::Update() {
}

/** Evento de movimiento de sliders */
void Viewer3DProperties::OnSliderScroll(wxScrollEvent &Event) {
   wxWindow* pwidgetwindow = GetWidget()->GetWindow();
   int value = USE_CONTROL(*pwidgetwindow, "ID_SLIDER_QUALITY", wxSlider, GetValue(), 1);
   GET_CONTROL(*pwidgetwindow, "ID_QUAL_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(value).c_str());
   value = USE_CONTROL(*pwidgetwindow, "ID_SLIDER_EXAGERATION", wxSlider, GetValue(), 1);
   GET_CONTROL(*pwidgetwindow, "ID_EXAG_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(value).c_str());
   value = USE_CONTROL(*pwidgetwindow, "ID_SLIDER_CAMERA", wxSlider, GetValue(), 1);
   GET_CONTROL(*pwidgetwindow, "ID_VEL_CAM_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(value).c_str());
}


void Viewer3DProperties::SetParameters() {
   wxWindow* pwidgetwindow = GetWidget()->GetWindow();
   long value = USE_CONTROL(*pwidgetwindow, "ID_SLIDER_EXAGERATION", wxSlider, GetValue(), 1);
   Configuration::SetParameterEx("v3d_exageracion", value);
   value = USE_CONTROL(*pwidgetwindow, "ID_SLIDER_CAMERA", wxSlider, GetValue(), 1);
   Configuration::SetParameterEx("v3d_velocidad_camara", value);
   value = USE_CONTROL(*pwidgetwindow, "ID_SLIDER_QUALITY", wxSlider, GetValue(), 1);
   value--;
   Configuration::SetParameterEx("v3d_performance", value);
   long terrainfactor = qualityTable[0][TERRAIN_COLUMN];
   long texturefactor = qualityTable[0][TEXTURE_COLUMN];
   int qualitycount = ARRAY_SIZE(qualityTable);

   if (value >= 0 && value < qualitycount) {
      terrainfactor = qualityTable[value][TERRAIN_COLUMN];
      texturefactor = qualityTable[value][TEXTURE_COLUMN];
   }

   Configuration::SetParameterEx("v3d_factor_terreno", terrainfactor);
   Configuration::SetParameterEx("v3d_factor_textura", texturefactor);

   wxColour colour = USE_CONTROL(*pwidgetwindow, "ID_BACKGROUND_COLOURCTRL",
                                 wxColourPickerCtrl, GetColour(), wxNullColour);

   Configuration::SetParameterEx("v3d_color", colour.GetAsString(wxC2S_HTML_SYNTAX).c_str());
}
}
