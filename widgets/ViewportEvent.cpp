/*! \file ViewportEvent.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <vector>
// Includes Suri
#include "suri/ViewportEvent.h"
#include "suri/DynamicViewerWidget.h"
// Includes Wx
// Defines
// forwards

namespace suri {

ViewportEvent::ViewportEvent(ViewportManagerInterface *pViewportManager,
                             ViewcontextManagerInterface *pViewcontextManager) :
      EventInterface(pViewportManager, pViewcontextManager), isDragging_(false), inside_(false),
      pViewportManager_(pViewportManager), pViewcontextManager_(pViewcontextManager),
      paintEnabled_(true), mouseEnabled_(true) {
}

ViewportEvent::~ViewportEvent() {
}

void ViewportEvent::OnLeft(wxMouseEvent&) {
}

void ViewportEvent::OnLeftUp(wxMouseEvent &Event) {
   double x = Event.GetX(), y = Event.GetY();
   if (isDragging_) {
      OnEndDrag(startDrag_, Coordinates(x, y), Event);
   } else if (inside_) {
      SetPoint(x, y, Event);
   }
   isDragging_ = false;
   Reset();
}

void ViewportEvent::OnLeftDown(wxMouseEvent &Event) {
   inside_ = true;
   startDrag_ = Coordinates(Event.GetX(), Event.GetY());
   OnStartDrag(startDrag_);
}

void ViewportEvent::OnLeftDoubleClick(wxMouseEvent&) {
}

void ViewportEvent::OnRight(wxMouseEvent&) {
}

void ViewportEvent::OnRightUp(wxMouseEvent &Event) {
   OnLeftUp(Event);
}

void ViewportEvent::OnRightDown(wxMouseEvent &Event) {
   OnLeftDown(Event);
}

void ViewportEvent::OnRightDoubleClick(wxMouseEvent&) {
}

void ViewportEvent::OnMiddle(wxMouseEvent&) {
}

void ViewportEvent::OnMiddleUp(wxMouseEvent &Event) {
   OnLeftUp(Event);
}

void ViewportEvent::OnMiddleDown(wxMouseEvent &Event) {
   OnLeftDown(Event);
}

void ViewportEvent::OnMiddleDoubleClick(wxMouseEvent&) {
}

void ViewportEvent::OnEnterWindow(wxMouseEvent&Event) {
   inside_ = false;
   isDragging_ = false;
   Reset();
   wxWindow *pwindow = GetEventWindow(Event);
   if (pwindow) {
      wxCursor cursor(wxCURSOR_ARROW);
      GetCursor(cursor);
      pwindow->SetCursor(cursor);
   }
}

void ViewportEvent::OnLeaveWindow(wxMouseEvent &Event) {
   inside_ = false;
   isDragging_ = false;
   Reset();
   wxWindow *pwindow = GetEventWindow(Event);
   if (pwindow) {
      wxCursor cursor(wxCURSOR_ARROW);
      pwindow->SetCursor(cursor);
   }
}

void ViewportEvent::OnWheel(wxMouseEvent&) {
}

void ViewportEvent::OnMouseMove(wxMouseEvent &Event) {
   double x = Event.GetX(), y = Event.GetY();
   UpdatePoint(x, y, Event);
   if (Event.Dragging() && inside_) {
      isDragging_ = true;
      OnDrag(startDrag_, Coordinates(x, y), Event);
   }
}

void ViewportEvent::OnMouse(wxMouseEvent&Event) {
   wxWindow *pwindow = GetEventWindow(Event);
   if (pwindow && Event.Dragging()) pwindow->Refresh();
}

void ViewportEvent::GetCursor(wxCursor &Cursor) {
}

/**
 * Se ejecuta cuando el usuario comienza drag sobre visualizador
 * @param[in] InitialCoordinate coordenada donde se inicio el drag.
 */
void ViewportEvent::OnStartDrag(const Coordinates &InitialCoordinate) {
}

/**
 * Se ejecuta cuando el usuario finaliza drag
 * @param[in] InitialCoordinate coordenada donde se inicio el drag.
 * @param[in] CurrentCoordinate coordenada actual del mouse
 * @param[in] Event evento de mouse.
 */
void ViewportEvent::OnEndDrag(const Coordinates &InitialCoordinate,
           const Coordinates &CurrentCoordinate, wxMouseEvent &Event) {
}

/**
 * Se ejecuta cada vez que el usuario mueve el mouse mientras realiza drag
 * @param[in] InitialCoordinate coordenada donde el usuario inicio drag
 * @param[in] CurrentCoordinate coordenada actual del mouse
 * @param[in] Event evento wx
 */
void ViewportEvent::OnDrag(const Coordinates &InitialCoordinate,
                          const Coordinates &CurrentCoordinate,
                          wxMouseEvent &Event) {
}

