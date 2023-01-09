/** \file QuickMeasureButton.h
 *
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef QUICKMEASUREBUTTON_H_
#define QUICKMEASUREBUTTON_H_

#include "suri/ToggleButton.h"
#include "suri/ViewerWidget.h"
#include "MeassureWidget.h"

#include "wx/wx.h"
#include "wx/bmpbuttn.h"
#include "wx/xrc/xmlres.h"

namespace suri {

class QuickMeasureButton : public ToggleButton {
public:
   /**
    * Ctor.
    */
   QuickMeasureButton(wxWindow *pToolbar, ViewerWidget* pViewer) :
         ToggleButton(pToolbar, wxT(tool_MEASURE_WIDGET),
                      wxT(tooltip_QUICK_MEASUREMENT), BUTTON_NULL),
         pToolbar_(pToolbar), pViewer_(pViewer) {
   }

protected:
   virtual bool DoSetValue(bool State) {
      if (pViewer_->GetMeasureWidget()) {
         if (State) {
            pViewer_->GetMeasureWidget()->ShowWidget();
         } else {
            pViewer_->GetMeasureWidget()->HideWidget();
         }
      }
      return ToggleButton::DoSetValue(State);
   }

private:
   wxWindow* pToolbar_;
   ViewerWidget* pViewer_;

};

}  // namespace suri

#endif  // QUICKMEASUREBUTTON_H_
