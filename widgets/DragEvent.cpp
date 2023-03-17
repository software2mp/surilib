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

// Includes Estandar

// Includes Suri
#include "suri/DragEvent.h"
#include "suri/Viewer2D.h"
#include "suri/DynamicViewerWidget.h"

// Includes Wx

// Defines

// forwards

namespace suri {

DragEvent::DragEvent(ViewportManagerInterface *pViewportManager,
                     ViewcontextManagerInterface *pViewcontextManager) :
      ViewportEvent(pViewportManager, pViewcontextManager),
      updating_(false), initialPosX_(0), initialPosY_(0), pwin_(NULL) {
}

DragEvent::~DragEvent() {
}

void DragEvent::Reset() {
   if (pwin_ && updating_) {
      wxMouseEvent event;
      event.SetEventObject(pwin_);
      pwin_ = NULL;
   }
   updating_ = false;
   initialPosX_ = 0;
   initialPosY_ = 0;
}

void DragEvent::GetCursor(wxCursor &Cursor) {
   Cursor = wxCursor(wxCURSOR_HAND);
}

void DragEvent::OnDrag(const Coordinates &InitialCoordinate,
                             const Coordinates &CurrentCoordinate,
                             wxMouseEvent &Event) {
   wxWindow *pviewerwindow = GetEventWindow(Event);
   // guarda coordenadas para que al salir de la ventana lea este subset
   // y el bmp desplazado y el renderizado coincidan.
   if (!updating_) {
      initial_ = InitialCoordinate;
   }
   final_ = CurrentCoordinate;
   updating_ = true;

   pwin_ = pviewerwindow;
   Viewer2D *pviewer =
         dynamic_cast<Viewer2D*>(GetViewportManager()->GetSelectedViewport());
   if (!pviewer) return;
   World *pviewerworld = pviewer->GetWorld();

   Subset world;
   pviewerworld->GetWorld(world);
   Coordinates wid;
   pviewerworld->Transform(InitialCoordinate, wid);

   if (world.IsInside(wid)) {
      // Offset del mouse drag
      int ax = CurrentCoordinate.x_ - InitialCoordinate.x_;
      int ay = CurrentCoordinate.y_ - InitialCoordinate.y_;
      // Crea los eventos en ax y ay para simular el movimiento de las scrollbar
      wxScrollWinEvent evtX(wxEVT_SCROLLWIN_THUMBTRACK, InitialCoordinate.x_ - ax,
                            wxHORIZONTAL);
      wxScrollWinEvent evtY(wxEVT_SCROLLWIN_THUMBTRACK, InitialCoordinate.y_ - ay,
                            wxVERTICAL);
      initialPosX_ = ax;
      initialPosY_ = ay;

      REPORT_DEBUG("DELTA X, DELTA Y: %d,%d", ax, ay);
      // Postea los 2 eventos a la scrollwindow
      pviewerwindow->ProcessEvent(evtX);
      pviewerwindow->ProcessEvent(evtY);
      // Redibuja la scrollwindow
      pviewerwindow->Refresh();
   }
}

void DragEvent::OnEndDrag(const Coordinates &InitialCoordinate,
                          const Coordinates &CurrentCoordinate, wxMouseEvent &Event) {
   Viewer2D *pviewer = dynamic_cast<Viewer2D*>(GetViewportManager()->GetSelectedViewport());
   DynamicViewerWidget *pdynamic =
         dynamic_cast<DynamicViewerWidget*>(GetViewportManager()->GetSelectedViewport());
   if (!pviewer && pdynamic)
      pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
   if (!pviewer)
      return;
   World *pviewerworld = pviewer->GetWorld();

   Coordinates initialwin, currentwin;
   pviewerworld->Transform(InitialCoordinate, initialwin);
   pviewerworld->Transform(CurrentCoordinate, currentwin);

   Subset window;
   pviewerworld->GetWindow(window);
   window = window + Coordinates(initialwin.x_ - currentwin.x_, initialwin.y_ - currentwin.y_);

   pviewer->ChangeWindow(window);
   updating_ = false;
}

void DragEvent::SetPoint(double Pixel, double Line, wxMouseEvent &Event) {
}

void DragEvent::UpdatePoint(double Pixel, double Line, wxMouseEvent &Event) {
}

PaintEventInterface::PaintEventFlags DragEvent::GetPaintFlags() const {
   return PaintEventInterface::NoneFlag;
}

} /* namespace suri */
