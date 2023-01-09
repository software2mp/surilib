/*! \file GcpAutoGenerationProcess.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */


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
