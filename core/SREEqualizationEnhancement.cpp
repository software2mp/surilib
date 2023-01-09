/** \file SREEqualizationEnhancement.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <cmath>
#include <string>

#include "SREEnhancement.h"
#include "SREEqualizationEnhancement.h"
#include "SREEnhancementFactory.h"
#include "suri/ParameterCollection.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Registro el realce de ecualizacion.
 */
REGISTER_ENHANCEMENT(EqualizationEnhancement, true)

/**
 * Ctor.
 */
EqualizationEnhancement::EqualizationEnhancement(int BandCount, int* pNumBins,
                                                 int* pAccumFreq) :
      Enhancement(BandCount, pNumBins), pAccumFreq_(pAccumFreq) {
}

/**
 * Dtor.
 */
EqualizationEnhancement::~EqualizationEnhancement() {

}

/**
 * Crea una lookup table a partir de un histograma de intensidad.
 */
int** EqualizationEnhancement::CreateLUT(int** ppHistA, int** ppHistB) {
   int** lut = new int*[bandCount_];
   for (int ix = 0; ix < bandCount_; ++ix) {
      lut[ix] = new int[pNumBins_[ix]];
      memset(lut[ix], 0, sizeof(int) * pNumBins_[ix]);
   }

   for (int bandix = 0; bandix < bandCount_; ++bandix) {
      int accum = 0;
      double scale_factor = (static_cast<double>(pNumBins_[bandix]) - 1.0)
            / static_cast<double>(pAccumFreq_[bandix]);
      for (int binix = 0; binix < pNumBins_[bandix]; ++binix) {
         accum += ppHistA[bandix][binix];
         int valr = static_cast<int>(floor(accum * scale_factor));
         if (valr > pNumBins_[bandix] - 1) {
            lut[bandix][binix] = pNumBins_[bandix] - 1;
         } else {
            lut[bandix][binix] = valr;
         }
      }
   }

   return lut;
}

/**
 * Crea una instancia de EqualizationEnhancement.
 */
Enhancement* EqualizationEnhancement::Create(suri::ParameterCollection& Params) {
   int bandcount = 0;
   Params.GetValue<int>("BandCount", bandcount);

   int* pnumbins = NULL;
   Params.GetValue<int*>("NumBins", pnumbins);

   int* paccumfreq = NULL;
   Params.GetValue<int*>("AccumFreq", paccumfreq);

   return new EqualizationEnhancement(bandcount, pnumbins, paccumfreq);
}

}  // namespace enhancement
}  // namespace raster
}  // namespace suri
