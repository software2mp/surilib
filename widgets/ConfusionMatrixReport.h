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

#ifndef CONFUSIONMATRIXREPORT_H_
#define CONFUSIONMATRIXREPORT_H_

// Includes standard
#include <string>
#include <map>

// Includes suri
#include "suri/ReportInterface.h"
#include "suri/StatisticsFunctions.h"
#include "SRDHistogram.h"
#include "SRDStatistics.h"

// Fordwards
class wxXmlNode;

namespace suri {

/**
 * Reporte con una matriz de confusion para las areas de entrenamiento
 * y otra para las de test.
 */
class ConfusionMatrixReport : public ReportInterface {
public:
   /** Tipo de area posibles en el reporte */
   enum AreaType {TEST_AREA = 0, TRAINNING_AREA};

   /** Ctor */
   ConfusionMatrixReport();
   /** Dtor */
   virtual ~ConfusionMatrixReport();

   /** Retorna un string con el contenido del reporte */
   virtual std::string GetContentAsText(const std::string &ReportFormat);

   /** Valida que el reporte este bien configurado */
   bool Validate() const;

   /** Obtiene estadisticas que se exportan a disco */
   Statistics GetStatistics(AreaType Type) const;
   /** Configura estadisticas que exportan a disco */
   void SetStatistics(const Statistics &Statistics, AreaType Type);
   /** Configura estadisticas que exportan a disco */
   void AddStatistics(suri::raster::data::StatisticsBase* pStatistics,
                      suri::raster::data::HistogramBase* pHistogram,
                      AreaType Type);

   /** Configura las opciones de salida que se pasan a TextFormtaConverter */
   void SetOutputOptions(const std::map<std::string, std::string> &OutputOptions);

private:
   /** Agrega un nodo con las estadisticas a pNode */
   bool AddStatisticsNode(const std::string &NodeName,
                           const Statistics &Statistics, wxXmlNode* pNode);
   /** Agrega un nodo con las estadisticas a pNode */
   bool AddStatisticsNode(const std::string &NodeName,
                          suri::raster::data::StatisticsBase* pStatistics,
                          suri::raster::data::HistogramBase* pHistogram,
                          wxXmlNode* pNode);

   /*! Estadisticas del reporte */
   std::map<AreaType, Statistics> statistics_;
   std::map<AreaType, suri::raster::data::StatisticsBase*> newstatistics_;
   typedef std::multimap<AreaType, suri::raster::data::HistogramBase*> HistogramMap;
   HistogramMap histograms_;

   std::map<std::string, std::string> outputOptions_; 
};

} /* namespace suri */
#endif /* CONFUSIONMATRIXREPORT_H_ */
