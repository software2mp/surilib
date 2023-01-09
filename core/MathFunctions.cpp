/** \file MathFunctions.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
