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
