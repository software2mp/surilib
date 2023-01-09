/*! \file SharpeningProcessPartProvider.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <string>

#include "SharpeningProcessPartProvider.h"
#include "suri/Part.h"
#include "suri/ProcessAdaptLayer.h"
#include "suri/ProcessAtributeProvider.h"
#include "suri/ProcessPartProviderInterface.h"
#include "suri/RasterElement.h"
#include "ImageFormatSelectionPart.h"
#include "SharpeningInputElementsPart.h"
#include "SharpeningInputPanPart.h"
#include "SharpeningInputRGBPart.h"

namespace suri { class Element; }

namespace suri {

/**
 * Ctor.
 */
SharpeningProcessPartProvider::SharpeningProcessPartProvider(ProcessAdaptLayer* pPal) :
      ProcessPartProviderInterface(pPal) {
}

/**
 * Dtor.
 */
SharpeningProcessPartProvider::~SharpeningProcessPartProvider() {

}

/**
 * Obtiene los parts necesarios para la configuracion visual del proceso.
 */
bool SharpeningProcessPartProvider::GetParts(std::vector<Part*>& ProcessParts) {
   SharpeningInputRGBPart* plowrespart = new SharpeningInputRGBPart(NULL);
   plowrespart->SetProcessLayer(pPal_);
   ProcessParts.push_back(plowrespart);

   SharpeningInputPanPart* phighrespart = new SharpeningInputPanPart(NULL);
   phighrespart->SetProcessLayer(pPal_);
   ProcessParts.push_back(phighrespart);

   // Archivo de salida
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
ProcessPartProviderInterface* SharpeningProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   SharpeningProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("SharpeningProcess") == 0) pprovider =
         new SharpeningProcessPartProvider(pPal);
   return pprovider;
}

}  // namespace suri
