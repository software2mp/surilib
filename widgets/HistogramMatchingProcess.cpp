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
#include <vector>
#include <complex>
#include <memory>

// Includes suri
#include "suri/HistogramMatchingProcess.h"
#include "suri/Image.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/DataTypes.h"
#include "suri/DatasourceInterface.h"
#include "suri/DatasourceManagerInterface.h"
#include "ClassifiedRasterDatasourceManipulator.h"
#include "ColorTable.h"
#include "suri/DataViewManager.h"
#include "suri/XmlFunctions.h"
#include "suri/Dimension.h"
#include "suri/DataTypes.h"
#include "SREEnhancement.h"
#include "suri/ParameterCollection.h"
#include "SRDStatistics.h"
#include "SRDHistogram.h"
#include "suri/StatisticsCalculator.h"
#include "SREEnhancementFactory.h"
#include "SREEnhancementUtils.h"

// Includes Wx
#include "wx/wx.h"
#include "wx/xml/xml.h"

/** namespace suri */
namespace suri {

/** Nombre del proceso **/
const std::string HistogramMatchingProcess::kProcessName = "HistogramMatchingProcess";
/**
 * Constructor
 * Asigna el tipo de dato del elemento creado por la herramienta.
 * @param[in] pInputElement: puntero al elemento sobre el que se aplica la herramienta
 * @param[in] WindowSubset subset de la imagen desplegada cuando se selecciono la herramienta
 * @param[out] pOutputElement referencia al elemento creado por la herramienta
 */
HistogramMatchingProcess::HistogramMatchingProcess(std::vector<Element*> InputElements,
                                           const Subset &WindowSubset,
                                           DataViewManager* pDataViewManager,
                                           bool ShowSpectralSelectionPart) :
      FileExporterProcess(InputElements, WindowSubset, pDataViewManager),
      pDatasourceManager_(pDataViewManager ?
                           pDataViewManager->GetDatasourceManager() : NULL)  {
   SetProcessName(kProcessName);
   sameBandCount_ = false;
}

/**
 * Destructor
 */
HistogramMatchingProcess::~HistogramMatchingProcess() {
}


bool HistogramMatchingProcess::SetEnhancement(wxXmlNode*&pNode, Element* pElementA,
                                                                             Element* pElementB){
   RasterElement* pRasterA = dynamic_cast<RasterElement*>(pElementA);
   RasterElement* pRasterB = dynamic_cast<RasterElement*>(pElementB);
   wxXmlNode *pnode = inputElements_[0]->GetNode(RENDERIZATION_NODE);
   std::string aux(Xml2String(pnode));
   namespace SRE = suri::raster::enhancement;
   namespace SREU = suri::raster::enhancement::utils;
   int** pLut = NULL;
   bool succes = false;
   if (pRasterA != NULL && pRasterB != NULL) {
      suri::raster::data::StatisticsBase* pStatisticsA = NULL;
      suri::raster::data::HistogramBase* pHistogramA = NULL;
      suri::raster::data::StatisticsCalculator calcA(pRasterA);
      calcA.CalculateStatistics(pStatisticsA, pHistogramA, false);

      suri::raster::data::StatisticsBase* pStatisticsB = NULL;
      suri::raster::data::HistogramBase* pHistogramB = NULL;
      suri::raster::data::StatisticsCalculator calcB(pRasterB);
      calcB.CalculateStatistics(pStatisticsB, pHistogramB, false);

      // matcheo
      std::auto_ptr<suri::ParameterCollection> params(new suri::ParameterCollection());

      params.get()->AddValue<int>("BandCount", pHistogramA->GetBandCount());
      params.get()->AddValue<int*>("NumBins", pHistogramA->GetNumBins());
      if (pHistogramA->GetBandCount() == pHistogramB->GetBandCount()) sameBandCount_ = true;
      SRE::Enhancement* pEnhancement = SRE::EnhancementFactory::GetInstance()->Create(
            "HistogramMatchingEnhancement", *params.get());
     std::string name = std::string("HistogramMatchingEnhancement");
     pLut = pEnhancement->CreateLUT(pHistogramA->GetBins(), pHistogramB->GetBins());
     succes = SREU::LutToXml(pNode, true, pHistogramA->GetBandCount(), name ,
                     pHistogramA->GetNumBins(), pHistogramA->GetMin(), pHistogramA->GetMax(), pLut);
   }
   return succes;
}
/**
 * Pemite configurar el RasterElement que se va a renderizar
 * @param[in] pRasterElement elemento raster a renderizar
 * @return true siempre
 */
bool HistogramMatchingProcess::ConfigureRaster(RasterElement *pRasterElement) {
   FileExporterProcess::ConfigureRaster(pRasterElement);
   return true;
}



void HistogramMatchingProcess::SetNewBandsCombinationNode(Element* pElement) {
   std::vector<int> outputbands;
   pAdaptLayer_->GetAttribute< std::vector<int> >
                          (ProcessAdaptLayer::HistogramMatchingOutputBandsAttr, outputbands);

   wxString combination = "";
   std::vector<int>::const_iterator it = outputbands.begin();
   std::stringstream ss;
   for (; it != outputbands.end(); ++it) {
      ss.str("");
      ss << (*it);
      combination+= " " + ss.str();
   }
   wxXmlNode* pnode = pElement->GetNode(RENDERIZATION_NODE);
   pElement->AddNode(pnode, BAND_COMBINATION_NODE, combination);

   pnode = pElement->GetNode(
         RENDERIZATION_NODE NODE_SEPARATION_TOKEN BAND_COMBINATION_NODE);
   std::string aux(Xml2String(pnode));
   return ;
}



LayerList * HistogramMatchingProcess::GetElementList() {
   std::vector<Element*> input;
   pAdaptLayer_->GetAttribute< std::vector<Element*> >
                  (ProcessAdaptLayer::HistogramMatchingInputAttr, input);
   wxXmlNode *pEnhancementNode = NULL;
   LayerList* pLayerList = FileExporterProcess::GetElementList();
   std::vector<Element*> plist = pLayerList->GetRenderizationOrderList();
   wxXmlNode *pnode = plist[0]->GetNode(RENDERIZATION_NODE);
   RasterElement* praster = dynamic_cast<RasterElement*>(Element::Create(
            "RasterElement", input[1]->GetNode(), input[1]->GetMetadataMap()));
   SetNewBandsCombinationNode(praster);
   SetEnhancement(pEnhancementNode, plist[0], praster);
   plist[0]->AddNode(pnode, pEnhancementNode);
   return pLayerList;
}

/**
 * Guarda en la imagen de salida la tabla de colores de la entrada.
 * @return informa si pudo configurar elemento
 */
bool HistogramMatchingProcess::ConfigureOutput() {
   if (!FileExporterProcess::ConfigureOutput())
      return false;
   return true;
}




/** Metodo que se llama cuando se configura el ProcessAdaptLayer del proceso**/
void HistogramMatchingProcess::DoOnProcessAdaptLayerSet() {
   FileExporterProcess::DoOnProcessAdaptLayerSet();
}

bool HistogramMatchingProcess::ConfigureProcess() {
   if (!pRasterModelOut_) return false;
   if (!sameBandCount_) return false;
   return FileExporterProcess::ConfigureProcess();
}

World * HistogramMatchingProcess::GetOutputWorld(){
   return RasterProcess::GetOutputWorld();
}

/**
 * Verifica si el nodo contiene una ecuacion definida.
 */
bool HistogramMatchingProcess::HasEquation(const wxXmlNode* pNode) const {
   bool hasequ = true;
   wxXmlNode* pchild = pNode->GetChildren();
   while (pchild != NULL) {
      if (pchild->GetName().Cmp("ecuacion") == 0) {
         wxString equ = pchild->GetNodeContent();
         if (equ.Len() == 0) {
            hasequ = false;
            break;
         }
      }
      pchild = pchild->GetChildren();
   }
   return hasequ;
}

}  // namespace
