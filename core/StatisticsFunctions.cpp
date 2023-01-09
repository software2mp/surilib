/*! \file StatisticsFunctions.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <algorithm>

// Includes Suri
#include "suri/xmlnames.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/XmlFunctions.h"
#include "suri/StatisticsFunctions.h"
#include "SRStatisticsCanvas.h"


/** Por ahora se elimina esta restriccion aunque el histograma todavia no este */
/** incluido en el nodo xml. */
#define __INCLUDE_HISTOGRAM_IN_STATISTICS_XML__
#define __HISTOGRAM_READ_FROM_NODE_OK__
/** valor de retorno del metodo GetDimension cuando las estadisticas son invalidas. */
#define INVALID_STATS_DIMENSION -1
/** valor de retorno del metodo GetDimension cuando las cuentas son cero (estadisticas vacias) */
#define EMPTY_STATS_DIMENSION 0

// Includes Wx
#include "wx/sstream.h"

// Includes App
// Defines

/** namespace suri */
namespace suri {
/* ++++++++++++++++++++ Funciones para el nodo estadistica ++++++++++++++++++ */

/**
 * Compara las dimensiones de las variables que corresponden con el flag recibido.
 * @param[in] ValidStatistics mascara para determinar sobre que variables
 * estadisticas se realiza la comprobacion de las dimensiones.
 * @return statsdimension cantidad de valores de las variables relevantes, retorna
 * INVALID_STATS_DIMENSION si la variable requerida esta vacia (significa que no
 * fue calculada) o si las dimensiones difieren entre variables requeridas
 * (cantidad de medias no es igual a la cantidad de varianzas).
 * @return EMPTY_STATS_DIMENSION si la cantidad de muestras es cero.
 */
int Statistics::GetDimension(StatisticsFlag ValidStatistics) const {
   std::vector<int> dimensions;
   // valor error por defecto
   int statsdimension = EMPTY_STATS_DIMENSION;

   // Si la cantidad es igual a cero devuelve valor de estadisticas vacias
   if (count_ == 0) {
      return statsdimension;
   }

   if (ValidStatistics & Mean) {
      dimensions.push_back(mean_.size());
   }

   if (ValidStatistics & Variance) {
      dimensions.push_back(variance_.size());
   }

   if (ValidStatistics & CovarianceMatrix) {
      dimensions.push_back(covarianceMatrix_.size());
      if (!covarianceMatrix_.empty()) {
         dimensions.push_back(covarianceMatrix_.at(0).size());
      }
   }

   if (ValidStatistics & MaxValue) {
      dimensions.push_back(max_.size());
   }

   if (ValidStatistics & MinValue) {
      dimensions.push_back(min_.size());
   }

#ifdef __HISTOGRAM_READ_FROM_NODE_OK__
   // No se valida el histograma hasta que se pueda leer del nodo
   if (ValidStatistics & Histogram) {
      dimensions.push_back(histogram_.size());
   }
#endif
   // Si el vector de dimensiones esta vacio o la dimensions es cero,
   // la dimension estadistica es -1 (indica que la estadistica no fue calculada)
   if (dimensions.empty() || dimensions.at(0) == 0) {
      statsdimension = INVALID_STATS_DIMENSION;
   }
   // Si el minimo valor del vector es igual al maximo, todos los elementos son
   // iguales.
   else if (*max_element(dimensions.begin(), dimensions.end())
         == *min_element(dimensions.begin(), dimensions.end())) {
      statsdimension = dimensions.at(0);
   }

   return statsdimension;
}

/**
 * Crea un nodo XML de estadisticas a partir de los parametros recibidos
 * @param[in] Stats estructura con las variables estadisticas
 * @return pstatsnode nodo estadisticas
 */
wxXmlNode* CreateStatsNode(const Statistics &Stats) {
   wxXmlNode *pstatsnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
   wxT(STATISTICS_NODE));
   // nodo de cuentas
   wxString content = wxString::Format(wxT(PERCENT_U), Stats.count_);
   new wxXmlNode(new wxXmlNode(pstatsnode, wxXML_ELEMENT_NODE, wxT(SAMPLES_NODE)),
                 wxXML_TEXT_NODE, wxEmptyString, content);

