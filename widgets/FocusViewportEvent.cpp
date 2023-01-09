/*! \file FocusViewportEvent.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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

