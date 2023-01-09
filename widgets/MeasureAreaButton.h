/** \file MeasureAreaButton.h
 *
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MEASUREAREABUTTON_H_
#define MEASUREAREABUTTON_H_

#include "MeassureAreaElementEditor.h"
#include "suri/ToggleButton.h"

#include "wx/wx.h"
#include "wx/bmpbuttn.h"
#include "wx/xrc/xmlres.h"

namespace suri {

/**
 * Boton para activar la herramienta de medicion de area y perimetro.
 */
class MeasureAreaButton : public ToggleButton {
public:
   /**
    * Ctor.
    */
   MeasureAreaButton(wxWindow *pParent, const wxString &Bitmap,
                     wxString ButtonTooltip, int Flags, MeassureAreaElementEditor* pEditor) :
         ToggleButton(pParent, Bitmap, ButtonTooltip, Flags), pEditor_(pEditor) {
   }

protected:
   virtual bool DoSetValue(bool State) {
      if (State && pEditor_ && !pEditor_->IsActive()) {
         pEditor_->Start();
      } else if (!State && pEditor_ && pEditor_->IsActive()) {
         pEditor_->Stop();
      }
      return ToggleButton::DoSetValue(State);
   }

   /** Metodo ejecutado al levantar/soltar el boton izquierdo del mouse. */
   virtual void DoOnLeftUp(wxMouseEvent &Event) { }

   /** Metodo ejecutado al levantar/soltar el boton derecho del mouse. */
   virtual void DoOnRightUp(wxMouseEvent &Event) { }

   /** Elimina el ultimo punto agregado al feature. */
   virtual void DoOnMiddleUp(wxMouseEvent &Event) { }

private:
   MeassureAreaElementEditor* pEditor_;
};

}  // namespace suri

#endif  // MEASUREAREABUTTON_H_
