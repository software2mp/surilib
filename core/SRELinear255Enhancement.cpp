/** \file SRELinear255Enhancement.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <cstring>

#include "SRELinear255Enhancement.h"
#include "SRELinearEnhancement.h"
#include "SREEnhancementFactory.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Registro el realce lineal 0-255.
 */
REGISTER_ENHANCEMENT(Linear255Enhancement, true)

/**
 * Ctor.
 */
Linear255Enhancement::Linear255Enhancement(int BandCount, int* pNumBins) :
      LinearEnhancement(BandCount, pNumBins) {

}

/**
 * Dtor.
 */
Linear255Enhancement::~Linear255Enhancement() {

}

/**
 * Crea una instancia de Linear255Enhancement.
 */
Enhancement* Linear255Enhancement::Create(suri::ParameterCollection& Params) {
   int bandcount = 0;
   Params.GetValue<int>("BandCount", bandcount);

   int* pnumbins = NULL;
   Params.GetValue<int*>("NumBins", pnumbins);

   return new Linear255Enhancement(bandcount, pnumbins);
}

}  // namespace enhancement
}  // namespace raster
}  // namespace suri
