/*! \file ForwardEventImpl.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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

