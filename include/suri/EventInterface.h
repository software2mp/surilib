/*! \file EventInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef EVENTINTERFACE_H_
#define EVENTINTERFACE_H_

// Includes Estandar

// Includes Suri
#include "suri/MouseEventInterface.h"
#include "suri/PaintEventInterface.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/ViewcontextManagerInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 *
 */
class EventInterface : public MouseEventInterface, public PaintEventInterface {
   EventInterface(const EventInterface&);
protected:
   EventInterface(ViewportManagerInterface *pViewportManager,
                  ViewcontextManagerInterface *pViewcontextManager) {
   }
public:
   virtual ~EventInterface() {
   }

   virtual void Enabled(bool Enable = true)=0;
   virtual bool IsEnabled() const=0;
protected:
private:
};

class ForwardEventImpl : public EventInterface {
   ForwardEventImpl(const ForwardEventImpl&);
public:
   ForwardEventImpl(ViewportManagerInterface *pViewportManager,
                    ViewcontextManagerInterface *pViewcontextManager);
   ForwardEventImpl(const std::string &ForwardMouseClass,
                    const std::string &ForwardPaintClass,
                    ViewportManagerInterface *pViewportManager,
                    ViewcontextManagerInterface *pViewcontextManager);
   ForwardEventImpl(MouseEventInterface *pForwardMouseClass,
                    PaintEventInterface *pForwardPaintClass,
                    ViewportManagerInterface *pViewportManager,
                    ViewcontextManagerInterface *pViewcontextManager);
   virtual ~ForwardEventImpl();

   virtual void OnLeft(wxMouseEvent&);
   virtual void OnLeftUp(wxMouseEvent&);
   virtual void OnLeftDown(wxMouseEvent&);
   virtual void OnLeftDoubleClick(wxMouseEvent&);
   virtual void OnRight(wxMouseEvent&);
   virtual void OnRightUp(wxMouseEvent&);
   virtual void OnRightDown(wxMouseEvent&);
   virtual void OnRightDoubleClick(wxMouseEvent&);
   virtual void OnMiddle(wxMouseEvent&);
   virtual void OnMiddleUp(wxMouseEvent&);
   virtual void OnMiddleDown(wxMouseEvent&);
   virtual void OnMiddleDoubleClick(wxMouseEvent&);
   virtual void OnEnterWindow(wxMouseEvent&);
   virtual void OnLeaveWindow(wxMouseEvent&);
   virtual void OnWheel(wxMouseEvent&);
   virtual void OnMouseMove(wxMouseEvent&);
   virtual void OnMouse(wxMouseEvent&);

   virtual MouseEventFlags GetMouseFlags() const;

   virtual void MouseEnabled(bool Enable = true);
   virtual bool IsMouseEnabled() const;

   virtual bool CanProcessEvent(wxMouseEvent&) const;

   virtual void OnPaint(wxPaintEvent&, wxDC&);
   virtual void OnBackgroundErase(wxEraseEvent&);
   virtual PaintEventFlags GetPaintFlags() const;

   virtual void PaintEnabled(bool Enable = true);
   virtual bool IsPaintEnabled() const;

   virtual bool CanProcessEvent(wxPaintEvent&) const;

   virtual void Enabled(bool Enable = true);
   virtual bool IsEnabled() const;
protected:
private:
   MouseEventInterface *pForwardedMouseClass_;
   PaintEventInterface *pForwardedPaintClass_;
};

} /* namespace suri */

#endif /* EVENTINTERFACE_H_ */
