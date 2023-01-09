/*! \file Histogram.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/Histogram.h"

// Includes standar
#include <vector>
#include <limits>
#include <cmath>


#include "suri/XmlFunctions.h"

#define AVOID_ROUNDERROR_OFFSETPERCENTAGE 0.1

/** namespace suri */
namespace suri {
/*
 * Constructor.
 * @param[in] Minimum minimo valor del histograma
 * @param[in] Maximum maximo valor del histograma
 * @param[in] BinCount cantidad de intervalos del histograma
 */
Histogram::Histogram(double Minimum, double Maximum, long BinCount) {
   SetParameters(Minimum, Maximum, BinCount);
}

/**
 * Destructor
 */
Histogram::~Histogram() {
}

/**
 * Reasigna el tamanio del histograma a los nuevos parametros.
 *	@param[in] Minimum limite minimo del histograma
 *	@param[in] Maximum limite maximo del histograma
 *	@param[in] BinCount intervalos en los que se divide el histograma
 */
void Histogram::SetParameters(double Minimum, double Maximum, long BinCount) {
   count_ = 0;
   histogram_.clear();

   if (BinCount == 0 || Maximum <= Minimum) {
      return;
   }

   minValue_ = std::numeric_limits<double>::max();
   maxValue_ = -std::numeric_limits<double>::max();

   minIntervalValue_ = Minimum;
   maxIntervalValue_ = Maximum;
   binCount_ = BinCount;

   if (BinCount > 1) {
      binSize_ = (Maximum - Minimum) / BinCount;
   } else {
      binSize_ = 0;
   }
}

/**
 * Devuelve el comienzo del primer bin con frecuencia mayor que 0
 *	@return limite inferior del histograma
 *	@return 0 si el histograma esta vacio (sin intervalos)
 */
double Histogram::GetMinimum() const {
   return !histogram_.empty() ? minValue_ : 0.0;
}

/**
 *	Devuelve el final del ultimo bin con frecuencia mayor que 0
 *	@return limite superior del histograma.
 *	@return 0 si el histograma esta vacio (sin intervalos)
 *	\attention como los intervalos incluyen al limite inferior y no al superior,
 *	el valor maximo del histograma es un valor que puede no pertenecer al
 *	histograma.
 */
double Histogram::GetMaximum() const {
   return !histogram_.empty() ? maxValue_ : 0.0;
}

/**
 *	Devuelve la cantidad de valores que tiene el histograma.
 *	@return cantidad de intervalos del histograma.
 */
long Histogram::GetBinCount() const {
   return binCount_;
}

/**
 * Devuelve el tamanio entre los intervalos, o cero si no hay intervalos.
 * @return tamanio de un intervalo (diferencia entre limite superior y inferior)
 */
double Histogram::GetBinSize() const {
   return binSize_;
}

/**
 * Incrementa la frecuencia del intervalo al cual corresponde el valor
 * recibido. Si el valor recibido es menor al limite minimo se asigna al primer
 * intervalo, y si es mayor o igual al limite maximo se asigna al ultimo.
 * @param[in] Value determina en que intervalo hay que incrementar la frecuencia
 * @param[in] Count cantdiad en la que se incrementa la frecuencia del bin
 * @return false si el histograma no tiene intervalos
 * @return true en cualquier otro caso
 */
bool Histogram::IncreaseValue(double Value, double Count) {
   if (SURI_ISNAN(Value)) return false;

   HistogramType::iterator it;
   if (Value < minIntervalValue_) {
      if (GetOrCreateBin(minIntervalValue_, it)) it->second += Count;
   } else if (Value > maxIntervalValue_) {
      if (GetOrCreateBin(maxIntervalValue_, it)) it->second += Count;
   } else if (GetOrCreateBin(Value, it)) {
      it->second += Count;
   }

   count_ += Count;
   return true;
}

/**
 * Devuelve la suma de las frecuencias absolutas de todos los intervalos
 * @return count suma de frecuencias de los intervalos
 */
int Histogram::GetCount() const {
   return count_;
}

/**
 *	Recibe un valor y devuelve la frecuencia absoluta del intervalo al que pertenece.
 *	Si el valor recibido es menor al limite minimo, o mayor o igual al limite
 *	maximo, la frecuencia devuelta es cero.
 *	@param[in] Value valor del cual se quiere conocer la frecuencia del intervalo
 *	al que pertenece.
 *	@return frequency frecuencia del intervalo
 */
int Histogram::GetFrequency(double Value) const {
   int frequency = 0;

   if (!histogram_.empty() && Value >= GetMinimum() && Value <= GetMaximum()) {
      HistogramType::const_iterator it;
      if (GetBin(Value, it)) frequency = it->second;
   }

   return frequency;
}

/**
 *	Recibe un valor y devuelve la frecuencia relativa del intervalo al que
 *	pertenece.
 *	@param[in] Value valor del cual se quiere conocer la frecuencia del intervalo
 * al que pertenece
 * @return frequency frecuencia relativa del intervalo
 */
double Histogram::GetNormalizedFrecuency(double Value) const {
   return (GetCount() > 0) ? static_cast<double>(GetFrequency(Value)) / GetCount() : 0.0;
}

/**
 * Obtiene una copia del histograma interno.
 * @param[out] Histogram histograma copia del interno
 */
void Histogram::GetHistogram(HistogramType &Histogram) const {
   Histogram = histogram_;
}

/**
 * Asigna un histograma a la propiedad interna.
 * @param[in] Histogram histograma.
 */
void Histogram::SetHistogram(const HistogramType &Histogram) {
   histogram_ = Histogram;
}

/**
 * Convierte un histograma en un nodo xml.
 * @param[in] Parameters histograma
 * @param[out] pHistogramNode nodo xml con una tabla que representa el histograma
 * @return true si pudo generar el nodo
 * @return false si no pudo generar el nodo
 */
bool Histogram::ToXml(const Histogram &Parameters, wxXmlNode* &pHistogramNode) {
   std::vector<std::vector<double> > histogramtable(1);

   // Recorro el histograma y genero vector con frecuencias
   double binsize = Parameters.binSize_;
   double lastbin = Parameters.GetMaximum();
   double currentbin = Parameters.GetMinimum() +
   binsize * AVOID_ROUNDERROR_OFFSETPERCENTAGE;
   size_t bincount = 0;
   while (currentbin < (lastbin + binsize)) {
      histogramtable[0].push_back(Parameters.GetFrequency(currentbin));
      currentbin += binsize;
      bincount++;
   }

   // Uso vector con frecuencias para generar nodo xml.
   wxXmlNode *phistogramnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                             wxT(HISTOGRAM_NODE));
   wxXmlNode *ptablenode = NULL;
   CreateTable(ptablenode, histogramtable);
   phistogramnode->AddChild(ptablenode);
   wxString minimum, maximum;
   minimum.sprintf("%f", Parameters.GetMinimum());
   maximum.sprintf("%f", Parameters.GetMaximum());
   phistogramnode->AddProperty(wxT(MINIMUM_PROPERTY), minimum);
   phistogramnode->AddProperty(wxT(MAXIMUM_PROPERTY), maximum);

