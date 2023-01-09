/*! \file DragEvent.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DRAGEVENT_H_
#define DRAGEVENT_H_

#include "ViewportEvent.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 *
 */
class DragEvent : public suri::ViewportEvent {
public:
   DragEvent(ViewportManagerInterface *pViewportManager,
             ViewcontextManagerInterface *pViewcontextManager);
   virtual ~DragEvent();

   virtual void Reset();
   virtual void GetCursor(wxCursor &Cursor);

   virtual void OnDrag(const Coordinates &InitialCoordinate,
                             const Coordinates &CurrentCoordinate, wxMouseEvent &Event);
   virtual void OnEndDrag(const Coordinates &InitialCoordinate,
                          const Coordinates &CurrentCoordinate, wxMouseEvent &Event);

   virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event);
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event);

   virtual PaintEventFlags GetPaintFlags() const;
protected:
private:
   bool updating_;
   Coordinates initial_;
   Coordinates final_;
   int initialPosX_; /*! posicion inicial del scroll en x */
   int initialPosY_; /*! posicion inicial del scroll en y */
   wxWindow *pwin_;
};

} /* namespace suri */

#endif /* DRAGEVENT_H_ */
