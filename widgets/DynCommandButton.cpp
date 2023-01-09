/*! \file DynCommandButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
