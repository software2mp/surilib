/*! \file MultipleStatisticsReport.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MULTIPLESTATISTICSREPORT_H_
#define MULTIPLESTATISTICSREPORT_H_

// Includes Estandar
#include <string>
#include <map>
#include <vector>

// Includes Suri
#include "suri/ReportInterface.h"
#include "suri/StatisticsFunctions.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/**Clase que representa un reporte de estadisticas que contiene mas de una
 * estadisticas (ej. estadisticas para areas de entrenamiento y otras
 * para areas de test.) */
class MultipleStatisticsReport : public suri::ReportInterface {
   /** ctor copia **/
   MultipleStatisticsReport(const MultipleStatisticsReport&);

public:
   /**ctor **/
   MultipleStatisticsReport();
   /** dtor **/
   virtual ~MultipleStatisticsReport();
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
                        suri::raster::data::StatisticsBase* pStatics, suri::raster::data::HistogramBase* pHistogram);
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
   /*!  opciones de salida que se pasan a TextFormtaConverter */
   std::map<std::string, std::string> outputOptions_;
   std::map<std::string, suri::raster::data::StatisticsBase*> newStatistics_;
   std::map<std::string, suri::raster::data::HistogramBase*> histograms_;
};

} /** namespace suri */

#endif /* MULTIPLESTATISTICSREPORT_H_ */
