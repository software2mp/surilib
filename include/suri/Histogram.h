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

#ifndef HISTOGRAM_H_
#define HISTOGRAM_H_

// Includes standar
#include <map>

// Includes Wx
#include "wx/xml/xml.h"

#define HISTOGRAM_8BIT_MIN          0.0
#define HISTOGRAM_8BIT_MAX          255.0
#define HISTOGRAM_8BIT_BIN_COUNT    256

#define HISTOGRAM_MIN         HISTOGRAM_8BIT_MIN
#define HISTOGRAM_MAX         HISTOGRAM_8BIT_MAX
#define HISTOGRAM_BIN_COUNT   HISTOGRAM_8BIT_BIN_COUNT

/** namespace suri */
namespace suri {
/** Clase que representa un histograma (intervalos y frecuencias) y sus operaciones. */
/**
 * Representa el histograma de valores.
 * Los intervalos del histograma (bins) incluyen al limite inferior y no
 * al superior. La frecuencia para un valor mayor o igual al valor devuelto
 * por GetMaximum retorna cero. La frecuencia para un valor menor a GetMinimum
 * retorna cero.
 */
class Histogram {
public:
   /** Tipo de dato histograma, double para el limite del intervalo y ulong para la frecuencia */
   typedef std::map<double, unsigned long> HistogramType;
   /** Ctor */
   Histogram(double Minimum = HISTOGRAM_MIN, double Maximum = HISTOGRAM_MAX,
             long BinCount = HISTOGRAM_BIN_COUNT);
   /** Dtor */
   virtual ~Histogram();
   /** Crea un histograma con la cantidad Bincount de intervalos entre Min y Max */
   void SetParameters(double Minimum, double Maximum, long BinCount);
   /** Devuelve el comienzo del primer bin con frecuencia mayor que 0 */
   double GetMinimum() const;
   /** Devuelve el final del ultimo bin con frecuencia mayor que 0 */
   double GetMaximum() const;
   /** Devuelve la cantidad de intervalos del histograma */
   long GetBinCount() const;
   /** Devuelve el tamanio del intervalo */
   double GetBinSize() const;
   /** Incrementa la frecuencia del intervalo al que corresponde el valor */
   bool IncreaseValue(double Value, double Count = 1);
   /** Devuelve la suma de las frecuencias de todos los intervalos */
   int GetCount() const;
   /** Dado un valor devuelve la frecuencia absoluta del intervalo al que pertenece */
   int GetFrequency(double Value) const;
   /** Dado un valor, devuelve la frecuencia relativa del intervalo al que pertenece */
   double GetNormalizedFrecuency(double Value) const;
   /** Obtiene una copia del histograma */
   void GetHistogram(HistogramType &Histogram) const;
   /** Asigna un histograma */
   void SetHistogram(const HistogramType &Histogram);
   /** Convierte un histograma a un nodo xml */
   static bool ToXml(const Histogram &Parameters, wxXmlNode* &pNode);
   /** Obtiene los parametros del histograma de un nodo xml */
   static bool FromXml(const wxXmlNode *pHistogramNode, Histogram &Parameters);

private:
   /** Retorna el bin que contiene a Value. */
   bool GetBin(double Value, HistogramType::const_iterator &Iterator) const;
   /** Retorna/Crea el bin que contiene a Value. */
   bool GetOrCreateBin(double Value, HistogramType::iterator &Iterator);

   HistogramType histogram_; /*! Representacion del histograma */
   double minValue_; /*! Valor minimo con frecuencia >0 del histograma */
   double maxValue_; /*! Valor maximo con frecuencia >0 del histograma */
   double binSize_; /*! Tamano del bin */
   long int binCount_; /*! Cantidad de bines solicitados */
   double minIntervalValue_; /*! Limite inferior del intervalo de trabajo */
   double maxIntervalValue_; /*! Limite superior del intervalo de trabajo */
   long int count_; /*! Cantidad de muestras. */
};
}

#endif /* HISTOGRAM_H_ */
