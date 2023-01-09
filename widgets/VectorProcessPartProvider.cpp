/*! \file VectorProcessPartProvider.cpp */
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
#include "VectorProcessPartProvider.h"
#include "suri/DataViewManager.h"
#include "suri/World.h"
#include "SpatialSubsetSelectionPart.h"

// Includes Wx
// Defines
// forwards

namespace suri {

VectorProcessPartProvider::VectorProcessPartProvider(ProcessAdaptLayer* pPal) :
      ProcessPartProviderInterface(pPal) {
}

VectorProcessPartProvider::~VectorProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool VectorProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   DataViewManager* pdvmgr = NULL;
   bool datafound = pPal_->GetAttribute<DataViewManager*>(
         ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   World* pinputworld = NULL;
   bool worldfound = pPal_->GetAttribute<World*>(
         ProcessAdaptLayer::InputWorldKeyAttr, pinputworld);
   if (!datafound || !worldfound)
      return false;
   bool showspatial = false;
   if (pPal_->GetAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr,
                                 showspatial) && showspatial) {
      SpatialSubsetSelectionPart* psubset = new SpatialSubsetSelectionPart(pdvmgr,
                                                                           pinputworld);
      psubset->SetProcessLayer(pPal_);
      ProcessParts.push_back(psubset);
   }

   return true;
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* VectorProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   VectorProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("VectorProcess") == 0)
      pprovider = new VectorProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
