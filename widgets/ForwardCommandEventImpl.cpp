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

// Includes Estandar
// Includes Suri
#include "suri/CommandEventInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

const CommandEventInterface::CommandEventFlags CommandEventInterface::None = 0x00000000;
const CommandEventInterface::CommandEventFlags CommandEventInterface::MenuEvent =
      0x00000001;
const CommandEventInterface::CommandEventFlags CommandEventInterface::All =
      CommandEventInterface::MenuEvent;

/*!
 *
 */
class EmptyCommandEvent : public CommandEventInterface {
public:
   EmptyCommandEvent() :
         enabled_(true) {
   }
   virtual ~EmptyCommandEvent() {
   }

   virtual void Execute(wxCommandEvent&) {
   }

   virtual CommandEventInterface::CommandEventFlags GetCommandFlags() const {
      return CommandEventInterface::None;
   }

   virtual void CommandEnabled(bool Enable = true) {
      enabled_ = Enable;
   }
   virtual bool IsCommandEnabled() const {
      return enabled_;
   }

   virtual bool CanProcessEvent(wxCommandEvent&) const {
      return true;
   }

   virtual int GetCommandId() const {
      return -1;
   }
protected:
private:
   bool enabled_;
};

/*!
 *
 *
 */
ForwardCommandEventImpl::ForwardCommandEventImpl() :
      pForwardedClass_(new EmptyCommandEvent) {

}
/*!
 *
 *
 */
ForwardCommandEventImpl::ForwardCommandEventImpl(const std::string &ForwardedClass) :
      pForwardedClass_(NULL) {
}

/*!
 *
 *
 */
ForwardCommandEventImpl::ForwardCommandEventImpl(
      CommandEventInterface *pForwardedInstance) :
      pForwardedClass_(pForwardedInstance) {
}

ForwardCommandEventImpl::~ForwardCommandEventImpl() {

}

void ForwardCommandEventImpl::Execute(wxCommandEvent &Event) {
   pForwardedClass_->Execute(Event);
}

CommandEventInterface::CommandEventFlags ForwardCommandEventImpl::GetCommandFlags() const {
   return pForwardedClass_->GetCommandFlags();
}

void ForwardCommandEventImpl::CommandEnabled(bool Enable) {
   pForwardedClass_->CommandEnabled(Enable);
}

bool ForwardCommandEventImpl::IsCommandEnabled() const {
   return pForwardedClass_->IsCommandEnabled();
}

int ForwardCommandEventImpl::GetCommandId() const {
   return pForwardedClass_->GetCommandId();
}

bool ForwardCommandEventImpl::CanProcessEvent(wxCommandEvent &Event) const {
   return pForwardedClass_->CanProcessEvent(Event);
}

} /** namespace suri */
