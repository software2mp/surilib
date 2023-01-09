/*! \file MultipleStatisticsReport.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */



// Includes Estandar
#include <iterator>
#include <map>
#include <vector>
#include <string>

// Includes Suri
#include "MultipleStatisticsReport.h"
#include "StatisticsReport.h"
#include "HtmlFunctions.h"
#include "suri/messages.h"
#include "logmacros.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
MultipleStatisticsReport::MultipleStatisticsReport() {
}

/** Destructor **/
MultipleStatisticsReport::~MultipleStatisticsReport() {
}

/**
 * Retorna un string con el contenido del reporte
 * @param[in] ReportFormat formato del reporte. Ej xml, html
 * @return reporte en formato pedido.
 */
std::string MultipleStatisticsReport::GetContentAsText(
      const std::string &ReportFormat) {
   std::string content;
   if (newStatistics_.empty()) {
      std::map<std::string, Statistics>::iterator it = statistics_.begin();
      StatisticsReport report;
      report.SetOutputOptions(outputOptions_);
      for (; it != statistics_.end(); ++it) {
         /** Se tomo como header de la estadistica la clave en el mapa **/
         AddHtmlHeader(content, _(it->first.c_str()));
         report.SetStatistics(it->second);
         std::string out = report.GetContentAsText(ReportFormat);
         content += out;
      }
   } else {
      std::map<std::string, suri::raster::data::StatisticsBase*>::iterator it =
            newStatistics_.begin();
      StatisticsReport report;
      report.SetOutputOptions(outputOptions_);
      for (; it != newStatistics_.end(); ++it) {
         /** Se tomo como header de la estadistica la clave en el mapa **/
         AddHtmlHeader(content, _(it->first.c_str()));
         std::map<std::string, suri::raster::data::HistogramBase*>::iterator hit =
               histograms_.find(it->first);
         suri::raster::data::HistogramBase* phisto =
               (hit != histograms_.end()) ? hit->second : NULL;
         report.SetStatistics(it->second, phisto);
         std::string out = report.GetContentAsText(ReportFormat);
         content += out;
      }
   }
   return content;
}

/** Agrega una estadistica al reporte
 *  @param StatisticsTitle titulo de la estadistica
 *  @param Statistics datos de la estadistica a agregar **/
void MultipleStatisticsReport::AddStatistics(const std::string& StatisticsTitle,
                                             const Statistics &Statistics) {
   statistics_[StatisticsTitle] = Statistics;
}

/** Agrega una estadistica al reporte **/
void MultipleStatisticsReport::AddStatistics(const std::string& StatisticsTitle,
                     suri::raster::data::StatisticsBase* pStatics,
      suri::raster::data::HistogramBase* pHistogram) {
   newStatistics_[StatisticsTitle] = pStatics;
   histograms_[StatisticsTitle] = pHistogram;
}

/** Obtiene una estadistica por el titulo
 *  @param StatisticsTitle titulo que se utiliza como clave para la busqueda
 *  @return estadistica encontrada, estadistica default
 **/
Statistics MultipleStatisticsReport::MultipleStatisticsReport::GetStatistics(
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
Statistics MultipleStatisticsReport::MultipleStatisticsReport::GetStatisticsByIndex(
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
bool MultipleStatisticsReport::RemoveStatistics(const std::string& StatisticsTitle) {
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
bool MultipleStatisticsReport::RemoveStatistics(int Index) {
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
void MultipleStatisticsReport::SetOutputOptions(
      const std::map<std::string, std::string> &OutputOptions) {
   outputOptions_ = OutputOptions;
}
} /** namespace suri */
