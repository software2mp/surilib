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

// Includes suri
#include "suri/messages.h"
#include "resources.h"
#include "TerrainPropertiesPart.h"
#include "suri/AuxiliaryFunctions.h"

// includes wx
#include "wx/xrc/xmlres.h"

// Defines

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(TerrainPropertiesPartEvent, TerrainPropertiesPart)
//	IMPLEMENT_EVENT_CALLBACK(OnUIUpdate, OnUIUpdate(event), wxUpdateUIEvent)
      IMPLEMENT_EVENT_CALLBACK(OnSliderScroll, OnSliderScroll(event), wxScrollEvent)
END_IMPLEMENT_EVENT_OBJECT/** \endcond */
/* Ctor. default.
 * @param[in] pElement elemento que quiero editar
 * @return instancia de la clase TerrainPropertiesPart
 */
TerrainPropertiesPart::TerrainPropertiesPart(TerrainElement *pElement) :
      pElement_(pElement), NEW_EVENT_OBJECT(TerrainPropertiesPartEvent)
{
   windowTitle_ = _(caption_TERRAIN_PROPERTIES);
}

/** Destructor */
TerrainPropertiesPart::~TerrainPropertiesPart() {
   DELETE_EVENT_OBJECT
   ;
}

/* Crea panel para propiedades del terreno
 * @return true.
 */
bool TerrainPropertiesPart::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_PANEL_TERRAIN_PROP"));
   return true;
}

/** Inicializa los controles */
void TerrainPropertiesPart::SetInitialValues() {
   if (!pElement_) {
      return;
   }
   int quality = pElement_->GetQuality();
   int exageration = pElement_->GetExagerationFactor();
   int camstep = pElement_->GetCameraStep();

   GET_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider)
      ->SetValue(quality);
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider)
      ->SetValue(exageration);
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider)
      ->SetValue(camstep);
   // Conexion a eventos de scroll
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider)->Connect(
         wxEVT_SCROLL_CHANGED,
         wxScrollEventHandler(TerrainPropertiesPartEvent::OnSliderScroll), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider)->Connect(
         wxEVT_SCROLL_CHANGED,
         wxScrollEventHandler(TerrainPropertiesPartEvent::OnSliderScroll), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider)->Connect(
         wxEVT_SCROLL_CHANGED,
         wxScrollEventHandler(TerrainPropertiesPartEvent::OnSliderScroll), NULL,
         pEventHandler_);
   GET_CONTROL(*pToolWindow_, "ID_QUAL_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(quality).c_str());
   GET_CONTROL(*pToolWindow_, "ID_EXAG_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(exageration).c_str());
   GET_CONTROL(*pToolWindow_, "ID_VEL_CAM_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(camstep).c_str());
}

/**
 * Indica si el Part tiene cambios para salvar.
 * Verifica si cambio el factor, la exageracion o la velocidad de la camara
 * @return true en caso de que existan cambios.
 * @return false en caso de que no existan cambios.
 */
bool TerrainPropertiesPart::HasChanged() {
   if (!pElement_) {
      return false;
   }
   int value = 0;
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider, GetValue(), 0);

   if (value != pElement_->GetQuality()) {
      return true;
   }

   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider,
         GetValue(), 0);
   if (value != pElement_->GetExagerationFactor()) {
      return true;
   }

   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider, GetValue(), 0);
   if (value != pElement_->GetCameraStep()) {
      return true;
   }

   return false;
}

/* Salva los cambios realizados del Part en el elemento.
 * @return true en caso de poder salvar los cambios
 * @return false en caso de no poder salvar los cambios
 */
bool TerrainPropertiesPart::CommitChanges() {
   if (!pElement_) {
      return false;
   }
   int value = 0;

   // obtengo valor del slider de calidad
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider, GetValue(), 0);
   value = std::max(0, value);
   value = std::min(pElement_->GetQualityCount() - 1, value);

   // seteo factor de terreno y textura en el elemento.
   pElement_->SetQuality(value);

   // obtengo valor del slider de exageracion.
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider,
         GetValue(), 0);
   // seteo factor de exageracion del terreno.
   pElement_->SetExagerationFactor((size_t) (value));

   // obtengo valor del slider de velocidad de camara.
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider, GetValue(), 0);
   // seteo paso de camara.
   pElement_->SetCameraStep((size_t) (value));

   pElement_->SetChanged();
   pElement_->SendViewerUpdate();
   modified_ = false;
   return true;
}

/* Restaura los valores originales del Part.
 * @return true.
 */
bool TerrainPropertiesPart::RollbackChanges() {
   SetInitialValues();
   return true;
}

/**
 * Objetivo: Devuelve el icono de propiedades
 * Por ahora no tiene en cuenta el X e Y dado que devuelve el de 32x32 pixels
 * @param[in] X ancho del icono.
 * @param[in] Y alto del icono.
 * @param[out] ToolBitmap icono a devolver.
 */
void TerrainPropertiesPart::GetWindowIcon(wxBitmap& ToolBitmap, int X, int Y) const {
   GET_BITMAP_RESOURCE(icon_ELEMENT_TERRAIN_PROPERTIES, ToolBitmap);
}
/** Evento de movimiento de sliders */
void TerrainPropertiesPart::OnSliderScroll(wxScrollEvent &Event) {
   int value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_QUALITY", wxSlider, GetValue(), 0);
   GET_CONTROL(*pToolWindow_, "ID_QUAL_VALUE_LABEL", wxStaticText)->SetLabel(
         NumberToString<int>(value).c_str());
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_EXAGERATION", wxSlider,
                       GetValue(), 0);
   GET_CONTROL(*pToolWindow_, "ID_EXAG_VALUE_LABEL", wxStaticText)->SetLabel(
               NumberToString<int>(value).c_str());
   value = USE_CONTROL(*pToolWindow_, "ID_SLIDER_CAMERA", wxSlider,
                          GetValue(), 0);
   GET_CONTROL(*pToolWindow_, "ID_VEL_CAM_VALUE_LABEL", wxStaticText)->SetLabel(
                NumberToString<int>(value).c_str());
}
}  // namespace suri
