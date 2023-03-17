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

#include <map>
#include <vector>

#include "Viewer2DSelectorPart.h"

#include <wx/sizer.h>

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(Viewer2DSelectorPartEvent, Viewer2DSelectorPart)
      IMPLEMENT_EVENT_CALLBACK(OnChBaseSelectionChanged, OnChBaseSelectionChanged(event), wxCommandEvent)
      IMPLEMENT_EVENT_CALLBACK(OnChWarpSelectionChanged, OnChWarpSelectionChanged(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

const std::string Viewer2DSelectorPart::kViewer = "Visualizador";

/**
 * Ctor
 */
Viewer2DSelectorPart::Viewer2DSelectorPart(bool Enable, bool Modified) :
      Part(wxT("ID_VIEWER_2D_SELECTOR_PART"), _(caption_VIEWER_2D_SELECTOR), Enable, Modified),
      NEW_EVENT_OBJECT(Viewer2DSelectorPartEvent) {

}

/**
 * Dtor
 */
Viewer2DSelectorPart::~Viewer2DSelectorPart() {
   DELETE_EVENT_OBJECT;
}

/**
 * Indica si el Part tiene cambios para salvar
 */
bool Viewer2DSelectorPart::HasChanged() {
   wxChoice* pchbase = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_2D_SELECTOR_CH_BASE"), wxChoice);

   wxChoice* pchwarp = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_2D_SELECTOR_CH_WARP"), wxChoice);

   if (!AreEqualChoices()
         && (pchbase->GetSelection() != kInitialBasePos
               || pchwarp->GetSelection() != kInitialWarpPos))
      return true;

   return false;
}

/**
 * Salva los cambios realizados en el Part.
 * (No aplica para este part)
 */
bool Viewer2DSelectorPart::CommitChanges() {
   return !AreEqualChoices();
}

/**
 * Restaura los valor's originales del Part.
 * (No aplica para este part)
 */
bool Viewer2DSelectorPart::RollbackChanges() {
   return true;
}

/**
 * Inicializa el part
 */
void Viewer2DSelectorPart::SetInitialValues() {

}

/**
 * Actualiza el estado de la parte
 */
void Viewer2DSelectorPart::Update() {

}

/**
 * Modifica el nombre del viewer.
 */
bool Viewer2DSelectorPart::ConfigureWidget() {
   InitializePart();
   return true;
}

/**
 * Devuelve si la parte tiene datos validos
 */
bool Viewer2DSelectorPart::HasValidData() {
   return !AreEqualChoices();
}

/**
 * Configura los visualizadores disponibles.
 *
 * @param[in] CurrentViewers Vector con todos los visualizadores actualmente siendo utilizados.
 */
void Viewer2DSelectorPart::SetCurrentViewers(
      const std::map<SuriObject::UuidType, std::string>& CurrentViewers) {
   currentViewers_ = CurrentViewers;
}

/**
 * Obtiene los visualizadores seleccionados.
 *
 * @return Devuelve los dos visualizadores seleccionados.
 */
std::vector<SuriObject::UuidType> Viewer2DSelectorPart::GetSelectedViewers() const {
   std::vector<SuriObject::UuidType> retviewers;
   retviewers.push_back(baseId_);
   retviewers.push_back(warpId_);
   return retviewers;
}

/**
 * Inicializa el part realizando todas las configuraciones necesarias.
 */
void Viewer2DSelectorPart::InitializePart() {
   InitializeChoices();
   Initialize();
}

/**
 * Inicializa los controles choice para la seleccion de visualizadores.
 */
void Viewer2DSelectorPart::InitializeChoices() {
   InitializeChoiceBase();
   InitializeChoiceWarp();
}

/**
 * Inicializa el control choice para visualizador base.
 */
void Viewer2DSelectorPart::InitializeChoiceBase() {
   baseSelection_ = kInitialBasePos;

   wxChoice* pchbase = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_2D_SELECTOR_CH_BASE"), wxChoice);

   int lastindex = -1;
   std::map<SuriObject::UuidType, std::string>::iterator it;
   for (it = currentViewers_.begin(); it != currentViewers_.end(); ++it) {
      lastindex = pchbase->Append(it->second);
      pchbase->SetClientData(lastindex,
                             const_cast<char*>(it->first.c_str()));
   }

   pchbase->Select(baseSelection_);
   baseId_ = static_cast<char*>(pchbase->GetClientData(baseSelection_));

   pchbase->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(Viewer2DSelectorPartEvent::OnChBaseSelectionChanged),
         NULL, pEventHandler_);
}

/**
 * Inicializa el control choice para visualizador warp.
 */
void Viewer2DSelectorPart::InitializeChoiceWarp() {
   warpSelection_ = kInitialWarpPos;

   wxChoice* pchwarp = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_2D_SELECTOR_CH_WARP"), wxChoice);

   int lastindex = -1;
   std::map<SuriObject::UuidType, std::string>::iterator it;
   for (it = currentViewers_.begin(); it != currentViewers_.end(); ++it) {
      lastindex = pchwarp->Append(it->second);
      pchwarp->SetClientData(lastindex,
                             const_cast<char*>(it->first.c_str()));
   }
   pchwarp->Select(warpSelection_);
   warpId_ = static_cast<char*>(pchwarp->GetClientData(warpSelection_));

   pchwarp->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(Viewer2DSelectorPartEvent::OnChWarpSelectionChanged),
         NULL, pEventHandler_);
}

