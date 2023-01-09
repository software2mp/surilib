/*! \file AboutPart.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
// Includes Suri
#include "suri/Configuration.h"
#include "AboutPart.h"
#include "wxmacros.h"

// Includes Wx
#include "wx/html/htmlwin.h"
#include "wx/xrc/xmlres.h"

// Defines

//forwards

namespace suri {

const std::string AboutPart::ABOUT_XRC_NAME = wxT("ID_ABOUT_PANEL"); /*! Id del panel */

AboutPart::AboutPart() :
      Part(ABOUT_XRC_NAME, caption_ABOUT_PART) {
}

AboutPart::~AboutPart() {
}

bool AboutPart::HasChanged() {
   return false;
}

/** Salva los cambios realizados en el Part. */
bool AboutPart::CommitChanges() {
   return true;
}

/** Restaura los valores originales del Part. */
bool AboutPart::RollbackChanges() {
   return true;
}

/** Inicializa el Part */
void AboutPart::SetInitialValues() {
}

/** Actualiza el estado del Part */
void AboutPart::Update() {
}

/** Permite modificar el widget luego de crear la ventana */
bool AboutPart::ConfigureWidget() {
   std::string aboutfile = suri::Configuration::GetParameter("app_help_dir", wxT("./"));
   aboutfile += "/about-Application.html";
   GET_CONTROL(*GetWidget()->GetWindow(), wxT("ID_ABOUT_HTMLWINDOW"),
         wxHtmlWindow)->LoadFile(wxFileName(aboutfile.c_str()));
   GetWidget()->GetWindow()->SetMinSize(wxSize(350, 470));
   return true;
}

}
