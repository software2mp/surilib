// <COPYRIGHT>
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

