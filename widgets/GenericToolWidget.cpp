/*! \file GenericToolWidget.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
// Includes Suri
#include "suri/GenericToolWidget.h"
#include "suri/ToolGroupManager.h"
#include "suri/Tool.h"
#include "suri/Button.h"
#include "suri/messages.h"
#include "suri/ViewerWidget.h"
#include "resources.h"
// Includes Wx
#include "wx/bmpbuttn.h"
#include "wx/xrc/xmlres.h"
// Includes App
#include "PointElementEditor.h"
#include "PixelVectorButton.h"
// Defines

namespace suri {
// Ctor
GenericToolWidget::GenericToolWidget(wxWindow *pParent,
                                       const std::string &WindowTitle,
                                       const std::string &WindowId) :
                                                         Widget(pParent) {
   windowTitle_ = WindowTitle;
   wantedWidth_ = 200;
   wantedHeight_ = 20;
   toolName_ = GetWindowTitle().c_str();
   windowId_ = WindowId;
}

// Dtor
GenericToolWidget::~GenericToolWidget() {
}

/** Creacion de la ventana */
bool GenericToolWidget::CreateToolWindow() {
   pToolWindow_ = wxXmlResource::Get()->LoadPanel(pParentWindow_,
                                                     windowId_.c_str());
   InitializeToolbar(pToolWindow_, pViewer_, pList_);

   pViewer_->Link(this);

   return true;
}

/** Retorna la ventana */
wxWindow *GenericToolWidget::GetToolWindow() const {
   return (const_cast<GenericToolWidget*>(this))->GetWindow();
}

bool GenericToolWidget::IsEnabled() {
   return true;
}

}  // namespace suri
