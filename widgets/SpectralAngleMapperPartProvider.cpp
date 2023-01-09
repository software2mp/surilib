/*! \file SpectralAngleMapperPartProvider.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "SpectralAngleMapperPartProvider.h"
#include "SpectralAngleMapperPart.h"
#include "suri/DataViewManager.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor */
SpectralAngleMapperPartProvider::SpectralAngleMapperPartProvider(
      ProcessAdaptLayer* pPal) :
      FileExporterProcessPartProvider(pPal) {
}

/** Destructor */
SpectralAngleMapperPartProvider::~SpectralAngleMapperPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool SpectralAngleMapperPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   SpectralAngleMapperPart* pmapperpart = GetMapperPart();
   if (!pmapperpart)
      return false;
   pmapperpart->SetProcessLayer(pPal_);
   ProcessParts.push_back(pmapperpart);
   return FileExporterProcessPartProvider::GetParts(ProcessParts);
}

/** En funcion de los parametros de entrada retorna el part **/
SpectralAngleMapperPart* SpectralAngleMapperPartProvider::GetMapperPart() {
   DataViewManager* pdvmgr = NULL;
   bool datafound = pPal_->GetAttribute<DataViewManager*>(
         ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);

   if (!datafound)
      return NULL;
   SpectralAngleMapperPart* pmapperpart = new SpectralAngleMapperPart(pdvmgr->GetLibraryManager());

   return pmapperpart;
}

/**
 *  Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* SpectralAngleMapperPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   SpectralAngleMapperPartProvider* pprovider = NULL;
   if (ProcessName.compare("SpectralAngleMapperProcess") == 0)
      pprovider = new SpectralAngleMapperPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
