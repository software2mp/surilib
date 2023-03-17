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
