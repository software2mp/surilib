/*! \file PrincipalComponentAnalysisPartProvider.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "PrincipalComponentAnalysisPartProvider.h"

#include "ImageFormatSelectionPart.h"
#include "MaskSelectionPart.h"
#include "PrincipalComponentAnalysisPart.h"
#include "PrincipalComponentAnalysisProcess.h"
#include "SingleLayerBandSelectionPart.h"
#include "SpatialSubsetSelectionPart.h"
#include "SpectralSelectionPart.h"
#include "suri/RasterElement.h"
#include "suri/World.h"

namespace suri {

/**
 * Ctor.
 */
PrincipalComponentAnalysisPartProvider::PrincipalComponentAnalysisPartProvider(
      ProcessAdaptLayer* pPal) : ProcessPartProviderInterface(pPal) {

}

/**
 * Dtor.
 */
PrincipalComponentAnalysisPartProvider::~PrincipalComponentAnalysisPartProvider() {

}

/**
 * Obtiene los parts necesarios para la configuracion visual del proceso.
 */
bool PrincipalComponentAnalysisPartProvider::GetParts(std::vector<Part*>& ProcessParts) {
   DataViewManager* pdvmgr = NULL;
   pPal_->GetAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr,
                                         pdvmgr);

   World* pworld = NULL;
   pPal_->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr, pworld);

   Element* pelement = NULL;
   pPal_->GetAttribute<Element*>(ProcessAdaptLayer::PCAnalysisInputKeyAttr, pelement);

   RasterElement* praster = dynamic_cast<RasterElement*>(pelement);

   // Seleccion espectral.
   SpectralSelectionPart* pspectral = new SpectralSelectionPart(
         new SingleLayerBandSelectionPart(praster));
   pspectral->SetProcessLayer(pPal_);
   ProcessParts.push_back(pspectral);

   // Seleccion componenetes principales.
   PrincipalComponentAnalysisPart* ppcanalysis = new PrincipalComponentAnalysisPart(praster);
   ppcanalysis->SetProcessLayer(pPal_);
   ProcessParts.push_back(ppcanalysis);

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
ProcessPartProviderInterface* PrincipalComponentAnalysisPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   PrincipalComponentAnalysisPartProvider* pprovider = NULL;
   if (ProcessName.compare(PrincipalComponentAnalysisProcess::kProcessName) == 0) pprovider =
         new PrincipalComponentAnalysisPartProvider(pPal);
   return pprovider;
}

}  // namespace suri
