// <COPYRIGHT>
#ifndef <::MAINCLASS_H_::>
#define <::MAINCLASS_H_::>

// Includes estandar
#include <map>
#include <string>

// Includes Suri
#include "suri/Widget.h"

// Includes Wx

// Includes App

// Defines

// forwards

/** clase principal */
class <::MainClass::>
{
   /** ctor privado, clase singleton */
   <::MainClass::>();
public:
// ---- IMPLEMENTACION POLITICA DE APPLICATION ---------------------------------
   /** dtor */
   virtual ~<::MainClass::>();
   /** metodo de obtencion de la instancia */
   static <::MainClass::>&Instance();
   /** retorna la ventana que la aplicacion considera principal */
   wxTopLevelWindow *GetWindow();
   /** carga un parametro como string */
   void SetParameter(const std::string &Parameter);
   /** carga los parametros completos */
   void SetParameters(std::multimap<std::string, std::string> &Parameters);
   /** inicializa la aplicacion */
   static bool InitializeApplication();
   /** inicializa los recursos de la aplicacion */
   static void InitializeResources();
   /** string que representa al bitmap del splash */
   static const char SplashBitmap_[];
   /** la ruta del archivo de configuracion */
   static const char ConfigFile_[];
// ---- FIN POLITICA APPLICATION -----------------------------------------------
protected:
private:
};

#endif /* <::MAINCLASS_H_::> */
