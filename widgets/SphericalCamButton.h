/*! \file SphericalCamButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SPHERICALCAMBUTTON_H_
#define SPHERICALCAMBUTTON_H_

// Includes suri
#include "suri/ToggleButton.h"

/** namespace suri */
namespace suri {
/** ToggleButton que activa la camara esferica */
/**
 * Activa la accion de SphericCam en TerrainGlCanvas
 * Permite cambiar la posicion: avanzar, desplasarce horizontalmente y
 * verticalmente.
 * Permite cambiar el angulo de la camara: Pitch(Cabeceo) y Roll(Alabeo).
 * \note hereda de ToggleButton para que se muestre que la opcion activa es la
 *       de SphericaCamButton
 */
class SphericalCamButton : public ToggleButton {
   /** Ctor. de Copia. */
   SphericalCamButton(const SphericalCamButton &SphericalCamButton);

public:
   /** Ctor */
   SphericalCamButton(wxWindow *pToolbar, TerrainGLCanvas *pGlCanvas) :
         ToggleButton(pToolbar, wxT(tool_SPHERICAL_CAMERA),
                      wxT(tooltip_SPHERICAL_CAMERA), BUTTON_NULL),
         pGlCanvas_(pGlCanvas) {
   }
   /** Dtor */
   virtual ~SphericalCamButton() {
   }
protected:
   /** Cambia cursor a mano */
   virtual void DoSetCursor(bool State) {
      if (State) {
         pViewerWindow_->SetCursor(wxCursor(wxCURSOR_ARROW));
      } else {
         pViewerWindow_->SetCursor(wxCursor(wxCURSOR_DEFAULT));
      }
   }
   /** Activa la accion TerrainGLCanvas::SphericCam en GlCanvas */
   virtual void OnButtonClicked(wxCommandEvent &Event) {
      pGlCanvas_->SetAction(TerrainGLCanvas::SphericCam);
   }
private:
   TerrainGLCanvas *pGlCanvas_; /*! GlCanvas */
};
}

#endif /*SPHERICALCAMBUTTON_H_*/
