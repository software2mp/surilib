/*! \file LabelText.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */



// Includes Estandar
#include <string>

// Includes Suri
#include "suri/LabelText.h"

// Includes Wx
#include "wx/stattext.h"

// Defines
// forwards

namespace suri {

/**
 * Constructor
 * @param Text texto que mostrara el label
 */
LabelText::LabelText(wxWindow *pViewerToolbar, const std::string& Text) :
      Widget(pViewerToolbar), text_(Text), enabled_(true), pTextCtrl_(NULL) {
}

/** Destructor */
LabelText::~LabelText() {
}

/** Se crea la ventana de herramienta en las clases derivadas */
bool LabelText::CreateToolWindow() {
   pTextCtrl_ = new wxStaticText(pParentWindow_, wxID_ANY, text_.c_str());
   pToolWindow_ = pTextCtrl_;
   pTextCtrl_->Enable(enabled_);
   return true;
}

/** Obtiene el texto del control grafico. */
std::string LabelText::GetValue() const {
   return pTextCtrl_->GetLabel().c_str();
}

/** Indica el texto a mostrar por el control grafico. */
void LabelText::SetValue(std::string& Value) {
   pTextCtrl_->SetLabel(Value.c_str());
}
/** Setea la propiedad enabled_ */
void LabelText::SetEnabled(bool Enabled) {
   enabled_ = Enabled;
   pTextCtrl_->Enable(enabled_);
}

/** Verifica si el boton se puede o no habilitar */
bool LabelText::IsEnabled() const {
   return enabled_;
}

} /** namespace suri */
