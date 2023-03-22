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

#ifndef APPLICATION_H_
#define APPLICATION_H_


// Includes standard
#include <vector>
#include <string>
#if !wxUSE_ON_FATAL_EXCEPTION
#  include <signal.h>
namespace suri {
/** Funcion utilizada para manejo de senales del SO. */
void OnSigSev(int SignalNumber);
}
#  ifdef __DEBUG__
#     include <iostream>
#  endif
#endif

// Includes Suri
#include "suri/Configuration.h"
#include "suri/messages.h"
#include "wxmacros.h"

// Includes Wx
#include "wx/wxprec.h"
#include "wx/html/htmprint.h"
#include "wx/html/helpctrl.h"
#include "wx/cmdline.h"
#include "wx/splash.h"
#include "wx/dir.h"
#include "wx/stdpaths.h"
#include "wx/xrc/xmlres.h"
#include "wx/apptrait.h"
// \attention (14/07/2009 - Alejandro): el include "wx/snglinst.h" deberia hacerse despues de
// crear el define __APPLICATION_IS_SINGLE_INSTANCE__ para que su inclusion sea condicional
#include "wx/snglinst.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

// Includes App

// Defines
/** Habilita logica para mantener 1 sola instancia de la aplicacion por maquina. */
#define __APPLICATION_IS_SINGLE_INSTANCE__
/** Si no se encuentra definia la version de la aplicacion define una desconocida para que no falle */
#ifndef APPLICATION_BUILD
/** String con la version y nro de revision con que se compilo el codigo */
#  define APPLICATION_BUILD "version desconocida build " SVN_REVISION
/** Si la version es indefinida pero se trata de una aplicacion RELEASE genera un error. */
#  ifndef __DEBUG__
/** Indica si se trata de una version apta para hacer un release */
/**
 * Si no se puede hacer un mapeo exacto a una revision del SVN, se considera
 * que la aplicacion no puede ser distribuida. Puede tener cambios que no
 * figuren en ningun lugar.
 */
#     define __APPLICATION_NOT_FIT_FOR_RELEASE__ "No se encuentra definido APPLICATION_BUILD que" \
        "es necesario para la aplicacion en modo Release. Definir un valor sensible antes de" \
        "incluir este archivo."
#  endif  // __DEBUG__
#endif   // APPLICATION_BUILD
#if 1
/** Para la version con splash con timeout */
#  define __SPLASH_TIMEOUT_MODE__ wxSPLASH_TIMEOUT
#else
// Para la version con splash que desaparece al terminar de inicializar
#  define __SPLASH_TIMEOUT_MODE__ wxSPLASH_NO_TIMEOUT
#endif
// Tiempo de splash en milisegundos
#  define __SPLASH_TIME__ 5000

