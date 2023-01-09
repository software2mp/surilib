/*! \file ZoomImageScaleButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ZOOMIMAGESCALEBUTTON_H_
#define ZOOMIMAGESCALEBUTTON_H_

// Includes suri
#include "suri/Button.h"
#include "resources.h"
#include "suri/RasterSpatialModel.h"
#include "suri/Viewer2D.h"

/** namespace suri */
namespace suri {
/** Button que cambia la ventana para que muestre todo el mundo */
/**
 * Calcula un window para que la imagen activa tenga una relacion entre el
 * pixel de la imagen y la pantalla de 1:1. Mantiene el centro de la ventana.
 */
class ZoomImageScaleButton : public Button {
   /** Ctor. de Copia. */
   ZoomImageScaleButton(const ZoomImageScaleButton &ZoomFullExtentButton);

public:
   /** Ctor */
   ZoomImageScaleButton(wxWindow *pToolbar, Viewer2D *pViewer) :
         Button(pToolbar, wxT(button_SIMUR_ZOOM_ONE_TO_ONE), wxT(tooltip_ZOOM_ONE_TO_ONE)),
         pViewer_(pViewer) {
   }
   /** Dtor */
   virtual ~ZoomImageScaleButton() {
   }
protected:
   /** Cambia la ventana para mostrar un pixel de imagen por pixel de pantalla */
   virtual void OnButtonClicked(wxCommandEvent &Event);

private:
   Viewer2D *pViewer_; /*! Viewer */
};
}

#endif /*ZOOMIMAGESCALEBUTTON_H_*/
