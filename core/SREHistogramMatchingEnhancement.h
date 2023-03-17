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

#ifndef LIB_SURI_CORE_SREHISTOGRAMMATCHINGENHANCEMENT_H_
#define LIB_SURI_CORE_SREHISTOGRAMMATCHINGENHANCEMENT_H_

#include "SREEnhancement.h"
#include "suri/ParameterCollection.h"

namespace suri {
namespace raster {
namespace enhancement {

/**
 * Clase que implementa un realce por combinacion de histogramas.
 */
class HistogramMatchingEnhancement : public Enhancement {
public:
   /**
    * Ctor.
    */
   HistogramMatchingEnhancement(int BandCount, int* pNumBins);

   /**
    * Dtor.
    */
   virtual ~HistogramMatchingEnhancement();

   /**
    * Crea una lookup table a partir de dos histogramas de intensidad.
    */
   virtual int** CreateLUT(int** ppHistA = NULL, int** ppHistB = NULL);

   /**
    * Crea una instancia de EqualizationEnhancement.
    */
   static Enhancement* Create(suri::ParameterCollection& Params);

private:
   /**
    * Calcula la funcion de distribucion acumulada para el histograma dado.
    */
   double** CalculateCdf(int** ppHist);
};

}  // namespace enhancement
}  // namespace raster
}  // namespace suri

#endif  // LIB_SURI_CORE_SREHISTOGRAMMATCHINGENHANCEMENT_H_
