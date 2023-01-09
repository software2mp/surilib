/*! \file BandMathProcessPartProvider.cpp */
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
#include "BandMathProcessPartProvider.h"
#include "suri/RasterElement.h"
#include "BandMathPart.h"
#include "SpectralSelectionPart.h"
#include "suri/DataViewManager.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
BandMathProcessPartProvider::BandMathProcessPartProvider(ProcessAdaptLayer* pPal) :
      FileExporterProcessPartProvider(pPal) {
}

/** Destructor **/
BandMathProcessPartProvider::~BandMathProcessPartProvider() {
}


/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool BandMathProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   std::vector<std::string> bandnames;
   std::vector<Element*> inputelements;
      bool elemsfound = pPal_->GetAttribute< std::vector<Element*> >(
            ProcessAdaptLayer::InputElementsKeyAttr, inputelements);
   // Obtiene los nombres de las bandas de entrada
   RasterElement *praster = RasterElement::Create(inputelements[0]->GetUrl().c_str(), inputelements[0]->GetMetadataMap());
   if (!praster || !elemsfound)
      return false;     
   RasterElement* pinputraster = dynamic_cast<RasterElement*>(inputelements[0]);
   for (int i = 0; i < pinputraster->GetBandCount(); ++i) {
      std::string bname;
      pinputraster->GetBandName(bname, i);
      praster->SetBandName(bname, i);
      int valid = 1;
      pinputraster->GetBandValidity(valid, i);
      praster->SetBandValidity(valid, i);
   }

   int bandcount = praster->GetBandCount();
   std::string bandname;
   for (int i = 0; i < bandcount; i++) {
      praster->GetBandName(bandname, i);
      int valid = 1;
      praster->GetBandValidity(valid, i);
      if (valid != 0)
         bandnames.push_back(bandname);
   }
   delete praster;
   DataViewManager* pdvmgr = NULL;
   pPal_->GetAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   BandMathPart* pband = new BandMathPart(bandnames, pdvmgr->GetLibraryManager());
   pband->SetProcessLayer(pPal_);
   ProcessParts.push_back(pband);
   bool includespectral = false;
   // Se indica que no es necesario el part de seleccion espectral
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, includespectral);

   bool ok = FileExporterProcessPartProvider::GetParts(ProcessParts);

   return ok;
}
/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* BandMathProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   BandMathProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("BandMathProcess") == 0)
      pprovider = new BandMathProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