   // nodo de medias
   content = join(Stats.mean_.begin(), Stats.mean_.end()).c_str();
   new wxXmlNode(new wxXmlNode(pstatsnode, wxXML_ELEMENT_NODE, wxT(MEAN_NODE)),
                 wxXML_TEXT_NODE, wxEmptyString, content);

   // nodo de varianzas
   content = join(Stats.variance_.begin(), Stats.variance_.end()).c_str();
   new wxXmlNode(new wxXmlNode(pstatsnode, wxXML_ELEMENT_NODE, wxT(VARIANCE_NODE)),
                 wxXML_TEXT_NODE, wxEmptyString, content);
   // nodo de minimos
   content = join(Stats.min_.begin(), Stats.min_.end()).c_str();
   new wxXmlNode(new wxXmlNode(pstatsnode, wxXML_ELEMENT_NODE, wxT(MIN_NODE)),
                 wxXML_TEXT_NODE, wxEmptyString, content);
   // nodo de maximos
   content = join(Stats.max_.begin(), Stats.max_.end()).c_str();
   new wxXmlNode(new wxXmlNode(pstatsnode, wxXML_ELEMENT_NODE, wxT(MAX_NODE)),
                 wxXML_TEXT_NODE, wxEmptyString, content);
   // nodo de matriz de covarianza
   wxXmlNode *pcovariancenode = new wxXmlNode(pstatsnode, wxXML_ELEMENT_NODE,
   wxT(COVARIANCE_MATRIX_NODE));
   CreateTable(pcovariancenode, wxT(COVARIANCE_MATRIX_NODE), Stats.covarianceMatrix_);
   // nodo de matriz de correlacion
   wxXmlNode *pcorrelationnode = new wxXmlNode(pstatsnode, wxXML_ELEMENT_NODE,
   wxT(CORRELATION_MATRIX_NODE));
   CreateTable(pcorrelationnode, wxT(CORRELATION_MATRIX_NODE),
               Stats.correlationMatrix_);

#ifdef __INCLUDE_HISTOGRAM_IN_STATISTICS_XML__
   // nodo de histograma
   wxXmlNode *pprev = pcovariancenode;
   for (size_t i = 0; i < Stats.histogram_.size(); i++) {
      wxXmlNode *phistogramnode = NULL;
      Histogram::ToXml(Stats.histogram_.at(i), phistogramnode);
      if (phistogramnode) {
         pstatsnode->InsertChildAfter(phistogramnode, pprev);
         pprev = phistogramnode;
      }
   }
#endif   // __INCLUDE_HISTOGRAM_IN_STATISTICS_XML__
   return pstatsnode;
}

