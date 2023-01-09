/*! \file DynCommandButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DYNCOMMANDBUTTON_H_
#define DYNCOMMANDBUTTON_H_

#include <string>

#include "suri/Button.h"
#include "suri/ObserverInterface.h"

#include "wx/bitmap.h"
#include "wx/bmpbuttn.h"

namespace suri {

class Command;
class ParameterCollection;

class DynCommandButton : public Button, public ObserverInterface {
public:
   static const std::string kActivatedBitmap;
   static const std::string kDeactivatedBitmap;
   static const std::string kActivatedTooltip;
   static const std::string kDeactivatedTooltip;

   /**
    * Ctor.
    */
   explicit DynCommandButton(Command* pCommand);

   /**
    * Dtor.
    */
   virtual ~DynCommandButton();

   /**
    * Recibe notificaciones de un Sujeto (Observable)
    */
   virtual void Update(NotificationInterface* pNotification);

protected:
   /**
    * Se llama cuando se setea el valor a true y estaba en false.
    */
   virtual void OnButtonClicked(wxCommandEvent& Event);

private:
   /** Comando asociado al slider. */
   Command* pCommand_;
   bool activated_;
   std::string activatedBitmap_;
   std::string deactivatedBitmap_;
   std::string activatedTooltip_;
   std::string deactivatedTooltip_;

   /**
    * Actualiza el estado del boton (visualmente)
    */
   void ChangeButtonStatus(bool Activate = true);

   /**
    * Metodo que ejecuta realmente el codigo necesario para este boton.
    */
   void DoExecute(bool Activate = true);
};

}  // namespace suri

#endif  // DYNCOMMANDBUTTON_H_
