/*! \file ClassStatisticsFileParser.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CLASSSTATISTICSFILEPARSER_H_
#define CLASSSTATISTICSFILEPARSER_H_

// Includes Estandar
#include <string>
#include <vector>
#include <map>
// Includes Suri
#include "suri/StatisticsFunctions.h"
#include "SRDStatistics.h"
#include "SRDHistogram.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Permite parsear un archivo de estadisticas exportado */
class ClassStatisticsFileParser {
public:
   /** Constructor */
   ClassStatisticsFileParser(std::string Path);
   /** Destructor */
   virtual ~ClassStatisticsFileParser();
   /** Estructura con la informacion necesaria para el algoritmo de clasificacion supervisada */
   struct ClassStatisticsInfo {
	   ClassStatisticsInfo() : pStatistics_(NULL), pHistogram_(NULL) {}
      std::string name_;
      suri::raster::data::StatisticsBase* pStatistics_;
      suri::raster::data::HistogramBase* pHistogram_;
   };

   /** Parsea el archivo recibido en el constructor de la clase */
   std::vector<ClassStatisticsInfo> Parse();

private:
   suri::raster::data::StatisticsBase* CreateStatisticsFromData(
         const std::string& DataType, int BandCount, long PixelCount,
         const std::vector<double>& Mean, const std::vector<double>& Min,
         const std::vector<double>& Max, const std::vector<double>& Variance,
         std::vector<std::vector<double> > covariancematrix);
   /** caracter delimitador del inicio de la clase **/
   static std::string StartClassChar;
   /** caracter delimitador del final de la clase **/
   static std::string FinishClassChar;
   /** caracter delimitador del inicio del atributo **/
   static std::string StartAttributesChar;
   /** caracter delimitador del final del atributo **/
   static std::string FinishAttributesChar;
   /** token que separa los valores del atributo**/
   static std::string AttributesToken;
   /** Campo correspondiente a los pixels */
   static std::string Pixels;
   /** Campo correspondiente a la media */
   static std::string Media;
   /** Campo correspondiente a la varianza */
   static std::string Varianza;
   /** Campo correspondiente a la correlacion */
   static std::string Correlacion;
   /** Campo correspondiente a la covarianza */
   static std::string Covarianza;
   /** Campo correspondiente a el minimo */
   static std::string Min;
   /** Campo correspondiente a el maximo */
   static std::string Max;
   static std::string Datatype;
   static std::string BandCount;
   std::string path_;
};

} /** namespace suri */

#endif /* CLASSSTATISTICSFILEPARSER_H_ */
