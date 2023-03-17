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

// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
#include "suri/ElementVectorSerializer.h"
#include "suri/GcpAutoGenerationProcess.h"
#include "suri/GcpDetectionSubprocess.h"
#include "suri/GcpGenerationStrategy.h"
#include "suri/GcpList.h"
#include "suri/RasterElement.h"
#include <suri/TextFileGcpLoader.h>
#include "WaveletModulusRenderer.h"
// Includes Wx
#include "wx/xml/xml.h"
// Defines
#define DAUBECHIES_OUTPUT 3
// forwards

namespace suri {

const std::string GcpAutoGenerationProcess::kProcessName = "FilterProcess";

/**
 * Destructor
 */
GcpAutoGenerationProcess::~GcpAutoGenerationProcess() {
}

GcpAutoGenerationProcess::GcpAutoGenerationProcess(const Parameters& params) {
   this->parameters_ = params;
   SetProcessName(kProcessName);
}

bool GcpAutoGenerationProcess::ConfigureOutput() {
   return true;
}

bool GcpAutoGenerationProcess::ConfigureProcess() {
   return true;
}


bool GcpAutoGenerationProcess::RunProcess() {
   GcpDetectionSubprocess baseGcpDetection(parameters_.pBaseElement_, parameters_.baseBands_);
   RasterElement* pBaseCandidates = baseGcpDetection.RunProcess();
   GcpGenerationStrategy gcpGenerator(parameters_.pBaseElement_,
                                      pBaseCandidates,
                                      parameters_.pWarpElement_,
                                      parameters_.seedGcpsPath_);
   gcpGenerator.SetAggressionLevel(parameters_.agressionLevel_);
   gcpGenerator.SetBaseBands(parameters_.baseBands_);
   gcpGenerator.SetWarpBands(parameters_.baseBands_);
   // Ojo! Constructor copia, no asignacion!
   this->gcpList_ = GcpList(gcpGenerator.RunProcess());
   delete pBaseCandidates;
   return true;
}

/**
 *
 * Serializacion
 *
 */

/**
 * Metodo que a partir de un xml que contiene los parametros del proceso configura el mismo.
 * @param[in] XmStr string en formato xml que contiene los parametros necesarios para la
 * correcta ejecucion del proceso
 * @return true en caso de poder configurar correctamente el proceso en funcion de los parametros
 * @return false en caso contrario
 **/
bool GcpAutoGenerationProcess::ConfigureProcessFromXmlString(const std::string& XmlStr) {
   wxStringInputStream ss(XmlStr);
   wxXmlDocument doc(ss);
   wxXmlNode* pRoot = doc.GetRoot();
   this->pAdaptLayer_ = ProcessAdaptLayer::DeserializeXml(pRoot->GetChildren());
   return (pAdaptLayer_ != NULL);
}

/** Metodo que obtiene los parametros asociados al proceso en formato XML.
 * Esto permite que el codigo cliente pueda hacer eso del xml resultante
 * (ej. para la exportacion del mismo)
 * @return string en formato xml con los parametros asociados al proceso
 * @return string vacio en caso contrario.
**/
std::string GcpAutoGenerationProcess::GetParametersAsXmlString() const {
   std::vector<Element*> elList;
   bool hasList = this->pAdaptLayer_->GetAttribute<std::vector<Element*> >(
         ProcessAdaptLayer::InputElementsKeyAttr, elList);
   if (hasList) {
      this->pAdaptLayer_->AddSerializer(ProcessAdaptLayer::InputElementsKeyAttr,
                                        new ElementVectorSerializer());
   }
   return this->pAdaptLayer_->GetAttributesAsXmlString();
}

void GcpAutoGenerationProcess::WriteGcpsToFile () const {
   TextFileGcpLoader loader;
   loader.Save(parameters_.outputFile, this->gcpList_);
}


} /** namespace suri */
