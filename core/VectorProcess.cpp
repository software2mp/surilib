/*! \file VectorProcess.cpp */
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

// Includes Suri
#include "suri/VectorProcess.h"
#include "SpatialSubsetSelectionPart.h"
#include "suri/LayerList.h"
#include "suri/Progress.h"
#include "suri/messages.h"
#include "suri/World.h"
#include "suri/DatasourceContainerInterface.h"
#include "XmlMetadataHandler.h"

// Includes Wx
// Defines
/** Cantidad de ciclos de la herramienta para la barra de progreso */
#define EXPORT_OPERATION_COUNT 3
#define PROCESS_DESCRIPTION "Proceso"

// forwards

namespace suri {

/** Constructor **/
VectorProcess::VectorProcess() : pController_(NULL) {
   this->pAdaptLayer_->SetProcessName("VectorProcess");
}

/** Destructor **/
VectorProcess::~VectorProcess() {
}

// ------ Metodos que administran la ejecucion del proceso ------
/** Corre el proceso y genera la salida usando los metodos de configuracion */
bool VectorProcess::RunProcess() {
   // TODO(Gabriel - TCK #3623): Ver de pasar a ProcessInterface?
   DatasourceContainerInterface* plist = GetRenderizationList();
   if (!plist || !pController_)
      return false;
   // Despliego barra de progreso
   Progress progression(EXPORT_OPERATION_COUNT, PROCESS_DESCRIPTION);
   progression.Update();
   pController_->SetRenderizationList(plist);
   World * pworld = GetWorld();
   if (!pworld)
      return false;
   pController_->SetWorld(pworld);
   // Llamado a configure
   ConfigureProcess();
   bool renderresult = true;
   renderresult = pController_->Render();
   progression.Update();

   // No son mas necesarios el mundo y la lista para el SaveObject
   pController_->SetMaskList(NULL);
   pController_->SetRenderizationList(NULL);
   pController_->SetWorld(NULL);
   // Elimina la lista, que debe existir por fuera del pSaveObject_->Save
   if (renderresult)
      renderresult = ConfigureOutput();
   delete plist;
   progression.Update();
   return renderresult;
}

/** Permite configurar la salida del proceso antes de correrlo */
bool VectorProcess::ConfigureProcess() {
   return false;
}

/** Una vez ejecutado el proceso hace visible la salida */
bool VectorProcess::ConfigureOutput() {
   return false;
}
#ifdef __UNUSED_CODE__
/** Agrega las partes. Sacar este metodo cuando se desacoplen los procesos */
bool VectorProcess::GetParts(std::vector<Part*> &ProcessParts) {
   return ;
}
#endif

/**
 * Exporta los metadatos de una capa a un archivo
 * @param[in] pDatasource fuente de datos que posee el elemento con los metadatos a exportar
 * @return true en caso de que pueda exportar correctamente los metadatos, false caso contrario
 */
bool VectorProcess::ExportMetadataToFile(DatasourceInterface* pDatasource, std::string Filename) {
   bool exportmetadata = false;
   pAdaptLayer_->GetAttribute<bool>(ProcessAdaptLayer::ExportMetadataKeyAttr, exportmetadata);
   if (exportmetadata) {
      MetadataExporterInterface* pexporter = new XmlMetadataHandler();
      return pexporter->ExportMetadata(pDatasource, Filename);
   }
   return false;
}
} /** namespace suri */
