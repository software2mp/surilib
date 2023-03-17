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
#include <cmath>
#include <algorithm>

// Includes Suri
#include "suri/MultiZoom.h"
#include "AspectPreservingWorld.h"
#include "suri/Viewer2D.h"
#include "suri/DynamicViewerWidget.h"

// Includes Wx
// Defines
// forwards

namespace suri {

MultiZoom::MultiZoom(ViewportManagerInterface *pViewportManager,
                     ViewcontextManagerInterface *pViewcontextManager) :
      ViewportEvent(pViewportManager, pViewcontextManager),
      rubberband_(Subset()), wasUpdating_(false) {
}

MultiZoom::~MultiZoom() {
}

void MultiZoom::Reset() {
   wasUpdating_ = false;
   wxRect rubberrect(rubberband_.ul_.x_, rubberband_.ul_.y_,
                     rubberband_.lr_.x_ - rubberband_.ul_.x_,
                     rubberband_.lr_.y_ - rubberband_.ul_.y_);
   rubberband_ = Subset();
   AS_VW(GetViewportManager()->GetSelectedViewport())->GetWindow()->Refresh(
         true, &rubberrect);
   AS_VW(GetViewportManager()->GetSelectedViewport())->GetWindow()->Update();
}

void MultiZoom::GetCursor(wxCursor &Cursor) {
   Cursor = wxCursor(wxCURSOR_MAGNIFIER);
}

void MultiZoom::SetSubsetAspectRatio(const Subset &OldSubset, Subset &CorrectedSubset) {
   Subset newwindow = OldSubset, aux;
   ViewerWidget* pviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   if (dynamic_cast<DynamicViewerWidget*>(pviewer))
      pviewer = dynamic_cast<DynamicViewerWidget*>(pviewer)->GetViewer();
   World *pworld = new AspectPreservingWorld, *pviewportworld = pviewer->GetWorld();
   if (!pworld || !pviewportworld) return;
   pworld->ResetWorld();
   pworld->SetSpatialReference(pviewportworld->GetSpatialReference());
   pviewportworld->GetWorld(aux);
   pworld->SetWorld(aux);
   pviewportworld->GetWindow(aux);
   pworld->SetWindow(aux);
   int x1, x2;
   pviewportworld->GetViewport(x1, x2);
   pworld->SetViewport(x1, x2);
   // \TODO(Javier - Sin TCK): Acelerar, extremadamente LENTO
   if (pworld->IsInitialized()) {
      // Aplico subset con coordenadas de mouse al world que copie
      pviewportworld->Transform(OldSubset.ul_, newwindow.ul_);
      pviewportworld->Transform(OldSubset.lr_, newwindow.lr_);
      pworld->SetWindow(newwindow);
      pworld->GetWindow(newwindow);
      pviewportworld->InverseTransform(newwindow.ul_, aux.ul_);
      pviewportworld->InverseTransform(newwindow.lr_, aux.lr_);
      newwindow = aux;
   }
   delete pworld;
   CorrectedSubset = newwindow;
}

void MultiZoom::OnDrag(const Coordinates &InitialCoordinate,
                             const Coordinates &CurrentCoordinate,
                             wxMouseEvent &Event) {
   if (Event.LeftIsDown() || Event.RightIsDown()) {
      ViewportInterface *pviewer = GetViewportManager()->GetSelectedViewport();
      Viewer2D* p2dviewer = dynamic_cast<Viewer2D*>(pviewer);
      DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(pviewer);
      if (!p2dviewer && pdynamic)
         p2dviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
      Subset newwindow;
      SetSubsetAspectRatio(Subset(InitialCoordinate, CurrentCoordinate), newwindow);
      rubberband_ = newwindow;
      wasUpdating_ = true;
      p2dviewer->Block();
   }
}

void MultiZoom::OnEndDrag(const Coordinates &InitialCoordinate,
                          const Coordinates &CurrentCoordinate, wxMouseEvent &Event) {
   Subset currentsubset(
         Coordinates(std::min(InitialCoordinate.x_, CurrentCoordinate.x_),
                     std::min(InitialCoordinate.y_, CurrentCoordinate.y_)),
         Coordinates(std::max(InitialCoordinate.x_, CurrentCoordinate.x_),
                     std::max(InitialCoordinate.y_, CurrentCoordinate.y_)));

   if (wasUpdating_) {
      ViewportInterface *pviewer = GetViewportManager()->GetSelectedViewport();
      Viewer2D* p2dviewer = dynamic_cast<Viewer2D*>(pviewer);
      DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(pviewer);
      if (!p2dviewer && pdynamic)
         p2dviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
      if (!p2dviewer)
         return;
      if (Event.LeftUp()) {
         const World *pworld = p2dviewer->GetWorld();

         Subset window;
         SetSubsetAspectRatio(currentsubset, window);

         // Verifico que sea una ventana valida. Si alguno de los valores viene
         // como NaN significa que la ventana ya no es valida (en este caso se
         // eligio el valor x de las coordenadas de vertice superior izquierdo)
         // Si es invalida se cancela el proceso para que no llegue al renderer
         // y queden las cosas como estan (ultima ventana valida)
         if (window.ul_.x_ != window.ul_.x_) {
            return;
         }

         pworld->Transform(Coordinates(window.ul_), window.ul_);
         pworld->Transform(Coordinates(window.lr_), window.lr_);
         p2dviewer->ChangeWindow(window);
      }
      if (Event.RightUp()) {
         Subset window;
         SetSubsetAspectRatio(currentsubset, window);
         int dragwidth = std::abs(window.ul_.x_ - window.lr_.x_);
         int dragheight = std::abs(window.ul_.y_ - window.lr_.y_);
         // calculo el centro del recuadro
         int dragcenterx = (window.ul_.x_ + window.lr_.x_) / 2;
         int dragcentery = (window.ul_.y_ + window.lr_.y_) / 2;
         int viewportwidth, viewportheight;
         const World *pworld = p2dviewer->GetWorld();
         pworld->GetViewport(viewportwidth, viewportheight);

         // busca ventana en viewport que centra ventana actual en recuadro
         window.ul_.x_ = viewportwidth / 2
               - static_cast<double>(viewportwidth) / dragwidth * dragcenterx;
         window.ul_.y_ = viewportheight / 2
               - static_cast<double>(viewportheight) / dragheight * dragcentery;
         window.lr_.x_ = viewportwidth / 2
               + static_cast<double>(viewportwidth) / dragwidth
                     * (viewportwidth - dragcenterx);
         window.lr_.y_ = viewportheight / 2
               + static_cast<double>(viewportheight) / dragheight
                     * (viewportheight - dragcentery);

         // Transforma coord de nueva ventana a mundo y cambia la ventana de viewer
         pworld->Transform(Coordinates(window.ul_), window.ul_);
         pworld->Transform(Coordinates(window.lr_), window.lr_);
         p2dviewer->ChangeWindow(window);
      }
      p2dviewer->UnBlock();
   }
   wasUpdating_ = false;
}

void MultiZoom::SetPoint(double Pixel, double Line, wxMouseEvent &Event) {
   Viewer2D *pviewer = dynamic_cast<Viewer2D*>(GetViewportManager()->GetSelectedViewport());
   DynamicViewerWidget* pdynamic =
         dynamic_cast<DynamicViewerWidget*>(GetViewportManager()->GetSelectedViewport());
   if (!pviewer && pdynamic)
      pviewer = dynamic_cast<Viewer2D*>(pdynamic->GetViewer());
   if (!pviewer)
      return;
   if (Event.LeftUp()) {
      pviewer->Block();
      pviewer->ZoomIn(Pixel, Line);
      pviewer->UnBlock();
   }
   if (Event.RightUp()) {
      pviewer->Block();
      pviewer->ZoomOut(Pixel, Line);
      pviewer->UnBlock();
   }
}

void MultiZoom::UpdatePoint(double Pixel, double Line, wxMouseEvent &Event) {
}

PaintEventInterface::PaintEventFlags MultiZoom::GetPaintFlags() const {
   return PaintEventInterface::Paint;
}

void MultiZoom::OnPaint(wxPaintEvent&Event, wxDC &dc) {
   if (wasUpdating_) {
      dc.SetLogicalFunction(wxINVERT);
      dc.SetBrush(wxBrush("red", wxTRANSPARENT));
      dc.DrawRectangle(rubberband_.ul_.x_, rubberband_.ul_.y_,
                       rubberband_.lr_.x_ - rubberband_.ul_.x_,
                       rubberband_.lr_.y_ - rubberband_.ul_.y_);
   }
}

} /* namespace suri */
