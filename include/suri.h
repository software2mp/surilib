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

#ifndef SURI_H_
#define SURI_H_

// Includes standard
#include <string>
#include <map>

// Includes suri
#include "logmacros.h"

/**
 * En este archivo se incluyen los .h mas importantes de la biblioteca surilib.
 */

/** namespace de la biblioteca */
namespace suri {
/** Estructura con la configuracion basica de la biblioteca */
/**
 *  Esta estructura contiene las variables de configuracion basica que requiere
 * la biblioteca.
 *
 *  Todos los directorios deben ser absolutos y terminar en el separador de la
 * plataforma (o de UNIX para compatibilidad win32/linux/mac?)
 */
typedef struct {
   std::string dataDir_; /*! Directorio de datos */
   std::string libBuild_; /*! String con la version de la biblioteca */
   /** Tipo de dato para el mapa de variables de entorno */
   typedef std::map<std::string, std::string> EnvironmentMapType;
   EnvironmentMapType variables_; /*! pares variable/valor para gdal */
} ParametersType;

extern ParametersType LibSuriConfig; /*! Variables de configuracion de la */
/* biblioteca Suri */
/** Funcion de inicializacion general para la biblioteca */
void Initialize(const std::string &SuriDataDir);
}  // namespace suri

#endif /* SURI_H_ */
