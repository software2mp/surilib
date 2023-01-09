/*! \file LibraryUtils.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
