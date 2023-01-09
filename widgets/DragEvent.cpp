/*! \file DragEvent.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
