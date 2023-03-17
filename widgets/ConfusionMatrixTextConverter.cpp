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

// Includes standard
#include <string>
#include <map>
#include <vector>
#include <set>

// Includes suri
#include "ConfusionMatrixTextConverter.h"
#include "suri/StatisticsFunctions.h"
#include "suri/XmlFunctions.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"
#include "HtmlFunctions.h"
#include "suri/messages.h"
#include "suri/xmlnames.h"
#include "ClassInformation.h"

// Includes wx
#include "wx/wx.h"
#include "wx/xrc/xmlres.h"
#include "wx/valgen.h"
#include "wx/xml/xml.h"
#include "wx/sstream.h"

#define TOTALCOUNT 1
#define HISTOGRAMROWCOUNT 256
#define MAP_SEPARATOR "|"

namespace suri {

/**
 * Se agregaron las opciones: url, tipo de dato, bandas selecciondas y
 * parte de la imagen leida.
 * Causa: falta de tiempo
 * Refactor pendiente: Agregar esta informacion al xml de entrada.
 * Ver TCK #841
 */
const std::string ConfusionMatrixTextConverter::URL_OP = "url";
const std::string ConfusionMatrixTextConverter::EXTENT_OP = "extent";
const std::string ConfusionMatrixTextConverter::DATATYPE_OP = "datatype";
const std::string ConfusionMatrixTextConverter::CLASSES_OP = "classes";
const std::string ConfusionMatrixTextConverter::TRAIN_ENCLOSURES_OP = "trainenclosures";
const std::string ConfusionMatrixTextConverter::TEST_ENCLOSURES_OP = "testenclosures";

/** Ctor */
ConfusionMatrixTextConverter::ConfusionMatrixTextConverter(
      std::map<std::string, std::string> Options) {
   options_ = Options;
}

/** Dtor */
ConfusionMatrixTextConverter::~ConfusionMatrixTextConverter() {
}

/**
 * Transforma el contenido del texto.
 * @param[in] InputText texto a convertir
 * @param[out] OutputText texto en nuevo formato.
 * @return true en caso de tener exito
 * @return false si ocurre un error.
 */
bool ConfusionMatrixTextConverter::Transform(const std::string &InputText,
                                             std::string &OutputText) {
   // Lee opciones con datos de la imagen
   std::string classesasstring, trainenclosuresasstring, testenclosures;
   if (!ReadOption(CLASSES_OP, classesasstring)
         //|| !ReadOption(TRAIN_ENCLOSURES_OP, trainenclosuresasstring)
         || !ReadOption(TEST_ENCLOSURES_OP, testenclosures)) 
      return false;

   // Obtengo el nombre de los enclosures
   /*std::vector < std::string > trainheaders = split<std::string>(trainenclosuresasstring);
   trainheaders.insert(trainheaders.begin(), html_CLASS);*/

   std::vector < std::string > testheaders = split<std::string>(testenclosures);
   testheaders.insert(testheaders.begin(), html_CLASS);

   // Cargo el nombre de las columnas
   std::map < std::string, std::string > classes = StringToMap(classesasstring,
                                                               MAP_SEPARATOR);
   // Borro "sin clase" del mapa ya que ordena las clases alfabeticamente y la queremos anteultima
   classes.erase(html_NO_CLASS);
   std::map<std::string, std::string>::iterator it = classes.begin();
   std::vector < std::string > rows;
   for (; it != classes.end(); it++)
      rows.push_back(it->first);
   rows.push_back(html_NO_CLASS);
   rows.push_back(html_TOTAL);

   // Calculo las matrices de confusion
   std::vector < std::vector<double> > trainningcm = GetConfusionMatrix(
         InputText, wxT("trainning"));
   std::vector < std::vector<double> > testingcm = GetConfusionMatrix(InputText,
                                                                      wxT("testing"));
   if (trainningcm.empty() && testingcm.empty())
      return false;

   // Configuro tablas en html
   std::string html = "";
   /*if (!trainningcm.empty()) {
      AddHtmlNumericTable(html, _(html_TRAINNINGCONFUSIONMATRIX), trainheaders, rows,
      trainningcm, 1, 0);
   }*/
   if (!testingcm.empty())
      AddHtmlNumericTable(html, _(html_TESTCONFUSIONMATRIX), testheaders, rows, testingcm, 1, 0);

   OutputText = html;
   return true;
}

/**
 * Agrega tabla html con matriz de confusion para el tipo indicado
 * @param[in] InputText texto a convertir
 * @param[in] StatsNodeName tipo de area
 * @param[out] Matriz de confusion
 */
std::vector<std::vector<double> > ConfusionMatrixTextConverter::GetConfusionMatrix(
      const std::string& InputText, const std::string& StatsNodeName) {
   std::string classesasstring, enclosuresasstring = "";
   if (!ReadOption(CLASSES_OP, classesasstring))
      return std::vector<std::vector<double> >();
   std::map < std::string, std::string > classes = StringToMap(classesasstring,
   MAP_SEPARATOR);

   // Obtengo las estadisticas calculadas
   wxStringInputStream inputstream(InputText.c_str());
   wxXmlDocument doc;
   if (!doc.Load(inputstream)) 
      return std::vector<std::vector<double> >();

   // Modifico la cantidad de bis para que sean 256
   // Codigo repetido en TextFormatConverter. Factorizar.
   // Guardo los histogramas en matriz de entrenamiento y test
   std::vector<suri::raster::data::HistogramBase*> histograms;
   wxXmlNode* pchild = doc.GetRoot()->GetChildren();
   while (pchild) {
      if (pchild->GetName().compare(StatsNodeName.c_str()) == 0) {
         wxXmlNode* pstatnode = pchild->GetChildren();
         // busco el nodo estadistica dentro de los nodos
         while (pstatnode->GetName().compare(STATISTICS_NODE) != 0)
            pstatnode = pstatnode->GetChildren();

         if (pstatnode && pstatnode->GetName().compare(STATISTICS_NODE) == 0) {
            // Si encontre el nodo estadistica entonces instancio el histograma correspondiente
            suri::raster::data::StatisticsBase* pstatistics = NULL;
            suri::raster::data::HistogramBase* phistogram = NULL;
            if (GetStats(pstatnode, pstatistics, phistogram) && phistogram)
               histograms.push_back(phistogram);
         }
      }
      pchild = pchild->GetNext();
   }
   if (histograms.empty())
      return std::vector< std::vector<double> >();
   std::vector < std::vector<double> > confusionmatrix(classes.size() + TOTALCOUNT);
   std::vector <double> noclassvector(classes.size());
   std::vector <double> auxvector(classes.size());
   std::map<std::string, std::string>::iterator it = classes.begin();
   /** Armo la  matriz de confusion en funcion de los histogramas obtenidos */
   int k = 0;
   for (size_t i = 0; i < histograms.size(); ++i) {
      int total = 0, j = 0;
      double range = (histograms[i]->GetMax()[0] - histograms[i]->GetMin()[0])
            / static_cast<double>(histograms[i]->GetNumBins()[0]);
      for (it = classes.begin(); it != classes.end(); ++it, ++j) {
         int classindex = StringToNumber<int>(it->second);
         double floatbinpos = (static_cast<double>(classindex) / range);
         int binpos = SURI_CEIL(floatbinpos) - 1;
         binpos = binpos < 0 ? 0 : binpos;
         if (binpos < histograms[i]->GetNumBins()[0]) {
            double freq = histograms[i]->GetBins()[0][binpos];
            // Guardo la fila donde deben ir los valores que se estan guardando en sin clase
            if (k != j && classindex == ClassInformation::NoClassIndex)
               k = j;
            confusionmatrix[j].push_back(freq);
            total += freq;
         } else {
            confusionmatrix[j].push_back(0);
         }
      }
      confusionmatrix[j].push_back(total);
   }
   /*
    * Ya que se coloca sinclase en la ultima posicion debe cambiarse la frecuencia con la anterior clase
    */
   return confusionmatrix;
}

/**
 * Retorna el valor de una opcion
 * @return informa si encontro la opcion
 */
bool ConfusionMatrixTextConverter::ReadOption(const std::string &Option,
                                              std::string &OptionValue) {
   // Codigo repetido en TextFormatConverter. Factorizar.
   std::map<std::string, std::string>::iterator it = options_.find(Option);
   if (it != options_.end()) OptionValue = it->second;

   return (it != options_.end());
}

} /* namespace suri */
