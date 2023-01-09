/*! \file MouseEventInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MOUSEEVENTINTERFACE_H_
#define MOUSEEVENTINTERFACE_H_

// Includes Estandar
#include <string>

// Includes Suri

// Includes Wx

// Defines

// forwards
class wxMouseEvent;

namespace suri {

class MouseEventInterface {
   MouseEventInterface(const MouseEventInterface&);

protected:
   MouseEventInterface() {
   }

public:
   typedef int MouseEventFlags;
   static const MouseEventFlags NoneFlag;
   static const MouseEventFlags LeftUp;
   static const MouseEventFlags LeftDown;
   static const MouseEventFlags LeftDClick;
   static const MouseEventFlags Left;
   static const MouseEventFlags AllLeft;
   static const MouseEventFlags RightUp;
   static const MouseEventFlags RightDown;
   static const MouseEventFlags RightDClick;
   static const MouseEventFlags Right;
   static const MouseEventFlags AllRight;
   static const MouseEventFlags MiddleUp;
   static const MouseEventFlags MiddleDown;
   static const MouseEventFlags MiddleDClick;
   static const MouseEventFlags Middle;
   static const MouseEventFlags AllMiddle;
   static const MouseEventFlags AllButtons;
   static const MouseEventFlags MouseEnter;
   static const MouseEventFlags MouseLeave;
   static const MouseEventFlags MouseMove;
   static const MouseEventFlags WheelUp;
   static const MouseEventFlags WheelDown;
   static const MouseEventFlags Any;
   static const MouseEventFlags Wheel;
   static const MouseEventFlags All;

   virtual ~MouseEventInterface() {
   }
   virtual void OnLeft(wxMouseEvent&)=0;
   virtual void OnLeftUp(wxMouseEvent&)=0;
   virtual void OnLeftDown(wxMouseEvent&)=0;
   virtual void OnLeftDoubleClick(wxMouseEvent&)=0;
   virtual void OnRight(wxMouseEvent&)=0;
   virtual void OnRightUp(wxMouseEvent&)=0;
   virtual void OnRightDown(wxMouseEvent&)=0;
   virtual void OnRightDoubleClick(wxMouseEvent&)=0;
   virtual void OnMiddle(wxMouseEvent&)=0;
   virtual void OnMiddleUp(wxMouseEvent&)=0;
   virtual void OnMiddleDown(wxMouseEvent&)=0;
   virtual void OnMiddleDoubleClick(wxMouseEvent&)=0;
   virtual void OnEnterWindow(wxMouseEvent&)=0;
   virtual void OnLeaveWindow(wxMouseEvent&)=0;
   virtual void OnWheel(wxMouseEvent&)=0;
   virtual void OnMouseMove(wxMouseEvent&)=0;
   virtual void OnMouse(wxMouseEvent&)=0;

   virtual MouseEventFlags GetMouseFlags() const=0;

   virtual void MouseEnabled(bool Enable = true)=0;
   virtual bool IsMouseEnabled() const=0;

   virtual bool CanProcessEvent(wxMouseEvent&) const=0;
};

class ForwardMouseEventImpl : public MouseEventInterface {
   ForwardMouseEventImpl(const ForwardMouseEventImpl&);

public:
   ForwardMouseEventImpl();
   ForwardMouseEventImpl(const std::string &ForwardedClass);
   ForwardMouseEventImpl(MouseEventInterface *pForwardedInstance);
   virtual ~ForwardMouseEventImpl();

   virtual void OnLeft(wxMouseEvent&);
   virtual void OnLeftUp(wxMouseEvent&);
   virtual void OnLeftDown(wxMouseEvent&);
   virtual void OnLeftDoubleClick(wxMouseEvent&);
   virtual void OnRight(wxMouseEvent&);
   virtual void OnRightUp(wxMouseEvent&);
   virtual void OnRightDown(wxMouseEvent&);
   virtual void OnRightDoubleClick(wxMouseEvent&);
   virtual void OnMiddle(wxMouseEvent&);
   virtual void OnMiddleUp(wxMouseEvent&);
   virtual void OnMiddleDown(wxMouseEvent&);
   virtual void OnMiddleDoubleClick(wxMouseEvent&);
   virtual void OnEnterWindow(wxMouseEvent&);
   virtual void OnLeaveWindow(wxMouseEvent&);
   virtual void OnWheel(wxMouseEvent&);
   virtual void OnMouseMove(wxMouseEvent&);
   virtual void OnMouse(wxMouseEvent&);

   virtual MouseEventFlags GetMouseFlags() const;

   virtual void MouseEnabled(bool Enable = true);
   virtual bool IsMouseEnabled() const;

   virtual bool CanProcessEvent(wxMouseEvent&) const;

private:
   MouseEventInterface *pForwardedClass_;
};

}  // namespace suri

#endif /* MOUSEEVENTINTERFACE_H_ */
