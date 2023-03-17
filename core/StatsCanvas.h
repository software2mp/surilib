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

#ifndef STATSCANVAS_H_
#define STATSCANVAS_H_

/**
 *
 */

// Includes estandar
// Includes Suri
#include "suri/XmlFunctions.h"
#include "suri/StatisticsFunctions.h"
// Includes Wx

// Includes App

// Defines

#include "MemoryCanvas.h"

/** namespace suri */
namespace suri {
/** En flush utiliza los datos en imagen para calcular las estadisticas. */
/**
 * Primero se utiliza canvas para renderizar, luego al hacer flush se
 * utilizan los datos para calcular las estadisticas.
 * Por razones de performance se puede elegir las estadisticas que se calculan
 * utilizando SetRequiredStats.
 */
class StatsCanvas : public MemoryCanvas {
public:
   /** ctor */
   StatsCanvas();
   /** dtor */
   virtual ~StatsCanvas();

// ----------------------------- VECTORIAL -----------------------------
   /** Canvas vectorial */
   virtual wxDC *GetDC();
   /** Computa los parametros deseados */
   virtual void Flush(const Mask *pMask);
// --------------------------- ESTADISTICAS ---------------------------
   /** Devuelve las estadisticas. */
   Statistics GetStatistics();
#ifdef __UNUSED_CODE__
   /** Todos estos metodos se reemplazan por el metodo GetStatistics, que */
   /** que devuelve la variable que contiene a todos los valores estadisticos */
   /** Retorna la cuenta de muestras */
   size_t GetCount() const;
   /** Retorna los valores de las medias computadas para cada banda */
   std::vector<double> GetMean() const;
   /** Retorna los valores de las varianzas computadas para cada banda */
   std::vector<double> GetVariance() const;
   /** Retorna los valores de la matriz de covarianza */
   std::vector<std::vector<double> > GetCovarianceMatrix() const;
   /** Retorna los valores minimos para cada banda */
   std::vector<double> GetMin() const;
   /** Retorna los valores maximos para cada banda */
   std::vector<double> GetMax() const;
#endif
   /** Vuelve a estado inicial los parametros estadisticos */
   void ResetStats();
   /** Establece que estadisticas van a ser computadas */
   void SetRequiredStats(Statistics::StatisticsFlag RequiredStats);

   /** Establece si debe resetearse el estado interno de las estadisticas o no */
   void SetReset(bool Reset);

private:
   Statistics statistics_;  // Estadisticas
   Statistics::StatisticsFlag requiredStats_;  // Indica que estdadisticas van a ser calculadas
   bool reset_;  // Indica si debe resetearse el estado interno de las estadisticas o no
};
}

#endif /* STATSCANVAS_H_ */
