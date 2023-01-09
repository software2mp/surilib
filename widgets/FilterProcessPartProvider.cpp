/*! \file FilterProcessPartProvider.cpp */
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
#include "FilterProcessPartProvider.h"
#include "FilterLibraryPart.h"
#include "FilterSelectionPart.h"
#include "suri/FilterProcess.h"
#include "suri/DataViewManager.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Ctor **/
FilterProcessPartProvider::FilterProcessPartProvider(ProcessAdaptLayer* pPal) :
      FileExporterProcessPartProvider(pPal) {
   if (pPal) {
      pPal->GetAttribute<int>(FilterProcess::FilterFlagsKey, flags_);
   }
}

/** Dtor **/
FilterProcessPartProvider::~FilterProcessPartProvider() {
}

/** Obtiene los parts necesarios para la configuracion visual del proceso */
bool FilterProcessPartProvider::GetParts(std::vector<Part*> &ProcessParts) {
   DataViewManager* pdvmgr = NULL;
   pPal_->GetAttribute<DataViewManager*>(ProcessAdaptLayer::DataViewManagerKeyAttr, pdvmgr);
   bool showFilterLibrary = (pPal_->GetProcessName().
         compare(FilterProcess::kChildProcessName) == 0);
   LibraryManager* libraryManager = pdvmgr ? pdvmgr->GetLibraryManager() : NULL;
   Part* pfilter;
   if (showFilterLibrary){
      pfilter = new FilterLibraryPart(libraryManager, flags_);
      ((FilterLibraryPart*) pfilter)->SetProcessLayer(pPal_);
   } else {
      pfilter = new FilterSelectionPart(libraryManager, flags_);
      ((FilterSelectionPart*) pfilter)->SetProcessLayer(pPal_);
   }
   ProcessParts.push_back(pfilter);
   return FileExporterProcessPartProvider::GetParts(ProcessParts);
}

/** Metodo para la creacion de un Part provider
 *  @return instancia del creador correspondiente
 *  @return NULL en caso contrario
 */
ProcessPartProviderInterface* FilterProcessPartProvider::Create(
      const std::string& ProcessName, ProcessAdaptLayer* pPal) {
   FilterProcessPartProvider* pprovider = NULL;
   if (ProcessName.compare(FilterProcess::kProcessName) == 0
         || ProcessName.compare(FilterProcess::kChildProcessName) == 0)
      pprovider = new FilterProcessPartProvider(pPal);
   return pprovider;
}
} /** namespace suri */
