/*! \file SRESquareRootEnhancement.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <cmath>

#include "SRESquareRootEnhancement.h"
#include "SRELinearEnhancement.h"
#include "SREEnhancementFactory.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Registro el realce de tipo raiz cuadrada.
 */
REGISTER_ENHANCEMENT(SquareRootEnhancement, true)

/**
 * Ctor.
 */
SquareRootEnhancement::SquareRootEnhancement(int BandCount, int* pNumBins) :
      LinearEnhancement(BandCount, pNumBins) {

}

/**
 * Dtor.
 */
SquareRootEnhancement::~SquareRootEnhancement() {

}

/**
 * Crea una lookup table a partir de un histograma de intensidad.
 */
int** SquareRootEnhancement::CreateLUT(int** ppHistA, int** ppHistB) {
   int** lut = new int*[bandCount_];
   for (int ix = 0; ix < bandCount_; ++ix) {
      lut[ix] = new int[pNumBins_[ix]];
      memset(lut[ix], 0, sizeof(int) * pNumBins_[ix]);
   }

   for (int ix = 0; ix < bandCount_; ++ix) {
      for (int jx = 0; jx < pNumBins_[ix]; ++jx) {
         lut[ix][jx] = floor(
               sqrt(static_cast<double>(pNumBins_[ix] - 1) * static_cast<double>(jx)));
      }
   }
   return lut;
}

/**
 * Crea una instancia de SquareRootEnhancement.
 */
Enhancement* SquareRootEnhancement::Create(suri::ParameterCollection& Params) {
   int bandcount = 0;
   Params.GetValue<int>("BandCount", bandcount);

   int* pnumbins = NULL;
   Params.GetValue<int*>("NumBins", pnumbins);

   return new SquareRootEnhancement(bandcount, pnumbins);
}

} /** namespace enhancement */
} /** namespace raster */
} /** namespace suri */
