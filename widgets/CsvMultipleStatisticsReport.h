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

#ifndef CSVMULTIPLESTATISTICSREPORT_H_
#define CSVMULTIPLESTATISTICSREPORT_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/ReportInterface.h"
#include "suri/StatisticsFunctions.h"

// Includes Wx
// Defines
// forwards

namespace suri {

class CsvMultipleStatisticsReport : public suri::ReportInterface {
   /** ctor copia **/
   CsvMultipleStatisticsReport(const CsvMultipleStatisticsReport&);

public:
   /**ctor **/
   CsvMultipleStatisticsReport();
   /** dtor **/
   virtual ~CsvMultipleStatisticsReport();
   /**
    * Retorna un string con el contenido del reporte
    * @param[in] ReportFormat formato del reporte. Ej xml, html
    * @return reporte en formato pedido.
    */
   virtual std::string GetContentAsText(const std::string &ReportFormat);
   /** Agrega una estadistica al reporte **/
   void AddStatistics(const std::string& StatisticsTitle, const Statistics &Statistics);
   /** Agrega una estadistica al reporte **/
   void AddStatistics(const std::string& StatisticsTitle,
                      suri::raster::data::StatisticsBase* pStatistics,
                      suri::raster::data::HistogramBase* pHistogram);
   /** Obtiene una estadistica por el titulo**/
   Statistics GetStatistics(const std::string& StatisticsTitle);
   /** Obtiene una estadistica por el indice */
   Statistics GetStatisticsByIndex(int Index);
   /** Elimina la estadistica que corresponde al titulo pasado por parametro**/
   bool RemoveStatistics(const std::string& StatisticsTitle);
   /** Elimina la estadistica que se encuentra en el indice pasado por parametro**/
   bool RemoveStatistics(int Index);
   /** Configura las opciones de salida que se pasan a TextFormtaConverter */
   void SetOutputOptions(const std::map<std::string, std::string> &OutputOptions);

private:
   /** mapa para almacenar las estadisticas que carga el reporte **/
   std::map<std::string, Statistics> statistics_;
   std::map<std::string, suri::raster::data::StatisticsBase*> newstatistics_;
   std::map<std::string, suri::raster::data::HistogramBase*> histograms_;
   /*!  opciones de salida que se pasan a TextFormtaConverter */
   std::map<std::string, std::string> outputOptions_;
};

} /** namespace suri */

#endif /* CSVMULTIPLESTATISTICSREPORT_H_ */
