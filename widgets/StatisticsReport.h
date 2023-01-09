/** \file StatisticsReport.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef STATISTICSREPORT_H_
#define STATISTICSREPORT_H_

// Includes standard
#include <string>
#include <map>

// Includes suri
#include "suri/StatisticsFunctions.h"
#include "suri/ReportInterface.h"
#include "SRDStatistics.h"
#include "SRDHistogram.h"

namespace suri {

/**
 * Reporte que contiene contiene estadisticas.
 */
class StatisticsReport : public ReportInterface {
public:
   /** Ctor */
   StatisticsReport();
   /** Dtor */
   virtual ~StatisticsReport();

   /** Retorna un string con el contenido del reporte */
   virtual std::string GetContentAsText(const std::string &ReportFormat);

   /** Obtiene estadisticas que se exportan a disco */
   Statistics GetStatistics() const;
   /** Configura estadisticas que exportan a disco */
   void SetStatistics(const Statistics &Statistics);
   /** Configura estadisticas que exportan a disco */
   void SetStatistics(suri::raster::data::StatisticsBase* pStatistics,
                      suri::raster::data::HistogramBase* pHistogram);
   /** Configura las opciones de salida que se pasan a TextFormtaConverter */
   void SetOutputOptions(const std::map<std::string, std::string> &OutputOptions);

   /**
    * Establece los flags para estadisticas.
    */
   void SetFlags(Statistics::StatisticsFlag Flags);

   /**
    * Devuelve los flags para estadisticas.
    */
   Statistics::StatisticsFlag GetFlags();

private:
   /*! Estadisticas calculadas por el proceso */
   Statistics statistics_;

   /** Nuevas estadisticas **/
   suri::raster::data::StatisticsBase* pStats_;
   suri::raster::data::HistogramBase* pHistogram_;

   /*!  opciones de salida que se pasan a TextFormtaConverter */
   std::map<std::string, std::string> outputOptions_;

   Statistics::StatisticsFlag flags_;
};

} /* namespace suri */
#endif /* STATISTICSREPORT_H_ */
