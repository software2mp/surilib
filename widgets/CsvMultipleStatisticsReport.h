/*! \file CsvMultipleStatisticsReport.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
