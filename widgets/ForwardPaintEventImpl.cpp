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

#include "suri/PaintEventInterface.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

const PaintEventInterface::PaintEventFlags PaintEventInterface::NoneFlag = 0x00000000;
const PaintEventInterface::PaintEventFlags PaintEventInterface::Paint = 0x00000001;
const PaintEventInterface::PaintEventFlags PaintEventInterface::Background = Paint << 1;
const PaintEventInterface::PaintEventFlags PaintEventInterface::All = Paint
      | Background;

class EmptyPaintEvent : public PaintEventInterface {
   EmptyPaintEvent(const EmptyPaintEvent&);
public:
   EmptyPaintEvent() :
         enabled_(true) {
   }
   virtual ~EmptyPaintEvent() {
   }
   virtual void OnPaint(wxPaintEvent&, wxDC&) {
   }
   virtual void OnBackgroundErase(wxEraseEvent&) {
   }
   virtual PaintEventFlags GetPaintFlags() const {
      return PaintEventInterface::NoneFlag;
   }

   virtual void PaintEnabled(bool Enable = true) {
      enabled_ = Enable;
   }
   virtual bool IsPaintEnabled() const {
      return enabled_;
   }

   virtual bool CanProcessEvent(wxPaintEvent&) const {
      return true;
   }
protected:
private:
   bool enabled_;
};

ForwardPaintEventImpl::ForwardPaintEventImpl() :
      pForwardedClass_(new EmptyPaintEvent) {

}

/*!
 *
 *
 */
ForwardPaintEventImpl::ForwardPaintEventImpl(const std::string &ForwardedClass) :
      pForwardedClass_(NULL) {

}

/*!
 *
 *
 */
ForwardPaintEventImpl::ForwardPaintEventImpl(PaintEventInterface *pForwardedInstance) :
      pForwardedClass_(pForwardedInstance) {

}

/*!
 *
 *
 */
ForwardPaintEventImpl::~ForwardPaintEventImpl() {

}

/*!
 *
 *
 */
void ForwardPaintEventImpl::OnPaint(wxPaintEvent &Event, wxDC &DeviceContext) {
   pForwardedClass_->OnPaint(Event, DeviceContext);
}

/*!
 *
 *
 */
void ForwardPaintEventImpl::OnBackgroundErase(wxEraseEvent &Event) {
   pForwardedClass_->OnBackgroundErase(Event);
}

/*!
 *
 *
 */
PaintEventInterface::PaintEventFlags ForwardPaintEventImpl::GetPaintFlags() const {
   return pForwardedClass_->GetPaintFlags();
}

/*!
 *
 *
 */
void ForwardPaintEventImpl::PaintEnabled(bool Enable) {
   pForwardedClass_->PaintEnabled(Enable);
}

/*!
 *
 *
 */
bool ForwardPaintEventImpl::IsPaintEnabled() const {
   return pForwardedClass_->IsPaintEnabled();
}

bool ForwardPaintEventImpl::CanProcessEvent(wxPaintEvent &Event) const {
   return pForwardedClass_->CanProcessEvent(Event);
}

}  // namespace suri

