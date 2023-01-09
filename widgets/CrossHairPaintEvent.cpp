/*! \file CrossHairPaintEvent.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


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
