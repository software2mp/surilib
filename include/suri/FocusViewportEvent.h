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

#ifndef FOCUSVIEWEREVENT_H_
#define FOCUSVIEWPORTEVENT_H_

// Includes Estandar

// Includes Suri
#include "suri/MouseEventInterface.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/SuriObject.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 *
 */
class FocusViewportEvent : public MouseEventInterface {
public:
   FocusViewportEvent(ViewportManagerInterface *pVportManager,
                      const SuriObject::UuidType &ViewerId);
   virtual ~FocusViewportEvent();

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

   virtual MouseEventInterface::MouseEventFlags GetMouseFlags() const;

   virtual void MouseEnabled(bool Enable = true);
   virtual bool IsMouseEnabled() const;

   virtual bool CanProcessEvent(wxMouseEvent&) const;

   virtual void FocusViewport(wxMouseEvent &Event)const;

private:
   ViewportManagerInterface *pVportManager_;
   SuriObject::UuidType viewerId_;
};

} /** namespace suri */

#endif /* FOCUSVIEWEREVENT_H_ */
