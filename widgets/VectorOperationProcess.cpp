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
#include "suri/VectorOperationProcess.h"
#include "suri/DataViewManager.h"
#include "DefaultDatasourceList.h"
#include "VectorOperationBuilder.h"
#include "suri/AuxiliaryFunctions.h"
#include "suri/XmlFunctions.h"

namespace suri {

/** Nombre del proceso **/
const std::string VectorOperationProcess::kProcessName = "VectorOperationProcess";

/** Constructor */
VectorOperationProcess::VectorOperationProcess(std::vector<DatasourceInterface*>& InputDatasources,
                                               const Subset &ViewerSubset, Element* &pOutputElement,
                                               DataViewManager* pDataViewManager) :
                                                 FileVectorExporterProcess(
                                                     InputDatasources, ViewerSubset, pOutputElement,
                                                     pDataViewManager) {
   nrings_ = 0;
   operation_ = VectorOperation::Union;
   SetProcessName(kProcessName);
   inputDatasources_ = InputDatasources;
   viewerSubset_ = ViewerSubset;
}

/** Destructor */
VectorOperationProcess::~VectorOperationProcess() {
}

/** Obtiene la lista de elementos a renderizar en el proceso **/
DatasourceContainerInterface* VectorOperationProcess::GetRenderizationList() {
   DatasourceContainerInterface* plist = new DefaultDatasourceList();
   VectorDatasource* pds = GetProcessedDatasource();
   // Arreglo para que utilice el renderizador correcto.
   SanitizeDatasource(pds);
   if (pds)
      plist->AddDatasource(pds);

   return plist;
}

/** Crea una fuente de datos vectorial con el buffer basandose en una
 * copia de la fuente original */
VectorDatasource* VectorOperationProcess::GetProcessedDatasource() {
   pAdaptLayer_->GetAttribute<VectorOperation::OperationType>(
         ProcessAdaptLayer::VectorOperationTypeKeyAttr, operation_);
   pAdaptLayer_->GetAttribute< std::vector<DatasourceInterface*> >(ProcessAdaptLayer::
         VectorOperationDatasourcesKeyAttr, datasources_);
   Option options;
   pAdaptLayer_->GetAttribute<double>(ProcessAdaptLayer::BufferDistanceKeyAttr, distancevalue_);
   distancevaluestr_ = DoubleToString(distancevalue_);
   options.SetOption(VectorOperationBuilder::DistanceValueKey, distancevaluestr_);
   pAdaptLayer_->GetAttribute<std::string>(ProcessAdaptLayer::BufferFieldKeyAttr, bufferfield_);
   options.SetOption(VectorOperationBuilder::DistanceFieldKey, bufferfield_);
   pAdaptLayer_->GetAttribute<int>(ProcessAdaptLayer::BufferQuantityKeyAttr, nrings_);
   nringsstr_ = IntToString(nrings_);
   options.SetOption(VectorOperationBuilder::NRingsKey, nringsstr_);
   pAdaptLayer_->GetAttribute<double>(
         ProcessAdaptLayer::BufferConversionFactorKeyAttr,conversionfactor_);
   conversionfactorstr_ = DoubleToString(conversionfactor_);
   options.SetOption(VectorOperationBuilder::ConversionFactorKey, conversionfactorstr_);
   pAdaptLayer_->GetAttribute<std::string>(
            ProcessAdaptLayer::MergeFieldsMapKey, datasourcefields_);
   options.SetOption(VectorOperationBuilder::MergeFieldsMapKey, datasourcefields_);
   VectorOperationBuilder* operationBuilder = new VectorOperationBuilder(options);
   VectorOperation* pVectorOperation = operationBuilder->GetOperation(operation_);
   VectorDatasource* pVectorDatasource = NULL;
   if (pVectorOperation) {
      pVectorDatasource = pVectorOperation->ProcessDatasource(operation_, datasources_);
   }
   return pVectorDatasource;
}

/**
 * Objetivo: Genera un archivo shp en el directorio temporal del sistema
 * @param[in]  HtmlSrc: contenido del archivo a generar.
 * @return Ok? path del archivo salvado : cadena vacia.
 */
std::string VectorOperationProcess::GetTempFile() {
   // genero archivo temporal
   std::string pathtemp = wxFileName::CreateTempFileName("").c_str();
   pathtemp += "sur.shp";
   return pathtemp;
}   
/** 
 * Metodo polimorfico que complementa a GetParametersAsXmlString para que las clases
 * agreguen sus atributos especificos
 * @return vector de wxXmlNode con los nodos xml de los parametros especificos del proceso     
 * @return vector vacio en caso de que no tenga parametros adicionales
 **/
std::vector<wxXmlNode*> VectorOperationProcess::DoGetXmlParameters() const {
   std::vector<wxXmlNode*> params;
   switch (operation_) {
      case VectorOperation::Buffer: {
         if (!bufferfield_.empty())
            params.push_back(CreateKeyValueNode("campo-buffer", bufferfield_));
         else
           params.push_back(CreateKeyValueNode("distancia", NumberToString<double>(distancevalue_)));
         params.push_back(CreateKeyValueNode("anillos", NumberToString<int>(nrings_)));
         params.push_back(CreateKeyValueNode("factor-conversion", conversionfactorstr_));      
         break;
      }
      case VectorOperation::MergeLayers: {
         params.push_back(CreateKeyValueNode("campos", datasourcefields_));
         break;
      }
      default: {
         params.push_back(CreateKeyValueNode("operacion", 
                           VectorOperation::OperationString[operation_]));
         std::vector<DatasourceInterface*>::const_iterator ds = datasources_.begin();
         std::string layers;
         for (; ds != datasources_.end(); ++ds) {
            layers += (*ds)->GetName();
            layers += ",";
         } 
         params.push_back(CreateKeyValueNode("capas", layers.substr(0, layers.size()-1)));
         break;
      }
   }
   return params;
}


} /** namespace suri */
