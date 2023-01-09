/*! \file wxSuriMouseEvent.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef __WX_SURI_MOUSE_EVENT__
#define __WX_SURI_MOUSE_EVENT__


// Includes Estandar
// Includes Suri
// Includes Wx
#include "wx/event.h"
#include "wx/app.h"
// Defines
// forwards

namespace suri {
#define wxEVENT_HANDLER_CAST( functype, func ) \
( wxObjectEventFunction )( wxEventFunction )wxStaticCastEvent( functype, func )

/**
 * Clase utilizada  en navegar, para que si se utiliza junto con la herramienta de edicion vectorial
 * se pueda crear vectores sin perder precision al momento de transformar pixel linea a coordenadas
 * de mundo, ya que wxMouseEvent solo guarda enteros.
 */
class wxSuriMouseEvent:public wxMouseEvent{
public:
   wxSuriMouseEvent(const wxSuriMouseEvent& Event);

   wxSuriMouseEvent(wxEventType mouseEventType = wxEVT_NULL);

   void SetPosition(double X, double Y);

   void GetPosition(double &X, double &Y)const;

   wxSuriMouseEvent* Clone() const;

   double x_;

   double y_;
};

typedef void (wxEvtHandler::*wxSuriMouseEventFunction)(wxSuriMouseEvent&);
#define wxSuriMouseEventHandler(func) wxEVENT_HANDLER_CAST(wxSuriMouseEventFunction,&func)
}
#endif
