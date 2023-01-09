/*! \file ForwardMouseEventImpl.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/MouseEventInterface.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards
namespace suri {

const MouseEventInterface::MouseEventFlags MouseEventInterface::NoneFlag = 0x00000000;
const MouseEventInterface::MouseEventFlags MouseEventInterface::LeftUp = 0x00000001;
const MouseEventInterface::MouseEventFlags MouseEventInterface::LeftDown = LeftUp << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::LeftDClick = LeftDown
      << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::Left = LeftDClick << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::AllLeft = Left
      | LeftDClick | LeftDown | LeftUp;
const MouseEventInterface::MouseEventFlags MouseEventInterface::RightUp = Left << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::RightDown = RightUp
      << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::RightDClick = RightDown
      << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::Right = RightDClick
      << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::AllRight = Right
      | RightDClick | RightDown | RightUp;
const MouseEventInterface::MouseEventFlags MouseEventInterface::MiddleUp = Right << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::MiddleDown = MiddleUp
      << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::MiddleDClick =
      MiddleDown << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::Middle = MiddleDClick
      << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::AllMiddle = Middle
      | MiddleDClick | MiddleDown | MiddleUp;
const MouseEventInterface::MouseEventFlags MouseEventInterface::AllButtons = AllLeft
      | AllRight | AllMiddle;
const MouseEventInterface::MouseEventFlags MouseEventInterface::MouseEnter = Middle
      << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::MouseLeave = MouseEnter
      << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::MouseMove = MouseLeave
      << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::WheelUp = MouseMove
      << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::WheelDown = WheelUp
      << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::Any = WheelDown << 1;
const MouseEventInterface::MouseEventFlags MouseEventInterface::Wheel = WheelUp
      | WheelDown;
const MouseEventInterface::MouseEventFlags MouseEventInterface::All = AllButtons
      | MouseEnter | MouseLeave | MouseMove | Wheel | Any;

class EmptyMouseEvent : public MouseEventInterface {
public:
   EmptyMouseEvent() :
         enabled_(true) {
   }
   virtual ~EmptyMouseEvent() {
   }

   virtual void OnLeft(wxMouseEvent&) {
   }
   virtual void OnLeftUp(wxMouseEvent&) {
   }
   virtual void OnLeftDown(wxMouseEvent&) {
   }
   virtual void OnLeftDoubleClick(wxMouseEvent&) {
   }
   virtual void OnRight(wxMouseEvent&) {
   }
   virtual void OnRightUp(wxMouseEvent&) {
   }
   virtual void OnRightDown(wxMouseEvent&) {
   }
   virtual void OnRightDoubleClick(wxMouseEvent&) {
   }
   virtual void OnMiddle(wxMouseEvent&) {
   }
   virtual void OnMiddleUp(wxMouseEvent&) {
   }
   virtual void OnMiddleDown(wxMouseEvent&) {
   }
   virtual void OnMiddleDoubleClick(wxMouseEvent&) {
   }
   virtual void OnEnterWindow(wxMouseEvent&) {
   }
   virtual void OnLeaveWindow(wxMouseEvent&) {
   }
   virtual void OnWheel(wxMouseEvent&) {
   }
   virtual void OnMouseMove(wxMouseEvent&) {
   }
   virtual void OnMouse(wxMouseEvent&) {
   }

   virtual MouseEventFlags GetMouseFlags() const {
      return MouseEventInterface::NoneFlag;
   }

   virtual void MouseEnabled(bool Enable = true) {
      enabled_ = Enable;
   }
   virtual bool IsMouseEnabled() const {
      return enabled_;
   }

   virtual bool CanProcessEvent(wxMouseEvent&) const {
      return true;
   }
protected:
private:
   bool enabled_;
};

/*!
 *
 *
 */
ForwardMouseEventImpl::ForwardMouseEventImpl() :
      pForwardedClass_(new EmptyMouseEvent) {
}

/*!
 *
 *
 */
ForwardMouseEventImpl::ForwardMouseEventImpl(const std::string &ForwardedClass) :
      pForwardedClass_(NULL) {
}

/*!
 *
 *
 */
ForwardMouseEventImpl::ForwardMouseEventImpl(MouseEventInterface *pForwardedInstance) :
      pForwardedClass_(pForwardedInstance) {
}

/*!
 *
 *
 */
ForwardMouseEventImpl::~ForwardMouseEventImpl() {
   delete pForwardedClass_;
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnLeft(wxMouseEvent &Event) {
   pForwardedClass_->OnLeft(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnLeftUp(wxMouseEvent &Event) {
   pForwardedClass_->OnLeftUp(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnLeftDown(wxMouseEvent &Event) {
   pForwardedClass_->OnLeftDown(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnLeftDoubleClick(wxMouseEvent &Event) {
   pForwardedClass_->OnLeftDoubleClick(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnRight(wxMouseEvent &Event) {
   pForwardedClass_->OnRight(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnRightUp(wxMouseEvent &Event) {
   pForwardedClass_->OnRightUp(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnRightDown(wxMouseEvent &Event) {
   pForwardedClass_->OnRightDown(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnRightDoubleClick(wxMouseEvent &Event) {
   pForwardedClass_->OnRightDoubleClick(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnMiddle(wxMouseEvent &Event) {
   pForwardedClass_->OnMiddle(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnMiddleUp(wxMouseEvent &Event) {
   pForwardedClass_->OnMiddleUp(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnMiddleDown(wxMouseEvent &Event) {
   pForwardedClass_->OnMiddleDown(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnMiddleDoubleClick(wxMouseEvent &Event) {
   pForwardedClass_->OnMiddleDoubleClick(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnEnterWindow(wxMouseEvent &Event) {
   pForwardedClass_->OnEnterWindow(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnLeaveWindow(wxMouseEvent &Event) {
   pForwardedClass_->OnLeaveWindow(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnWheel(wxMouseEvent &Event) {
   pForwardedClass_->OnWheel(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnMouseMove(wxMouseEvent &Event) {
   pForwardedClass_->OnMouseMove(Event);
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::OnMouse(wxMouseEvent &Event) {
   pForwardedClass_->OnMouse(Event);
}

/*!
 *
 *
 */
MouseEventInterface::MouseEventFlags ForwardMouseEventImpl::GetMouseFlags() const {
   return pForwardedClass_->GetMouseFlags();
}

/*!
 *
 *
 */
void ForwardMouseEventImpl::MouseEnabled(bool Enable) {
   pForwardedClass_->MouseEnabled(Enable);
}

bool ForwardMouseEventImpl::IsMouseEnabled() const {
   return pForwardedClass_->IsMouseEnabled();
}

bool ForwardMouseEventImpl::CanProcessEvent(wxMouseEvent &Event) const {
   return pForwardedClass_->CanProcessEvent(Event);
}

}  // namespace suri

