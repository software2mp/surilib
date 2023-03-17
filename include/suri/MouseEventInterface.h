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
