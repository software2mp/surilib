/*! \file ZoomButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ZOOMBUTTON_H_
#define ZOOMBUTTON_H_

// Include standard
#include <cmath>

// Includes suri
#include "suri/ToggleButton.h"
#include "suri/Coordinates.h"

/** namespace suri */
namespace suri {
class Viewer2D;

/** ToggleButton que realiza zoomIn y zoomOut */
/**
 * Cuando se activa el boton espera eventos de click sobre el
 * viewer y modifica la ventana. Permite hacer zoom(in/out) eligiendo
 * un recuadro o haciendo click directamente.
 * Botton izquierdo: zoom in
 * Boton derecho: zoom out
 */
class ZoomButton : public ToggleButton {
   /** Ctor. de Copia. */
   ZoomButton(const ZoomButton &ZoomButton);

public:
   /** Ctor */
   ZoomButton(wxWindow *pToolbar, Viewer2D *pViewer);
   /** Dtor */
   virtual ~ZoomButton();
protected:
   /** Cambia cursor a magnificador */
   virtual void DoSetCursor(bool State);
   /** si se sale de la ventana se finaliza el zoom */
   virtual void DoOnLeaveWindow(wxMouseEvent &Event);
   /** inicializa el zoom in */
   virtual void DoOnLeftDown(wxMouseEvent &Event);
   /** realiza zoom in en viewer2D */
   virtual void DoOnLeftUp(wxMouseEvent &Event);
   /** inicializa el zoom out */
   virtual void DoOnRightDown(wxMouseEvent &Event);
   /** realiza zoom out en viewer2D */
   virtual void DoOnRightUp(wxMouseEvent &Event);
   /** dibuja el rectangulo si alguno de los dos zoom esta activado */
   virtual void DoOnMouseMove(wxMouseEvent &Event);
private:
   /** Dibuja un rectangulo en el window del viewer en las coordenadas indicadas */
   void DrawRectangleWithInvertedColor(const Coordinates &Point1,
                                       const Coordinates &Point2);
   /** Calcula rectangulo que tendra la ventana si se suelta el mouse en ese momento. */
   Coordinates GetNewPointCoordinates(wxMouseEvent &Event);

   Viewer2D *pViewer_; /*! Viewer */
   long initialDragX_; /*! Coordenada X del punto de inicio del drag */
   long initialDragY_; /*! Coordenada Y del punto de inicio del drag */
   long lastDrawX_; /*! Coordenada X del punto de finalizacion del drag */
   long lastDrawY_; /*! Coordenada Y del punto de finalizacion del drag */
   bool zoomInActivated_; /*! Flag : Estoy haciendo zoom in */
   bool zoomOutActivated_; /*! Flag : Estoy haciendo zoom out */
   bool drawRectangle_; /*! Flag : indica si se debe dibujar el rectangulo de */
   /* zoom */
};
}

#endif /*ZOOMBUTTON_H_*/
