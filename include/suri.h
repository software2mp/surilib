/*! \file suri.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
