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

#include "suri/FocusViewportEvent.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

FocusViewportEvent::FocusViewportEvent(ViewportManagerInterface *pVportManager,
                                       const SuriObject::UuidType &ViewerId) :
      pVportManager_(pVportManager), viewerId_(ViewerId) {
}

FocusViewportEvent::~FocusViewportEvent() {
}

void FocusViewportEvent::OnLeft(wxMouseEvent &Event) {
   FocusViewport(Event);
}

void FocusViewportEvent::OnLeftUp(wxMouseEvent&) {

}

void FocusViewportEvent::OnLeftDown(wxMouseEvent&) {

}

void FocusViewportEvent::OnLeftDoubleClick(wxMouseEvent&) {

}

void FocusViewportEvent::OnRight(wxMouseEvent &Event) {
   FocusViewport(Event);
}

void FocusViewportEvent::OnRightUp(wxMouseEvent&) {

}

void FocusViewportEvent::OnRightDown(wxMouseEvent&) {

}

void FocusViewportEvent::OnRightDoubleClick(wxMouseEvent&) {

}

void FocusViewportEvent::OnMiddle(wxMouseEvent &Event) {
   FocusViewport(Event);
}

void FocusViewportEvent::OnMiddleUp(wxMouseEvent&) {

}

void FocusViewportEvent::OnMiddleDown(wxMouseEvent&) {

}

void FocusViewportEvent::OnMiddleDoubleClick(wxMouseEvent&) {

}

void FocusViewportEvent::OnEnterWindow(wxMouseEvent&) {

}

void FocusViewportEvent::OnLeaveWindow(wxMouseEvent&) {

}

void FocusViewportEvent::OnWheel(wxMouseEvent &Event) {
   FocusViewport(Event);
}

void FocusViewportEvent::OnMouseMove(wxMouseEvent&) {

}

void FocusViewportEvent::OnMouse(wxMouseEvent&) {

}

MouseEventInterface::MouseEventFlags FocusViewportEvent::GetMouseFlags() const {
   return MouseEventInterface::AllButtons;
}

void FocusViewportEvent::MouseEnabled(bool Enable) {

}
bool FocusViewportEvent::IsMouseEnabled() const {
   return true;
}

bool FocusViewportEvent::CanProcessEvent(wxMouseEvent&Event) const {
   return true;
}


void FocusViewportEvent::FocusViewport(wxMouseEvent &Event)const {
   if (pVportManager_->GetSelectedViewport()->GetId() != viewerId_) {
      pVportManager_->SelectViewport(viewerId_);
      // si quiero que corte el evento usar Event.Skip para
      // que no propague (ej. evita hacer zoom al cambiar el foco)
   }
}

}  /** namespace suri */

