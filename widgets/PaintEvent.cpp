/*! \file PaintEvent.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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

