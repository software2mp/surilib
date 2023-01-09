/*! \file GenericToolWidget.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GENERICTOOLWIDGET_H_
#define GENERICTOOLWIDGET_H_

// Includes standard

// Includes Suri
#include "suri/ToolGroupManager.h"
#include "suri/Tool.h"
#include "suri/Widget.h"

// Includes Wx
#include "wx/wx.h"
// Includes App

// Defines

namespace suri {
/**
 * Herramienta grafica para todos los tools que se configuran con xml
 */
class GenericToolWidget : public Tool, public Widget, public ToolGroupManager {
public:
   /** Ctor */
   GenericToolWidget(wxWindow *pParent, const std::string &WindowTitle,
                                             const std::string &WindowId);
   /** Dtor */
   virtual ~GenericToolWidget();

   bool CreateToolWindow();

   virtual wxWindow *GetToolWindow() const;

   virtual bool IsEnabled();

protected:
   std::string windowId_;
};
}  // namespace suri

#endif /* GENERICTOOLWIDGET_H_ */
