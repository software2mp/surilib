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

#ifndef MOUSEEVENT_H_
#define MOUSEEVENT_H_

// Includes Estandar

// Includes Suri
#include "suri/wxSuriMouseEvent.h"
#include "suri/MouseEventInterface.h"

// Includes Wx
#include <wx/event.h>

// Defines

// forwards

namespace suri {

class MouseEvent : public wxEvtHandler {
public:
   MouseEvent(MouseEventInterface *pParent);
   virtual ~MouseEvent();

   void OnLeft(wxMouseEvent &Event);
   void OnLeftUp(wxMouseEvent &Event);
   void OnLeftDown(wxMouseEvent &Event);
   void OnLeftDoubleClick(wxMouseEvent &Event);
   void OnRight(wxMouseEvent &Event);
   void OnRightUp(wxMouseEvent &Event);
   void OnRightDown(wxMouseEvent &Event);
   void OnRightDoubleClick(wxMouseEvent &Event);
   void OnMiddle(wxMouseEvent &Event);
   void OnMiddleUp(wxMouseEvent &Event);
   void OnMiddleDown(wxMouseEvent &Event);
   void OnMiddleDoubleClick(wxMouseEvent &Event);
   void OnEnterWindow(wxMouseEvent &Event);
   void OnLeaveWindow(wxMouseEvent &Event);
   void OnWheel(wxMouseEvent &Event);
   void OnMouseMove(wxMouseEvent &Event);
   void OnMouse(wxMouseEvent &Event);

   void OnLeftUp(wxSuriMouseEvent &Event);


   MouseEventInterface *GetHandler();
protected:
private:
   MouseEventInterface *pParent_; /*Herramienta padre*/
};

}  // namespace suri

#endif   // MOUSEEVENT_H_
