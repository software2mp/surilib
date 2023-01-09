/** \file SRELinearEnhancement.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <cstring>

#include "SRELinearEnhancement.h"
#include "SREEnhancementFactory.h"
#include "suri/ParameterCollection.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Registro el realce lineal.
 */
REGISTER_ENHANCEMENT(LinearEnhancement, true)

/**
 * Ctor.
 */
LinearEnhancement::LinearEnhancement(int BandCount, int* pNumBins, double** ppBAFP) :
      Enhancement(BandCount, pNumBins), ppBAFP_(ppBAFP) {
}

/**
 * Dtor.
 */
LinearEnhancement::~LinearEnhancement() {
}

/**
 * Crea una lookup table a partir de un histograma de intensidad.
 */
int** LinearEnhancement::CreateLUT(int** ppHistA, int** ppHistB) {
   int** lut = new int*[bandCount_];
   for (int ix = 0; ix < bandCount_; ++ix) {
      lut[ix] = new int[pNumBins_[ix]];
      memset(lut[ix], 0, sizeof(int) * pNumBins_[ix]);
   }

   for (int ix = 0; ix < bandCount_; ++ix) {
      for (int jx = 0; jx < pNumBins_[ix]; ++jx) {
         lut[ix][jx] = jx;
      }
   }

   return lut;
}

/**
 * Crea una instancia de EqualizationEnhancement.
 */
Enhancement* LinearEnhancement::Create(suri::ParameterCollection& Params) {
   int bandcount = 0;
   Params.GetValue<int>("BandCount", bandcount);

   int* pnumbins = NULL;
   Params.GetValue<int*>("NumBins", pnumbins);

   double** ppbafp = NULL;
   Params.GetValue<double**>("BAFP", ppbafp);

   return new LinearEnhancement(bandcount, pnumbins, ppbafp);
}

}  // namespace enhancement
}  // namespace raster
}  // namespace suri
