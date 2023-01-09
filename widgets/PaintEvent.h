/*! \file PaintEvent.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef PAINTEVENT_H_
#define PAINTEVENT_H_

// Includes Estandar
#include <deque>

// Includes Suri

// Includes Wx
#include <wx/event.h>

// Defines

// forwards

namespace suri {

// forwards
class PaintEventInterface;

class PaintEvent : public wxEvtHandler {
public:
   PaintEvent(PaintEventInterface *pParent);
   virtual ~PaintEvent();

   PaintEventInterface *GetHandler();
   void Push(PaintEventInterface* pPainter);
   void Pop();
   void OnPaint(wxPaintEvent& Event);
protected:
private:
   std::deque<PaintEventInterface*> painters_;
};

}  // namespace suri

#endif /* PAINTEVENT_H_ */
