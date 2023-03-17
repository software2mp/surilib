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
