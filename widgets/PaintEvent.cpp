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

#include "PaintEvent.h"

// Includes Estandar
// Includes Suri
#include "suri/PaintEventInterface.h"

// Includes Wx
#include <wx/wx.h>
#include <wx/dcbuffer.h>

// Defines

// forwards

namespace suri {

PaintEventInterface *PaintEvent::GetHandler() {
   if (painters_.empty()) return NULL;
   return painters_.back();
}

PaintEvent::PaintEvent(PaintEventInterface *pParent) {
   if (pParent) {
      Push(pParent);
      if (pParent->GetPaintFlags()
            & (PaintEventInterface::Paint | PaintEventInterface::Background))
         Connect(wxEVT_PAINT, wxPaintEventHandler(PaintEvent::OnPaint));
   }
}

PaintEvent::~PaintEvent() {
   Disconnect(wxEVT_PAINT, wxPaintEventHandler(PaintEvent::OnPaint));
   while (!painters_.empty()) {
      Pop();
   }
}

void PaintEvent::Push(PaintEventInterface*p) {
   if (p) painters_.push_back(p);
}

void PaintEvent::Pop() {
   painters_.pop_back();
}

void PaintEvent::OnPaint(wxPaintEvent& Event) {
   Event.Skip();
   if (painters_.empty()) {
      return;
   }

   wxWindow *pwindow = wxDynamicCast(Event.GetEventObject(), wxWindow);
   if (!pwindow) return;
   wxAutoBufferedPaintDC paintdc(pwindow);
   std::deque<PaintEventInterface*>::iterator it = painters_.begin();

   wxEraseEvent eraseevt(Event.GetId(), &paintdc);
   eraseevt.Skip();
   bool backgrounderasehandled = false;
   while (it != painters_.end() && eraseevt.GetSkipped()) {
      PaintEventInterface *ppainter = *it;
      if ((ppainter->GetPaintFlags() & PaintEventInterface::Background)
            && ppainter->IsPaintEnabled() && ppainter->CanProcessEvent(Event)) {
         ppainter->OnBackgroundErase(eraseevt);
         backgrounderasehandled = true;
      }
      ++it;
   }
   // si no se produjo un evento background, entonces lo limpia en forma default
   if (!backgrounderasehandled) {
      wxBrush oldbrush = paintdc.GetBrush();
      wxPen oldpen = paintdc.GetPen();
      paintdc.SetBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
      paintdc.SetPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
      paintdc.DrawRectangle(pwindow->GetClientSize());
      paintdc.SetBrush(oldbrush);
      paintdc.SetPen(oldpen);
   }

   it = painters_.begin();
   while (it != painters_.end() && Event.GetSkipped()) {
      PaintEventInterface *ppainter = *it;
      if (ppainter->GetPaintFlags() & PaintEventInterface::Paint
            && ppainter->IsPaintEnabled() && ppainter->CanProcessEvent(Event)) {
         ppainter->OnPaint(Event, paintdc);
      }
      ++it;
   }
}

}  // namespace suri

