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
#include "CrossHairPaintEvent.h"
#include "suri/Viewer3D.h"
#include "suri/Navigator.h"


// Includes Wx
// Defines
// forwards

namespace suri {

CrossHairPaintEvent::CrossHairPaintEvent(ViewerWidget* pViewer,
                                         ViewportManagerInterface* pViewportManager,
      ViewcontextManagerInterface* pViewcontextManager, Navigator* pNavigator) :
      ViewportEvent(pViewportManager, pViewcontextManager), pViewer_(pViewer),
      pNavigator_(pNavigator) {
}

CrossHairPaintEvent::~CrossHairPaintEvent() {
   Viewer3D* pViewer3D = dynamic_cast<Viewer3D*>(pViewer_);
   if (pViewer3D) {
      pViewer3D->EraseCrosshair();
   }
}

/** Dibuja cruz invirtiendo colores */
void CrossHairPaintEvent::DrawCrossWithInvertedColor(const Coordinates &Point, wxDC &dc) {
   Coordinates pixel;
   // Obtengo coordenadas en pixel-linea
   pViewer_->GetWorld()->InverseTransform(Point, pixel);
   // Dibujo lineas
   Viewer3D* pView3d = dynamic_cast<Viewer3D*>(pViewer_);
   if (pView3d) {
      pView3d->DrawCrosshair(pixel);
   } else {
      dc.SetLogicalFunction(wxINVERT);
      dc.SetBrush(wxBrush("red", wxTRANSPARENT));
      dc.CrossHair(static_cast<int>(pixel.x_), static_cast<int>(pixel.y_));
   }
}

PaintEventInterface::PaintEventFlags CrossHairPaintEvent::GetPaintFlags() const {
   return PaintEventInterface::Paint;
}

/**  Metodos de ViewportEvent  **/
void CrossHairPaintEvent::OnPaint(wxPaintEvent& Event, wxDC &dc) {
   DoOnPaint(Event, dc);
}

/**
 * Metodo Update que se ejecuta cuando pWorldWindow es modificado. Es el
 * callback que se registra en pWorldViewer_.
 * Dibuja cruz en viewer
 * @param[in] Event evento
 */
void CrossHairPaintEvent::DoOnPaint(wxPaintEvent &Event, wxDC &dc) {
   if (!pNavigator_)
      return;

   Coordinates coord(0, 0, 0);
   // Veo que las coordenadas del navegador sean validas y se muestren
   if (pNavigator_->GetLastPoint(coord) && (pNavigator_->IsShow(coord))) {
      if (pViewer_->GetWorld() != NULL) {
         Subset currentwindow;
         pViewer_->GetWorld()->GetWindow(currentwindow);
      }

      // Si se pudo transformar las coordenadas lat long a coord de mundo dibujo cruz
      if (pNavigator_->GetLastPointInWorldCoordinates(coord)) {
         DrawCrossWithInvertedColor(coord, dc);
      }
   }
}
} /** namespace suri */
