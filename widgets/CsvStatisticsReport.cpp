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
