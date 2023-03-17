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
