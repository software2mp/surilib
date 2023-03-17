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
