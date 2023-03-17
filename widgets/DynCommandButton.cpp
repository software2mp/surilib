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

#include "suri/DynCommandButton.h"
#include "suri/Command.h"
#include "suri/ParameterCollection.h"
#include "suri/StateChangeNotification.h"
#include "resources.h"

#include "wxmacros.h"
#include "wx/bitmap.h"
#include "wx/bmpbuttn.h"

namespace suri {

const std::string DynCommandButton::kActivatedBitmap = "ActivatedBitmap";
const std::string DynCommandButton::kDeactivatedBitmap = "DeactivatedBitmap";
const std::string DynCommandButton::kActivatedTooltip = "ActivatedTooltip";
const std::string DynCommandButton::kDeactivatedTooltip = "DeactivatedTooltip";

/**
 * Ctor.
 */
DynCommandButton::DynCommandButton(Command* pCommand) :
      Button(NULL, pCommand->GetDecoration(), pCommand->GetDescription(), BUTTON_NULL),
      pCommand_(pCommand), activated_(false) {

   ParameterCollection* pparams = pCommand->GetInputParameters();
   pparams->GetValue<std::string>(kActivatedBitmap, activatedBitmap_);
   pparams->GetValue<std::string>(kDeactivatedBitmap, deactivatedBitmap_);
   pparams->GetValue<std::string>(kActivatedTooltip, activatedTooltip_);
   pparams->GetValue<std::string>(kDeactivatedTooltip, deactivatedTooltip_);

   pCommand_->Attach(this);
}

/**
 * Dtor.
 */
DynCommandButton::~DynCommandButton() {

}

/**
 * Recibe notificaciones de un Sujeto (Observable)
 */
void DynCommandButton::Update(NotificationInterface* pNotification) {
   // Esto esta asi por la forma en que esta implementado el manejo de los
   // comandos. SE MIRA PERO NO SE TOCA (PREGUNTAR ANTES)
   StateChangeNotification* pnotif = dynamic_cast<StateChangeNotification*>(pNotification);
   if (pnotif && pnotif->GetEventId().compare("Activate") == 0 && !activated_) {
      activated_ = true;
      ChangeButtonStatus(activated_);
   } else if (pnotif && pnotif->GetEventId().compare("Deactivate") == 0 && activated_) {
      activated_ = false;
      DoExecute(activated_);
   }
}

/**
 * Se llama cuando se setea el valor a true y estaba en false.
 */
void DynCommandButton::OnButtonClicked(wxCommandEvent& Event) {
   activated_ = (activated_) ? false : true;
   DoExecute(activated_);
   Event.Skip(false);
}

/**
 * Actualiza el estado del boton (visualmente)
 */
void DynCommandButton::ChangeButtonStatus(bool Activate) {
   wxBitmapButton* pbutton = dynamic_cast<wxBitmapButton*>(pToolWindow_);
   if (pbutton) {
      wxBitmap bitmap;
      if (Activate) {
         GET_BITMAP_RESOURCE(activatedBitmap_.c_str(), bitmap);
         pbutton->SetLabel(activatedTooltip_.c_str());
      } else {
         GET_BITMAP_RESOURCE(deactivatedBitmap_.c_str(), bitmap);
         pbutton->SetLabel(deactivatedTooltip_.c_str());
      }
      pbutton->SetBitmapLabel(bitmap);
   }
}

/**
 * Metodo que ejecuta realmente el codigo necesario para este boton.
 */
void DynCommandButton::DoExecute(bool Activate) {
   ChangeButtonStatus(Activate);
   pCommand_->SetActive(Activate);
   pCommand_->Execute();
}

} // namespace suri
