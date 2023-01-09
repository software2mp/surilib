/** \file MeasureDistanceButton.h
 *
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MEASUREDISTANCEBUTTON_H_
#define MEASUREDISTANCEBUTTON_H_

#include "MeassureDistanceElementEditor.h"
#include "suri/ToggleButton.h"

#include "wx/wx.h"
#include "wx/bmpbuttn.h"
#include "wx/xrc/xmlres.h"

namespace suri {

/**
 * Boton para activar la herramienta de medicion de distancia.
 */
class MeasureDistanceButton : public ToggleButton {
public:
   /**
    * Ctor.
    */
   MeasureDistanceButton(wxWindow *pParent, const wxString &Bitmap,
                         wxString ButtonTooltip, int Flags,
                         MeassureDistanceElementEditor* pEditor) :
         ToggleButton(pParent, Bitmap, ButtonTooltip, Flags), pEditor_(pEditor) {
   }


protected:
   /**
    * Evento asignar valor.
    */
   virtual bool DoSetValue(bool State) {
      if (State && pEditor_ && !pEditor_->IsActive()) {
         pEditor_->Start();
      } else if (!State && pEditor_ && pEditor_->IsActive()) {
         pEditor_->Stop();
      }
      return ToggleButton::DoSetValue(State);
   }

   /**
    * Metodo ejecutado al levantar/soltar el boton izquierdo del mouse.
    */
   virtual void DoOnLeftUp(wxMouseEvent &Event) { }

   /**
    * Metodo ejecutado al levantar/soltar el boton derecho del mouse.
    */
   virtual void DoOnRightUp(wxMouseEvent &Event) { }

   /**
    * Elimina el ultimo punto agregado al feature.
    */
   virtual void DoOnMiddleUp(wxMouseEvent &Event) { }

private:
   MeassureDistanceElementEditor* pEditor_;
};

}  // namespace suri

#endif  // MEASUREDISTANCEBUTTON_H_
