/*! \file FileExporterProcessPartProvider.cpp */
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
#include "ImageFormatSelectionPart.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
FileExporterProcessPartProvider::FileExporterProcessPartProvider(ProcessAdaptLayer* pPal) :
      RasterProcessPartProvider(pPal) {
}

/** Destructor **/
FileExporterProcessPartProvider::~FileExporterProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool FileExporterProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   if (!RasterProcessPartProvider::GetParts(ProcessParts))
      return false;
   /** Crea una parte de seleccion de formato y nombre del archivo. */
   ImageFormatSelectionPart* pformat = new ImageFormatSelectionPart;
   pformat->SetEnabled(true);
   pformat->SetProcessLayer(pPal_);
   ProcessParts.push_back(pformat);
   return true;
}
/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* FileExporterProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   FileExporterProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare("FileExporterProcess") == 0 ||
       ProcessName.compare("BandStackingProcess") ==0)
      pprovider = new FileExporterProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
