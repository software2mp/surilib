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

#include <sstream>
#include <string>
#include <map>

#include "SingleViewerSelectionPart.h"

#include "wx/colour.h"
#include "wx/gdicmn.h"

namespace suri {

START_IMPLEMENT_EVENT_OBJECT(SingleViewerSelectionPartEvent, SingleViewerSelectionPart)
      IMPLEMENT_EVENT_CALLBACK(OnChViewerChanged, OnChViewerChanged(event), wxCommandEvent)
END_IMPLEMENT_EVENT_OBJECT

const std::string SingleViewerSelectionPart::SINGLE_VIEWER_SELECTION_XRC_NAME =
      wxT("ID_SINGLE_VIEWER_SELECTION_PANEL"); /*! Id del panel */
const std::string SingleViewerSelectionPart::kViewer = "Visualizador";

/** Ctor */
SingleViewerSelectionPart::SingleViewerSelectionPart() :
      Part(SINGLE_VIEWER_SELECTION_XRC_NAME, wxT(caption_SINGLE_VIEWER_PART)),
      NEW_EVENT_OBJECT(SingleViewerSelectionPartEvent) {
}

/** Dtor */
SingleViewerSelectionPart::~SingleViewerSelectionPart() {
}

/** Indica si el Part tiene cambios para salvar. */
bool SingleViewerSelectionPart::HasChanged() {
   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                                  wxT("ID_VIEWER_SELECTION_CHOICE"), wxChoice);

   if (pchoice->GetSelection() != kInitialPos)
      return true;

   return false;
}

/** Salva los cambios realizados en el Part. */
bool SingleViewerSelectionPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool SingleViewerSelectionPart::RollbackChanges() {
   return true;
}

/** Inicializa el part */
void SingleViewerSelectionPart::SetInitialValues() {
}

/** Actualiza el estado del Part */
void SingleViewerSelectionPart::Update() {
}

/**
 * Modifica el nombre del viewer.
 */
bool SingleViewerSelectionPart::ConfigureWidget() {
   InitializePart();
   return true;
}

/**
 * Configura los visualizadores disponibles.
 *
 * @param[in] CurrentViewers Vector con todos los visualizadores actualmente siendo utilizados.
 */
void SingleViewerSelectionPart::SetCurrentViewers(
      const std::map<suri::SuriObject::UuidType, std::string>& CurrentViewers) {
   currentViewers_ = CurrentViewers;
}

/**
 * Handler para el evento de cambio de seleccion de choice.
 */
void SingleViewerSelectionPart::OnChViewerChanged(wxCommandEvent &Event) {
   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_SELECTION_CHOICE"), wxChoice);

   selectedViewerId_ = static_cast<char*>(pchoice->GetClientData(pchoice->GetSelection()));
}

/**
 * Devuelve el visualizador seleccionado.
 *
 * @return Devuelve el id del visualizador seleccionado.
 */
SuriObject::UuidType SingleViewerSelectionPart::GetSelectedViewer() {
  return selectedViewerId_;
}

/**
 * Inicializa el part realizando todas las configuraciones necesarias.
 */
void SingleViewerSelectionPart::InitializePart() {
   InitializeChoice();
}

/**
 * Inicializa el combo de seleccion de visualizadores.
 */
void SingleViewerSelectionPart::InitializeChoice() {
   wxChoice* pchoice = XRCCTRL(*(this->GetWidget()->GetWindow()),
                               wxT("ID_VIEWER_SELECTION_CHOICE"), wxChoice);

   int lastindex = -1;
   std::map<SuriObject::UuidType, std::string>::iterator it;
   for (it = currentViewers_.begin(); it != currentViewers_.end(); ++it) {
      lastindex = pchoice->Append(it->second);
      pchoice->SetClientData(lastindex, const_cast<char*>(it->first.c_str()));
   }

   pchoice->Select(kInitialPos);

   pchoice->Connect(
         wxEVT_COMMAND_CHOICE_SELECTED,
         wxCommandEventHandler(SingleViewerSelectionPartEvent::OnChViewerChanged),
         NULL, pEventHandler_);

   selectedViewerId_ = static_cast<char*>(pchoice->GetClientData(pchoice->GetSelection()));
}

}  // namespace suri

