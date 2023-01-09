/*! \file DatasourceUnblockedNotification.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "suri/DatasourceUnblockedNotification.h"
#include "suri/ViewcontextInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
DatasourceUnblockedNotification::DatasourceUnblockedNotification(
      const SuriObject::UuidType& DatasourceId) :
      datasourceId_(DatasourceId) {
}

/** Destructor */
DatasourceUnblockedNotification::~DatasourceUnblockedNotification() {
}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool DatasourceUnblockedNotification::ApplyNotification(ObserverInterface* pObserver) {
   ViewcontextInterface* pviewcontext = dynamic_cast<ViewcontextInterface*>(pObserver);
   if (pviewcontext) {
      pviewcontext->UnblockLayerByDatasourceId(datasourceId_);
   }
   return true;
}
} /** namespace suri**/
