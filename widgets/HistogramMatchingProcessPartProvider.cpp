/*! \file HistogramMatchingProcessPartProvider.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
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
#include "HistogramMatchingProcessPartProvider.h"
#include "HistogramMatchingPart.h"
#include "ImageFormatSelectionPart.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
HistogramMatchingProcessPartProvider::HistogramMatchingProcessPartProvider(ProcessAdaptLayer* pPal) :
      FileExporterProcessPartProvider(pPal) {
}

/** Destructor **/
HistogramMatchingProcessPartProvider::~HistogramMatchingProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool HistogramMatchingProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   std::vector<Element*> elementList;
   pPal_->GetAttribute< std::vector<Element*> >(ProcessAdaptLayer::InputRasterElementsAttr,
                                                elementList);
   HistogramMatchingPart* pmosaic = new HistogramMatchingPart(elementList);
   pmosaic->SetEnabled(true);
   pmosaic->SetProcessLayer(pPal_);
   ProcessParts.push_back(pmosaic);
   bool showespectral = true;
   bool showmask = false;
   bool includespatial = true;
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::IncludeMaskKeyAttr, showmask);
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::SpectralPartKeyAttr, showespectral);
   pPal_->AddAttribute<bool>(ProcessAdaptLayer::IncludeSpatialSelKeyAttr, includespatial);
   if (!FileExporterProcessPartProvider::GetParts(ProcessParts))
      return false;
   return true;
}
/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* HistogramMatchingProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   HistogramMatchingProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("HistogramMatchingProcess") == 0 )
      pprovider = new HistogramMatchingProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
