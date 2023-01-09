/*! \file VectorOperationProcess.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
