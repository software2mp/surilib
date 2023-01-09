/*! \file TasseledCapTransformation.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "TasseledCapTransformation.h"

namespace suri {
namespace raster {
namespace transformation {

/**
 * Ctor.
 */
TasseledCapTransformation::TasseledCapTransformation(const std::vector<int>& OutBands) :
      outBands_(OutBands) {

}

/**
 * Dtor.
 */
TasseledCapTransformation::~TasseledCapTransformation() {

}

/**
 * Devuelve la cantidad de bandas de salida.
 */
int TasseledCapTransformation::GetOutputBandCount() {
   return outBands_.size();
}

}  // namespace transformation
}  // namespace raster
}  // namespace suri
