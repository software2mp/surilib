/*! \file WorldButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef WORLDBUTTON_H_
#define WORLDBUTTON_H_

// Include suri
#include "suri/Button.h"

/** namespace suri */
namespace suri {
#ifdef __ENABLE_WORLDWATCHER_TOOL__

/** Button que activa el widget con la ventana y el mundo */
class WorldButton : public Button
{
   /** Ctor. de Copia. */
   WorldButton(const WorldButton &WorldButton);

public:
   /** Ctor */
   WorldButton(wxWindow *pToolbar, World *pWorld) :
   Button(pToolbar, wxArtProvider::GetBitmap(wxART_REPORT_VIEW) ),
   pWorld_(pWorld) {
      windowTitle_=_(tooltip_PANORAMA_WINDOW);
   }
protected:
   /** Crea WorldWidget para mostrar el mundo y la ventana */
   virtual void OnButtonClicked(wxCommandEvent &Event) {
      new WorldWidget(pWorld_);
   }
private:
   World *pWorld_; /*! Informacion del mundo */
};
#endif   // __ENABLE_WORLDWATCHER_TOOL__
}

#endif /*WORLDBUTTON_H_*/
