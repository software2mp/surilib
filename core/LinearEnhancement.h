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

#ifndef LINEARENHANCEMENT_H_
#define LINEARENHANCEMENT_H_

// Include suri
#include "Enhancement.h"

/** namespace suri */
namespace suri {
/** Clase que crea un realce (Lut) de tipo lineal */
/**
 * Clase que crea Luts a partir de parametros estadisticos utilizando una funcion
 * de realce lineal. Esta funcion es una recta que pasa por los puntos formados
 * por (minimo valor de la banda, minimo valor del rango de salida) y (maximo
 * valor de la banda, maximo valor del rango de salida).
 */
class LinearEnhancement : public Enhancement {
public:
   /** Ctor. */
   LinearEnhancement();
   /** Dtor. */
   virtual ~LinearEnhancement();
   /** Crea una Lut a partir de los parametros recibidos */
   virtual LookUpTable CreateLut(const Statistics &Stats,
                                 const std::pair<double, double> &OutputRange,
                                 int BinCount,
                                 size_t BandNumber) const;
   /** Devuelve los parametros estadisticos relevantes para la funcion de realce */
   virtual void GetValidParameters(Statistics::StatisticsFlag &Parameters) const;
protected:
   /** Obtiene el minimo y el maximo relevantes de una banda para la funcion de realce */
   virtual void GetLimits(const Statistics &Stats, size_t BandNumber, double &Min,
                          double &Max) const;
   /** Calcula la pendiente y la ordenada al origen de la recta */
   void CalculateFunctionParameters(double Min, double Max,
                                    const std::pair<double, double> &OutputRange,
                                    double &Slope,
                                    double &Intercept) const;
};
}

#endif /* LINEARENHANCEMENT_H_ */
