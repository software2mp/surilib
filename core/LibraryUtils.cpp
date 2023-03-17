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
