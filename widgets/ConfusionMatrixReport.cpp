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

// Includes suri
#include "ConfusionMatrixReport.h"
#include "TextFormatConverterInterface.h"
#include "logmacros.h"

// Includes wx
#include "wx/sstream.h"
#include "wx/xml/xml.h"

// Defines
#ifdef __LINUX__
#define CONFUSION_MATRIX_HEADER "<html><head><meta http-equiv=\"Content-Type\""                \
              "content=\"text/html; charset=utf-8\"><title></title></head>"                    \
              "<body bgcolor=\"#AAAAAA\">"                                                     \
              "<font face=\"Lucida Grande, Lucida Sans Unicode\">"                             \
              "<table align=\"center\" cellpadding=\"0\" cellspacing=\"0\">"                   \
              "<tr><td>"                                                                       \
              "<table width=\"560\" bgcolor=\"#FFFFFF\" cellspacing=\"0\" cellpadding=\"0\">"  \
              "<tr><td>"                                                                       \
              "<table bgcolor=\"#BBBBBB\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\">" \
              "<tr>"                                                                           \
              "<td width=\"90%\" valign=\"center\">"                                           \
              "<font size=\"5\" color=\"#FFFFFF\"><em>Resultado</em></font>"                   \
              "</td>"                                                                          \
              "<td width=\"10%\" valign=\"center\" align=\"right\"></td>"                     \
              "</tr>"                                                                          \
              "</table>"
#else
#define CONFUSION_MATRIX_HEADER "<html><head><title></title></head>"                           \
              "<body bgcolor=\"#AAAAAA\">"                                                     \
              "<font face=\"Lucida Grande, Lucida Sans Unicode\">"                             \
              "<table align=\"center\" cellpadding=\"0\" cellspacing=\"0\">"                   \
              "<tr><td>"                                                                       \
              "<table width=\"560\" bgcolor=\"#FFFFFF\" cellspacing=\"0\" cellpadding=\"0\">"  \
              "<tr><td>"                                                                       \
              "<table bgcolor=\"#BBBBBB\" width=\"100%\" cellspacing=\"0\" cellpadding=\"0\">" \
              "<tr>"                                                                           \
              "<td width=\"90%\" valign=\"center\">"                                           \
              "<font size=\"5\" color=\"#FFFFFF\"><em>Resultado</em></font>"                   \
              "</td>"                                                                          \
              "<td width=\"10%\" valign=\"center\" align=\"right\"></td>"                     \
              "</tr>"                                                                          \
              "</table>"
#endif

#define CONFUSION_MATRIX_END "</td></tr></table></td></tr></table></font></body></html>"

namespace suri {

/** Ctor */
ConfusionMatrixReport::ConfusionMatrixReport() {
}

/** Dtor */
ConfusionMatrixReport::~ConfusionMatrixReport() {
}

/**
 * Retorna un string con el contenido del reporte
 * @param[in] ReportFormat formato del reporte. Ej xml, html
 * @return reporte en formato pedido.
 */
std::string ConfusionMatrixReport::GetContentAsText(const std::string &ReportFormat) {
   if (ReportFormat.compare("html") != 0) {
      REPORT_DEBUG("D: Error. La matriz de confusion solo se pueden obtener como html");
      return "";
   }

   if (!Validate()) {
      REPORT_DEBUG("D: Las estadisticas estan mal configuradas");
      return "";
   }

   // Tranformo las estadisticas a formato xml
   wxXmlNode *pconfusionmatrixnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,
                                                      wxT("confusionmatrix"));
   if (!newstatistics_.empty()) {
      // Agregar los histogramas de entrenamiento y test de las clases
      std::pair<HistogramMap::iterator, HistogramMap::iterator> eqit = histograms_.equal_range(TRAINNING_AREA);
      for (HistogramMap::iterator it = eqit.first; it != eqit.second; ++it) {
         AddStatisticsNode(wxT("trainning"), newstatistics_[TRAINNING_AREA],
                                 it->second, pconfusionmatrixnode);
      }
      eqit = histograms_.equal_range(TEST_AREA);
      for (HistogramMap::iterator it = eqit.first; it != eqit.second; ++it)
         AddStatisticsNode(wxT("testing"), newstatistics_[TEST_AREA], it->second, pconfusionmatrixnode);
   } else {
      if (statistics_.find(TRAINNING_AREA) != statistics_.end())
         AddStatisticsNode(wxT("trainning"), statistics_[TRAINNING_AREA], pconfusionmatrixnode);
      else if (statistics_.find(TEST_AREA) != statistics_.end())
         AddStatisticsNode(wxT("testing"), statistics_[TEST_AREA], pconfusionmatrixnode);  
   }
   wxXmlDocument doc;
   doc.SetRoot(pconfusionmatrixnode);
   wxStringOutputStream stream;
   if (!doc.Save(stream))
      return "";
   std::string xmlconfusionmatrix = stream.GetString().c_str();

