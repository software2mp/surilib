/*! \file ClassStatisticsFileParser.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <fstream>

// Includes Suri
#include "suri/ClassStatisticsFileParser.h"
#include "suri/AuxiliaryFunctions.h"
#include "ClassStatisticsHtmlListItem.h"
#include "SRStatisticsCanvas.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** caracter delimitador del inicio de la clase **/
std::string ClassStatisticsFileParser::StartClassChar = "{";
/** caracter delimitador del final de la clase **/
std::string ClassStatisticsFileParser::FinishClassChar = "}";
/** caracter delimitador del inicio del atributo **/
std::string ClassStatisticsFileParser::StartAttributesChar = "[";
/** caracter delimitador del final del atributo **/
std::string ClassStatisticsFileParser::FinishAttributesChar = "]";
/** token que separa los valores del atributo**/
std::string ClassStatisticsFileParser::AttributesToken = ",";
/** Campo correspondiente a los pixels */
std::string ClassStatisticsFileParser::Pixels = "pixel";
/** Campo correspondiente a la media */
std::string ClassStatisticsFileParser::Media = "media";
/** Campo correspondiente a la varianza */
std::string ClassStatisticsFileParser::Varianza = "varianza";
/** Campo correspondiente a la correlacion */
std::string ClassStatisticsFileParser::Correlacion = "correlacion";
/** Campo correspondiente a la covarianza */
std::string ClassStatisticsFileParser::Covarianza = "covarianza";
/** Campo correspondiente a los minimos */
std::string ClassStatisticsFileParser::Min = "minimo";
/** Campo correspondiente a los maximos */
std::string ClassStatisticsFileParser::Max = "max";

std::string ClassStatisticsFileParser::Datatype = "datatype";
std::string ClassStatisticsFileParser::BandCount = "bandcount";

/** Constructor */
ClassStatisticsFileParser::ClassStatisticsFileParser(std::string Path) : path_(Path) {
}

/** Destructor */
ClassStatisticsFileParser::~ClassStatisticsFileParser() {
}

/**
 * Parsea el archivo recibido en el constructor de la clase
 * @param[in] Classes vector con clases del archivo parseado
 * @return items de la jerarquia HtmlListItemInterface que contiene las clases parseadas
 */
