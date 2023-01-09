/*! \file CsvCsvStatisticsReport.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes suri
#include "CsvStatisticsReport.h"
#include "TextFormatConverterInterface.h"
#include "logmacros.h"

// Defines
#define STATISTICS_HEADER "\nResultado\n"

namespace suri {

/** Constructor */
CsvStatisticsReport::CsvStatisticsReport() :
      pStats_(NULL), pHistogram_(NULL), flags_(Statistics::None) {
}

/** Destructor */
CsvStatisticsReport::~CsvStatisticsReport() {
}

/**
 * Retorna un string con el contenido del reporte
 * @param[in] ReportFormat formato del reporte. Ej xml, html
 * @return reporte en formato pedido.
 */
std::string CsvStatisticsReport::GetContentAsText(const std::string &ReportFormat) {
   if (ReportFormat.compare("csv") != 0) {
      REPORT_DEBUG("D: Error. Las estadisticas solo se pueden obtener como csv");
      return 0;
   }

   // Tranformo las estadisticas a formato csv
   std::string xmlstats;
   if (!pStats_ && !pHistogram_)
      SaveStatsToXmlString(statistics_, xmlstats);
   else
      SaveStatsToXmlString(pStats_, pHistogram_, GetFlags(), xmlstats);

   std::string flags = NumberToString<int>(GetFlags());
   outputOptions_.insert(std::make_pair<std::string, std::string>("stats_flags", flags));

   std::string csvstats;
   TextFormatConverterInterface* pxmltocsvconverter =
         TextFormatConverterInterface::Create(
               "xml:statistics", "csv:statistics", outputOptions_);
   if (!pxmltocsvconverter || !pxmltocsvconverter->Transform(xmlstats, csvstats)) {
      TextFormatConverterInterface::Destroy(pxmltocsvconverter);
      return "";
   }
   TextFormatConverterInterface::Destroy(pxmltocsvconverter);
   return csvstats;
}


/**
 * Obtiene estadisticas que se exportan a disco
 * @return estadisticas que exporta el boton a disco
 */
Statistics CsvStatisticsReport::GetStatistics() const {
   return statistics_;
}

/**
 * Configura estadisticas que exportan a disco
 * @param[in] Statistics estadisticas que exporta el boton a disco
 */
void CsvStatisticsReport::SetStatistics(const Statistics &Statistics) {
   statistics_ = Statistics;
}

/** Configura estadisticas que exportan a disco */
void CsvStatisticsReport::SetStatistics(suri::raster::data::StatisticsBase* pStatistics,
                                        suri::raster::data::HistogramBase* pHistogram) {
   pStats_ = pStatistics;
   pHistogram_ = pHistogram;
}
/**
 * Configura las opciones de salida que se pasan a TextFormtaConverter
 * @param[in] OutputOptions opciones de salida que se pasan a
 * TextFormtaConverter
 */
void CsvStatisticsReport::SetOutputOptions(
      const std::map<std::string, std::string> &OutputOptions) {
   outputOptions_ = OutputOptions;
}

/**
 * Establece los flags para estadisticas.
 */
void CsvStatisticsReport::SetFlags(Statistics::StatisticsFlag Flags) {
   flags_ = Flags;
}

/**
 * Devuelve los flags para estadisticas.
 */
Statistics::StatisticsFlag CsvStatisticsReport::GetFlags() {
   return flags_;
}

} /** namespace suri */
