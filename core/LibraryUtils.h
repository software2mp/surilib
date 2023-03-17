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

#ifndef LIBRARYUTILS_H_
#define LIBRARYUTILS_H_

#include <cstring>
#include <string>

#include <wx/strconv.h>
#include <wx/stdpaths.h>
#include <wx/filefn.h>
#include <wx/filename.h>

namespace suri {

#define DIR_CUR            "./"
#define DIR_CUR_LEN        strlen(DIR_CUR)
#define DIR_USR_DATA       "USR_DATA"
#define DIR_USR_DATA_LEN   strlen(DIR_USR_DATA)
#define DIR_SYS_DATA       "SYS_DATA"
#define DIR_SYS_DATA_LEN   strlen(DIR_SYS_DATA)

/**
 * Devuelve una ruta valida basado en la configuracion de la app.
 * Por ejemplo si path es "./" devuelve la ruta desde C:
 */
std::string ResolvePath(const std::string& Path);

/**
 * Crea el directorio para el archivo si la ruta no existe.
 */
void EnsureFilePath(const std::string& Filename);

}  // namespace suri

#endif  // LIBRARYUTILS_H_