   int rows = 0, columns = 0;
   GetDimension(ptablenode, rows, columns);
   // una fila (frecuencia) y N columnas (bins)
   if (rows == 1 && static_cast<size_t>(columns) == bincount) {
      pHistogramNode = phistogramnode;
   } else {
      delete phistogramnode;
   }
   // si son iguales distintos de NULL tuvo exito
   return pHistogramNode == phistogramnode && phistogramnode;
}

/**
 * Obtiene los parametros del histograma de un nodo xml. Pasa los valores del nodo a una tabla de 1 fila
 * y cantidad de columnas igual a cantidad de intervalos. Asigna estos valores a la variable histograma
 * (minimo, maximo y cantidad de intervalos), y asigna cada frecuencia de la tabla en el intervalo que
 * corresponde.
 * @param[in] pHistogramNode nodo xml con el histograma
 * @param[out] Histogram variable de tipo histograma con los valores
 * obtenidos del xml.
 * @return bool que indica si pudo configurar Histogram correcatamente
 */
bool Histogram::FromXml(const wxXmlNode *pHistogramNode, Histogram &Histogram) {
   if (!pHistogramNode || pHistogramNode->GetName() != wxT(HISTOGRAM_NODE)
         || pHistogramNode->GetChildren() == NULL) {
      return false;
   }
   wxXmlNode* ptablenode = pHistogramNode->GetChildren();

   std::vector<std::vector<double> > histogramtable;
   int rows = 0, columns = 0;
   GetDimension(ptablenode, rows, columns);
   // 1 fila (frecuencias) y al menos 1 columna (bin)
   if (rows != 1 && columns < 1) {
      return false;
   }
   LoadTable(ptablenode, histogramtable);

   // Obtengo los parametros de las propiedades del nodo
   wxString binpropertyvalue, minpropertyvalue, maxpropertyvalue;
   if (!pHistogramNode->GetPropVal(wxT(MINIMUM_PROPERTY), &minpropertyvalue)
         || !pHistogramNode->GetPropVal(wxT(MAXIMUM_PROPERTY), &maxpropertyvalue)) {
      return false;
   }

   double minimum = 0, maximum = 0;
   minpropertyvalue.ToDouble(&minimum);
   maxpropertyvalue.ToDouble(&maximum);
   // Si la cantidad de bines es cero, o el maximo es menor al minimo es un nodo invalido
   if (maximum < minimum) {
      return false;
   }

   double segment = (maximum - minimum) / (columns - 1);
   // Asigno el tamanio para el nuevo histograma
   Histogram.SetParameters(minimum, maximum, static_cast<int>(columns));

   // Carga el histograma
   for (int i = 0; i < columns; i++)
      Histogram.IncreaseValue(minimum + i * segment, histogramtable.at(0).at(i));

   return true;
}

