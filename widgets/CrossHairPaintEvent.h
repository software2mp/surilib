/*! \file CrossHairPaintEvent.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CROSSHAIRPAINTEVENT_H_
#define CROSSHAIRPAINTEVENT_H_

// Includes Suri
#include "suri/ViewportEvent.h"

namespace suri {

// forwards
class Navigator;

class CrossHairPaintEvent : public ViewportEvent {
public:
   CrossHairPaintEvent(ViewerWidget* pViewer,
                       ViewportManagerInterface* pViewportManager,
                       ViewcontextManagerInterface* pViewcontextManager,
                       Navigator* pNavigator);
   virtual ~CrossHairPaintEvent();
   /**  Metodos de ViewportEvent  **/
   virtual void OnPaint(wxPaintEvent&Event, wxDC &dc);

   virtual void Reset() {}
   // on click (u otro evento, wheel, etc)
   virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event) {}
   // on move
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event) {}
   /** Dibuja cruz invirtiendo colores */
   void DrawCrossWithInvertedColor(const Coordinates &Point, wxDC &dc);
   virtual PaintEventFlags GetPaintFlags() const;

private:
   /** Metodo de actualizacion del mundo */
   /**
    *  Metodo Update que se ejecuta cuando pWorldWindow es modificado. Es el
    * callback que se registra en pWorldViewer_.
    */
   void DoOnPaint(wxPaintEvent &Event, wxDC &dc);
   ViewerWidget* pViewer_;
   Navigator* pNavigator_;
};

} /** namespace suri */

#endif /* CROSSHAIRPAINTEVENT_H_ */
