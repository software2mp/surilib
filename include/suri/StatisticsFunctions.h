/*!  \file StatisticsFunctions.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef STATISTICSFUNCTIONS_H_
#define STATISTICSFUNCTIONS_H_

// Includes standard
#include <vector>
#include <string>

// Includes Suri
#include "Histogram.h"
#include "SRDStatistics.h"
#include "SRDHistogram.h"

// Includes Wx
#include "wx/xml/xml.h"

// Includes App

// Defines

/** namespace suri */
namespace suri {
/** Estructura para el nodo estadistica */
/**
 * Estructura que guarda los datos estadisticos de una imagen.
 * Existen funciones auxiliares para transformar en nodo y modificar
 * el flag de estadisticas validas.
 */
struct Statistics {
   /**
    * Flag que permite definir que variables de la clase Statistics son
    * tenidas el cuenta por la funcion GetDimensions para realizar la comprobacion
    * de consistencia de tamanio.
    */
   enum {
      None = 0, Mean = 1, Variance = 2, CovarianceMatrix = 4, CorrelationMatrix = 8,
      MaxValue = 16, MinValue = 32, Histogram = 64, All = 127
   };

   typedef int StatisticsFlag;

   Statistics() :
         count_(0) {
   }
   size_t count_; /*! cantidad de muestras */
   std::vector<double> mean_; /*! Media */
   std::vector<double> variance_; /*! Varianza */
   std::vector<std::vector<double> > covarianceMatrix_; /*! Matriz de covarianza */
   std::vector<std::vector<double> > correlationMatrix_; /*! Matriz de correlacion */
   std::vector<double> min_; /*! Valor minimo */
   std::vector<double> max_; /*! Valor maximo */
   std::vector<suri::Histogram> histogram_; /*! Histograma */
   /** Realiza la comprobacion de consistencia en el tamanio de las estadisticas */
   int GetDimension(StatisticsFlag ValidStatistics = All) const;
};


/** Convierte la estructura a un nodo XML */
wxXmlNode* CreateStatsNode(const Statistics &Stats);
/** Convierte la estructura a un nodo XML */
wxXmlNode* CreateStatsNode(suri::raster::data::StatisticsBase* pStats,
                           suri::raster::data::HistogramBase* pHistogram,
                           Statistics::StatisticsFlag Flags);

/** Convierte un nodo a la estructura */
bool GetStats(const wxXmlNode *pStatsNode, Statistics &Stats);

/** Convierte un nodo a la estructura */
bool GetStats(const wxXmlNode *pStatsNode, suri::raster::data::StatisticsBase* &pStats,
              suri::raster::data::HistogramBase* &pHistogram);
/** Convierte la estructura a un string con formato XML */
bool SaveStatsToXmlString(const Statistics &Stats, std::string &StatsXml);
/**
 * Convierte la estructura a un string con formato XML
 */
bool SaveStatsToXmlString(suri::raster::data::StatisticsBase* pStats,
                          suri::raster::data::HistogramBase* pHistogram,
                          Statistics::StatisticsFlag Flags,
                          std::string &StatsXml);

/** Convierte un string xml a la estructura de estadisticas */
bool GetStatsFromXmlString(std::string StatsXml, Statistics &Stats);

/** Convierte un string xml a la estructura de estadisticas
 */
bool GetStatsFromXmlString(std::string StatsXml,
                           suri::raster::data::StatisticsBase*& pStats,
                           suri::raster::data::HistogramBase*& pHistogram);
/** Obtiene el limite que deja el n% a la izquierda */
double GetMinNPercent(const Histogram &Histogram, double Percent);
/** Obtiene el limite que deja n% valores a la derecha */
double GetMaxNPercent(const Histogram &Histogram, double Percent);
/** Obtiene la moda a partir del histogra */
bool GetMode(const Histogram &Histogram, double &Mode);

/**
 * Obtiene la moda  partir del histograma
 * @param[in] pHistogram histogramas a partir del cual quiero
 * calcular la moda para cada histograma.
 * @param[out] Mode Vector con valores central del primer bin que contiene la
 * mayor frecuencia
 * @return bool que indica si pudo encontrar una moda.
 */
bool GetMode(suri::raster::data::HistogramBase* pHistogram, std::vector<double> &Mode);
}

#endif /*STATISTICFUNCTIONS_H_*/
