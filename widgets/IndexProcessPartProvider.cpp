/*! \file IndexProcessPartProvider.cpp */
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
#include "IndexProcessPartProvider.h"
#include "suri/RasterElement.h"
#include "IndexSelectionPart.h"
#include "suri/DataViewManager.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Ctor **/
IndexProcessPartProvider::IndexProcessPartProvider(ProcessAdaptLayer* pPal) :
      FileExporterProcessPartProvider(pPal) {
}

/** Dtor **/
IndexProcessPartProvider::~IndexProcessPartProvider() {
}
/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool IndexProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   std::vector<Element*> inputelements;
   bool includespectral = false;
   // Se indica que no es necesario el part de seleccion espectral
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, includespectral);
   bool elemsfound = pPal_->GetAttribute< std::vector<Element*> >(
         ProcessAdaptLayer::InputElementsKeyAttr, inputelements);
   if (!elemsfound)
      return false;
   // Crea un elemento con el Filename de elemento recibido
   RasterElement *praster = RasterElement::Create(inputelements[0]->GetUrl().c_str(),
                                                  inputelements[0]->GetMetadataMap());
   if (!praster)
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
   DataViewManager* pdvmgr = NULL;
   pPal_->GetAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   IndexSelectionPart* pindex = new IndexSelectionPart(praster, pdvmgr ? pdvmgr->GetLibraryManager() : NULL);
   pindex->SetProcessLayer(pPal_);
   ProcessParts.push_back(pindex);
   delete praster;
   return FileExporterProcessPartProvider::GetParts(ProcessParts);
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* IndexProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   IndexProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("IndexProcess") == 0)
      pprovider = new IndexProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