/** namespace suri */
namespace suri {
#ifdef __DEBUG__
/** clase traits para aplicaciones suri */
/**
 * Clase que define las caracteristicas de un objeto wxApplication.
 *
 * Define el LogTarget como una ventana de log que replica los mensages
 * a la facilidad de loggin default.
 */
class SuriTraits : public wxGUIAppTraits {
public:
   /** Constructor */
   SuriTraits() {}
   /** Destructor */
   virtual ~SuriTraits() {}
   /** Crea la facilidad de log de la aplicacion */
   virtual wxLog *CreateLogTarget();
};
#endif

template<class MainClass>
/**
 * Clase que representa a la aplicacion
 * \note Define a new application type, each program should derive a class from wxApp
 */
class Application : public wxApp {
public:
   /** Metodo que inicializa la configuracion de la aplicacion */
   bool InitializeConfig();
   /** Inicializa variables con rutas estandar del SO */
   void InitializeOsVariables();
   /** Metodo que inicializa la biblioteca suri */
   void InitializeLibrary();
   /** Metodo que carga los catalogos de traduccion y localizacion */
   void LocalizeApplication();
   /** Muestra el splash */
   void ShowSplash();
   /** Oculta el splash */
   void HideSplash();
   /** Constructor */
   Application();
   /**
    * This one is called on application startup and is a good place for the app initialization
    * (doing it here and not in the ctor allows to have an error return: if OnInit() returns
    * false, the application terminates)
    */
   virtual bool OnInit();
   /** Rutina que se ejecuta al cerrarse la aplicacion. No existe wx en esta instancia */
   virtual int OnExit();
   /** Maneja las excepciones y senales */
   virtual void OnFatalException();
   /** Devuelve el objeto de impresion. */
   /**
    * \attention el puntero devuelto no debe ser eliminado.
    */
   wxHtmlEasyPrinting * const GetPrinter() {
      return pPrinter_;
   }
   /** Muestra el panel de ayuda. */
   /**
    * \todo(04/06/2009 - Alejandro): verificar si el puntero devuelto debe ser
    * eliminado o no.
    */
   wxHtmlHelpController* const GetHelp();
   /** Inicializa el parseador. */
   virtual void OnInitCmdLine(wxCmdLineParser& parser);
   /** Valida parametros recibidos. */
   virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
#ifdef __DEBUG__
   /** Traits especificos */
   virtual SuriTraits *CreateTraits();
#endif
#ifdef __APPLICATION_IS_SINGLE_INSTANCE__
   /** Verifica si hay una instancia de la aplicacion corriendo */
   bool IsInstanceRunning();
#endif  //   __APPLICATION_IS_SINGLE_INSTANCE__

private:
#ifdef __APPLICATION_IS_SINGLE_INSTANCE__
   /** Verifica que solo exista una instancia de la aplicacion */
   wxSingleInstanceChecker* pInstanceChecker_;
#endif  //   __APPLICATION_IS_SINGLE_INSTANCE__
   wxHtmlEasyPrinting* pPrinter_;  /*! utilizada para imprimir htmls. */
   wxHtmlHelpController* pHelpController_; /*! utilizada para mostrar ayuda de la app. */
   wxLocale* pLocale_;  /*! Idioma de la aplicacion */
   wxString cmlParameter_;  /*! primer parametro de la linea de comandos */
   wxSplashScreen* pApplicationSplash_;  /*! ventana de splash */
};

#ifdef __DEBUG__
wxLog *SuriTraits::CreateLogTarget() {
   wxLog::SetActiveTarget(wxGUIAppTraits::CreateLogTarget());
   wxLogWindow *plogwin = new wxLogWindow(NULL, wxT("Log"));
   plogwin->Show();
   wxLog *plog = wxLog::GetActiveTarget();
   if (plog) {
      plog->SetVerbose(true);
   }
   return plogwin;
}
#endif

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
// IMPLEMENT_APP(Application)

// ============================================================================
// implementation
// ============================================================================

/** Template que inicializa las variables de configuracion */
/**
 * En windows utiliza el directorio de ejecucion como la base para el archivo
 * de configuracion.
 *
 * En linux copia el archivo de configuracion del directorio de datos al
 * directorio de usuario modificando su nombre para que respete los estandares
 * (.<nombre_ejecutable>.xml). Para el TCK #869, se tiene en cuenta la fecha de
 * modificacion del archivo origen y destino y se sobreescribe en caso de que
 * el segundo sea mas viejo que el primero.
 *
 * En caso DEBUG, en linux, el directorio de datos se toma como el directorio
 * de desarrollo. Esto permite modificar la configuracion usada por el software
 * sin necesidad de modificar la configuracion versionada.
 * \todo replicar en windows.
 *
 * @return True en caso de poder cargar la configuracion.
 */
template<class MainClass>
bool Application<MainClass>::InitializeConfig() {
   std::string configfilename = MainClass::ConfigFile_;
   std::string usedconfigfilename = wxString(
#ifdef __LINUX__  // En Linux utiliza archivo oculto (prefijo un .)
                              wxString(wxT(".")) +
#endif   // __LINUX__
                              GetAppName() + wxT(".xml")).c_str();
   wxFileName fn(wxStandardPaths().GetExecutablePath());
   fn.MakeAbsolute();
   std::string configdir = (fn.GetVolume() + fn.GetVolumeSeparator()
                              + fn.GetPathWithSep(wxPATH_UNIX)).c_str();
   // Inicializo el archivo de configuracion
   std::string configfile = (wxStandardPaths().GetUserConfigDir() + wxT("/") +
                             usedconfigfilename).c_str();
   wxString originalconfig = wxStandardPaths().GetDataDir() +
#ifdef __LINUX__   // En windows la configuracion se encuentra con el .exe
                             wxT("/data/") +
#endif   // __LINUX__
                             wxT("/") +
                             configfilename;

#ifdef __DEBUG__
   originalconfig = configdir + configfilename;
   configfile = configdir + usedconfigfilename;
#endif   // __DEBUG__

   wxString installationfn(originalconfig);
   wxString installedfn(configfile);

   bool installationfnexists = wxFileName(installationfn).FileExists();
   bool installedfnexists =  wxFileName(installedfn).FileExists();
   if (!installationfnexists) return false;
   if (!installedfnexists){
      wxXmlDocument* doc=new wxXmlDocument();
      wxString nodename=wxT("configuration");
      wxString empty=wxT("");
      wxXmlNode* root=new wxXmlNode(wxXML_ELEMENT_NODE,nodename,empty);
      doc->SetRoot(root);
      doc->Save(installedfn);
   }
   suri::Configuration::SetConfigFile(installedfn.c_str(), installationfn.c_str());

   InitializeOsVariables();

   // Cargo el volatil con el directorio de aplicacion
   suri::Configuration::SetParameter("app_base_dir_volatile", 
      suri::Configuration::GetParameter("app_user_data", configdir));
   // Cargo la version del software
   suri::Configuration::SetParameter("app_version", APPLICATION_BUILD);

   return true;
}

/**
 * Inicializa variables de configuracion utilizando las rutas especiales del
 * sistema operativo que corresponde.
 * \post Se modifica el nombre de la aplicacion al que trae la configuracion
 * Las vairables son:
 *  - app_data_dir : Utiliza el directorio de recursos
 *  - app_temp_dir : Utiliza el directorio temporal
 *  - app_user_data : Utiliza el directorio de datos de usuario
 */
template<class MainClass>
void Application<MainClass>::InitializeOsVariables() {
   // Si no esta definido, utilizo AppName como el nombre corto de la aplicacion
   if (suri::Configuration::GetParameter("app_short_name", "").empty()) {
      suri::Configuration::SetParameter("app_short_name", GetAppName().c_str());
   }

   // Cargo variable con directorio de datos de la aplicacion.
   wxFileName fn = wxFileName(wxStandardPaths().GetResourcesDir() + "/data/");
#ifdef __DEBUG__
   // En modo debug, si existe el directorio data en el nivel de ejecutable,
   // lo usa, sino sigue con el de configuracion. Sirve para ejecutar una instalacion
   // en modo debug o ejecutar en modo debug en entorno de desarrollo.
   wxFileName newfn = wxFileName(wxStandardPaths().GetExecutablePath());
   newfn = wxFileName(newfn.GetVolume() + newfn.GetVolumeSeparator() +
                      newfn.GetPathWithSep(wxPATH_UNIX) + wxT("data/"));
   newfn.MakeAbsolute();
   if (wxFileName::DirExists(newfn.GetFullPath())) {
      fn = newfn;
   }
#endif  // __DEBUG__
   std::string datadir = (fn.GetVolume() + fn.GetVolumeSeparator() +
                          fn.GetPathWithSep(wxPATH_UNIX)).c_str();
   suri::Configuration::SetParameter("app_data_dir", datadir);

   // Cargo variable con directorio temporal.
   fn = wxFileName(wxStandardPaths().GetTempDir() + "/" + GetAppName() + ".temp/temp/");
   std::string tempdir = (fn.GetVolume() + fn.GetVolumeSeparator() +
                          fn.GetPathWithSep(wxPATH_UNIX)).c_str();
   suri::Configuration::SetParameter("app_temp_dir", tempdir);

   // Cargo variable con directorio de datos con el home del usuario.
   fn = wxFileName(wxStandardPaths().GetUserDataDir() + "/");
#ifdef __DEBUG__
   // En modo debug, si existe el directorio data en el nivel de ejecutable, lo usa, sino sigue
   // con el de configuracion. Sirve para ejecutar una instalacion en modo debug o ejecutar en
   // modo debug en entorno de desarrollo.
   newfn = wxFileName(wxStandardPaths().GetExecutablePath());
   newfn = wxFileName(newfn.GetVolume() + newfn.GetVolumeSeparator() +
                      newfn.GetPathWithSep(wxPATH_UNIX) + wxT("data/"));
   newfn.MakeAbsolute();
   if (wxFileName::DirExists(newfn.GetFullPath())) {
      fn = newfn;
   }
#endif  // __DEBUG__
   std::string usrdata = (fn.GetVolume() + fn.GetVolumeSeparator() +
                          fn.GetPathWithSep(wxPATH_UNIX)).c_str();
   suri::Configuration::SetParameter("app_user_data", usrdata);
}

/**
 * Template que inicializa las librerias sociadas a la aplicacion
 */
template<class MainClass>
void Application<MainClass>::InitializeLibrary() {
   // Inicializacion wx
   wxXmlResource::Get()->InitAllHandlers();

   // Inicializacion suri
   std::string argv = suri::Configuration::GetParameter("app_data_dir", wxT("./"));
   suri::LibSuriConfig.variables_["GDAL_SKIP"] = suri::Configuration::GetParameter(
         "lib_ignored_image_formats", wxT(""));
   suri::Initialize(argv + "gdal/");
}

/** Template que carga los catalogos de traduccion y localizacion */
template<class MainClass>
void Application<MainClass>::LocalizeApplication() {
   // Creo el objeto de localizacion
   pLocale_ = new wxLocale;
   // Busco el idioma deseado
   std::string language = suri::Configuration::GetParameter("app_language", wxT(""));
   const wxLanguageInfo *planginf = wxLocale::FindLanguageInfo(language);
   int langid = planginf ? planginf->Language : wxLocale::GetSystemLanguage();

   // Normally this wouldn't be necessary as the catalog files would be found in the default
   // locations, but when the program is not installed the catalogs are in the build directory
   // where we wouldn't find them by default
   std::string langdir = suri::Configuration::GetParameter("app_lang_dir", wxT("./"));
   // Reconvierto appdir para la plataforma porque la busqueda de catalogo usa filename nativos
   wxFileName fn = wxFileName(langdir, wxPATH_UNIX);
   langdir = fn.GetFullPath();
   wxLocale::AddCatalogLookupPathPrefix(langdir);
   // Inicializo
   {
#ifdef __DEBUG__
      wxLogNull nolog;
#endif
      if (!pLocale_->Init(langid)) {
         // Si falla el locale, agrego el directorio con las traducciones en espanol
         wxFileName extradir(wxT("es/LC_MESSAGES"), wxT(""), wxPATH_UNIX);
         wxLocale::AddCatalogLookupPathPrefix(langdir + extradir.GetPath());
         pLocale_->AddCatalog("wxstd");
         SHOW_ERROR(message_UNSUPPORTED_LANGUAGE);
         // No se traduce, lo hace SHOW_
      }
   }

   // Seteo el LC_NUMERIC a C para que use . como separador de decimal
   // ( gdal/ogr lo requieren de esta forma )
   setlocale(LC_NUMERIC, "C");
   {
#ifdef __DEBUG__
      wxLogNull nolog;
#endif
      // Initialize the catalogs we'll be using
      pLocale_->AddCatalog("Application");
      pLocale_->AddCatalog("suri");
      pLocale_->AddCatalog("resources");
   // this catalog is installed in standard location on Linux systems and
   // shows that you may make use of the standard message catalogs as well
   // if it's not installed on your system, it is just silently ignored.
#ifdef __LINUX__
      pLocale_->AddCatalog(_T("fileutils"));
#endif
   }


   REPORT_DEBUG("D:locale LC_ALL: %s", setlocale(LC_ALL, NULL));
   REPORT_DEBUG("D:locale LC_COLLATE: %s", setlocale(LC_COLLATE, NULL));
   REPORT_DEBUG("D:locale LC_CTYPE: %s", setlocale(LC_CTYPE, NULL));
#ifdef __LINUX__
   REPORT_DEBUG("D:locale LC_MESSAGES: %s", setlocale(LC_MESSAGES, NULL));
#endif
   REPORT_DEBUG("D:locale LC_MONETARY: %s", setlocale(LC_MONETARY, NULL));
   REPORT_DEBUG("D:locale LC_NUMERIC: %s", setlocale(LC_NUMERIC, NULL));
   REPORT_DEBUG("D:locale LC_TIME: %s", setlocale(LC_TIME, NULL));
}

#ifdef __APPLICATION_IS_SINGLE_INSTANCE__
/** Template que verifica que no exista otra instancia de la aplicacion corriendo */
template<class MainClass>
bool Application<MainClass>::IsInstanceRunning() {
   // Verifica que no exista otra instancia de la aplicacion.
   pInstanceChecker_ = new wxSingleInstanceChecker(GetAppName(),
                                                   wxStandardPaths().GetTempDir());
   if (pInstanceChecker_->IsAnotherRunning()) {
      delete pInstanceChecker_;
      pInstanceChecker_ = NULL;
      SHOW_ERROR(_(message_APPLICATION_INSTANCE_RUNNING));
      return true;
   }
   return false;
}
#endif   // __APPLICATION_IS_SINGLE_INSTANCE__
/** Template para mostrar la imagen de presentacion de la aplicacion */
template<class MainClass>
void Application<MainClass>::ShowSplash() {
#if __SPLASH_TIME__ > 0
   wxBitmap splashbmp;
   GET_BITMAP_RESOURCE(MainClass::SplashBitmap_, splashbmp);

   pApplicationSplash_ = new wxSplashScreen(splashbmp,
           wxSPLASH_CENTRE_ON_SCREEN | __SPLASH_TIMEOUT_MODE__,
           __SPLASH_TIME__, NULL, wxID_ANY, wxDefaultPosition, wxDefaultSize,
           wxBORDER_NONE | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP);
   pApplicationSplash_->CenterOnScreen();
#endif
}

/** Template para ocultar la imagen de presentacion de la aplicacion */
template<class MainClass>
void Application<MainClass>::HideSplash() {
   if (!pApplicationSplash_) {
      return;
   }
#if  __SPLASH_TIMEOUT_MODE__ != wxSPLASH_TIMEOUT
   // Solo se debe hacer Destroy cuando se desea que el splash desaparezca al
   // finalizar la construccion de la ventana principal
   pApplicationSplash_->Destroy();
#else
   pApplicationSplash_->GetSplashStyle();  // dummy
#endif /* wxSPLASH_TIMEOUT==__SPLASH_TIMEOUT_MODE__ */
}

/**
 * Constructor
 */
template<class MainClass>
Application<MainClass>::Application() :
      pInstanceChecker_(NULL), pPrinter_(NULL), pHelpController_(NULL), pLocale_(NULL),
      cmlParameter_(wxT("")), pApplicationSplash_(NULL) {
// (Pablo - TCK #10637): Esto está para que en Ubuntu cargue los scrollers
//  (horizontal y vertical) como los de Windows, en vez de los de Unity.
#ifdef __LINUX__
   wxSetEnv(wxS("LIBOVERLAY_SCROLLBAR"), wxS("0"));
#endif
}

/**
 * Se ejecuta al iniciar la aplicacion
 * Inicializacion de la aplicacion
 */
template<class MainClass>
bool Application<MainClass>::OnInit() {
   if (IsInstanceRunning()) {
      return false;
   }
   // call the base class initialization method, currently it only parses a
   // few common command-line options but it could be do more in the future
   cmlParameter_ = wxT("");
   if (!wxApp::OnInit()) {
      return false;
   }
   // Inicializo los recursos de aplicacion
   MainClass::InitializeResources();
#ifdef __DEBUG__
   if (cmlParameter_ == wxT("nosplash")) {
      cmlParameter_ = wxT("");
   } else
#endif   // __DEBUG__
   ShowSplash();

   if (!InitializeConfig()) {
      SHOW_ERROR(message_CONFIGURATION_ERROR);
      return false;
   }

   InitializeLibrary();

   if (!MainClass::InitializeApplication()) {
      SHOW_ERROR(message_APPLICATION_INITIALIZATION_ERROR);
      return false;
   }

   LocalizeApplication();
   // Valido la existencia de los archivos necesarios para correr el programa
   std::vector<std::string> fileserror;
   if (!suri::CheckManifestFiles(fileserror)) {
      SHOW_ERROR(message_INSTALL_FILES_NOT_FOUND);
      // No se traduce, lo hace SHOW_
      std::vector<std::string>::iterator it = fileserror.begin();
      for (; it != fileserror.end(); it++)
         REPORT_ERROR("D:Falta: %s", (*it).c_str());
#ifndef __DEBUG__
      // En modo release aborto la ejecucion
      return false;
#endif
   }

#if wxUSE_ON_FATAL_EXCEPTION
   wxHandleFatalExceptions(true);
#else
#  ifndef __WXMSW__
#     warning "D:El sistema no maneja excecpiones fatales"
#  endif
   REPORT_DEBUG("D:El sistema no maneja excepciones fatales");
   signal(SIGSEGV, OnSigSev);
#endif

   MainClass& manager = MainClass::Instance();
   manager.SetParameter(cmlParameter_.c_str());
   wxTopLevelWindow *pwindow = manager.GetWindow();
   if (pwindow) {
      // Busco el nombre de la aplicacion
      if (pwindow->GetTitle().IsEmpty()) {
         pwindow->SetTitle(suri::Configuration::GetParameter("app_name", wxT("")));
      }
      SetExitOnFrameDelete(true);
      pwindow->Center();

      // Genero el objeto de impresion.
      pPrinter_ = new wxHtmlEasyPrinting(_(caption_PRINT));
      // Setea los margenes en 0
      pPrinter_->GetPageSetupData()->SetMarginTopLeft(wxPoint(0, 0));
      pPrinter_->GetPageSetupData()->SetMarginBottomRight(wxPoint(0, 0));
      pHelpController_ = NULL;
      cmlParameter_ = wxT("");
   } else {
      SHOW_ERROR(message_MAIN_TOOL_NOT_CREATED);
      // No se traduce, lo hace SHOW_
      return false;
   }
   HideSplash();
   wx_static_cast(wxApp*, wxApp::GetInstance())->SetTopWindow(pwindow);
   // Despues de toda la inicializacion, muestro la advertencia en modo Release
#if !defined(__DEBUG__) && defined(__APPLICATION_NOT_FIT_FOR_RELEASE__)
   wxMessageBox(__APPLICATION_NOT_FIT_FOR_RELEASE__, _("Advertencia"),
                 wxOK | wxICON_ERROR);
#endif

   return true;
}

/** Se ejecuta al terminar la ejecucion */
/**
 *  No existen los objetos wx en esta instancia.
 *  Debe regresar el valor de la funcion overlodeada
 */
template<class MainClass>
int Application<MainClass>::OnExit() {
   // Cargo el volatil con el directorio de aplicacion
   suri::Configuration::SetParameter("app_base_dir_volatile", "");
   delete pPrinter_;
   delete pHelpController_;
   delete pLocale_;
#ifdef __APPLICATION_IS_SINGLE_INSTANCE__
   delete pInstanceChecker_;
   pInstanceChecker_ = NULL;
#endif
   int retval = wxApp::OnExit();
   return retval;
}

/** Se ejecuta frente a una excepcion o senal */
template<class MainClass>
void Application<MainClass>::OnFatalException() {
   // Cargo el volatil con el directorio de aplicacion
   suri::Configuration::SetParameter("app_base_dir_volatile", "");
#ifdef __DEBUG__
   std::cerr << "D:Error desconocido; Terminando la aplicacion" << std::endl;
#endif
}

/** Crea/devuelve un puntero al objeto de ayuda de la aplicacion. */
/**
 * Crea/devuelve un puntero al objeto de ayuda de la aplicacion.
 * Si no existe lo crea, pasandole como parametro el directorio donde se
 * encuentran los html de la ayuda de la aplicacion.
 * Si existe, solo lo devuelve.
 * @return puntero al objeto de ayuda de la App.
 * \attention quien invoque a este metodo es responsable del puntero devuelto,
 * por lo tanto debe hacerse cargo de su eliminacion.
 */
template<class MainClass>
wxHtmlHelpController* const Application<MainClass>::GetHelp() {
   if (!pHelpController_) {
      pHelpController_ = new wxHtmlHelpController();
      std::string helpdir = suri::Configuration::GetParameter("app_help_dir",
                                                              wxT("./"));
      helpdir += "ayuda.hhp";
      pHelpController_->AddBook(wxFileName(helpdir), false);
   }
   return pHelpController_;
}

/** Inicializa el parseador de la aplicacion. */
/**
 * Inicializa Parseador de la aplicacion.
 * \pre   Parser debe ser instanciado antes de invocar a OnCmdLineParsed.
 * \post Parser configurado para recibir parametros de tipo cadena.
 * @param[in]   Parser parseador de la aplicacion.
 * @param[out]   Parser parseador de la aplicacion.
 * \note se agrega flag wxCMD_LINE_PARAM_MULTIPLE ya que si solo se configura
 * con wxCMD_LINE_PARAM_OPTIONAL falla al invocar la aplicacion desde la linea
 * de comandos con mas de 1 parametro.
 */
template<class MainClass>
void Application<MainClass>::OnInitCmdLine(wxCmdLineParser& Parser) {
   Parser.AddParam(_(message_FIRST_APP_PARAMETER), wxCMD_LINE_VAL_STRING,
                   wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE);
   wxApp::OnInitCmdLine(Parser);
}

/**
 * Obtiene el primer parametro que se le pasa a la aplicacion.
 * \pre  El metodo OnInitCmdLine debe ser invocado antes de invocar a
 * OnCmdLineParsed
 * @param[in]  Parser parseador de la aplicacion.
 * @return     true
 */
template<class MainClass>
bool Application<MainClass>::OnCmdLineParsed(wxCmdLineParser &Parser) {
   cmlParameter_ = wxT("");
   if (Parser.GetParamCount() > 0) {
      cmlParameter_ = Parser.GetParam(0);
   }
   return wxApp::OnCmdLineParsed(Parser);
}

#if !wxUSE_ON_FATAL_EXCEPTION
/**
 * Ante un error finaliza la aplicacion.
 * \note no hace nada con el parametro recibido.
 * @param[in]  SignalNumber numero de senal del SO.
 */
void OnSigSev(int SignalNumber) {
   // Cargo el volatil con el directorio de aplicacion
   suri::Configuration::SetParameter("app_base_dir_volatile", "");
#ifdef __DEBUG__
   std::cerr << "D:Error desconocido; Terminando la aplicacion" << std::endl;
#endif
   wxTheApp->ExitMainLoop();
   _Exit(0);
}
#endif

#ifdef __DEBUG__
/** Traits especificos */
template<class MainClass>
SuriTraits *Application<MainClass>::CreateTraits() {
   return new SuriTraits;
}
#endif

}  // namespace suri

#endif   // APPLICATION_H_
