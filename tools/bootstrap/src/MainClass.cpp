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

#include "<::MainClass::>.h"

// Includes estandar
#include <stirng>
#include <map>

// Tiene definido SVN_REVISION que indica la revision del codigo
// El contenido se genera durante la compilacion
#include "svn_version.h"

// Includes Suri
// lo declaro aca porque se usa en Application.h
#define message_FIRST_APP_PARAMETER "Descripcion del primer parametro"
#include "suri/Application.h"
#include "suri/messages.h"

// Includes Wx

// Includes App

// Defines

// forwards

/** Declara el objeto aplicacion */
template <class MainClass>
DECLARE_APP(suri::Application<<::MainClass::>>)
/** Implementa el objeto aplicacion */
IMPLEMENT_APP(suri::Application<<::MainClass::>>)

// ---- IMPLEMENTACION POLITICA DE APPLICATION ---------------------------------

#ifdef image_SPLASH_APPLICATION
/** string que representa al bitmap del splash */
const char <::MainClass::>::SplashBitmap_[] = image_SPLASH_APPLICATION;
#else
const char <::MainClass::>::SplashBitmap_[] = ""; /*! Bitmap del splash */
#endif
#ifdef filename_CONFIGURATION
/** la ruta del archivo de configuracion */
const char <::MainClass::>::ConfigFile_[] = filename_CONFIGURATION;
#else
/** la ruta del archivo de configuracion */
const char <::MainClass::>::ConfigFile_[] = "configuracion.xml"; /*! Ver Arriba */
#endif

<::MainClass::>::<::MainClass::>() {
}

<::MainClass::>::~<::MainClass::>() {
}

/** metodo de obtencion de la instancia */
<::MainClass::>& <::MainClass::>::Instance() {
   static <::MainClass::> *psingleinstance = new <::MainClass::>;
   return *psingleinstance;
}

/** retorna la ventana que la aplicacion considera principal */
wxTopLevelWindow *<::MainClass::>::GetWindow() {
   /*
    * Stub. Crea un Dialogo con OK/Cancel
    */
   wxDialog *pdialog = new wxDialog;
   pdialog->Create(NULL, wxID_ANY, wxEmptyString);
   pdialog->SetSizer(new wxBoxSizer(wxVERTICAL));
   pdialog->GetSizer()->AddStretchSpacer(1);
   pdialog->GetSizer()->Add(new wxStaticText(pdialog, wxID_ANY,
                  wxT("Hello World : <::MainClass::>"),
                  wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE),
                  0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 0);
   pdialog->GetSizer()->AddStretchSpacer(1);
   pdialog->GetSizer()->Add(pdialog->CreateStdDialogButtonSizer(wxOK|wxCANCEL),
                           0, wxALL|wxEXPAND, 0);
   pdialog->Show();
   return pdialog;
}

/** carga un parametro como string */
void <::MainClass::>::SetParameter(const std::string &Parameter) {
}

/** carga los parametros completos */
void <::MainClass::>::SetParameters(std::multimap<std::string, std::string> &Parameters) {
}

/** inicializa la aplicacion */
bool <::MainClass::>::InitializeApplication() {
   return true;
}

/** inicializa los recursos de la aplicacion */
void <::MainClass::>::InitializeResources() {
}
// ---- FIN POLITICA APPLICATION -----------------------------------------------

