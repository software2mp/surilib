/*! \file VectorOperationProcessPartProvider.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "VectorOperationProcessPartProvider.h"
#include "suri/DataViewManager.h"
#include "suri/World.h"
#include "VectorOperationPart.h"
#include "BufferPart.h"
#include "MergeLayersPart.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
VectorOperationProcessPartProvider::VectorOperationProcessPartProvider(ProcessAdaptLayer* pPal) :
      FileVectorExporterProcessPartProvider(pPal) {
}

/** Destructor **/
VectorOperationProcessPartProvider::~VectorOperationProcessPartProvider() {
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* VectorOperationProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   VectorOperationProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("VectorOperationProcess") == 0 ||
         ProcessName.compare("VectorBufferProcess") == 0 ||
         ProcessName.compare("MergeLayersProcess") == 0)
      pprovider = new VectorOperationProcessPartProvider(pPal);
   return pprovider;
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool VectorOperationProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   std::vector<DatasourceInterface*> inputdatasources;
   pPal_->GetAttribute<std::vector<DatasourceInterface*> >(
         ProcessAdaptLayer::VectorOperationDatasourcesKeyAttr, inputdatasources);
   DataViewManager* pdvmgr = NULL;
   bool datafound = pPal_->GetAttribute<DataViewManager*>(
         ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   World* pinputworld = NULL;
   bool worldfound = pPal_->GetAttribute<World*>(
         ProcessAdaptLayer::InputWorldKeyAttr, pinputworld);
   if (!datafound || !worldfound)
      return false;
   bool showvectoroperation = true;
   if (pPal_->GetAttribute<bool>(ProcessAdaptLayer::IncludeVectorOperationKeyAttr,
                                 showvectoroperation) && showvectoroperation) {
      VectorOperationPart* psubset = new VectorOperationPart();
      psubset->SetProcessLayer(pPal_);
      ProcessParts.push_back(psubset);
   } else if (pPal_->GetAttribute<bool>(ProcessAdaptLayer::IncludeBufferKeyAttr,
                                 showvectoroperation) && showvectoroperation) {
      BufferPart* pbufferpart = new BufferPart(pdvmgr, pinputworld);
      pbufferpart->SetProcessLayer(pPal_);
      ProcessParts.push_back(pbufferpart);
   } else if (pPal_->GetAttribute<bool>(ProcessAdaptLayer::IncludeMergeLayersKeyAttr,
                                        showvectoroperation) && showvectoroperation) {
      MergeLayersPart* pmergerpat = new MergeLayersPart(inputdatasources);
      pmergerpat->SetProcessLayer(pPal_);
      ProcessParts.push_back(pmergerpat);
   }

   return  FileVectorExporterProcessPartProvider::GetParts(ProcessParts);
}

} /** namespace suri */
