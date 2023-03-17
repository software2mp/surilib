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

#include "ZoomButton.h"

// Includes standard

// Includes suri
#include "suri/Viewer2D.h"
#include "suri/messages.h"
#include "suri/Coordinates.h"
#include "suri/Configuration.h"
#include "resources.h"
#include "AspectPreservingWorld.h"

// Includes Wx
// Includes App
// Defines
/** Tolerancia en cantidad de pixels, utilizado en zoom in. */
#define DRAG_TOLERANCE 3
/** Tolerancia en cantidad de pixels, utilizado en zoom out. */
#define DRAG_TOLERANCE_OUT 3

/** namespace suri */
namespace suri {
/** Ctor */
ZoomButton::ZoomButton(wxWindow *pToolbar, Viewer2D *pViewer) :
      ToggleButton(
            pToolbar,
            wxT(tool_SIMUR_ZOOM),
            wxT(tooltip_ZOOM),
            BUTTON_LEFT_UP | BUTTON_RIGHT_UP | BUTTON_LEAVE | BUTTON_LEFT_DOWN
                  | BUTTON_RIGHT_DOWN | BUTTON_MOVE),
      pViewer_(pViewer), zoomInActivated_(false), zoomOutActivated_(false),
      drawRectangle_(false) {
}

/** Dtor */
ZoomButton::~ZoomButton() {
}

/**
 * Cambia cursor a magnificador
 * @param[in] State estado de activacion del boton
 */
void ZoomButton::DoSetCursor(bool State) {
   if (State) {
      pViewerWindow_->SetCursor(wxCursor(wxCURSOR_MAGNIFIER));
   } else {
      pViewerWindow_->SetCursor(wxCursor(wxCURSOR_DEFAULT));
   }
}

/**
 * Desactiva el zoom se sale de la ventana
 * @param[in] Event evento de mouse generado cuando sale de la ventana
 */
void ZoomButton::DoOnLeaveWindow(wxMouseEvent &Event) {
   if ((zoomInActivated_) || (zoomOutActivated_)) {
      DrawRectangleWithInvertedColor(Coordinates(initialDragX_, initialDragY_),
                                     Coordinates(lastDrawX_, lastDrawY_));
      zoomInActivated_ = false;
      zoomOutActivated_ = false;
   }
}

/**
 * Inicializa recuandro para zoom in
 * @param[in] Event evento de mouse cuando se presiona el boton izquierdo
 * \post si zoomout no esta en proceso se inicializa zoom in
 * \post si zoomout esta en proceso se anula su aplicacion
 */
void ZoomButton::DoOnLeftDown(wxMouseEvent &Event) {
   if (zoomOutActivated_) {
      // si el zoom out esta activo lo elimino
      zoomOutActivated_ = false;
      DrawRectangleWithInvertedColor(Coordinates(initialDragX_, initialDragY_),
                                     Coordinates(lastDrawX_, lastDrawY_));
   } else {
      // Activo zoom in
      initialDragX_ = Event.GetX();
      initialDragY_ = Event.GetY();
      lastDrawX_ = Event.GetX();
      lastDrawY_ = Event.GetY();
      zoomInActivated_ = true;
   }
}

/** realiza zoom in en viewer2D */
/**
 * Hace zoom in
 * @param[in] Event evento de mouse cuando se libera boton izquierdo
 * \post Si la zona supera la tolerancia se cambia la ventana para que la
 * ventana actual contenga la zona marcada
 * \post Si la zona no supera la tolerancia se llama a zoom in del viewer
 */
void ZoomButton::DoOnLeftUp(wxMouseEvent &Event) {
   if ((!zoomInActivated_) || (zoomOutActivated_)) {
      return;
   }

   DrawRectangleWithInvertedColor(Coordinates(initialDragX_, initialDragY_),
                                  Coordinates(lastDrawX_, lastDrawY_));

   Coordinates newpointwindow = GetNewPointCoordinates(Event);
   Coordinates newpoint;
   pViewer_->GetWorld()->InverseTransform(newpointwindow, newpoint);

   const World *pworld = pViewer_->GetWorld();
   long dragrectx = (long) std::abs(initialDragX_ - newpoint.x_);
   long dragrecty = (long) std::abs(initialDragY_ - newpoint.y_);
   if (zoomInActivated_ && Event.LeftUp() && dragrectx > DRAG_TOLERANCE
         && dragrecty > DRAG_TOLERANCE) {
      Subset window;
      // Transformo coordenadas del viewport a mundo y cambio ventana de viewer
      Coordinates coordinate_ul(std::min((double) initialDragX_, newpoint.x_),
                                std::min((double) initialDragY_, newpoint.y_));
      pworld->Transform(coordinate_ul, window.ul_);
      Coordinates coordinate_lr(std::max(initialDragX_, (long) newpoint.x_),
                                std::max(initialDragY_, (long) newpoint.y_));
      pworld->Transform(coordinate_lr, window.lr_);
      pViewer_->ChangeWindow(window);
   } else
   // Si no se cumple con tolerancia se aplica zoom in de viewer
   if (zoomInActivated_ && Event.LeftUp()
         && ((dragrectx <= DRAG_TOLERANCE) || (dragrecty <= DRAG_TOLERANCE))) {
      int Factor = Configuration::GetParameter("app_zoom_modifier", 2);
      pViewer_->ZoomIn(Event.GetX(), Event.GetY(), Factor);
   }

   zoomInActivated_ = false;
}

/**
 * Inicializa recuandro para zoom in
 * @param[in] Event evento de mouse cuando se presiona boton derecho
 * \post si zoomin no esta en proceso se inicializa zoom in
 * \post si zoomin esta en proceso se anula su aplicacion
 */
void ZoomButton::DoOnRightDown(wxMouseEvent &Event) {
   if (zoomInActivated_) {
      // si el zoom in esta activo lo elimino
      zoomInActivated_ = false;
      DrawRectangleWithInvertedColor(Coordinates(initialDragX_, initialDragY_),
                                     Coordinates(lastDrawX_, lastDrawY_));
   } else {
      // activo el zoom out
      initialDragX_ = Event.GetX();
      initialDragY_ = Event.GetY();
      lastDrawX_ = Event.GetX();
      lastDrawY_ = Event.GetY();
      zoomOutActivated_ = true;
   }
}

/** realiza zoom out en viewer2D */
/**
 * Hace zoom out
 * @param[in] Event evento de mouse cuando se libera boton derecho
 * \post Si la zona supera la tolerancia se cambia la ventana para que la
 * ventana actual este contenida en la zona marcada
 * \post Si la zona no supera la tolerancia se llama a zoom out del viewer
 */
void ZoomButton::DoOnRightUp(wxMouseEvent &Event) {
   if ((!zoomOutActivated_) || (zoomInActivated_)) {
      return;
   }

   DrawRectangleWithInvertedColor(Coordinates(initialDragX_, initialDragY_),
                                  Coordinates(lastDrawX_, lastDrawY_));

   // Encuentro el nunto del recuadro y lo transformo a coord viewport
   Coordinates newpointwindow = GetNewPointCoordinates(Event);
   Coordinates newpoint;
   pViewer_->GetWorld()->InverseTransform(newpointwindow, newpoint);

   // Calculo la distancia del drag y comparo con tolerancias
   int dragwidth = (int) std::abs(initialDragX_ - newpoint.x_);
   int dragheight = (int) std::abs(initialDragY_ - newpoint.y_);
   if (zoomOutActivated_ && Event.RightUp() && dragwidth > DRAG_TOLERANCE_OUT
         && dragheight > DRAG_TOLERANCE_OUT) {
      Subset window;
      // calculo el centro del recuadro
      int dragcenterx = std::min(initialDragX_, (long) newpoint.x_) + dragwidth / 2;
      int dragcentery = std::min(initialDragY_, (long) newpoint.y_) + dragheight / 2;
      int viewportwidth, viewportheight;
      const World *pworld = pViewer_->GetWorld();
      pworld->GetViewport(viewportwidth, viewportheight);

      // busca ventana en viewport que centra ventana actual en recuadro
      window.ul_.x_ = viewportwidth / 2
            - (double) viewportwidth / dragwidth * dragcenterx;
      window.ul_.y_ = viewportheight / 2
            - (double) viewportheight / dragheight * dragcentery;
      window.lr_.x_ = viewportwidth / 2
            + (double) viewportwidth / dragwidth * (viewportwidth - dragcenterx);
      window.lr_.y_ = viewportheight / 2
            + (double) viewportheight / dragheight * (viewportheight - dragcentery);

      // Transforma coord de nueva ventana a mundo y cambia la ventana de viewer
      pworld->Transform(Coordinates(window.ul_), window.ul_);
      pworld->Transform(Coordinates(window.lr_), window.lr_);
      pViewer_->ChangeWindow(window);
   } else
   // Si no se cumple con tolerancia se aplica zoom out de viewer
   if (zoomOutActivated_ && Event.RightUp()
         && ((dragwidth <= DRAG_TOLERANCE_OUT) || (dragheight <= DRAG_TOLERANCE_OUT))) {
      int Factor = Configuration::GetParameter("app_zoom_modifier", 2);
      pViewer_->ZoomOut(Event.GetX(), Event.GetY(), Factor);
   }

   zoomOutActivated_ = false;
}

/**
 * dibuja el rectangulo si alguno de los dos zoom esta activado. El rectangulo
 * respeta el aspecto del viewport
 * @param[in] Event de mouse que se activa cuando se mueve el mouse
 * \attention cuando el zoom es muy grande falla el seteo de la ventana y no
 * puede dibujarse el rectangulo. Esto se debe a que la clase World considera
 * que los extremos de la ventana son muy cercanos y por lo tanto el mismo punto
 */
void ZoomButton::DoOnMouseMove(wxMouseEvent &Event) {
   if (zoomInActivated_ || zoomOutActivated_) {
      // Elimino rectangulo con coordenadas anteriores
      DrawRectangleWithInvertedColor(Coordinates(initialDragX_, initialDragY_),
                                     Coordinates(lastDrawX_, lastDrawY_));
      // Busco el extremo del rectangulo nuevo y lo convierto a coor viewport
      Coordinates newpoint = GetNewPointCoordinates(Event);
      Coordinates newpointviewport;
      pViewer_->GetWorld()->InverseTransform(newpoint, newpointviewport);
      drawRectangle_ = false;
      int dragwidth = (int) std::abs(initialDragX_ - newpoint.x_);
      int dragheight = (int) std::abs(initialDragY_ - newpoint.y_);
      if (dragwidth > DRAG_TOLERANCE_OUT && dragheight > DRAG_TOLERANCE_OUT) {
         // Guardo el nuevo extremo del rectangulo y lo dibujo
         lastDrawX_ = (long) newpointviewport.x_;
         lastDrawY_ = (long) newpointviewport.y_;
         // fuerzo que dibuje el rectangulo
         drawRectangle_ = true;
         DrawRectangleWithInvertedColor(Coordinates(initialDragX_, initialDragY_),
                                        Coordinates(lastDrawX_, lastDrawY_));
      }
   }
}

/**
 * Dibuja un rectangulo en el window del viewer en las coordenadas indicadas
 * @param[in] Point1 cualquiera de los dos extremos del rectangulo
 * @param[in] Point2 el otro extremo del rectangulo
 */
void ZoomButton::DrawRectangleWithInvertedColor(const Coordinates &Point1,
                                                const Coordinates &Point2) {
   // retorno si no esta mostrandose el rectangulo
   if (!drawRectangle_) {
      return;
   }
   wxClientDC wxdc(pViewer_->GetWindow());
   wxdc.SetLogicalFunction(wxINVERT);
   wxdc.SetBrush(wxBrush("red", wxTRANSPARENT));
   wxdc.DrawRectangle((int) std::min(Point1.x_, Point2.x_),
                      (int) std::min(Point1.y_, Point2.y_),
                      (int) std::abs(Point1.x_ - Point2.x_),
                      (int) std::abs(Point1.y_ - Point2.y_));
}

/**
 * Le aplica la ventana formada con las coordenadas iniciales y las del mouse
 * al mundo lo que produce una modificacion en la misma,
 * luego ajusta la nueva ventana para que coincida con el
 * punto inicial y finalmente retorna la coordenada del otro extremo
 * @param[in] Event wxMouseEvent con informacion de donde se hizo click
 * @return coordenada donde deberia terminar la ventana
 * \todo Remplazar por clone cuando este implementado en world
 */
Coordinates ZoomButton::GetNewPointCoordinates(wxMouseEvent &Event) {
   Subset newwindow;

   // Copio World
   Subset aux;
   World* pworld = new AspectPreservingWorld();
   pworld->ResetWorld();
   pworld->SetSpatialReference(pViewer_->GetWorld()->GetSpatialReference());
   pViewer_->GetWorld()->GetWorld(aux);
   pworld->SetWorld(aux);
   pViewer_->GetWorld()->GetWindow(aux);
   pworld->SetWindow(aux);
   int x1, x2;
   pViewer_->GetWorld()->GetViewport(x1, x2);
   pworld->SetViewport(x1, x2);

   // Aplico subset con coordenadas de mouse al world que copie
   pViewer_->GetWorld()->Transform(Coordinates(initialDragX_, initialDragY_),
                                   newwindow.ul_);
   pViewer_->GetWorld()->Transform(Coordinates(Event.GetX(), Event.GetY()),
                                   newwindow.lr_);
   Coordinates initialpoint(newwindow.ul_);
   pworld->SetWindow(newwindow);
   pworld->GetWindow(newwindow);
   delete pworld;

   // Calculo coordenada de nuevo punto con info de la ventana																			newwindow.ul_);
   double deltax = newwindow.lr_.x_ - newwindow.ul_.x_, deltay = newwindow.lr_.y_
         - newwindow.ul_.y_;

   return Coordinates(initialpoint.x_ + deltax, initialpoint.y_ + deltay);
}
}

