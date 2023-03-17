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

// Includes Estandar
#include <iterator>

// Includes Suri
#include "CsvMultipleStatisticsReport.h"
#include "CsvStatisticsReport.h"
#include "CsvFunctions.h"
#include "suri/messages.h"
#include "logmacros.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
CsvMultipleStatisticsReport::CsvMultipleStatisticsReport() {
}

/** Destructor **/
CsvMultipleStatisticsReport::~CsvMultipleStatisticsReport() {
}

/**
 * Retorna un string con el contenido del reporte
 * @param[in] ReportFormat formato del reporte. Ej xml, html
 * @return reporte en formato pedido.
 */
std::string CsvMultipleStatisticsReport::GetContentAsText(
      const std::string &ReportFormat) {
   CsvStatisticsReport report;
   report.SetOutputOptions(outputOptions_);

   std::string strflags = outputOptions_["stats_flags"];
   int flags = StringToNumber<int>(strflags);

   report.SetFlags(flags);

   std::string content;
   if (newstatistics_.empty()) {
      std::map<std::string, Statistics>::iterator it = statistics_.begin();
      for (; it != statistics_.end(); ++it) {
         /** Se tomo como header de la estadistica la clave en el mapa **/
         AddCsvHeader(content, _(it->first.c_str()));

         report.SetStatistics(it->second);
         std::string out = report.GetContentAsText(ReportFormat);
         content += out;
      }
   } else {
      std::map<std::string, suri::raster::data::StatisticsBase*>::iterator it =
            newstatistics_.begin();
      for (; it != newstatistics_.end(); ++it) {
         /** Se tomo como header de la estadistica la clave en el mapa **/
         AddCsvHeader(content, _(it->first.c_str()));

         report.SetStatistics(it->second, histograms_[it->first]);
         std::string out = report.GetContentAsText(ReportFormat);
         content += out;
      }
   }
   return content;
}

/** Agrega una estadistica al reporte
 *  @param StatisticsTitle titulo de la estadistica
 *  @param Statistics datos de la estadistica a agregar **/
void CsvMultipleStatisticsReport::AddStatistics(const std::string& StatisticsTitle,
                                                const Statistics &Statistics) {
   statistics_[StatisticsTitle] = Statistics;
}

/** Agrega una estadistica al reporte **/
void CsvMultipleStatisticsReport::AddStatistics(const std::string& StatisticsTitle,
                   suri::raster::data::StatisticsBase* pStatistics,
                   suri::raster::data::HistogramBase* pHistogram) {
   newstatistics_[StatisticsTitle] = pStatistics;
   histograms_[StatisticsTitle] = pHistogram;
}

/** Obtiene una estadistica por el titulo
 *  @param StatisticsTitle titulo que se utiliza como clave para la busqueda
 *  @return estadistica encontrada, estadistica default
 **/
Statistics CsvMultipleStatisticsReport::CsvMultipleStatisticsReport::GetStatistics(
      const std::string& StatisticsTitle) {
   Statistics stats;
   std::map<std::string, Statistics>::iterator it = statistics_.find(StatisticsTitle);
   if (it != statistics_.end())
      stats = it->second;
   return stats;
}

/** Obtiene una estadistica por el indice
 *  @param Index indice de la estadistica a obtener
 *  @return estadistica encontrada, estadistica default
 */
Statistics CsvMultipleStatisticsReport::CsvMultipleStatisticsReport::GetStatisticsByIndex(
      int Index) {
   std::map<std::string, Statistics>::iterator it = statistics_.begin();
   std::advance(it, Index);
   Statistics stats;
   if (it != statistics_.end()) {
      stats = it->second;
   }
   return stats;
}

/** Elimina la estadistica que corresponde al titulo pasado por parametro
 *  @param StatisticsTitle titulo que se utiliza como clave para la busqueda
 *  de la estadistica
 *  @return true en caso de eliminar la estadistica, false en caso contrario
 */
bool CsvMultipleStatisticsReport::RemoveStatistics(const std::string& StatisticsTitle) {
   std::map<std::string, Statistics>::iterator it = statistics_.find(StatisticsTitle);
   if (it != statistics_.end()) {
      statistics_.erase(it);
      return true;
   }
   return false;
}

/** Elimina la estadistica que se encuentra en el indice pasado por parametro
 *  @param Index indice a eliminar
 *  @return true en caso de eliminar la estadistica, false en caso contrario**/
bool CsvMultipleStatisticsReport::RemoveStatistics(int Index) {
   std::map<std::string, Statistics>::iterator it = statistics_.begin();
   std::advance(it, Index);
   if (it != statistics_.end()) {
      statistics_.erase(it);
      return true;
   }
   return false;
}

/**
 * Configura las opciones de salida que se pasan a TextFormtaConverter
 * @param[in] OutputOptions opciones de salida que se pasan a
 * TextFormtaConverter
 */
void CsvMultipleStatisticsReport::SetOutputOptions(
      const std::map<std::string, std::string> &OutputOptions) {
   outputOptions_ = OutputOptions;
}

} /** namespace suri */
