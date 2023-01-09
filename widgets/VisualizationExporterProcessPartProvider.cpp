/*! \file VisualizationExporterProcessPartProvider.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Suri
#include "VisualizationExporterProcessPartProvider.h"
#include "OutputPageConfigPart.h"
#include "RasterOutputSizePart.h"

namespace suri {

/** Constructor */
VisualizationExporterProcessPartProvider::VisualizationExporterProcessPartProvider(
      ProcessAdaptLayer* pPal) : FileExporterProcessPartProvider(pPal) {
   pReprojectionParameters_ = NULL;

   if (pPal)
      pPal->GetAttribute<ReprojectionProcess::ReprojectionParameters*>(
            ReprojectionProcess::ReprojectionParamsKeyAttr, pReprojectionParameters_);
}

/** Destructor */
VisualizationExporterProcessPartProvider::~VisualizationExporterProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool VisualizationExporterProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   if (!pPal_)
      return false;

   World* pworld = NULL;
   pPal_->GetAttribute<World*>(ProcessAdaptLayer::InputWorldKeyAttr, pworld);
   World* poutputworld = new World();
   // Creo el part de configuracion de pagina
   ui::OutputPageConfigPart* ppageconfig = new ui::OutputPageConfigPart(
         pReprojectionParameters_, pworld, poutputworld);
   ppageconfig->SetProcessLayer(pPal_);
   ProcessParts.push_back(ppageconfig);
   return FileExporterProcessPartProvider::GetParts(ProcessParts);
}

/**
 * Metodo para la creacion de un Part provider
 * @return instancia del creador correspondiente
 * @return NULL en caso contrario
 */
ProcessPartProviderInterface* VisualizationExporterProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   VisualizationExporterProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("VisualizationExporterProcess") == 0)
      pprovider = new VisualizationExporterProcessPartProvider(pPal);
   return pprovider;
}

} /** namespace suri */
