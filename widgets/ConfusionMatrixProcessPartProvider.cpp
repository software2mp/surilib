/*! \file ConfusionMatrixProcessPartProvider.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <vector>
#include <string>

// Includes Suri
#include "ConfusionMatrixProcessPartProvider.h"
#include "ConfusionMatrixPart.h"
#include "suri/DataViewManager.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
ConfusionMatrixProcessPartProvider::ConfusionMatrixProcessPartProvider(
      ProcessAdaptLayer* pPal) :
      StaticsExporterProcessPartProvider(pPal) {
}

/** Dtor **/
ConfusionMatrixProcessPartProvider::~ConfusionMatrixProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool ConfusionMatrixProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   std::vector<Element*> inputelements;
   bool elemsnfound = pPal_->GetAttribute<std::vector<Element*> >(
         ProcessAdaptLayer::InputElementsKeyAttr, inputelements);
   DataViewManager* pdvmgr = NULL;
   bool datafound = pPal_->GetAttribute<DataViewManager*>(
         ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   if (!elemsnfound || !datafound)
      return false;
   ConfusionMatrixPart* pconf = new ConfusionMatrixPart(inputelements[0],
                                                        pdvmgr->GetDatasourceManager());
   pconf->SetProcessLayer(pPal_);
   ProcessParts.push_back(pconf);
   bool includespectral = false;
   // Se indica que no es necesario el part de seleccion espectral
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, includespectral);

   return RasterProcessPartProvider::GetParts(ProcessParts);
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* ConfusionMatrixProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   ConfusionMatrixProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("ConfussionMatrixProcess") == 0)
      pprovider = new ConfusionMatrixProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
