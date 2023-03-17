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

// Includes standard
#include <vector>

// Includes suri
#include "MathFunctions.h"
#include "suri/AuxiliaryFunctions.h"

namespace suri {

/**
 * Calcula matriz LU a partir de matriz de correlacion.
 * No tengo que modificar los pivotes debido a que la matriz
 * es de correlacion y tiene los maximos de cada fila en la diagonal.
 * @param[in] Matrix matriz de correlacion
 * @param[out] Lu matriz LU
 */
void CalculateLu(const std::vector<std::vector<double> > Matrix,
                                    std::vector<std::vector<double> > &Lu) {
   int matrixsize = Matrix.size();
   for (int i = 0; i < matrixsize; i++) {
      Lu.push_back(std::vector<double>());
      for (int j = 0; j < matrixsize; j++)
         Lu[i].push_back(Matrix[i][j]);
   }

   // Busco el valor mas chico en la diagonal
   double minlu = Lu[0][0];
   for (int i = 1; i < matrixsize; i++)
      if (minlu > Lu[i][i]) {
         minlu = Lu[i][i];
      }
   if (FLOAT_COMPARE(minlu, 0.0)) {
      minlu = 0.01;
   }
   minlu *= 0.01;

   // Recorro las columnas.
   for (int i = 0; i < matrixsize; i++) {
      // Si el valor de la diagonal es 0 lo remplazo con epsilon
      if (FLOAT_COMPARE(Lu[i][i], 0.0)) {
         Lu[i][i] = minlu;
      }

      // Recorro las filas.
      for (int j = i + 1; j < matrixsize; j++) {
         // Calculo pivote
         Lu[j][i] = Lu[j][i] / Lu[i][i];
         // Para la fila recalculo los valores
         for (int k = i + 1; k < matrixsize; k++)
            Lu[j][k] = Lu[j][k] - Lu[i][k] * Lu[j][i];
      }
   }
}

}  // namespace suri
