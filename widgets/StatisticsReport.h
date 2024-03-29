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
