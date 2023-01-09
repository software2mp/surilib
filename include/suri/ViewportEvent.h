/*! \file ViewportEvent.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
