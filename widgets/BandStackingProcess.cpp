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

#include <vector>
#include <string>

#include "suri/BandStackingProcess.h"
#include "BandInfo.h"
#include "suri/XmlFunctions.h"

namespace suri {

/** Nombre del proceso **/
const std::string BandStackingProcess::kProcessName = "BandStackingProcess";

/** Ctor. */
BandStackingProcess::BandStackingProcess(Element *pElement,
                                         const Subset &WindowSubset,
                                         DataViewManager* pDataViewManager) :
               FileExporterProcess(pElement, WindowSubset, pDataViewManager) {
   SetProcessName(kProcessName);
}


/** Ctor. */
BandStackingProcess::BandStackingProcess(std::vector<Element*> InputElements,
                                         const Subset &ViewerSubset,
                                         DataViewManager* pDataViewManager) :
            FileExporterProcess(InputElements, ViewerSubset, pDataViewManager) {
}

BandStackingProcess::~BandStackingProcess() {
}

/** 
 * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
 * agreguen sus atributos especificos
 * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
 * @return vector vacio en caso de que no tenga parametros adicionales
**/
std::vector<wxXmlNode*> BandStackingProcess::DoGetXmlParameters() const {
   std::vector<wxXmlNode*> params;
   /** Configuracion de seleccion espectral **/   
   std::vector<BandInfo> bands;
   pAdaptLayer_->GetAttribute<std::vector<BandInfo> >(ProcessAdaptLayer::BandInfoKeyAttr, bands);
   std::string stackingbands;
   for (size_t b = 0; b < bands.size(); ++b) {
      stackingbands += bands[b].GetElement()->GetUrl().c_str();
      stackingbands += ",";
   }
#ifdef __WINDOWS__
   stackingbands = wxString(wxConvUTF8.cMB2WC(stackingbands.c_str()),
                  wxCSConv(suri::XmlElement::xmlEncoding_.c_str())).c_str();
#endif
   params.push_back(CreateKeyValueNode("bandas-origen", stackingbands.substr(0, stackingbands.size()-1)));
   return params;
}

}  /** namespace suri */
