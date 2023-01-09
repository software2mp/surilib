/*! \file EyeCamButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef EYECAMBUTTON_H_
#define EYECAMBUTTON_H_

// Includes suri
#include "suri/ToggleButton.h"

/** namespace suri */
namespace suri {
/** ToggleButton que activa la camara con vista sobre terreno */
/**
 * Activa la accion de EyeCam en TerrainGlCanvas
 * Permite cambiar el angulo de la camara con respecto a los 3 ejes.
 * Los movimientos son: Pitch(Cabeceo), Roll(Alabeo) y Yaw(Guinado)
 * \note hereda de ToggleButton para que se muestre que la opcion activa es la
 *       de EyeCam
 */
class EyeCamButton : public ToggleButton {
   /** Ctor. de Copia. */
   EyeCamButton(const EyeCamButton &EyeCamButton);

public:
   /** Ctor */
   EyeCamButton(wxWindow *pToolbar, TerrainGLCanvas *pGlCanvas) :
         ToggleButton(pToolbar, wxT(tool_EYE_CAMERA), wxT(tooltip_EYE_CAMERA),
                      BUTTON_NULL),
         pGlCanvas_(pGlCanvas) {
   }
   /** Dtor */
   virtual ~EyeCamButton() {
   }
protected:
   /** Cambia cursor a flecha */
   virtual void DoSetCursor(bool State) {
      if (State) {
         pViewerWindow_->SetCursor(wxCursor(wxCURSOR_ARROW));
      } else {
         pViewerWindow_->SetCursor(wxCursor(wxCURSOR_DEFAULT));
      }
   }
   /** Activa la accion TerrainGLCanvas::EyeCam en GlCanvas */
   virtual void OnButtonClicked(wxCommandEvent &Event) {
      pGlCanvas_->SetAction(TerrainGLCanvas::EyeCam);
   }
private:
   TerrainGLCanvas *pGlCanvas_; /*! TerrainGLCanvas */
};
}

#endif /*EYECAMBUTTON_H_*/
