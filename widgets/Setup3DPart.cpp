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

#include "Setup3DPart.h"
#include "resources.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/Configuration.h"

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(Setup3DPartEvent, Setup3DPart)
      IMPLEMENT_EVENT_CALLBACK(OnSliderScroll, OnSliderScroll(event), wxScrollEvent)
END_IMPLEMENT_EVENT_OBJECT/** \endcond */

/** Ctor. */
Setup3DPart::Setup3DPart() : NEW_EVENT_OBJECT(Setup3DPartEvent){
   windowTitle_ = wxT(window_title_3D_CONFIGURATION);
}
/** Destructor- */
Setup3DPart::~Setup3DPart() {
   // TODO Auto-generated destructor stub
}

/** Crea el part (el interior de la ventana)*/
bool Setup3DPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_PANEL_TERRAIN_PROP"));
   modified_ = false;
   return true;
}
/** Asigna el icono para el part.  */
void Setup3DPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_GENERIC_PROPERTIES, ToolBitmap); // Agregar el icono indicado.
}
/** Inicializa los controles */
void Setup3DPart::SetInitialValues() {
   iquality_ = Configuration::GetParameter(std::string("v3d_quality"), 1);
   iexageration_ = Configuration::GetParameter(std::string("v3d_exageration"), 1);
   icamera_ = Configuration::GetParameter(std::string("v3d_camera"), 1);
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider)
      ->SetValue(iquality_);
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider)
      ->SetValue(iexageration_);
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider)
      ->SetValue(icamera_);
   // Conectamos
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider)->Connect(
         wxEVT_SCROLL_CHANGED,
         wxScrollEventHandler(Setup3DPartEvent::OnSliderScroll), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider)->Connect(
         wxEVT_SCROLL_CHANGED,
         wxScrollEventHandler(Setup3DPartEvent::OnSliderScroll), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider)->Connect(
         wxEVT_SCROLL_CHANGED,
         wxScrollEventHandler(Setup3DPartEvent::OnSliderScroll), NULL,
         pEventHandler_);
   // Seteamos los labels
   GET_CONTROL(*pToolWindow_, "ID_QUAL_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(iquality_).c_str());
   GET_CONTROL(*pToolWindow_, "ID_EXAG_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(iexageration_).c_str());
   GET_CONTROL(*pToolWindow_, "ID_VEL_CAM_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(icamera_).c_str());
   modified_ = false;
}
/** Indica si el Part tiene cambios para salvar. */
bool Setup3DPart::HasChanged() {
   int value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider, GetValue(), 1);
   if (value != iquality_) {
      return true;
   }
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider,
         GetValue(), 1);
   if (value != iexageration_) {
      return true;
   }
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider, GetValue(), 1);
   if (value != icamera_) {
      return true;
   }
   return false;
}
/** Salva los cambios realizados en el Part. */
bool Setup3DPart::CommitChanges() {
   // obtengo valor del slider de calidad
   int value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider, GetValue(), 1);
   Configuration::SetParameter(std::string("v3d_quality"), long(value));


   // obtengo valor del slider de exageracion.
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider,
         GetValue(), 1);
   // seteo factor de exageracion del terreno.
   Configuration::SetParameter(std::string("v3d_exageration"), long(value));

   // obtengo valor del slider de velocidad de camara.
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider, GetValue(), 1);
   // seteo paso de camara.
   Configuration::SetParameter(std::string("v3d_camera"), long(value));

   modified_ = false;
   return true;
}
/** Restaura los valores originales del Part. */
bool Setup3DPart::RollbackChanges() {
   SetInitialValues();
   return true;
}
/** Actualiza el estado de la parte */
void Setup3DPart::Update() {

}
/** Evento de movimiento de sliders */
void Setup3DPart::OnSliderScroll(wxScrollEvent &Event) {
   int value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider, GetValue(), 1);
   GET_CONTROL(*pToolWindow_, "ID_QUAL_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(value).c_str());
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider,
                       GetValue(), 1);
   GET_CONTROL(*pToolWindow_, "ID_EXAG_VALUE_LABEL", wxStaticText)->SetLabel(
               NumberToString<int>(value).c_str());
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider,
                          GetValue(), 1);
   GET_CONTROL(*pToolWindow_, "ID_VEL_CAM_VALUE_LABEL", wxStaticText)->SetLabel(
                NumberToString<int>(value).c_str());
}


} /* namespace suri */
