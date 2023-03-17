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

// Inclues standar
#include <algorithm>

// Includes suri
#include "LinearEnhancement.h"
#include "suri/AuxiliaryFunctions.h"

/** namespace suri */
namespace suri {

/** Registra la clase entre los tipos de realce */
AUTO_REGISTER_CLASS(Enhancement, LinearEnhancement, 0);

/**
 * Constructor
 */
LinearEnhancement::LinearEnhancement() {
}

/**
 * Destructor
 */
LinearEnhancement::~LinearEnhancement() {
}

/**
 * Crea una Lut utilizando para transformar los valores de entrada una funcion
 * lineal.
 * @param[in] Stats estadisticas de la banda
 * @param[in] OutputRange rango para los valores de salida
 * @param[in] BinCount numero de bins (segementos) entre los que se divide el
 *    rango de valores
 * @param[in] BandNumber numero de banda de la imagen
 * @return lut tabla de busqueda correspondiente al realce, o una lut vacia si
 *    fallo alguno de los parametros.
 */
LookUpTable LinearEnhancement::CreateLut(const Statistics &Stats,
                                         const std::pair<double, double> &OutputRange,
                                         int BinCount, size_t BandNumber) const {
   LookUpTable lut;
   double slope = 0, intercept = 0, segment = 0, min = 0, max = 0;
   GetLimits(Stats, BandNumber, min, max);
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

/**
 * Devuelve el parametro relevante para la funcion de realce, en este caso
 * el maximo y el minimo.
 * @param[out] Parameters minimo y maximo
 */
void LinearEnhancement::GetValidParameters(
      Statistics::StatisticsFlag &Parameters) const {
   Parameters = Statistics::MinValue | Statistics::MaxValue;
}

/**
 * Obtiene el maximo y el minimo valor de la banda.
 * @param[in] Stats estadisticas de las bandas
 * @param[in] BandNumber numero de banda de la cual se quieren el max y min
 * @param[out] Min minimo valor de la banda
 * @param[out] Max maximo valor de la banda
 */
void LinearEnhancement::GetLimits(const Statistics &Stats, size_t BandNumber,
                                  double &Min, double &Max) const {
   if (Stats.min_.size() < BandNumber || Stats.max_.size() < BandNumber) {
      return;
   }
   Min = Stats.min_[BandNumber];
   Max = Stats.max_[BandNumber];
}

/**
 * Obtiene la pendiente y la ordenada al origen de la recta.
 * @param[in] Min minimo valor
 * @param[in] Max maximo valor
 * @param[in] OutputRange rango de valores de salida
 * @param[out] Slope pendiente de la recta
 * @param[out] Intercept ordenada al origen de la recta
 */
void LinearEnhancement::CalculateFunctionParameters(
      double Min, double Max, const std::pair<double, double> &OutputRange,
      double &Slope,
      double &Intercept) const {
   if (Max != Min) {
      Slope = (OutputRange.second - OutputRange.first) / (Max - Min);
      Intercept = (OutputRange.first * Max - OutputRange.second * Min) / (Max - Min);
   }
   // Si el maximo y el minimo son iguales devuelve el valor del Min si esta dentro
   // del rango de salida, o el limite del rango que supero (superior o inferior)
   else {
      Slope = 0;
      Intercept =
            (Min < OutputRange.first) ? OutputRange.first :
                                        std::min(Min, OutputRange.second);
   }
}

}
