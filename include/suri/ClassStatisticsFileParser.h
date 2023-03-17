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
