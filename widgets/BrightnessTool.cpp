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
#include "suri/Operations.h"
#include "suri/RasterElement.h"
#include "suri/messages.h"
#include "suri/BrightnessTool.h"
#include "suri/AuxiliaryFunctions.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"

// Includes App

// Defines

// forward
void InitAdjustToolXmlResource();

/** namespace suri */
namespace suri {
/** \cond MACRO_IMPLEMENT_EVENT_OBJECT */
START_IMPLEMENT_EVENT_OBJECT(BrightnessEvent, BrightnessTool)
      IMPLEMENT_EVENT_CALLBACK(OnSliderScroll, OnSliderScroll(event), wxScrollEvent)
END_IMPLEMENT_EVENT_OBJECT
/** \endcond */
/**
 * Ctor. default.
 * @param[in] pParent: puntero a la ventana padre.
 */
BrightnessTool::BrightnessTool(wxWindow *pParent) :
      Widget(pParent), NEW_EVENT_OBJECT(BrightnessEvent),
      pListViewer_(NULL),
      pElementViewer_(NULL)

{
   windowTitle_ = GetWindowTitle();
   // creo la herramienta
   CreateTool();
   pListViewer_ = new ListView<BrightnessTool>(this, &BrightnessTool::Update);
   pElementViewer_ = new CallbackView<BrightnessTool>(this, &BrightnessTool::Update);
}

/**
 * Dtor.
 */
BrightnessTool::~BrightnessTool() {
   // elimino el helper de eventos
   DELETE_EVENT_OBJECT;
   // Desasocio los viewers antes de eliminarlos
   LayerList *playerlist = pListViewer_->GetList();
   if (Model::IsValid(playerlist)) {
      LayerList::ElementListType::iterator it = playerlist->GetElementIterator();
      for (; it != playerlist->GetElementIterator(false); ++it) {
         Element *pelement = *it;
         pelement->UnregisterViewer(pElementViewer_);
      }
      playerlist->UnregisterViewer(pListViewer_);
   }
   delete pListViewer_;
   delete pElementViewer_;
}

/**
 * Creacion de la ventana.
 * Crea(carga) un toolbar con 2 slider(brillo y contraste). Asocia los eventos
 * a los controles.
 * @return true.
 */
bool BrightnessTool::CreateToolWindow() {
   // tamanio minimo deseado de la ventana del visualizador en modo
   // standalone (isAutonomous_==true)
   wantedWidth_ = 200;
   wantedHeight_ = 0;
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                  wxT("ID_ADJUST_TOOL"));
   toolName_ = GetWindowTitle().c_str();
   CONNECT_TOOL_EVENT(BrightnessEvent, OnSliderScroll, wxEVT_SCROLL_CHANGED,
                      wxScrollEventHandler);
   GET_CONTROL(*pToolWindow_, wxT("ID_CONTRAST_SLIDER"), wxSlider)->SetMin(0);
   GET_CONTROL(*pToolWindow_, wxT("ID_CONTRAST_SLIDER"), wxSlider)->SetMax(20);
   GET_CONTROL(*pToolWindow_, wxT("ID_CONTRAST_SLIDER"), wxSlider)->SetValue(10);
   return true;
}

/**
 * Devuelve el titulo de la ventana.
 */
const wxString BrightnessTool::GetWindowTitle() const {
   return _(caption_ADJUSTMENTS_TOOL);
}

/**
 * Registra el viewer a la lista de elementos.
 * @param[in]	pList: Lista de elementos.
 *  * @return informa si la operacion se pudo realizar(siempre true)
 */
bool BrightnessTool::DoSetList(LayerList *pList) {
   pList->RegisterViewer(pListViewer_);
   return true;
}

/** Actualizacion de la lista => verificar elemento seleccionado */
/**
 * Recibe la actualizacion de la lista o del elemento activo.
 *
 * Recorre la lista buscando el elemento activo y actualiza los sliders
 * en funcion de los valores de brillo y contraste que obtuvo.
 *
 * Para el elemento activo, registra la vista (View) de elementos
 * (pElementViewer_), y asi poder registrar los nuevos cambios.
 *  Responde a una actualizacion de la lista, toma los valores del primer
 * @param[in]	pModel: modelo que llama al Update.
 * \todo No utilizar GetElementIterator.
 * \todo Usar grupos de elementos para recorrer solo los raster.
 */
