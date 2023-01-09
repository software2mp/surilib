/*! \file Histogram.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
