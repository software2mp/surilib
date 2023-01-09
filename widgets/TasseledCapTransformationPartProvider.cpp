/*! \file TasseledCapTransformationPartProvider.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "TasseledCapTransformationPartProvider.h"

#include "ImageFormatSelectionPart.h"
#include "MaskSelectionPart.h"
#include "SpatialSubsetSelectionPart.h"
#include "TasseledCapTransformationPart.h"
#include "suri/RasterElement.h"
#include "suri/World.h"

namespace suri {

/**
 * Ctor.
 */
TasseledCapTransformationPartProvider::TasseledCapTransformationPartProvider(
      ProcessAdaptLayer* pPal) : ProcessPartProviderInterface(pPal) {

}

/**
 * Dtor.
 */
TasseledCapTransformationPartProvider::~TasseledCapTransformationPartProvider() {

}

/**
 * Obtiene los parts necesarios para la configuracion visual del proceso.
 */
bool TasseledCapTransformationPartProvider::GetParts(std::vector<Part*>& ProcessParts) {
   DataViewManager* pdvmgr = NULL;
   pPal_->GetAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr,
                                         pdvmgr);

   World* pworld = NULL;
   pPal_->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr, pworld);

   Element* pelement = NULL;
   pPal_->GetAttribute<Element*>(ProcessAdaptLayer::TasseledCapInputKeyAttr, pelement);

   RasterElement* praster = dynamic_cast<RasterElement*>(pelement);

   TasseledCapTransformationPart* ptasseledcap = new TasseledCapTransformationPart(praster);
   ptasseledcap->SetProcessLayer(pPal_);
   ProcessParts.push_back(ptasseledcap);

   // Seleccion espacial.
   SpatialSubsetSelectionPart* psubset = new SpatialSubsetSelectionPart(pdvmgr, pworld);
   psubset->SetProcessLayer(pPal_);
   ProcessParts.push_back(psubset);

   // Mascara.
   MaskSelectionPart* pmask = new MaskSelectionPart(pdvmgr, pelement);
   pmask->SetProcessLayer(pPal_);
   ProcessParts.push_back(pmask);

   // Archivo de salida.
   ImageFormatSelectionPart* pformat = new ImageFormatSelectionPart();
   pformat->SetEnabled(true);
   pformat->SetProcessLayer(pPal_);
   ProcessParts.push_back(pformat);

   return true;
}

/**
 * Metodo para la creacion de un Part provider.
 *
 * @return instancia del creador correspondiente o NULL en caso contrario.
 */
ProcessPartProviderInterface* TasseledCapTransformationPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   TasseledCapTransformationPartProvider* pprovider = NULL;
   if (ProcessName.compare("TasseledCapProcess") == 0) pprovider =
         new TasseledCapTransformationPartProvider(pPal);
   return pprovider;
}

} /** namespace suri */
