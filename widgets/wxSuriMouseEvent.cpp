/*! \file wxSuriMouseEvent.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


// Includes Estandar
// Includes Suri
#include "suri/wxSuriMouseEvent.h"
// Includes Wx
// Defines
// forwards

namespace suri{
wxSuriMouseEvent::wxSuriMouseEvent(const wxSuriMouseEvent& Event):wxMouseEvent(Event){
  double x,y;
  Event.GetPosition(x, y);
  this->SetPosition(x, y);
}
wxSuriMouseEvent::wxSuriMouseEvent(wxEventType mouseEventType):wxMouseEvent(mouseEventType) {
   this->SetPosition(0, 0);
}


wxSuriMouseEvent* wxSuriMouseEvent::Clone() const {
    wxSuriMouseEvent* evnt = new wxSuriMouseEvent(*this);
    return  evnt;
 }


void wxSuriMouseEvent::SetPosition(double X, double Y) {
   x_ = X;
   y_ = Y;
}
void wxSuriMouseEvent::GetPosition(double &X, double &Y) const {
   X = x_;
   Y = y_;
}
}
