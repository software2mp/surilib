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

#ifndef VIEWPORTEVENT_H_
#define VIEWPORTEVENT_H_

// Includes Estandar

// Includes Suri
#include "EventInterface.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/ViewcontextManagerInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 *
 */
class ViewportEvent : public EventInterface {
public:
   ViewportEvent(ViewportManagerInterface *pViewportManager,
                 ViewcontextManagerInterface *pViewcontextManager);
   virtual ~ViewportEvent();
   virtual void OnLeft(wxMouseEvent&);
   virtual void OnLeftUp(wxMouseEvent &Event);
   virtual void OnLeftDown(wxMouseEvent &Event);
   virtual void OnLeftDoubleClick(wxMouseEvent&);
   virtual void OnRight(wxMouseEvent&);
   virtual void OnRightUp(wxMouseEvent &Event);
   virtual void OnRightDown(wxMouseEvent &Event);
   virtual void OnRightDoubleClick(wxMouseEvent&);
   virtual void OnMiddle(wxMouseEvent&);
   virtual void OnMiddleUp(wxMouseEvent &Event);
   virtual void OnMiddleDown(wxMouseEvent &Event);
   virtual void OnMiddleDoubleClick(wxMouseEvent&);
   virtual void OnEnterWindow(wxMouseEvent&Event);
   virtual void OnLeaveWindow(wxMouseEvent &Event);
   virtual void OnWheel(wxMouseEvent&);
   virtual void OnMouseMove(wxMouseEvent &Event);
   virtual void OnMouse(wxMouseEvent&Event);

   virtual void Reset()=0;
   virtual void GetCursor(wxCursor &Cursor);
   /** Se ejecuta cuando el usuario comienza drag sobre visualizador */
   virtual void OnStartDrag(const Coordinates &InitialCoordinate);
   /** Se ejecuta cuando el usuario finaliza drag */
   virtual void OnEndDrag(const Coordinates &InitialCoordinate,
                          const Coordinates &CurrentCoordinate, wxMouseEvent &Event);
   /** Se ejecuta cada vez que el usuario mueve el mouse mientras realiza drag */
   virtual void OnDrag(const Coordinates &InitialCoordinate,
                             const Coordinates &CurrentCoordinate,
                             wxMouseEvent &Event);
   /** Indica si se esta realizando drag con el mouse sobre le visualizador */
   virtual bool IsDragging();

   virtual void SetPoint(double Pixel, double Line, wxMouseEvent &Event)=0;  // on click (u otro evento, wheel, etc)
   virtual void UpdatePoint(double Pixel, double Line, wxMouseEvent &Event)=0;  // on move

   virtual MouseEventFlags GetMouseFlags() const;

   virtual void MouseEnabled(bool Enable = true);
   virtual bool IsMouseEnabled() const;

   virtual void OnPaint(wxPaintEvent&Event, wxDC &dc);

   virtual void OnBackgroundErase(wxEraseEvent&);
   virtual PaintEventFlags GetPaintFlags() const;

   virtual void PaintEnabled(bool Enable = true);
   virtual bool IsPaintEnabled() const;

   virtual void Enabled(bool Enable = true);
   virtual bool IsEnabled() const;

   virtual bool CanProcessEvent(wxPaintEvent &Event) const;
   virtual bool CanProcessEvent(wxMouseEvent &Event) const;
   virtual bool CanProcessEvent(wxEvent &Event) const;

protected:
   wxWindow *GetEventWindow(const wxEvent &Event) const;

   ViewportManagerInterface *GetViewportManager() const;
   ViewcontextManagerInterface *GetViewcontextManager() const;
   /** Retorna el ViewerWidget donde se realizo el evento */
   virtual ViewerWidget* GetEventViewer(wxEvent &Event) const;
   /** Refresca la ventana del visualizador */
   void RefreshViewers(const std::vector<SuriObject::UuidType> &Ids);
   /** Retorna subset de busqueda centrado en la coordenada y con srs de mundo */
   static Subset GetWorldSearchBox(const Coordinates Coordinate, World* pWorld);

   // Ancho del Box en pixels que retorna GetWorldSearchBox.
   static const int SEARCH_PIXEL_BUFFER = 7;

   bool isDragging_; /*! Flag que indica si estaba draggeando */
   bool inside_; /*! Flag que indica si estaba draggeando */
   Coordinates startDrag_;

private:
   ViewportManagerInterface *pViewportManager_;
   ViewcontextManagerInterface *pViewcontextManager_;
   bool paintEnabled_;
   bool mouseEnabled_;
};

} /* namespace suri */

#endif /* VIEWPORTEVENT_H_ */