/** Convierte la estructura a un nodo XML */
wxXmlNode* CreateStatsNode(suri::raster::data::StatisticsBase* pStats,
                           suri::raster::data::HistogramBase* pHistogram,
                           Statistics::StatisticsFlag Flags) {
   if (!pStats)
      return NULL;
   wxXmlNode *pstatsnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
   wxT(STATISTICS_NODE));
   pstatsnode->AddProperty(wxT(DATA_TYPE_NODE), pStats->GetDataName());
   pstatsnode->AddProperty(wxT(BANDS_NODE),
                           NumberToString<int>(pStats->GetBandCount()));
   // nodo de cuentas
   wxString content = wxString::Format(wxT(PERCENT_U),
                                       (size_t) pStats->GetPointCount(0));
   new wxXmlNode(new wxXmlNode(pstatsnode, wxXML_ELEMENT_NODE, wxT(SAMPLES_NODE)),
                 wxXML_TEXT_NODE, wxEmptyString, content);

  	/** Si la media es nula se entiende que no se mostraran los parametros basicos **/
	{
		// nodo de medias
   	std::vector<double> mean(pStats->GetBandCount());
   	for (int b = 0; b < pStats->GetBandCount(); ++b)
      	mean[b] = pStats->GetMean(b);
	   content = join(mean.begin(), mean.end()).c_str();
	   new wxXmlNode(new wxXmlNode(pstatsnode, wxXML_ELEMENT_NODE, wxT(MEAN_NODE)),
                 wxXML_TEXT_NODE, wxEmptyString, content);
	   // nodo de varianzas
	   std::vector<double> variance(pStats->GetBandCount());
	   for (int b = 0; b < pStats->GetBandCount(); ++b)
	      variance[b] = pStats->GetVariance(b);
	
	   content = join(variance.begin(), variance.end()).c_str();
	
   	new wxXmlNode(new wxXmlNode(pstatsnode, wxXML_ELEMENT_NODE, wxT(VARIANCE_NODE)),
	                 wxXML_TEXT_NODE, wxEmptyString, content);
	
	   // nodo de minimos
	   std::vector<double> min(pStats->GetBandCount());
	   for (int b = 0; b < pStats->GetBandCount(); ++b)
	      min[b] = pStats->GetMin(b);

	   content = join(min.begin(), min.end()).c_str();
	   new wxXmlNode(new wxXmlNode(pstatsnode, wxXML_ELEMENT_NODE, wxT(MIN_NODE)),
	                 wxXML_TEXT_NODE, wxEmptyString, content);

   	// nodo de maximos
	   std::vector<double> max(pStats->GetBandCount());
	   for (int b = 0; b < pStats->GetBandCount(); ++b)
   	   max[b] = pStats->GetMax(b);
	
	   content = join(max.begin(), max.end()).c_str();
	
   	new wxXmlNode(new wxXmlNode(pstatsnode, wxXML_ELEMENT_NODE, wxT(MAX_NODE)),
                 wxXML_TEXT_NODE, wxEmptyString, content);
	}

   // Nodo acumulador para covarianza.
   if (Flags & Statistics::CovarianceMatrix || Flags & Statistics::CorrelationMatrix) {
      wxXmlNode* paccum4covar = new wxXmlNode(pstatsnode, wxXML_ELEMENT_NODE,
                                              wxT(ACCUM_COVAR_NODE));
      CreateTable(paccum4covar, wxT(ACCUM_COVAR_NODE), pStats->GetAccum4Covar());

      // Node para establecer si se pide el calculo de autovectores y autovalores
      // y sobre que matriz (correlacion o covarianza)
      char eigctt[8] = { 0 };
      sprintf(eigctt, "%d", pStats->GetEig());
      new wxXmlNode(new wxXmlNode(pstatsnode, wxXML_ELEMENT_NODE, wxT(EIG_NODE)),
                    wxXML_TEXT_NODE, wxEmptyString, eigctt);
   }

   // nodo de histograma
   if (pHistogram) {
      std::vector<wxXmlNode *> histograms;
      pHistogram->ToXml(histograms);
      for (size_t i = 0; i < histograms.size(); ++i)
         pstatsnode->AddChild(histograms[i]);
   }
   return pstatsnode;
}
/**
 * Recibe un nodo estadistica y crea una variable de tipo statistics.
 * @param[in] pStatsNode nodo estadisticas
 * @param[out] Stats estadisticas obtenidas del nodo
 * @return stats variable con los valores estadisticos obtenidos del nodo
 */
