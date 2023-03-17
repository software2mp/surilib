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
