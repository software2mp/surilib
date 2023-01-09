/*! \file DragButton.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard

// Includes suri
#include "DragButton.h"
#include "suri/messages.h"
#include "resources.h"
#include "suri/World.h"
#include "suri/NavigationWidget.h"
#include "suri/VectorEditor.h"
#include "suri/Configuration.h"

// Includes Wx
#include "wx/wx.h"

// Includes app

// Defines

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] pToolbar toolbar en el que se inserta el boton
 * @param[in] pViewer puntero al viewer
 */
DragButton::DragButton(wxWindow *pToolbar, Viewer2D *pViewer) :
      ToggleButton(pToolbar, wxT(tool_SIMUR_HAND), wxT(tooltip_DRAG),
                   BUTTON_LEFT_UP | BUTTON_LEFT_DOWN | BUTTON_LEAVE | BUTTON_MOVE),
      pViewer_(pViewer), initialDragX_(0), initialDragY_(0) {
}

/**
 * Destructor
 */
DragButton::~DragButton() {
}

/**
 * Cambia cursor a mano
 * @param[in] State estado del cursor (puntero o mano)
 */
void DragButton::DoSetCursor(bool State) {
   if (!pViewer_ || !pViewer_->GetWindow())
      return;
   if (State) {
      pViewer_->GetWindow()->SetCursor(wxCursor(wxCURSOR_HAND));
   } else {
      pViewer_->GetWindow()->SetCursor(wxCursor(wxCURSOR_DEFAULT));
   }
}

/**
 * Cambia subset de la ventana
 * @param[in] Event evento generado al levantar el boton izq
 */
void DragButton::DoOnLeftUp(wxMouseEvent &Event) {
   // DRAGGING-------------------------------------------------------------------
   // Si esta dragueando el click inicial fue dentro del viewer2D,
   // y el drag sea sobre el mundo
   Subset world;
   pViewer_->GetWorld()->GetWorld(world);
   double wx, wy;
   pViewer_->GetWorld()->V2WTransform(initialDragX_, initialDragY_, wx, wy);
   Coordinates coords(wx, wy);
   if (insideLeft_ && isDraggingLeft_ && world.IsInside(coords)) {
      int x, y;
      // Calcula el centro de la pantalla
      pViewer_->GetWorld()->GetViewport(x, y);
      x = x / 2;
      y = y / 2;
      // ACA VA LA LLAMADA AL ZOOMIN CON FACTOR 1
      // Desplaza al centro de la pantalla lo que se movio dragueando
      // con el mouse
      // Calcula el delta dragueado con el mouse
      int ax = Event.GetX() - initialDragX_;
      int ay = Event.GetY() - initialDragY_;
      REPORT_DEBUG("D:Fin %i,%i", ax, ay);
      pViewer_->ZoomIn(x - ax, y - ay, 1);
   }
}

/**
 * Cambia posicion de la imagen mostrada
 * @param[in] Event evento generado al mover el mouse
 */
void DragButton::DoOnMouseMove(wxMouseEvent &Event) {
   // Si esta draggeando y el drag empezo dentro de la ventana
   if (Event.LeftIsDown() && Event.Dragging() && insideLeft_) {
      if (!isDraggingLeft_) {
         initialDragX_ = Event.GetX();
         initialDragY_ = Event.GetY();
         initialPosX_ = pViewerWindow_->GetScrollPos(wxHORIZONTAL);
         initialPosY_ = pViewerWindow_->GetScrollPos(wxVERTICAL);
      }
      // Si esta haciendo drag desplaza la imagen hasta el leftclickup
      int sx, sy;
      pViewer_->GetWorld()->GetViewport(sx, sy);

      Subset world;
      pViewer_->GetWorld()->GetWorld(world);
      double widx, widy;
      pViewer_->GetWorld()->V2WTransform(initialDragX_, initialDragY_, widx, widy);
      Coordinates coordsInitialDrag(widx, widy);

      if (world.IsInside(coordsInitialDrag)) {
         // Offset del mouse drag
         int ax = Event.GetX() - initialDragX_;
         int ay = Event.GetY() - initialDragY_;
         // Crea los eventos en ax y ay para simular el movimiento de las scrollbar
         wxScrollWinEvent evtX(wxEVT_SCROLLWIN_THUMBTRACK, initialPosX_ - ax,
                               wxHORIZONTAL);
         wxScrollWinEvent evtY(wxEVT_SCROLLWIN_THUMBTRACK, initialPosY_ - ay,
                               wxVERTICAL);
         REPORT_DEBUG("DELTA X, DELTA Y: %d,%d", ax, ay);
         // Postea los 2 eventos a la scrollwindow
         pViewerWindow_->AddPendingEvent(evtX);
         pViewerWindow_->AddPendingEvent(evtY);
         // Redibuja la scrollwindow
         pViewerWindow_->Refresh();
      }
   }
}

/**
 * Si el mouse sale de la pantalla mientras se hace drag la ventana muestra
 * la mitad del dibujo
 * @param[in] Event evento generado al salir de una ventana
 */
void DragButton::DoOnLeaveWindow(wxMouseEvent &Event) {
   if (!isDraggingLeft_) {
      REPORT_AND_RETURN("D:Salio de LeaveWindow sin accion porque no draggeaba");
   }
   // DRAG-----------------------------------------------------------------------
   // Verifica que este drageando, el click haya sido en la ventana, y el drag
   // sea sobre el mundo
   Subset world;
   pViewer_->GetWorld()->GetWorld(world);
   double wx, wy;
   pViewer_->GetWorld()->V2WTransform(initialDragX_, initialDragY_, wx, wy);
   Coordinates coords(wx, wy);

   if (insideLeft_ && world.IsInside(coords)) {
      // Valida que se este dragueando y que al dispararse el evento
      // el mouse este fuera de la pantalla porque a veces
      // se esta ejecutnado el evento aunque el mouse no este fuera de la
      // pantalla (Si hago drag, suelto el boton pero sigo moviendo el
      // mouse)
      // Se usa <= y >= para mas seguridad
      int sx, sy;
      pViewerWindow_->GetClientSize(&sx, &sy);

      if (isDraggingLeft_
            && (Event.GetX() <= 0 || Event.GetX() >= sx || Event.GetY() <= 0
                  || Event.GetY() >= sy)) {
         // Calcula el centro de la pantalla
         int x, y;
         //					pScreenCanvas_->GetScreenSize(x,y);
         pViewer_->GetWorld()->GetViewport(x, y);
         x = x / 2;
         y = y / 2;

         // ACA VA LA LLAMADA AL ZOOMIN CON FACTOR 1
         // Desplaza al centro de la pantalla lo que se movio dragueando
         // con el mouse
         // Calcula el delta dragueado con el mouse
         int ax = Event.GetX() - initialDragX_;
         int ay = Event.GetY() - initialDragY_;
         pViewer_->ZoomIn(x - ax, y - ay, 1);
      }
   }
}
}  // namespace suri

