/** \file SRELinear2PercentEnhancement.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <cstdio>
#include <cstring>
#include <cmath>

#include "SRELinear2PercentEnhancement.h"
#include "SRELinearEnhancement.h"
#include "SREEnhancementFactory.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Registro el realce lineal 2 porciento.
 */
REGISTER_ENHANCEMENT(Linear2PercentEnhancement, true)

/**
 * Ctor.
 */
Linear2PercentEnhancement::Linear2PercentEnhancement(int BandCount,
      int* pNumBins, double** ppBAFP) :
      LinearEnhancement(BandCount, pNumBins, ppBAFP) {

}

/**
 * Dtor.
 */
Linear2PercentEnhancement::~Linear2PercentEnhancement() {

}

/**
 * Crea una lookup table a partir de un histograma de intensidad.
 */
int** Linear2PercentEnhancement::CreateLUT(int** ppHistA, int** ppHistB) {
   int** lut = new int*[bandCount_];
   for (int ix = 0; ix < bandCount_; ++ix) {
      lut[ix] = new int[pNumBins_[ix]];
      memset(lut[ix], 0, sizeof(int) * pNumBins_[ix]);
   }

   for (int bandix = 0; bandix < bandCount_; ++bandix) {
      int low = 0;
      int high = 0;
      int bin_adjustment = 0;

      // Saco las frecuencias bajo 2%
      for (int jx = 0; jx < 256; ++jx) {
         if (ppBAFP_[bandix][jx] < 2.0) {
            ++low;
         } else {
            break;
         }
      }

      // Saco las frecuencias sobre 98%
      for (int jx = 255; jx > -1; --jx) {
         if (ppBAFP_[bandix][jx] > 98.0) {
            ++high;
         } else {
            break;
         }
      }

      if (low > 0 || high > 0) {
         if (low > 0)
            ++bin_adjustment;
         if (high > 0)
            ++bin_adjustment;

         int lowbin = low;
         int highbin = (pNumBins_[bandix] - high);

         double scale_factor = (double) (pNumBins_[bandix] - bin_adjustment)
               / (double) (highbin - lowbin);

         int binscount = (highbin - lowbin);

         for (int kx = 0; kx < lowbin; ++kx)
            lut[bandix][kx] = 0;

         for (int kx = pNumBins_[bandix] - 1; kx >= highbin; --kx)
            lut[bandix][kx] = 255;

         for (int jx = 0; jx <= binscount; ++jx) {
            lut[bandix][jx + lowbin] = floor(jx * scale_factor);
         }
      } else {
         lut = LinearEnhancement::CreateLUT();
      }
   }

   return lut;
}


/**
 * Crea una instancia de EqualizationEnhancement.
 */
Enhancement* Linear2PercentEnhancement::Create(suri::ParameterCollection& Params) {
   int bandcount = 0;
   Params.GetValue<int>("BandCount", bandcount);

   int* pnumbins = NULL;
   Params.GetValue<int*>("NumBins", pnumbins);

   double** ppbafp = NULL;
   Params.GetValue<double**>("BAFP", ppbafp);

   return new Linear2PercentEnhancement(bandcount, pnumbins, ppbafp);
}

}  // namespace enhancement
}  // namespace raster
}  // namespace suri
