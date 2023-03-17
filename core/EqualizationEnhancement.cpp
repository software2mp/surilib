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

#include "EqualizationEnhancement.h"

// Includes suri
#include "suri/AuxiliaryFunctions.h"

/** namespace suri */
namespace suri {
/** Registra la clase entre los tipos de realce */
AUTO_REGISTER_CLASS(Enhancement, EqualizationEnhancement, 0)

/**
 * Constructor
 */
EqualizationEnhancement::EqualizationEnhancement() {
}

/**
 * Destructor
 */
EqualizationEnhancement::~EqualizationEnhancement() {
}

/**
 * Crea una lut aplicando la formula de ecualizacion de histograma.
 *	@param[in] Stats estadisticas de la imagen
 *	@param[in] OutputRange rango de valores de salida
 *	@param[in] BinCount cantidad de intervalos
 *	@param[in] BandNumber numero de banda sobre la que se calcula el realce
 *	@return lut tabla lut resultante.
 */
LookUpTable EqualizationEnhancement::CreateLut(
      const Statistics &Stats, const std::pair<double, double> &OutputRange,
      int BinCount,
      size_t BandNumber) const {
   LookUpTable lut;
   double min = 0, max = 0, segment = 0, index = 0, outputvalue = 0;
   std::map<double, double> normalizedhistogram;
   // Obtiene el histograma normalizado acumulativo
   CalculateFunctionParameters(Stats.histogram_[BandNumber], normalizedhistogram);
   // Obtiene el maximo y minimo del histograma
   min = Stats.histogram_[BandNumber].GetMinimum();
   max = Stats.histogram_[BandNumber].GetMaximum();
   // Calcula el tamanio de cada bin en la entrada
   segment = (max - min) / (BinCount - 1);
   double outputrange = (OutputRange.second - OutputRange.first);

   if (min != max) {
      // Crea la lut
      for (long i = 0; i < BinCount; i++) {
         // index va de min a max, verifica rango para que no este fuera del histograma
         index = std::min(i * segment + min, max);

         std::map<double, double>::iterator it = normalizedhistogram.lower_bound(index);
         if (it == normalizedhistogram.end())
            it--;
         outputvalue = outputrange * it->second + OutputRange.first;
         // verifica el rango de la salida
         outputvalue = std::max(outputvalue, OutputRange.first);
         outputvalue = std::min(outputvalue, OutputRange.second);
         lut.AddBin(index, SURI_ROUND(int, outputvalue));
      }
   }
   return lut;
}

/**
 * Devuelve los parametros relevantes para la fuancion de realce ecualizacion
 * de histograma
 * @param[out] Parameters parametros relevantes: maximo, minimo e histograma.
 */
void EqualizationEnhancement::GetValidParameters(
      Statistics::StatisticsFlag &Parameters) const {
   Parameters = Statistics::MaxValue | Statistics::MinValue | Statistics::Histogram;
}

/**
 *	Pasa el histograma original a un histograma normalizado acumulativo (divide
 *	cada frecuencia por la cantidad de elementos).
 *	@param[in] Histogram histograma de la imagen
 *	@param[out] NormalizedHistogram histograma normalizado acumulativo.
 */
void EqualizationEnhancement::CalculateFunctionParameters(
      const Histogram &Histogram, std::map<double, double> &NormalizedHistogram) const {
   Histogram::HistogramType histogram;
   // Obtiene una copia del histograma original
   Histogram.GetHistogram(histogram);
   Histogram::HistogramType::iterator histogramiterator = histogram.begin();
   Histogram::HistogramType::iterator iterend = histogram.end();

   double accumvalue = 0.0;

   while (histogramiterator != iterend) {
      // Acumula la frecuencia realtiva
      accumvalue += Histogram.GetNormalizedFrecuency(histogramiterator->first);

      if (accumvalue > 1.0) {
         accumvalue = 1.0;
      }
      // Inserta los valores en el histograma normalizado
      NormalizedHistogram.insert(std::make_pair(histogramiterator->first, accumvalue));
      histogramiterator++;
   }
}
}
