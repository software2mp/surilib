/*! \file ZoomFullExtentButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ZOOMFULLEXTENTBUTTON_H_
#define ZOOMFULLEXTENTBUTTON_H_

// Includes suri
#include "suri/Button.h"

/** namespace suri */
namespace suri {
/** Button que cambia la ventana para que muestre todo el mundo */
/**
 * Modifica el window del viewer para que coincida con el extent del raster.
 */
class ZoomFullExtentButton : public Button {
   /** Ctor. de Copia. */
   ZoomFullExtentButton(const ZoomFullExtentButton &ZoomFullExtentButton);

public:
   /** Ctor */
   ZoomFullExtentButton(wxWindow *pToolbar, Viewer2D *pViewer) :
         Button(pToolbar, wxT(button_SIMUR_ZOOM_FULL_EXTENT)), pViewer_(pViewer) {
      windowTitle_ = _(tooltip_ZOOM_FULL_EXTENT);
   }
   /** Dtor */
   virtual ~ZoomFullExtentButton() {
   }
protected:
   /** Cambia la ventana para mostrar todo el mundo */
   virtual void OnButtonClicked(wxCommandEvent &Event) {
      Subset world;
      pViewer_->GetWorld()->GetWorld(world);
      pViewer_->ChangeWindow(world);
   }
private:
   Viewer2D *pViewer_; /*! Viewer */
};
}

#endif /*ZOOMFULLEXTENTBUTTON_H_*/
