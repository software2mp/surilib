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
#include "suri/EventInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

ForwardEventImpl::ForwardEventImpl(ViewportManagerInterface *pViewportManager,
                                   ViewcontextManagerInterface *pViewcontextManager) :
      EventInterface(pViewportManager, pViewcontextManager),
      pForwardedMouseClass_(new ForwardMouseEventImpl),
      pForwardedPaintClass_(new ForwardPaintEventImpl) {
}

ForwardEventImpl::ForwardEventImpl(const std::string &ForwardMouseClass,
                                   const std::string &ForwardPaintClass,
                                   ViewportManagerInterface *pViewportManager,
                                   ViewcontextManagerInterface *pViewcontextManager) :
      EventInterface(pViewportManager, pViewcontextManager),
      pForwardedMouseClass_(new ForwardMouseEventImpl(ForwardMouseClass)),
      pForwardedPaintClass_(new ForwardPaintEventImpl(ForwardPaintClass)) {
}

ForwardEventImpl::ForwardEventImpl(MouseEventInterface *pForwardMouseClass,
                                   PaintEventInterface *pForwardPaintClass,
                                   ViewportManagerInterface *pViewportManager,
                                   ViewcontextManagerInterface *pViewcontextManager) :
      EventInterface(pViewportManager, pViewcontextManager),
      pForwardedMouseClass_(NULL), pForwardedPaintClass_(NULL) {
}

ForwardEventImpl::~ForwardEventImpl() {
   // \todo: Si se cargaron con el ctor que recibe punteros, no deben eliminarse
   delete pForwardedMouseClass_;
   delete pForwardedPaintClass_;
}

void ForwardEventImpl::OnLeft(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnLeft(Event);
}

void ForwardEventImpl::OnLeftUp(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnLeftUp(Event);
}

void ForwardEventImpl::OnLeftDown(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnLeftDown(Event);
}

void ForwardEventImpl::OnLeftDoubleClick(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnLeftDoubleClick(Event);
}

void ForwardEventImpl::OnRight(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnRight(Event);
}

void ForwardEventImpl::OnRightUp(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnRightUp(Event);
}

void ForwardEventImpl::OnRightDown(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnRightDown(Event);
}

void ForwardEventImpl::OnRightDoubleClick(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnRightDoubleClick(Event);
}

void ForwardEventImpl::OnMiddle(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnMiddle(Event);
}

void ForwardEventImpl::OnMiddleUp(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnMiddleUp(Event);
}

void ForwardEventImpl::OnMiddleDown(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnMiddleDown(Event);
}

void ForwardEventImpl::OnMiddleDoubleClick(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnMiddleDoubleClick(Event);
}

void ForwardEventImpl::OnEnterWindow(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnEnterWindow(Event);
}

void ForwardEventImpl::OnLeaveWindow(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnLeaveWindow(Event);
}

void ForwardEventImpl::OnWheel(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnWheel(Event);
}

void ForwardEventImpl::OnMouseMove(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnMouseMove(Event);
}

void ForwardEventImpl::OnMouse(wxMouseEvent &Event) {
   pForwardedMouseClass_->OnMouse(Event);
}

MouseEventInterface::MouseEventFlags ForwardEventImpl::GetMouseFlags() const {
   return pForwardedMouseClass_->GetMouseFlags();
}

void ForwardEventImpl::MouseEnabled(bool Enable) {
   pForwardedMouseClass_->MouseEnabled(Enable);
}

bool ForwardEventImpl::IsMouseEnabled() const {
   return pForwardedMouseClass_->IsMouseEnabled();
}

bool ForwardEventImpl::CanProcessEvent(wxMouseEvent&Event) const {
   return pForwardedMouseClass_->CanProcessEvent(Event);
}

void ForwardEventImpl::OnPaint(wxPaintEvent &Event, wxDC &PaintDC) {
   pForwardedPaintClass_->OnPaint(Event, PaintDC);
}

void ForwardEventImpl::OnBackgroundErase(wxEraseEvent &Event) {
   pForwardedPaintClass_->OnBackgroundErase(Event);
}

PaintEventInterface::PaintEventFlags ForwardEventImpl::GetPaintFlags() const {
   return pForwardedPaintClass_->GetPaintFlags();
}

void ForwardEventImpl::PaintEnabled(bool Enable) {
   pForwardedPaintClass_->PaintEnabled(Enable);
}

bool ForwardEventImpl::IsPaintEnabled() const {
   return pForwardedPaintClass_->IsPaintEnabled();
}

bool ForwardEventImpl::CanProcessEvent(wxPaintEvent&Event) const {
   return pForwardedPaintClass_->CanProcessEvent(Event);
}

void ForwardEventImpl::Enabled(bool Enable) {

}

bool ForwardEventImpl::IsEnabled() const {
   return false;
}

} /** namespace suri */

