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