/**
 * Retorna el iterador que apunta al bin donde deberia estar Value. Si no
 * lo encuentra retorna false.
 * @param[in] Value Valor que quiero buscar en histograma
 * @param[out] Iterator Iterador de histogram_ que apunta a bin correspondiente
 * @return true si Iterador esta configurado.
 * @return false si no encontro el bin o Value esta fuera de rango
 */
bool Histogram::GetBin(double Value, HistogramType::const_iterator &Iterator) const {
   if (Value < minIntervalValue_ || Value >= maxIntervalValue_ + binSize_) return false;

   HistogramType::const_iterator it = histogram_.upper_bound(Value);

   // Si no existe el bin retorno false
   if (it == histogram_.begin() || (Value - (--it)->first) > binSize_) return false;

   Iterator = it;
   return true;
}

/**
 * Retorna el iterador que apunta al bin que contiene a Value.
 * Si encuentra el bin en el mapa lo retorna, si no lo encuentra lo crea.
 * @param[in] Value Valor que quiero buscar en histograma
 * @param[out] Iterator Iterador de histogram_ que apunta a bin correspondiente
 * @return bool que indica si Iterador esta configurado. Falla Value esta
 * fuera del rango configurado para el histograma.
 */
bool Histogram::GetOrCreateBin(double Value, HistogramType::iterator &Iterator) {
   if (Value < minIntervalValue_ || Value >= maxIntervalValue_ + binSize_) return false;

   HistogramType::iterator it = histogram_.upper_bound(Value);

   // Si no existe el bin lo genero
   if (it == histogram_.begin() || (Value - (--it)->first) > binSize_) {
      double binvalue = minIntervalValue_
            + static_cast<long>(floor((Value - minIntervalValue_) / binSize_))
                  * binSize_;

      std::pair<HistogramType::iterator, bool> result = histogram_.insert(
            std::pair<double, unsigned long>(binvalue, 0));

      if (!result.second) return false;

      if (binvalue < minValue_)
         minValue_ = std::max(binvalue, minIntervalValue_);

      if (binvalue + binSize_ > maxValue_)
         maxValue_ = std::min(binvalue + binSize_, maxIntervalValue_);

      it = result.first;
   }

   Iterator = it;
   return true;
}

}
