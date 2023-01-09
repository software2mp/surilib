/*! \file RasterDriver.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/RasterDriver.h"

// Includes suri
#include "logmacros.h"

/** namespace suri */
namespace suri {
/** ctor */
/**
 *  Inicializa las propiedades en valores inocuos.
 *  pSource_ queda NULL porque la fuente de los drivers es el archivo.
 */
RasterDriver::RasterDriver() :
      driverName_("RasterDriver"), dataName_("void"), dataSize_(0), fileName_(""),
      band_(0), bandCount_(1) {
}

/** dtor */
RasterDriver::~RasterDriver() {
}

/**
 * @return tamanio X del bloque
 */
int RasterDriver::GetBlockSizeX() const {
   int temp, dummy;
   GetBlockSize(temp, dummy);
   return temp;
}

/**
 * @return tamanio Y del bloque
 */
int RasterDriver::GetBlockSizeY() const {
   int temp, dummy;
   GetBlockSize(dummy, temp);
   return temp;
}
}
