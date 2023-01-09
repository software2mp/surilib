/*! \file RasterProcessPartProvider.cpp */
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
#include "RasterProcessPartProvider.h"
#include "SpatialSubsetSelectionPart.h"
#include "SpectralSelectionPart.h"
#include "MaskSelectionPart.h"
#include "suri/RasterElement.h"
#include "MultiLayerBandSelectionPart.h"
#include "SingleLayerBandSelectionPart.h"
#include "BandInfo.h"
#include "CsvFormatSelectionPart.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/** ctor **/
RasterProcessPartProvider::RasterProcessPartProvider(ProcessAdaptLayer* pPal) :
      ProcessPartProviderInterface(pPal) {
}

/** dtor **/
RasterProcessPartProvider::~RasterProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool RasterProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   if (!pPal_)
      return false;
   DataViewManager* pdvmgr = NULL;
   bool datafound = pPal_->GetAttribute<DataViewManager*>(
         ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   World* pworld = NULL;
   bool worldfound = pPal_->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr,
                                                 pworld);
   std::vector<Element*> inputelements;
   bool elemsnfound = pPal_->GetAttribute< std::vector<Element*> >(
         ProcessAdaptLayer::InputElementsKeyAttr, inputelements);
   bool includemask = false;
   bool maskfound = pPal_->GetAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr,
                                              includemask);
   if (!datafound || !worldfound || !elemsnfound || !maskfound)
      return false;
   bool includespectral = true;
   // Se indica que no es necesario el part de seleccion espectral
   pPal_->GetAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, includespectral);

   if (includespectral && !CreateSpectralSectionPart(ProcessParts, inputelements)) {
      return false;
   } else {
      std::vector<BandInfo> bands;
      RasterElement* praster = dynamic_cast<RasterElement*>(inputelements[0]);
      if (praster) {
         for (int b = 0; b < praster->GetBandCount(); ++b) {
            int valid = 1;
            praster->GetBandValidity(valid, b);
            std::string bname;
            praster->GetBandName(bname, b);
            if (valid != 0)
               bands.push_back(BandInfo(praster, b, bname, 0, 0, ""));
         }
         pPal_->AddSerializableAttribute<BandInfo>(ProcessAdaptLayer::BandInfoKeyAttr, bands);
      }
   }

   bool includespatial = true;
   pPal_->GetAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr, includespatial);
   if (includespatial) {
      SpatialSubsetSelectionPart* psubset = new SpatialSubsetSelectionPart(pdvmgr, pworld);
      psubset->SetProcessLayer(pPal_);
      ProcessParts.push_back(psubset);
   }
   if (includemask) {
      MaskSelectionPart* pmask = new MaskSelectionPart(pdvmgr, inputelements[0]);
      pmask->SetProcessLayer(pPal_);
      ProcessParts.push_back(pmask);
   }
   bool includecsvformat = false;
   // Se indica que es necesario el part de guardar archivo csv
   pPal_->GetAttribute<bool>(ProcessAdaptLayer::CsvFormatPartKeyAttr, includecsvformat);
   if (includecsvformat) {
      CsvFormatSelectionPart* pformat = new CsvFormatSelectionPart;
      pformat->SetProcessLayer(pPal_);
      ProcessParts.push_back(pformat);
   }
   return true;
}

/** Metodo que agrega el part de seleccion espacial */
bool RasterProcessPartProvider::CreateSpectralSectionPart(
      std::vector<Part*> &ProcessParts, std::vector<Element*>& InputElements) {
   if (InputElements.size() == 0)
      return false;

   // Agrega part de seleccion espectral (Se libera en el part de seleccion)
   std::vector<RasterElement*> rasters;

   for (int ix = 0, lenix = InputElements.size(); ix < lenix; ++ix) {
      RasterElement *praster = RasterElement::Create(
            InputElements[ix]->GetUrl().c_str(), InputElements[ix]->GetMetadataMap());
      if (!praster)
         return false;
      praster->SetName(InputElements[ix]->GetName());
      rasters.push_back(praster);
      RasterElement* pinputraster = dynamic_cast<RasterElement*>(InputElements[ix]);
      for (int i = 0; i < pinputraster->GetBandCount(); ++i) {
         std::string bname;
         pinputraster->GetBandName(bname, i);
         praster->SetBandName(bname, i);
         int valid = 1;
         pinputraster->GetBandValidity(valid, i);
         praster->SetBandValidity(valid, i);
      }
   }
   SpectralSelectionPart* pspectralpart = NULL;
   if (rasters.size() > 1)
      pspectralpart = new SpectralSelectionPart(
            new MultiLayerBandSelectionPart(rasters));
   else
      pspectralpart = new SpectralSelectionPart(
            new SingleLayerBandSelectionPart(rasters[0]));
   pspectralpart->SetProcessLayer(pPal_);
   ProcessParts.push_back(pspectralpart);
   return true;
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* RasterProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   RasterProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("RasterProcess") == 0)
      pprovider = new RasterProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
