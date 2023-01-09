/*! \file MouseEvent.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MOUSEEVENT_H_
#define MOUSEEVENT_H_

// Includes Estandar

// Includes Suri
#include "suri/wxSuriMouseEvent.h"
#include "suri/MouseEventInterface.h"

// Includes Wx
#include <wx/event.h>

// Defines

// forwards

namespace suri {

class MouseEvent : public wxEvtHandler {
public:
   MouseEvent(MouseEventInterface *pParent);
   virtual ~MouseEvent();

   void OnLeft(wxMouseEvent &Event);
   void OnLeftUp(wxMouseEvent &Event);
   void OnLeftDown(wxMouseEvent &Event);
   void OnLeftDoubleClick(wxMouseEvent &Event);
   void OnRight(wxMouseEvent &Event);
   void OnRightUp(wxMouseEvent &Event);
   void OnRightDown(wxMouseEvent &Event);
   void OnRightDoubleClick(wxMouseEvent &Event);
   void OnMiddle(wxMouseEvent &Event);
   void OnMiddleUp(wxMouseEvent &Event);
   void OnMiddleDown(wxMouseEvent &Event);
   void OnMiddleDoubleClick(wxMouseEvent &Event);
   void OnEnterWindow(wxMouseEvent &Event);
   void OnLeaveWindow(wxMouseEvent &Event);
   void OnWheel(wxMouseEvent &Event);
   void OnMouseMove(wxMouseEvent &Event);
   void OnMouse(wxMouseEvent &Event);

   void OnLeftUp(wxSuriMouseEvent &Event);


   MouseEventInterface *GetHandler();
protected:
private:
   MouseEventInterface *pParent_; /*Herramienta padre*/
};

}  // namespace suri

#endif   // MOUSEEVENT_H_