/**
 * Inicializacion general.
 */
void Viewer2DSelectorPart::Initialize() {}

/**
 * Handler para el evento de cambio de seleccion de choice base.
 */
void Viewer2DSelectorPart::OnChBaseSelectionChanged(wxCommandEvent &Event) {
   wxChoice* pchbase = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_2D_SELECTOR_CH_BASE"), wxChoice);

   baseSelection_ = pchbase->GetSelection();
   baseId_ = static_cast<char*>(pchbase->GetClientData(baseSelection_));

   wxPanel* ppnlerr = XRCCTRL(*(this->GetWidget()->GetWindow()),
                              wxT("ID_VIEWER_2D_PANEL_ERR_MSG"), wxPanel);

   if (AreEqualChoices()) {
      ppnlerr->Show();
   } else {
      ppnlerr->Hide();
   }
   RefreshContainer();
}

/**
 * Handler para el evento de cambio de seleccion de choice warp.
 */
void Viewer2DSelectorPart::OnChWarpSelectionChanged(wxCommandEvent &Event) {
   wxChoice* pchwarp = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_2D_SELECTOR_CH_WARP"), wxChoice);

   warpSelection_ = pchwarp->GetSelection();
   warpId_ = static_cast<char*>(pchwarp->GetClientData(warpSelection_));

   wxPanel* ppnlerr = XRCCTRL(*(this->GetWidget()->GetWindow()),
                              wxT("ID_VIEWER_2D_PANEL_ERR_MSG"), wxPanel);

   if (AreEqualChoices()) {
      ppnlerr->Show();
   } else {
      ppnlerr->Hide();
   }
   RefreshContainer();
}

/**
 * Verifica si los visualizadores seleccionados son distintos.
 *
 * @return true si esta seleccionado el mismo visualizador.
 */
bool Viewer2DSelectorPart::AreEqualChoices() const {
   if (baseId_.compare(warpId_) == 0) {
      return true;
   }
   return false;
}

/**
 * Redibujo panel padre (Fix luego de esconder o mostrar un elemento)
 */
void Viewer2DSelectorPart::RefreshContainer() {
   wxWindow* pframe = dynamic_cast<wxWindow*>(this->GetWidget()->GetFrame());
   wxPanel* pPanel = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                 wxT("ID_VIEWER_2D_SELECTOR_PANEL"), wxPanel);
   pPanel->Layout();
   pframe->Fit();
}

}  // namespace suri
