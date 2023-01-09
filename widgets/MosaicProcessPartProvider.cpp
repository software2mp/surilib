/*! \file MosaicProcessPartProvider.cpp */
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
#include "FileExporterProcessPartProvider.h"
#include "MosaicProcessPartProvider.h"
#include "MosaicConfigurationPart.h"
#include "ImageFormatSelectionPart.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
MosaicProcessPartProvider::MosaicProcessPartProvider(ProcessAdaptLayer* pPal) :
      FileExporterProcessPartProvider(pPal) {
}

/** Destructor **/
MosaicProcessPartProvider::~MosaicProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool MosaicProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   std::vector<Element*> elementList;
   pPal_->GetAttribute< std::vector<Element*> >(ProcessAdaptLayer::InputRasterElementsAttr,
                                                elementList);
   MosaicConfigurationPart* pmosaic = new MosaicConfigurationPart(elementList);
   pmosaic->SetEnabled(true);
   pmosaic->SetProcessLayer(pPal_);
   ProcessParts.push_back(pmosaic);
   bool showespectral = false;
   bool showmask = false;
   bool showspatial = false;
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr, showmask);
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, showespectral);
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr, showspatial);
   if (!FileExporterProcessPartProvider::GetParts(ProcessParts))
      return false;
   return true;
}
/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* MosaicProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   MosaicProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("MosaicProcess") == 0 )
      pprovider = new MosaicProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
