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

#include "Linear2PercentEnhancement.h"

/** Porcentaje de valores a cada lado de la distribucion que son excluidos para el calculo del */
// realce
#define EXCLUDED_VALUES_PERCENT 2

/** namespace suri */
namespace suri {
/** Registra la clase entre los tipos de realce */
AUTO_REGISTER_CLASS(Enhancement, Linear2PercentEnhancement, 0);

/** Constructor */
Linear2PercentEnhancement::Linear2PercentEnhancement() {
}

/** Destructor */
Linear2PercentEnhancement::~Linear2PercentEnhancement() {
}

/**
 *	Devuelve los parametros estadisticos relevantes para la funcion de realce
 *	@param[out] Parameters histograma, el maximo y el minimo, necesarios para
 *	el calculo de la lut.
 */
void Linear2PercentEnhancement::GetValidParameters(
      Statistics::StatisticsFlag &Parameters) const {
   Parameters = Statistics::Histogram;
}

/**
 * Obtiene el minimo y el maximo relevantes de una banda para la funcion de
 * realce, valores que excluyen el 2% de los valores a cada extremo del histograma.
 * @param[in] Stats estadisticas
 * @param[in] BandNumber numero de banda
 * @param[out] Min valor minimo, que deja fuera el 2% de los valores.
 * @param[out] Max valor maximo, que deja fuera el 2% de los valores.
 */
void Linear2PercentEnhancement::GetLimits(const Statistics &Stats, size_t BandNumber,
                                          double &Min, double &Max) const {
   Min = GetMinNPercent(Stats.histogram_[BandNumber], EXCLUDED_VALUES_PERCENT);
   Max = GetMaxNPercent(Stats.histogram_[BandNumber], EXCLUDED_VALUES_PERCENT);
}
}
