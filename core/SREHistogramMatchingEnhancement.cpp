/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
