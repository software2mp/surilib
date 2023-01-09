/*! \file MouseEvent.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "MouseEvent.h"

// Includes Estandar

// Includes Suri
#include "suri/MouseEventInterface.h"
#include "suri/wxSuriMouseEvent.h"

// Includes Wx

// Defines

// forwards
namespace suri {

MouseEventInterface *MouseEvent::GetHandler() {
   return pParent_;
}

MouseEvent::MouseEvent(MouseEventInterface *pParent) :
      pParent_(pParent) {
   if (pParent) {
      MouseEventInterface::MouseEventFlags mouseflags = pParent_->GetMouseFlags();
      if (mouseflags & MouseEventInterface::Left) {
         Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MouseEvent::OnLeft));
         Connect(wxEVT_LEFT_UP, wxMouseEventHandler(MouseEvent::OnLeft));
         Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(MouseEvent::OnLeft));
      }
      if (mouseflags & MouseEventInterface::LeftDown)
         Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MouseEvent::OnLeftDown));
      if (mouseflags & MouseEventInterface::LeftUp)
         Connect(wxEVT_LEFT_UP, wxSuriMouseEventHandler(MouseEvent::OnLeftUp));
      if (mouseflags & MouseEventInterface::LeftDClick)
         Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(MouseEvent::OnLeftDoubleClick));

      if (mouseflags & MouseEventInterface::Middle) {
         Connect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(MouseEvent::OnMiddle));
         Connect(wxEVT_MIDDLE_UP, wxMouseEventHandler(MouseEvent::OnMiddle));
         Connect(wxEVT_MIDDLE_DCLICK, wxMouseEventHandler(MouseEvent::OnMiddle));
      }
      if (mouseflags & MouseEventInterface::MiddleDown)
         Connect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(MouseEvent::OnMiddleDown));
      if (mouseflags & MouseEventInterface::MiddleUp)
         Connect(wxEVT_MIDDLE_UP, wxMouseEventHandler(MouseEvent::OnMiddleUp));
      if (mouseflags & MouseEventInterface::MiddleDClick)
         Connect(wxEVT_MIDDLE_DCLICK,
                 wxMouseEventHandler(MouseEvent::OnMiddleDoubleClick));

      if (mouseflags & MouseEventInterface::Right) {
         Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(MouseEvent::OnRight));
         Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(MouseEvent::OnRight));
         Connect(wxEVT_RIGHT_DCLICK, wxMouseEventHandler(MouseEvent::OnRight));
      }
      if (mouseflags & MouseEventInterface::RightDown)
         Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(MouseEvent::OnRightDown));
      if (mouseflags & MouseEventInterface::RightUp)
         Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(MouseEvent::OnRightUp));
      if (mouseflags & MouseEventInterface::RightDClick)
         Connect(wxEVT_RIGHT_DCLICK,
                 wxMouseEventHandler(MouseEvent::OnRightDoubleClick));

      if (mouseflags & MouseEventInterface::MouseEnter)
         Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(MouseEvent::OnEnterWindow));
      if (mouseflags & MouseEventInterface::MouseLeave)
         Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(MouseEvent::OnLeaveWindow));

      if (mouseflags & MouseEventInterface::Wheel)
         Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(MouseEvent::OnWheel));

      if (mouseflags & MouseEventInterface::MouseMove)
         Connect(wxEVT_MOTION, wxMouseEventHandler(MouseEvent::OnMouseMove));

      if (mouseflags & MouseEventInterface::Any) {
         Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MouseEvent::OnMouse));
         Connect(wxEVT_LEFT_UP, wxMouseEventHandler(MouseEvent::OnMouse));
         Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(MouseEvent::OnMouse));
         Connect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(MouseEvent::OnMouse));
         Connect(wxEVT_MIDDLE_UP, wxMouseEventHandler(MouseEvent::OnMouse));
         Connect(wxEVT_MIDDLE_DCLICK, wxMouseEventHandler(MouseEvent::OnMouse));
         Connect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(MouseEvent::OnMouse));
         Connect(wxEVT_RIGHT_UP, wxMouseEventHandler(MouseEvent::OnMouse));
         Connect(wxEVT_RIGHT_DCLICK, wxMouseEventHandler(MouseEvent::OnMouse));
         Connect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(MouseEvent::OnMouse));
         Connect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(MouseEvent::OnMouse));
         Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(MouseEvent::OnMouse));
         Connect(wxEVT_MOTION, wxMouseEventHandler(MouseEvent::OnMouse));
      }
   }
}

MouseEvent::~MouseEvent() {
   Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MouseEvent::OnLeftDown));
   Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler(MouseEvent::OnLeftUp));
   Disconnect(wxEVT_MIDDLE_DOWN, wxMouseEventHandler(MouseEvent::OnMiddleDown));
   Disconnect(wxEVT_MIDDLE_UP, wxMouseEventHandler(MouseEvent::OnMiddleUp));
   Disconnect(wxEVT_RIGHT_DOWN, wxMouseEventHandler(MouseEvent::OnRightDown));
   Disconnect(wxEVT_RIGHT_UP, wxMouseEventHandler(MouseEvent::OnRightUp));
   Disconnect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(MouseEvent::OnLeftDoubleClick));
   Disconnect(wxEVT_MIDDLE_DCLICK,
              wxMouseEventHandler(MouseEvent::OnMiddleDoubleClick));
   Disconnect(wxEVT_RIGHT_DCLICK, wxMouseEventHandler(MouseEvent::OnRightDoubleClick));
   Disconnect(wxEVT_ENTER_WINDOW, wxMouseEventHandler(MouseEvent::OnEnterWindow));
   Disconnect(wxEVT_LEAVE_WINDOW, wxMouseEventHandler(MouseEvent::OnLeaveWindow));
   Disconnect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(MouseEvent::OnWheel));
   Disconnect(wxEVT_MOTION, wxMouseEventHandler(MouseEvent::OnMouseMove));
   pParent_ = NULL;
}

void MouseEvent::OnLeft(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnLeft(Event);
}

void MouseEvent::OnLeftUp(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnLeftUp(Event);
}


void MouseEvent::OnLeftUp(wxSuriMouseEvent &Event) {
   Event.Skip();
   double x,y;
   Event.GetPosition(x, y);
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnLeftUp(Event);
}



void MouseEvent::OnLeftDown(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnLeftDown(Event);
}

void MouseEvent::OnLeftDoubleClick(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnLeftDoubleClick(Event);
}

void MouseEvent::OnRight(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnRight(Event);
}

void MouseEvent::OnRightUp(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnRightUp(Event);
}

void MouseEvent::OnRightDown(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnRightDown(Event);
}

void MouseEvent::OnRightDoubleClick(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnRightDoubleClick(Event);
}

void MouseEvent::OnMiddle(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnMiddle(Event);
}

void MouseEvent::OnMiddleUp(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnMiddleUp(Event);
}

void MouseEvent::OnMiddleDown(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnMiddleDown(Event);
}

void MouseEvent::OnMiddleDoubleClick(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnMiddleDoubleClick(Event);
}

void MouseEvent::OnEnterWindow(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnEnterWindow(Event);
}

void MouseEvent::OnLeaveWindow(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnLeaveWindow(Event);
}

void MouseEvent::OnWheel(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnWheel(Event);
}

void MouseEvent::OnMouseMove(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnMouseMove(Event);
}

void MouseEvent::OnMouse(wxMouseEvent &Event) {
   Event.Skip();
   if (!pParent_->IsMouseEnabled() || !pParent_->CanProcessEvent(Event)) {
      return;
   }
   pParent_->OnMouse(Event);
}

}  // namespace suri
