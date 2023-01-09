/** \file StatisticsReport.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes suri
#include "StatisticsReport.h"
#include "TextFormatConverterInterface.h"
#include "logmacros.h"

// Defines
#define STATISTICS_HEADER "<html><head><title></title></head>"                                 \
              "<body bgcolor=\"#AAAAAA\">"                                                     \
              "<font face=\"Lucida Grande, Lucida Sans Unicode\">"                             \
              "<table align=\"center\" cellpadding=\"0\" cellspacing=\"0\">"                   \
              "<tr><td>"                                                                       \
              "<table width=\"560\" bgcolor=\"#FFFFFF\" cellspacing=\"0\" cellpadding=\"0\">"  \
              "<tr><td>"                                                                       \
              "<table bgcolor=\"#BBBBBB\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\">" \
              "<tr>"                                                                           \
              "<td width=\"90%\" valign=\"center\">"                                           \
              "<font size=\"5\" color=\"#FFFFFF\"><em>Resultado</em></font>"                   \
              "</td>"                                                                          \
              "<td width=\"10%\" valign=\"center\" align=\"right\"></td>"                     \
              "</tr>"                                                                          \
              "</table>"

#define STATISTICS_END "</td></tr></table></td></tr></table></font></body></html>"

namespace suri {

/** Ctor */
StatisticsReport::StatisticsReport() : pStats_(NULL), flags_(Statistics::None) {
}

/** Dtor */
StatisticsReport::~StatisticsReport() {
}

/**
 * Retorna un string con el contenido del reporte
 * @param[in] ReportFormat formato del reporte. Ej xml, html
 * @return reporte en formato pedido.
 */
std::string StatisticsReport::GetContentAsText(const std::string &ReportFormat) {
   if (ReportFormat.compare("html") != 0) {
      REPORT_DEBUG("D: Error. Las estadisticas solo se pueden obtener como html");
      return 0;
   }

   // Tranformo las estadisticas a formato html
   std::string xmlstats;
   if (pStats_)
      SaveStatsToXmlString(pStats_, pHistogram_, GetFlags(), xmlstats);
   else
      SaveStatsToXmlString(statistics_, xmlstats);

   std::string flags = NumberToString<int>(GetFlags());
   outputOptions_.insert(std::make_pair("stats_flags", flags));

   std::string htmlstats;
   TextFormatConverterInterface* pxmltohtmlconverter =
         TextFormatConverterInterface::Create(
               "xml:statistics", "html:statistics", outputOptions_);
   if (!pxmltohtmlconverter || !pxmltohtmlconverter->Transform(xmlstats, htmlstats)) {
      TextFormatConverterInterface::Destroy(pxmltohtmlconverter);
      return "";
   }
   TextFormatConverterInterface::Destroy(pxmltohtmlconverter);

   // Agrego tags html, header y body a estadisticas.
   std::string htmlpage = STATISTICS_HEADER;
   htmlpage += htmlstats;
   htmlpage += STATISTICS_END;

   return htmlpage;
}


/**
 * Obtiene estadisticas que se exportan a disco
 * @return estadisticas que exporta el boton a disco
 */
Statistics StatisticsReport::GetStatistics() const {
   return statistics_;
}

/**
 * Configura estadisticas que exportan a disco
 * @param[in] Statistics estadisticas que exporta el boton a disco
 */
void StatisticsReport::SetStatistics(const Statistics &Statistics) {
   statistics_ = Statistics;
}

/** Configura estadisticas que exportan a disco */
void StatisticsReport::SetStatistics(suri::raster::data::StatisticsBase* pStatistics,
                                     suri::raster::data::HistogramBase* pHistogram) {
   pStats_ = pStatistics;
   pHistogram_ = pHistogram;
}

/**
 * Configura las opciones de salida que se pasan a TextFormtaConverter
 * @param[in] OutputOptions opciones de salida que se pasan a
 * TextFormtaConverter
 */
void StatisticsReport::SetOutputOptions(
      const std::map<std::string, std::string> &OutputOptions) {
   outputOptions_ = OutputOptions;
}

/**
 * Establece los flags para estadisticas.
 */
void StatisticsReport::SetFlags(Statistics::StatisticsFlag Flags) {
   flags_ = Flags;
}

/**
 * Devuelve los flags para estadisticas.
 */
Statistics::StatisticsFlag StatisticsReport::GetFlags() {
   return flags_;
}

} /* namespace suri */
