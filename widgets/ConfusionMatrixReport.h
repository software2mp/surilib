/** \file ConfusionMatrixReport.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
