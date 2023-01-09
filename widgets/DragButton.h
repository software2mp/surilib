/*! \file DragButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DRAGBUTTON_H_
#define DRAGBUTTON_H_

// Includes standard

// Includes suri
#include "suri/ToggleButton.h"
#include "suri/Viewer2D.h"

// Includes Wx

// Includes app

// Defines

/** namespace suri */
namespace suri {
/** ToggleButton que Permite mover la ventana */
/**
 * Se moviliza la ventana y se muestra la parte del mundo que se encuentre dentro
 * de ella.
 * Limita el desplazamiento para no perder la imagen
 * \note hereda de ToggleButton para que se muestre que la opcion activa es la
 *  de Drag
 */
class DragButton : public ToggleButton {
   /** Ctor. de Copia. */
   DragButton(const DragButton &DragButton);

public:
   /** Ctor. */
   DragButton(wxWindow *pToolbar, Viewer2D *pViewer);
   /** Dtor. */
   virtual ~DragButton();
protected:
   /** Cambia cursor a mano */
   virtual void DoSetCursor(bool State);
   /** Cambia subset de la ventana */
   virtual void DoOnLeftUp(wxMouseEvent &Event);
   /** Cambia posicion de la imagen mostrada */
   virtual void DoOnMouseMove(wxMouseEvent &Event);
   /** Si el mouse sale de la pantalla mientras se hace drag la ventana muestra la mitad del dibujo */
   virtual void DoOnLeaveWindow(wxMouseEvent &Event);
private:
   Viewer2D *pViewer_; /*! viewer */
   int initialDragX_; /*! posicion inicial de la ventana en x */
   int initialDragY_; /*! posicion inicial de la ventana en y */
   int initialPosX_; /*! posicion inicial del scroll en x */
   int initialPosY_; /*! posicion inicial del scroll en y */
};
}
#endif /*DRAGBUTTON_H_*/
