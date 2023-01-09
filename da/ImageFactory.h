/*! \file ImageFactory.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef IMAGEFACTORY_H_
#define IMAGEFACTORY_H_

// Includes suri
#include "suri/Image.h"
#include "TFactory.h"

namespace suri {
/** Defino fabrica de imagenes */
typedef TFactory<Image> ImageFactory;
/** Defino singleton para fabrica de imagenes */
typedef TSingleton<ImageFactory> TheImageFactory;
/** Funcion que registra los tipos de imagenes */
void RegisterAllImages();
}

#endif /*IMAGEFACTORY_H_*/
