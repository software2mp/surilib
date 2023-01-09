/*! \file LibraryUtils.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "LibraryUtils.h"

#include "suri/Configuration.h"

#define MKDIR_PERMISSIONS  0777

namespace suri {

/**
 * Devuelve una ruta valida basado en la configuracion de la app.
 * Por ejemplo si path es "./" devuelve la ruta desde C:
 * \todo (danieltesta-#3842): ver bien como se pueden interpretar
 * los distintos paths. Ej: C:/Data, data/, /data
 *
 * DIR_SYS_DATA:
 * - Unix: prefix/share/appname
 * - Windows: directorio del ejecutable
 *
 * DIR_USR_DATA:
 * - Unix: ~/.appname
 * - Windows: C:\Documents and Settings\username\Application Data\appname
 */
std::string ResolvePath(const std::string& Path) {
   std::string retpath;
   if (Path.find(DIR_CUR) == 0) {
      retpath.clear();
      retpath.append(suri::Configuration::GetParameter("app_base_dir_volatile", ""));
      if (Path.length() > DIR_CUR_LEN) {
         retpath.append(Path.substr(DIR_CUR_LEN));
         retpath.append("/");
      }
   } else if (Path.find(DIR_USR_DATA) == 0) {
      wxFileName dirname(wxStandardPaths::Get().GetUserDataDir(), wxEmptyString);
      retpath = (dirname.GetVolume() + dirname.GetVolumeSeparator()
            + dirname.GetPathWithSep(wxPATH_UNIX)).c_str();
      if (Path.length() > DIR_USR_DATA_LEN) {
         retpath.append(Path.substr(DIR_USR_DATA_LEN + 1));
         retpath.append("/");
      }
   } else if (Path.find(DIR_SYS_DATA) == 0) {
#ifndef __DEBUG__
      wxFileName dirname(wxStandardPaths::Get().GetDataDir(), wxEmptyString);
      retpath = (dirname.GetVolume() + dirname.GetVolumeSeparator()
            + dirname.GetPathWithSep(wxPATH_UNIX)).c_str();
#else
      // En modo DEBUG el directorio de sistema esta donde este la app
      // para hacer mas facil el desarrollo.
      retpath = suri::Configuration::GetParameter("app_base_dir_volatile", "");
#endif  // __DEBUG__
      if (Path.length() > DIR_SYS_DATA_LEN) {
         retpath.append(Path.substr(DIR_SYS_DATA_LEN + 1));
         retpath.append("/");
      }
   }

   return retpath.empty() ? Path : retpath;
}

/**
 * Crea el directorio para el archivo si la ruta no existe.
 */
void EnsureFilePath(const std::string& Filename) {
   wxFileName fname(Filename);
   if (!wxDirExists(fname.GetPath())) {
      wxFileName::Mkdir(fname.GetPath(), MKDIR_PERMISSIONS, wxPATH_MKDIR_FULL);
   }
}

}
