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

// Includes estandar
#include <utility>

// Includes Suri
#include "suri/AuxiliaryFunctions.h"
#include "Linear255Enhancement.h"

// Includes Wx

// Includes App

// Defines

namespace suri {

/** Registra la clase entre los tipos de realce */
AUTO_REGISTER_CLASS(Enhancement, Linear255Enhancement, 0);

Linear255Enhancement::Linear255Enhancement() {
}

Linear255Enhancement::~Linear255Enhancement() {
}

/** Crea una lut para la banda indicada con los parametros recibios */
LookUpTable Linear255Enhancement::CreateLut(const Statistics &Stats,
                                            const std::pair<double, double> &OutputRange,
                                            int BinCount, size_t BandNumber) const {
   LookUpTable lut;
   double slope = 0, intercept = 0, segment = 0, min = 0.0, max = 255.0;
   // Obtiene la pendiente y la ordenada de la recta (funcion lineal de realce)
   CalculateFunctionParameters(min, max, OutputRange, slope, intercept);
   // Calcula el tamanio de cada bin
   segment = (max - min) / (BinCount - 1);
   // Si los limites son iguales asigna cero a la cantiad de bins para agregar
   // una sola entrada en la tabla.
   if (max == min) {
      BinCount = 0;
   }
   // El for es hasta <= la cantidad de bins, porque es necesario agregar
   // bins+1 entradas
   for (int i = 0; i < BinCount; i++)
      lut.AddBin(min + i * segment, SURI_ROUND(int, (min+i*segment)*slope+intercept));
   return lut;
}

/** Devuelve los parametros relevantes segun la funcion de realce */
void Linear255Enhancement::GetValidParameters(
      Statistics::StatisticsFlag &Parameters) const {
   Parameters = Statistics::MinValue | Statistics::MaxValue;
}

}
