/*! \file MultiZoom.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MULTIZOOM_H_
#define MULTIZOOM_H_

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
class MultiZoom : public ViewportEvent {
public:
   MultiZoom(ViewportManagerInterface *pViewportManager,
             ViewcontextManagerInterface *pViewcontextManager);
   virtual ~MultiZoom();

   virtual void Reset();
   virtual void GetCursor(wxCursor &Cursor);
   virtual void SetSubsetAspectRatio(const Subset &OldSubset, Subset &CorrectedSubset);
   virtual void OnDrag(const Coordinates &InitialCoordinate,
                             const Coordinates &CurrentCoordinate, wxMouseEvent &Event);
   virtual void OnEndDrag(const Coordinates &InitialCoordinate,
                          const Coordinates &CurrentCoordinate, wxMouseEvent &Event);

   virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event);
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event);

   virtual PaintEventFlags GetPaintFlags() const;

   virtual void OnPaint(wxPaintEvent&Event, wxDC &dc);
protected:
private:
   Subset rubberband_;
   bool wasUpdating_;
};

} /* namespace suri */

#endif /* MULTIZOOM_H_ */