/**
 * Indica si se esta realizando drag con el mouse sobre le visualizador
 * @return bool que indica si se esta realizando drag.
 */
bool ViewportEvent::IsDragging() {
   return isDragging_;
}

MouseEventInterface::MouseEventFlags ViewportEvent::GetMouseFlags() const {
   return MouseEventInterface::All;
}

void ViewportEvent::MouseEnabled(bool Enable) {
   mouseEnabled_ = Enable;
}

bool ViewportEvent::IsMouseEnabled() const {
   return mouseEnabled_;
}

void ViewportEvent::OnPaint(wxPaintEvent&Event, wxDC &dc) {
}

void ViewportEvent::OnBackgroundErase(wxEraseEvent&) {
}

PaintEventInterface::PaintEventFlags ViewportEvent::GetPaintFlags() const {
   return PaintEventInterface::NoneFlag;
}

void ViewportEvent::PaintEnabled(bool Enable) {
   paintEnabled_ = Enable;
}

bool ViewportEvent::IsPaintEnabled() const {
   return paintEnabled_;
}

void ViewportEvent::Enabled(bool Enable) {
   MouseEnabled(Enable);
   PaintEnabled(Enable);
}

bool ViewportEvent::IsEnabled() const {
   return paintEnabled_ && mouseEnabled_;
}

bool ViewportEvent::CanProcessEvent(wxPaintEvent &Event) const {
   return CanProcessEvent(*wxDynamicCast(&Event, wxEvent));
}

bool ViewportEvent::CanProcessEvent(wxMouseEvent &Event) const {
   return CanProcessEvent(*wxDynamicCast(&Event, wxEvent));
}

bool ViewportEvent::CanProcessEvent(wxEvent &Event) const {
   wxWindow *pwindow = GetEventWindow(Event);
   ViewerWidget *pviewer = AS_VW(pViewportManager_->GetSelectedViewport());
   DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(pviewer);
   if (pdynamic)
      pviewer = pdynamic->GetViewer();
   return pwindow && pviewer && (pviewer->GetWindow() == pwindow);
}

wxWindow *ViewportEvent::GetEventWindow(const wxEvent &Event) const {
   return wxDynamicCast(Event.GetEventObject(), wxWindow);
}

ViewportManagerInterface *ViewportEvent::GetViewportManager() const {
   return pViewportManager_;
}

ViewcontextManagerInterface *ViewportEvent::GetViewcontextManager() const {
   return pViewcontextManager_;
}

/**
 * Retorna el ViewerWidget donde se realizo el evento
 * @param[in] Event evento a analizar
 * @return ViewerWidget del visualizador donde ocurrio el evento.
 */
ViewerWidget* ViewportEvent::GetEventViewer(wxEvent &Event) const {
   wxWindow *pwindow = GetEventWindow(Event);
   ViewerWidget *pviewer = NULL;
   std::vector<SuriObject::UuidType> ids = GetViewportManager()->GetViewportIds();
   std::vector<SuriObject::UuidType>::iterator it = ids.begin();
   for (; it != ids.end() && !pviewer; ++it) {
      ViewerWidget* ptempviewer = AS_VW(GetViewportManager()->GetViewport(*it));
      DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(ptempviewer);
      if (pdynamic)
         ptempviewer = pdynamic->GetViewer();
      if (ptempviewer->GetWindow() == pwindow)
         pviewer = ptempviewer;
   }
   return pviewer;
}

/** 
 * Refresca la ventana del visualizador 
 * @param[in] Ids id de los visualizadores a actualizar
 */
void  ViewportEvent::RefreshViewers(const std::vector<SuriObject::UuidType> &Ids) {
   std::vector<SuriObject::UuidType>::const_iterator it = Ids.begin();
   for (; it != Ids.end(); ++it) {
      if (GetViewportManager()->IsViewportActive(*it)) {
         ViewerWidget* ptempviewer = AS_VW(GetViewportManager()->GetViewport(*it));
         ptempviewer->GetWindow()->Refresh();
      }
   }
}

/** 
 * Retorna subset de busqueda centrado en la coordenada y con srs de mundo 
 * @param[in] Coordinate coordenada central del subset en pixel-linea.
 * @param[in] pWorld mundo sobre el que se esta trabajando
 * @return subset de busqueda centrado en la coordenada y con srs de mundo 
 */
Subset ViewportEvent::GetWorldSearchBox(const Coordinates Coordinate, World* pWorld) {
   Subset viewportsubset = Subset(Coordinate, Coordinate) + SEARCH_PIXEL_BUFFER;

   Subset worldsubset;
   pWorld->Transform(viewportsubset.ul_, worldsubset.ul_);
   pWorld->Transform(viewportsubset.lr_, worldsubset.lr_);
   return worldsubset;
}

} /* namespace suri */