void BrightnessTool::Update(Model *pModel) {
   if (!pList_) {
      return;
   }
   int brightness = 0;
   int contrast = CONTRAST_SIZE / 2;
   LayerList::ElementListType::iterator it = pList_->GetElementIterator();
   while (it != pList_->GetElementIterator(false)) {
      bool registered = false;
      if ((*it)->IsActive()) {
         GetBrightnessContrast(*it, brightness, contrast);
         if (!registered) {
            (*it)->RegisterViewer(pElementViewer_);
            registered = true;
         } else {
            (*it)->UnregisterViewer(pElementViewer_);
         }
      } else {
         (*it)->UnregisterViewer(pElementViewer_);
      }

      it++;
   }

   GET_CONTROL(*pToolWindow_, wxT("ID_BRIGHTNESS_SLIDER"), wxSlider)
      ->SetValue(brightness);
   GET_CONTROL(*pToolWindow_, wxT("ID_CONTRAST_SLIDER"), wxSlider)->SetValue(contrast);
   GET_CONTROL(*pToolWindow_, wxT("ID_BRIGHT_VALUE_LABEL"), wxStaticText)->SetLabel(
         NumberToString<int>(brightness).c_str());
   GET_CONTROL(*pToolWindow_, wxT("ID_CONTRASTE_VALUE_LABEL"), wxStaticText)->SetLabel(
         NumberToString<int>(contrast).c_str());
}

/**
 * Devuelve el estado de la herramienta.
 * Para determinar el estado verifica la cantidad de elementos activos que hay
 * en la lista, si dicha cantidad es > 0 la herramienta se Activa.
 * @return true en caso de estar activa.
 * @return false C.O.C
 */
bool BrightnessTool::IsEnabled() {
   return pList_ && pList_->GetActiveCount() != 0;
}

/**
 * Evento de movimiento de sliders
 * Recorre la lista buscando el elemento Raster activo y le setea los valores
 * de brillo y contraste establecidos por el usuario con los sliders.
 * @param[in]	Event: evento.
 */
void BrightnessTool::OnSliderScroll(wxScrollEvent &Event) {
   if (!pList_) {
      return;
   }
   GET_CONTROL(*pToolWindow_, wxT("ID_BRIGHTNESS_SLIDER"), wxSlider)->Freeze();
   GET_CONTROL(*pToolWindow_, wxT("ID_CONTRAST_SLIDER"), wxSlider)->Freeze();
   REPORT_DEBUG("D:BC Changed!");
   int brightness =
         USE_CONTROL(*pToolWindow_, wxT("ID_BRIGHTNESS_SLIDER"), wxSlider, GetValue(), 0);
   int contrast =
         USE_CONTROL(*pToolWindow_, wxT("ID_CONTRAST_SLIDER"), wxSlider, GetValue(), 0);

   GET_CONTROL(*pToolWindow_, wxT("ID_BRIGHT_VALUE_LABEL"), wxStaticText)->SetLabel(
         NumberToString<int>(brightness).c_str());
   GET_CONTROL(*pToolWindow_, wxT("ID_CONTRASTE_VALUE_LABEL"), wxStaticText)->SetLabel(
         NumberToString<int>(contrast).c_str());
   LayerList::ElementListType::iterator it = pList_->GetElementIterator();
   while (it != pList_->GetElementIterator(false)) {
      if ((*it)->IsActive() && dynamic_cast<RasterElement*>(*it)) {
         SetBrightnessContrast(*it, brightness, contrast);
         (*it)->SetChanged();
         (*it)->SendViewerUpdate();
      }
      it++;
   }
   GET_CONTROL(*pToolWindow_, wxT("ID_BRIGHTNESS_SLIDER"), wxSlider)->Thaw();
   GET_CONTROL(*pToolWindow_, wxT("ID_CONTRAST_SLIDER"), wxSlider)->Thaw();
}
}  // namespace suri
