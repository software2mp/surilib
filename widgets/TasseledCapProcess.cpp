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

#include <sstream>

#include "TasseledCapProcess.h"

#include "DataCastRenderer.h"
#include "FileRenderization.h"
#include "suri/XmlFunctions.h"

#include "wx/xml/xml.h"

namespace suri {

/** Nombre del proceso **/
const std::string TasseledCapProcess::kProcessName = "TasseledCapProcess";
const std::string TasseledCapProcess::kToken = " ";

/**
 * Ctor.
 */
TasseledCapProcess::TasseledCapProcess(Element* pInputElement,
                                       const Subset& WindowSubset,
                                       DataViewManager* pDataViewManager) :
      FileExporterProcess(pInputElement, WindowSubset, pDataViewManager) {
   SetProcessName(kProcessName);
   pRenderizationObject_ = new FileRenderization();
}

/**
 * Dtor.
 */
TasseledCapProcess::~TasseledCapProcess() {

}

/**
 * Pemite configurar el RasterElement que se va a renderizar.
 */
bool TasseledCapProcess::ConfigureRaster(RasterElement* pRasterElement) {
   std::string sensor;
   GetProcessAdaptLayer()->GetAttribute<std::string>(ProcessAdaptLayer::TasseledCapSensorKeyAttr, sensor);

   std::vector<int> outputbands;
   GetProcessAdaptLayer()->GetAttribute<std::vector<int> >(ProcessAdaptLayer::TasseledCapOutputBandsKeyAttr, outputbands);

   std::stringstream ss;
   for (int ix = 0, lenix = outputbands.size(); ix < lenix; ++ix) {
      if (ss.str().length() > 0) ss << kToken;
      ss << outputbands[ix];
   }
   std::string obstr = ss.str();

   wxXmlNode* prnode = pRasterElement->GetNode(wxT(RENDERIZATION_NODE));

   wxXmlNode* ptcnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(TASSELEDCAP_NODE));

   wxXmlNode* psensornode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(TASSELEDCAP_SENSOR_NODE));
   new wxXmlNode(psensornode, wxXML_TEXT_NODE, wxT(""), sensor.c_str());

   wxXmlNode* poutputnode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT(TASSELEDCAP_OUTPUT_NODE));
   new wxXmlNode(poutputnode, wxXML_TEXT_NODE, wxT(""), obstr.c_str());

   ptcnode->AddChild(psensornode);
   ptcnode->AddChild(poutputnode);

   std::stringstream inputss;
   for (int ix = 0, lenix = pRasterElement->GetBandCount(); ix < lenix; ++ix) {
      if (inputss.str().length() > 0) inputss << kToken;
      inputss << ix;
   }
   std::string ibstr = inputss.str();

   pRasterElement->AddNode(prnode, BAND_COMBINATION_NODE, wxT(ibstr.c_str()));
   pRasterElement->AddNode(prnode, ptcnode);
   pRasterElement->AddNode(prnode, GetDataCastRenderizationNode());

   return true;
}

/**
 * Otiene el tamano del objeto a salvar.
 */
wxLongLong TasseledCapProcess::GetNeededSpace() {
   int hight = 0, width = 0;

   World* pworld = (pOutputWorld_)? pOutputWorld_ : pInputWorld_;
   pworld->GetViewport(hight, width);

   return hight * width * sizeof(double) + 1000000;
}

/**
 * Retorna el nodo datacast configurado con el tipo de dato de salida.
 */
wxXmlNode* TasseledCapProcess::GetDataCastRenderizationNode() {
   DataCastRenderer::Parameters parameters;
   parameters.destinationDataType_ = "double";
   if (parameters.destinationDataType_.empty())
      return NULL;
   return DataCastRenderer::GetXmlNode(parameters);
}
/** 
 * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
 * agreguen sus atributos especificos
 * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
 * @return vector vacio en caso de que no tenga parametros adicionales
 **/
std::vector<wxXmlNode*> TasseledCapProcess::DoGetXmlParameters() const {
   std::vector<wxXmlNode*> params;       
   std::string sensor;
   pAdaptLayer_->GetAttribute<std::string>(ProcessAdaptLayer::TasseledCapSensorKeyAttr, sensor);
   params.push_back(CreateKeyValueNode("sensor", sensor));
   std::vector<int> outputbands;
   pAdaptLayer_->GetAttribute<std::vector<int> >(ProcessAdaptLayer::TasseledCapOutputBandsKeyAttr, outputbands);
   std::string bands;
   for (size_t i = 0; i < outputbands.size(); ++i)
      bands += NumberToString<int>(outputbands[i]) + ",";
   params.push_back(CreateKeyValueNode("bandas-sensor", bands.substr(0, bands.size()-1)));
   return params;
}
} /** namespace suri */