bool GetStats(const wxXmlNode *pStatsNode, Statistics &Stats) {
   Statistics stats;
   bool retval = false;
   if (!pStatsNode || pStatsNode->GetName() != wxT(STATISTICS_NODE)) {
      return retval;
   }

   wxXmlNode *pchildnode = pStatsNode->GetChildren();

   while (pchildnode != NULL) {
      wxString nodename = pchildnode->GetName();
      // si hay algun nodo de los que reconoce, entonces tuvo exito
      if (nodename == wxT(SAMPLES_NODE) || nodename == wxT(MEAN_NODE)
            || nodename == wxT(VARIANCE_NODE) || nodename == wxT(MIN_NODE)
            || nodename == wxT(MAX_NODE) || nodename == wxT(COVARIANCE_MATRIX_NODE)
            || nodename == wxT(CORRELATION_MATRIX_NODE)) {
         retval = true;
      }
      std::string nodecontent = pchildnode->GetNodeContent().c_str();
      // Si es el nodo muestras asigna count_
      if (nodename == wxT(SAMPLES_NODE)) {
         stats.count_ = StringToNumber<int>(nodecontent);
      }

      // Si es el nodo media asigna mean
      if (nodename == wxT(MEAN_NODE)) {
         stats.mean_ = split<double>(nodecontent);
      }

      // Si es el nodo varianza asigna variance
      if (nodename == wxT(VARIANCE_NODE)) {
         stats.variance_ = split<double>(nodecontent);
      }

      // Si es el nodo minimo asigna min
      if (nodename == wxT(MIN_NODE)) {
         stats.min_ = split<double>(nodecontent);
      }

      // Si es el nodo maximo asigna max
      if (nodename == wxT(MAX_NODE)) {
         stats.max_ = split<double>(nodecontent);
      }

      // Si es el nodo covarianza carga la matriz de covarianzas
      if (nodename == wxT(COVARIANCE_MATRIX_NODE)) {
         LoadTable(pchildnode, stats.covarianceMatrix_);
      }

      // Si es el nodo correlacion carga la matriz de correlacion
      if (nodename == wxT(CORRELATION_MATRIX_NODE)) {
         LoadTable(pchildnode, stats.correlationMatrix_);
      }

#ifdef __INCLUDE_HISTOGRAM_IN_STATISTICS_XML__
      // Si es un nodo de histograma lo agrega al vector
      if (nodename == wxT(HISTOGRAM_NODE)) {
         Histogram histogram;
         if (Histogram::FromXml(pchildnode, histogram))
            stats.histogram_.push_back(histogram);
      }
#endif   // __INCLUDE_HISTOGRAM_IN_STATISTICS_XML__
      pchildnode = pchildnode->GetNext();
   }
   // si tuvo exito, actualiza los datos
   if (retval) {
      Stats = stats;
   }
   return retval;
}

/** Convierte un nodo a la estructura */
bool GetStats(const wxXmlNode *pStatsNode, suri::raster::data::StatisticsBase* &pStats,
              suri::raster::data::HistogramBase* &pHistogram) {
   if (!pStatsNode || pStatsNode->GetName() != wxT(STATISTICS_NODE)) {
      return false;
   }

   pHistogram = NULL;
   wxString datatype, bcount;

   pStatsNode->GetPropVal(wxT(DATA_TYPE_NODE), &datatype);
   pStatsNode->GetPropVal(wxT(BANDS_NODE), &bcount);
   pStats = suri::render::StatisticsCanvas::CreateStatisticsFromDataType(
         datatype.c_str(), StringToNumber<int>(bcount.c_str()));
   if (!pStats)
      return false;

   wxXmlNode *pchildnode = pStatsNode->GetChildren();
	bool basicstatsinclude = false;
   while (pchildnode != NULL) {
      wxString nodename = pchildnode->GetName();
      std::string nodecontent = pchildnode->GetNodeContent().c_str();
      // Si es el nodo muestras asigna count_
      if (nodename == wxT(SAMPLES_NODE)) {
         for (int b = 0; b < pStats->GetBandCount(); ++b) {
            pStats->pPointCount_[b] = StringToNumber<int>(nodecontent.c_str());
         }
      } else if (nodename == wxT(MEAN_NODE)) {
			basicstatsinclude = true;
         std::vector<double> means = split<double>(nodecontent);
         pStats->pMean_ = new double[means.size()];
         for (size_t i = 0; i < means.size(); ++i)
            pStats->pMean_[i] = means[i];
      } else if (nodename == wxT(VARIANCE_NODE)) {
         std::vector<double> variance = split<double>(nodecontent);
         pStats->pAccumVariance_ = new double[variance.size()];
         for (size_t i = 0; i < variance.size(); ++i)
            pStats->pAccumVariance_[i] = variance[i];
      } else if (nodename == wxT(MIN_NODE)) {
         std::vector<double> min = split<double>(nodecontent);
         pStats->pMin_ = new double[min.size()];
         for (size_t i = 0; i < min.size(); ++i)
            pStats->pMin_[i] = min[i];
      } else if (nodename == wxT(MAX_NODE)) {
         std::vector<double> max = split<double>(nodecontent);
         pStats->pMax_ = new double[max.size()];
         for (size_t i = 0; i < max.size(); ++i)
            pStats->pMax_[i] = max[i];
      } else if (nodename == wxT(ACCUM_COVAR_NODE)) {
         std::vector<std::vector<double> > accum4covar;
         LoadTable(pchildnode, accum4covar);
         pStats->SetAccum4Covar(accum4covar);
      } else if (nodename == wxT(HISTOGRAM_NODE) && !pHistogram) {
         /** el tipo de dato es anecdotico **/
         pHistogram = suri::raster::data::Histogram<int>::FromXml(pchildnode);
      } else if (nodename == wxT(EIG_NODE)) {
         pStats->SetEig(StringToNumber<int>(nodecontent.c_str()));
      }
      pchildnode = pchildnode->GetNext();
   }

	if (!basicstatsinclude) {
		delete[] pStats->pMean_;
		pStats->pMean_ = NULL;
	}

   return true;
}
/**
 * Convierte la estructura a un string con formato XML
 * @param[in] Stats estructura con las variables estadisticas
 * @param[out] StatsXml string con xml de estadisticas
 * @return bool que indica si pudo configurar xml correctamente
 */
