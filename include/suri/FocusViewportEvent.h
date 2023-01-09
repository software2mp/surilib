/*! \file FocusViewportEvent.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FOCUSVIEWEREVENT_H_
#define FOCUSVIEWPORTEVENT_H_

// Includes Estandar

// Includes Suri
#include "suri/MouseEventInterface.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/SuriObject.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 *
 */
class FocusViewportEvent : public MouseEventInterface {
public:
   FocusViewportEvent(ViewportManagerInterface *pVportManager,
                      const SuriObject::UuidType &ViewerId);
   virtual ~FocusViewportEvent();

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

   virtual MouseEventInterface::MouseEventFlags GetMouseFlags() const;

   virtual void MouseEnabled(bool Enable = true);
   virtual bool IsMouseEnabled() const;

   virtual bool CanProcessEvent(wxMouseEvent&) const;

   virtual void FocusViewport(wxMouseEvent &Event)const;

private:
   ViewportManagerInterface *pVportManager_;
   SuriObject::UuidType viewerId_;
};

} /** namespace suri */

#endif /* FOCUSVIEWEREVENT_H_ */
