/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