bool SaveStatsToXmlString(const Statistics &Stats, std::string &StatsXml) {
   wxXmlNode* pstatsnode = CreateStatsNode(Stats);
   if (pstatsnode == NULL)
      return false;

   wxXmlDocument doc;
   doc.SetRoot(pstatsnode);
   wxStringOutputStream stream;
   if (!doc.Save(stream))
      return false;

   StatsXml = stream.GetString().char_str();
   return true;
}

/**
 * Convierte la estructura a un string con formato XML
 */
bool SaveStatsToXmlString(suri::raster::data::StatisticsBase* pStats,
                          suri::raster::data::HistogramBase* pHistogram,
                          Statistics::StatisticsFlag Flags,
                          std::string &StatsXml) {
   wxXmlNode* pstatsnode = CreateStatsNode(pStats, pHistogram, Flags);
   if (pstatsnode == NULL)
      return false;

   wxXmlDocument doc;
   doc.SetRoot(pstatsnode);
   wxStringOutputStream stream;
   if (!doc.Save(stream))
      return false;

   StatsXml = stream.GetString().char_str();
   return true;
}
/**
 * Convierte un string xml a la estructura de estadisticas
 * @param[in] StatsXml xml con las estadisticas que quiero leer
 * @param[out] Stats Estructura con estadisticas configuradas
 * @return bool que indica si tuvo exito
 */
bool GetStatsFromXmlString(std::string StatsXml, Statistics &Stats) {
   wxStringInputStream inputstream(StatsXml.c_str());
   wxXmlDocument doc;
   if (!doc.Load(inputstream))
      return false;

   return GetStats(doc.GetRoot(), Stats);
}

/** Convierte un string xml a la estructura de estadisticas **/
bool GetStatsFromXmlString(std::string StatsXml,
                           suri::raster::data::StatisticsBase* &pStats,
                           suri::raster::data::HistogramBase* &pHistogram) {
   wxStringInputStream inputstream(StatsXml.c_str());
   wxXmlDocument doc;
   if (!doc.Load(inputstream))
      return false;

   return GetStats(doc.GetRoot(), pStats, pHistogram);
}

/**
 * Recibe un porcentaje y devuelve el limite del intervalo que deja a la izquierda
 * dicho porcentaje de valores.
 * @param[in] Histogram histograma
 * @param[in] Percent porcentaje de valores que se quiere dejar a la izquierda
 * @return binlimit limite del intervalo que deja a la izquierda el porcentaje de valores
 */
