/*! \file SREHistogramMatchingEnhancement.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <cstdio>
#include <cstring>
#include <cmath>

#include "SREHistogramMatchingEnhancement.h"
#include "SREEnhancementFactory.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Registro el realce por combinacion de histogramas.
 */
REGISTER_ENHANCEMENT(HistogramMatchingEnhancement, false)

/**
 * Ctor.
 */
HistogramMatchingEnhancement::HistogramMatchingEnhancement(int BandCount, int* pNumBins) :
      Enhancement(BandCount, pNumBins) {

}

/**
 * Dtor.
 */
HistogramMatchingEnhancement::~HistogramMatchingEnhancement() {

}

/**
 * Crea una lookup table a partir de dos histogramas de intensidad.
 */
int** HistogramMatchingEnhancement::CreateLUT(int** ppHistA, int** ppHistB) {
   int** lut = new int*[bandCount_];
   for (int ix = 0; ix < bandCount_; ++ix) {
      lut[ix] = new int[pNumBins_[ix]];
      memset(lut[ix], 0, sizeof(int) * pNumBins_[ix]);
   }

   double** ppcdfa = CalculateCdf(ppHistA);
   double** ppcdfb = CalculateCdf(ppHistB);

   for (int bandix = 0; bandix < bandCount_; ++bandix) {
      for (int binix = 0; binix < pNumBins_[bandix]; ++binix) {
         int jx = pNumBins_[bandix] - 1;
         do {
            lut[bandix][binix] = jx;
            --jx;
         } while (jx >= 0 && ppcdfa[bandix][binix] <= ppcdfb[bandix][jx]);
      }
   }

   for (int bandix = 0; bandix < bandCount_; ++bandix) {
      delete[] ppcdfa[bandix];
      delete[] ppcdfb[bandix];
   }

   delete[] ppcdfa;
   delete[] ppcdfb;

   return lut;
}

/**
 * Crea una instancia de EqualizationEnhancement.
 */
Enhancement* HistogramMatchingEnhancement::Create(suri::ParameterCollection& Params) {
   int bandcount = 0;
   Params.GetValue<int>("BandCount", bandcount);

   int* pnumbins = NULL;
   Params.GetValue<int*>("NumBins", pnumbins);

   return new HistogramMatchingEnhancement(bandcount, pnumbins);
}

/**
 * Calcula la funcion de distribucion acumulada para el histograma dado.
 */
double** HistogramMatchingEnhancement::CalculateCdf(int** ppHist) {
   // Cdf
   double** ppcdf = new double*[bandCount_];
   for (int bandix = 0; bandix < bandCount_; ++bandix) {
      ppcdf[bandix] = new double[pNumBins_[bandix]];
      memset(ppcdf[bandix], 0, sizeof(double) * pNumBins_[bandix]);
   }

   // Para guardar la suma de todos los valores del histograma (por banda)
   double* pHistAccum = new double[bandCount_];
   double* pHistAccumSum = new double[bandCount_];
   for (int bandix = 0; bandix < bandCount_; ++bandix) {
      pHistAccum[bandix] = 0.0;
      pHistAccumSum[bandix] = 0.0;
   }

   // Suma de todos los valores del histograma (por banda)
   for (int bandix = 0; bandix < bandCount_; ++bandix) {
      for (int binix = 0; binix < pNumBins_[bandix]; ++binix) {
         pHistAccumSum[bandix] += static_cast<double>(ppHist[bandix][binix]);
      }
   }

   // valores iniciales
   for (int bandix = 0; bandix < bandCount_; ++bandix) {
      pHistAccum[bandix] = static_cast<double>(ppHist[bandix][0]);
      ppcdf[bandix][0] = pHistAccum[bandix] / pHistAccumSum[bandix];
   }

   // Generacion de CDF
   for (int bandix = 0; bandix < bandCount_; ++bandix) {
      for (int binix = 0; binix < pNumBins_[bandix]; ++binix) {
         pHistAccum[bandix] += static_cast<double>(ppHist[bandix][binix]);
         ppcdf[bandix][binix] = pHistAccum[bandix] / pHistAccumSum[bandix];
      }
   }

   delete[] pHistAccum;
   pHistAccum = NULL;

   delete[] pHistAccumSum;
   pHistAccumSum = NULL;

   return ppcdf;
}

}  // namespace enhancement
}  // namespace raster
}  // namespace suri
