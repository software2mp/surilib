/*! \file ToggleButton.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TOGGLEBUTTON_H_
#define TOGGLEBUTTON_H_

// Includes suri
#include "suri/Button.h"

class wxToggleBitmapButton;

/** namespace suri */
namespace suri {
/** Button que queda precionado */
/**
 * Especializacion de Button para utilizar un boton que queda presionado.
 */
class ToggleButton : public Button {
   /** Ctor. de Copia. */
   ToggleButton(const ToggleButton &ToggleButton);

public:
   /** Ctor */
   ToggleButton(wxWindow *pParent, wxString ButtonTooltip, int Flag);
   /** Ctor */
   ToggleButton(wxWindow *pParent, const wxBitmap &Bitmap, wxString ButtonTooltip =
         wxT(tooltip_BUTTON),
                int Flag = BUTTON_NULL);
   /** Ctor */
   ToggleButton(wxWindow *pParent, const wxString &Bitmap, wxString ButtonTooltip =
         wxT(tooltip_BUTTON),
                int Flag = BUTTON_NULL);
   /** Dtor */
   virtual ~ToggleButton() {
   }

   virtual bool GetValue() const;

protected:
   /** Evento click en boton */
   virtual void OnButtonClicked(wxCommandEvent &Event);
   /** Evento asignar valor */
   virtual bool DoSetValue(bool State);
   /** Evento crear ToolWindow */
   virtual bool DoCreateToolWindow();
private:
   wxToggleBitmapButton *pButton_; /*! Boton con un bitmap que tiene 2 estados */
};
}

#endif /*TOGGLEBUTTONTOOL_H_*/