double GetMinNPercent(const Histogram &Histogram, double Percent) {
   double binlimit = 0;

   if (Percent < 0 || Percent > 100) {
      return binlimit;
   }

   double percent = 0;

   Histogram::HistogramType histogram;
   Histogram.GetHistogram(histogram);
   Histogram::HistogramType::reverse_iterator iterhistogram = histogram.rbegin();
   Histogram::HistogramType::reverse_iterator iterend = histogram.rend();

   while (iterhistogram != iterend) {
      percent += Histogram.GetNormalizedFrecuency(iterhistogram->first) * 100;
      if (percent >= 100 - Percent) {
         binlimit = iterhistogram->first;
         break;
      }
      iterhistogram++;
   }

   return binlimit;
}

/**
 * Recibe un porcentaje y devuelve el limite del intervalo que deja a la derecha
 * dicho porcentaje de valores.
 * @param[in] Histogram histograma
 * @param[in] Percent porcentaje de valores que se quiere dejar a la derecha
 * @return binlimit limite del intervalo que deja a la derecha el porcentaje de valores
 */
double GetMaxNPercent(const Histogram &Histogram, double Percent) {
   double binlimit = 0;

   if (Percent < 0 || Percent > 100) {
      return binlimit;
   }

   double percent = 0;

   Histogram::HistogramType histogram;
   Histogram.GetHistogram(histogram);
   Histogram::HistogramType::iterator iterhistogram = histogram.begin();
   Histogram::HistogramType::iterator iterend = histogram.end();

   while (iterhistogram != iterend) {
      percent += Histogram.GetNormalizedFrecuency(iterhistogram->first) * 100;
      if (percent >= 100 - Percent) {
         binlimit = iterhistogram->first;
         break;
      }
      iterhistogram++;
   }

   return binlimit;
}

/** 
 * Obtiene la moda a partir del histograma
 * @param[in] Histogram histograma a partir del cual quiero
 * calcular la moda.
 * @param[out] Mode Valor central del primer bin que contiene la
 * mayor frecuencia
 * @return bool que indica si pudo encontrar una moda.
 */
bool GetMode(const Histogram &Histogram, double &Mode) {
   double modebin = 0;
   unsigned long count = 0;

   Histogram::HistogramType histogram;
   Histogram.GetHistogram(histogram);
   Histogram::HistogramType::iterator iterhistogram = histogram.begin();
   Histogram::HistogramType::iterator iterend = histogram.end();

   while (iterhistogram != iterend) {
      if (iterhistogram->second > count) {
         modebin = iterhistogram->first;
         count = iterhistogram->second;
      }
      iterhistogram++;
   }

   if (count > 0)
      Mode = modebin + Histogram.GetBinSize() / 2;
   return count > 0;
}

/**
 * Obtiene la moda  partir del histograma
 * @param[in] pHistogram histogramas a partir del cual quiero
 * calcular la moda para cada histograma.
 * @param[out] Mode Vector con valores central del primer bin que contiene la
 * mayor frecuencia
 * @return bool que indica si pudo encontrar una moda.
 */
bool GetMode(suri::raster::data::HistogramBase* pHistogram, std::vector<double> &Mode) {
   for (int i = 0; i < pHistogram->GetBandCount(); ++i) {
      double modebin = 0;
      unsigned long count = 0;
      double min = pHistogram->GetMin()[i];
      double max = pHistogram->GetMax()[i];
      double binsize = (max - min)
            / static_cast<double>(pHistogram->GetNumBins()[i]);
      for (int bin = 0; bin < pHistogram->GetNumBins()[i]; ++bin) {
         double freq = pHistogram->GetBins()[i][bin];
         double currentbin = bin * (binsize);
         if (freq > count) {
            modebin = currentbin;
            count = freq;
         }
      }
      if (count > 0) {
         Mode.push_back(modebin + binsize / 2);
      }
   }
   return !Mode.empty();
}
}

