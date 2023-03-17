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

#ifndef EQUALIZATIONENHANCEMENT_H_
#define EQUALIZATIONENHANCEMENT_H_

// Includes suri
#include "Enhancement.h"

/** namespace suri */
namespace suri {
/** Clase para generar un realce (Lut) de Ecualizacion de Histograma */
/**
 * Clase que crea una Lut que aplica un realce de tipo Ecualizacion de Histograma.
 * Devuelve cuales son los parametros estadisticos necesarios para este realce
 * (Min, Max e Histograma). Calcula los parametros necesarios para aplicar la
 * funcion que crea los valores para el realce.
 */
class EqualizationEnhancement : public Enhancement {
public:
   /** Constructor */
   EqualizationEnhancement();
   /** Dtor. */
   virtual ~EqualizationEnhancement();
private:
   /** Crea una Lut a partir de los parametros recibidos */
   virtual LookUpTable CreateLut(const Statistics &Stats,
                                 const std::pair<double, double> &OutputRange,
                                 int BinCount,
                                 size_t BandNumber) const;
   /** Devuelve los parametros estadisticos relevantes para la funcion de realce */
   virtual void GetValidParameters(Statistics::StatisticsFlag &Parameters) const;
   /** Obtiene el histograma normalizado acumulado */
   void CalculateFunctionParameters(
         const Histogram &Histogram,
         std::map<double, double> &NormalizedHistogram) const;
};
}

#endif /* EQUALIZATIONENHANCEMENT_H_ */
