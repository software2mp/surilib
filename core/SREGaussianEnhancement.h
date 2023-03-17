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

#ifndef SREGAUSSIANENHANCEMENT_H_
#define SREGAUSSIANENHANCEMENT_H_

#include <map>

#include "SREEnhancement.h"
#include "suri/ParameterCollection.h"

namespace suri {
namespace raster {
namespace enhancement {

class GaussianEnhancement : public Enhancement {
public:
   typedef std::pair<double, double> range;

   /**
    * Ctor.
    */
   GaussianEnhancement(int BandCount, int* pNumBins, double* pMin, double* pMax,
                       double** ppBAFP, double* pMean, double* pStdev);

   /**
    * Dtor.
    */
   virtual ~GaussianEnhancement();

   /**
    * Crea una lookup table a partir de un histograma de intensidad.
    */
   virtual int** CreateLUT(int** ppHistA = NULL, int** ppHistB = NULL);

   /**
    * Crea una instancia de GaussianEnhancement.
    */
   static Enhancement* Create(suri::ParameterCollection& Params);

private:
   double* pMin_;
   double* pMax_;
   double* pMean_;
   double* pStdev_;
   double** ppBAFP_;  // Porcentaje de frecuencia acumulada por bin.
   std::map<range, int> gaussRangeTable_;

   /**
    * Metodo auxiliar que llena la tabla de rangos para la dist normal.
    */
   void LoadRangeTable();

   /**
    * Obtiene el valor de intensidad correspondiente al rango dado.
    */
   int GetValueFromMap(const std::pair<double, double>& Pair);
};

}  // namespace enhancement
}  // namespace raster
}  // namespace suri

#endif /* SREGAUSSIANENHANCEMENT_H_ */