   // Tranformo las estadisticas a formato html
   std::string htmlconfusionmatrix;
   TextFormatConverterInterface* pxmltohtmlconverter =
         TextFormatConverterInterface::Create(
               "xml:confusionmatrix", "html:confusionmatrix", outputOptions_);
   if (!pxmltohtmlconverter || !pxmltohtmlconverter->Transform(
                                    xmlconfusionmatrix, htmlconfusionmatrix)) {
      TextFormatConverterInterface::Destroy(pxmltohtmlconverter);
      return "";
   }
   TextFormatConverterInterface::Destroy(pxmltohtmlconverter);

   // Agrego tags html, header y body a estadisticas.
   std::string htmlpage = CONFUSION_MATRIX_HEADER;
   htmlpage += htmlconfusionmatrix;
   htmlpage += CONFUSION_MATRIX_END;

   return htmlpage;
}


/**
 * Obtiene estadisticas que se exportan a disco
 * @param[in] Type tipo de estadisticas. Entrenamiento o Test
 * @return estadisticas que exporta el boton a disco
 */
Statistics ConfusionMatrixReport::GetStatistics(AreaType Type) const {
   std::map<AreaType, Statistics>::const_iterator it = statistics_.find(Type);
   return it != statistics_.end() ? it->second : Statistics();
}

/**
 * Configura estadisticas que exportan a disco
 * @param[in] Statistics estadisticas que exporta el boton a disco
 * @param[in] Type tipo de estadisticas. Entrenamiento o Test
 */
void ConfusionMatrixReport::SetStatistics(const Statistics &Statistics, AreaType Type) {
   statistics_[Type] = Statistics;
}

/** Configura estadisticas que exportan a disco */
void ConfusionMatrixReport::AddStatistics(suri::raster::data::StatisticsBase* pStatistics,
                   suri::raster::data::HistogramBase* pHistogram,
                   AreaType Type) {
   newstatistics_[Type] = pStatistics;
   histograms_.insert(std::make_pair(Type, pHistogram));
}

/**
 * Configura las opciones de salida que se pasan a TextFormtaConverter
 * @param[in] OutputOptions opciones de salida que se pasan a
 * TextFormtaConverter
 */
void ConfusionMatrixReport::SetOutputOptions(
      const std::map<std::string, std::string> &OutputOptions) {
   outputOptions_ = OutputOptions;
}

/** 
 * Valida que el reporte este bien configurado 
 * @return bool que indica si el reporte esta bien configurado
 */
bool ConfusionMatrixReport::Validate() const {
   if (newstatistics_.empty()) {
   return statistics_.find(TEST_AREA) != statistics_.end() ||
                  statistics_.find(TRAINNING_AREA) != statistics_.end();
   } else{
      return newstatistics_.find(TEST_AREA) != newstatistics_.end() ||
            newstatistics_.find(TRAINNING_AREA) != newstatistics_.end();
   }
}

/**
 * Agrega un nodo con las estadisticas a pNode
 * @param[in] NodeName nombre del nodo que se debe crear
 * @param[in] Statistics estadisticas que se desea agregar
 * @param[out] pNode nodo donde se agregan las estadisticas
 * @return bool que indica si tuvo exito.
 */
bool ConfusionMatrixReport::AddStatisticsNode(const std::string &NodeName,
                        const Statistics &Statistics, wxXmlNode* pNode) {
   wxXmlNode *pnewnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, NodeName.c_str());
   if (!pNode->InsertChild(pnewnode, NULL))
      return false;
   wxXmlNode* pstatsnode = CreateStatsNode(Statistics);
   pnewnode->AddChild(pstatsnode);
   return true;
}
/** Agrega un nodo con las estadisticas a pNode */
bool ConfusionMatrixReport::AddStatisticsNode(const std::string &NodeName,
                       suri::raster::data::StatisticsBase* pStatistics,
                       suri::raster::data::HistogramBase* pHistogram,
                       wxXmlNode* pNode) {
   wxXmlNode *pnewnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, NodeName.c_str());
   wxXmlNode* pchild = pNode->GetChildren();
   while (pchild && pchild->GetNext())
      pchild = pchild->GetNext();
   if (!pchild && !pNode->InsertChild(pnewnode, NULL))
      return false;
   else if (pchild && !pNode->InsertChildAfter(pnewnode, pchild))
      return false;

   Statistics::StatisticsFlag flags = Statistics::Mean | Statistics::Variance
         | Statistics::MinValue | Statistics::MaxValue;

   wxXmlNode* pstatsnode = CreateStatsNode(pStatistics, pHistogram, flags);
   pnewnode->AddChild(pstatsnode);
   return true;
}
} /* namespace suri */