std::vector<ClassStatisticsFileParser::ClassStatisticsInfo> ClassStatisticsFileParser::Parse() {
   std::vector<ClassStatisticsInfo> classes;
   int index = -1;
   std::string attrname;
   std::ifstream csv;
   csv.open(path_.c_str(), std::ifstream::in);
   std::string line;
   std::string datatype;
   int bandcount = 0;
   long pixelcount = 0;
   std::vector<double> mean, min, max, variance;
   std::vector< std::vector<double> > covariancematrix;
   while (!csv.eof()) {
      getline(csv, line);
      // Si encuentra "{" y "}" en la linea crea la estructura que contiene la informacion
      if ((line.find(StartClassChar) != std::string::npos)
            && (line.find(FinishClassChar) != std::string::npos)) {
         if (index >= 0) {
            // configuracion de estadisticas de clase act
            classes[index].pStatistics_ = CreateStatisticsFromData(datatype, bandcount,
                                                                   pixelcount, mean,
                                                                   min, max, variance,
                                                                   covariancematrix);
            mean.clear();
            min.clear();
            max.clear();
            variance.clear();
            covariancematrix.clear();
         }

         // Inicializacion nueva clase
         line.erase(std::remove(line.begin(), line.end(), '{'), line.end());
         line.erase(std::remove(line.begin(), line.end(), '}'), line.end());
         ClassStatisticsInfo aclass;
         aclass.name_ = line;
         classes.push_back(aclass);
         index++;
         continue;
      }

      if ((line.find(StartAttributesChar) != std::string::npos)
            && (line.find(FinishAttributesChar) != std::string::npos)) {
         attrname = line;
         continue;
      }

      if (line.find(Datatype) != std::string::npos) {
         std::vector < std::string > attributes = tokenizer(line, AttributesToken);
         if (attributes.size() > 1)
            datatype = attributes[1];
      } else if (line.find(BandCount) != std::string::npos) {
         std::vector < std::string > attributes = tokenizer(line, AttributesToken);
         if (attributes.size() > 1)
            bandcount = StringToNumber<int>(attributes[1]);
      } else if (attrname.find(Pixels) != std::string::npos) {
         pixelcount = StringToNumber<long>(line);
      } else if (attrname.find(Media) != std::string::npos) {
         std::vector<std::string> attributes = tokenizer(line, AttributesToken);
         if (attributes.size() > 1)
            mean.push_back(StringToNumber<double>(attributes[1]));
      } else if (attrname.find(Covarianza) != std::string::npos) {
         std::vector<std::string> attributes = tokenizer(line, AttributesToken);
         if (attributes.size() > 1) {
            std::vector<double> covariancematrixfile;
            for (size_t i = 1; i < attributes.size(); ++i)
               covariancematrixfile.push_back(StringToNumber<double>(attributes[i]));
            covariancematrix.push_back(covariancematrixfile);
         }
      } else if (attrname.find(Varianza) != std::string::npos) {
         std::vector<std::string> attributes = tokenizer(line, AttributesToken);
         if (attributes.size() > 1)
            variance.push_back(StringToNumber<double>(attributes[1]));
      } else if (attrname.find(Correlacion) != std::string::npos) {
#ifdef __CORRELATION_MATRIX_SUPPORT__
         std::vector<std::string> attributes = tokenizer(line, AttributesToken);
         if (attributes.size() > 1) {
            std::vector<double> correlationmatrixfile;
            for (size_t i = 1; i < attributes.size(); ++i)
               correlationmatrixfile.push_back(StringToNumber<double>(attributes[i]));
            correlationmatrix.push_back(correlationmatrixfile);
         }
#endif
      } else if (attrname.find(Min) != std::string::npos) {
         std::vector<std::string> attributes = tokenizer(line, AttributesToken);
         if (attributes.size() > 1)
            min.push_back(StringToNumber<double>(attributes[1]));
      } else if (attrname.find(Max) != std::string::npos) {
         std::vector < std::string > attributes = tokenizer(line, AttributesToken);
         if (attributes.size() > 1)
            max.push_back(StringToNumber<double>(attributes[1]));
      }
   }
   // cuando termina de leer falta generar la ultima clase
   classes[index].pStatistics_ = CreateStatisticsFromData(datatype, bandcount,
                                                          pixelcount, mean,
                                                          min, max, variance,
                                                          covariancematrix);
   return classes;
}

suri::raster::data::StatisticsBase* ClassStatisticsFileParser::CreateStatisticsFromData(
      const std::string& DataType, int BandCount, long PixelCount,
      const std::vector<double>& Mean, const std::vector<double>& Min,
      const std::vector<double>& Max, const std::vector<double>& Variance,
      std::vector<std::vector<double> > covariancematrix) {
   suri::raster::data::StatisticsBase* pstats =
         suri::render::StatisticsCanvas::CreateStatisticsFromDataType(DataType,
                                                                      BandCount);
   if (pstats) {
      for (int b = 0; b < BandCount; ++b)
         pstats->pPointCount_[b] = PixelCount;

      std::vector<double>::const_iterator mit = Mean.begin();
      for (int i = 0; mit != Mean.end(); ++mit, ++i)
         pstats->pMean_[i] = *mit;

      std::vector<double>::const_iterator vit = Variance.begin();
      /** se multiplica por la cantidad de puntos porque en la clasificacion se
       * utiliza la varianza acumulada **/
      for (int i = 0; vit != Variance.end(); ++vit, ++i)
         pstats->pAccumVariance_[i] = (*vit) * static_cast<double>(PixelCount);

      std::vector<double>::const_iterator minit = Min.begin();
      for (int i = 0; minit != Min.end(); ++minit, ++i)
         pstats->pMin_[i] = *minit;

      std::vector<double>::const_iterator maxit = Max.begin();
      for (int i = 0; maxit != Max.end(); ++maxit, ++i)
         pstats->pMax_[i] = *maxit;

      pstats->ppAccum4Covar_ = covariancematrix;
   }
   return pstats;
}
} /** namespace suri */
