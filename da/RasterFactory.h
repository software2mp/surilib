/*! \file RasterFactory.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RASTERFACTORY_H_
#define RASTERFACTORY_H_

// Includes suri
#include "suri/RasterBand.h"
#include "TFactory.h"

/** namespace suri */
namespace suri {
/** Defino una fabrica de bandas */
typedef TFactory<RasterBand> RasterBandFactory;
/** Defino un singleton para la fabrica de bandas */
typedef TSingleton<RasterBandFactory> TheBandFactory;

void RegisterAllRasterBands();
}

#endif /*RASTERFACTORY_H_*/
