/*! \file SaveViewButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SAVEVIEWBUTTON_H_
#define SAVEVIEWBUTTON_H_

// Includes standard

// Includes Suri
#include "suri/messages.h"
#include "suri/Configuration.h"
#include "suri/Button.h"
#include "resources.h"
// Includes Wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
// Includes App

// Defines


namespace suri {
/** Button guarda imagen en disco con lo mostrado en el viewer del contexto al que pertenece. */
class SaveViewButton : public Button {
   /** Ctor. de Copia. */
   SaveViewButton(const SaveViewButton &SaveViewButton);
public:
   /** Ctor */
   SaveViewButton(wxWindow *pToolbar);
   /** Dtor */
   virtual ~SaveViewButton() {
   }
protected:
   /** Guarda la imagen mostrada por pantalla en formato elegido */
   virtual void OnButtonClicked(wxCommandEvent &Event);
};
} // namespace suri

#endif /* SAVEVIEWBUTTON_H_ */
