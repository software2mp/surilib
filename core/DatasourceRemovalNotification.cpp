/*! \file DatasourceRemovalNotification.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar

// Includes Suri
#include "DatasourceRemovalNotification.h"
#include "suri/DatasourceInterface.h"
#include "suri/SuriObject.h"
#include "DefaultViewcontext.h"
#include "suri/LayerInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
DatasourceRemovalNotification::DatasourceRemovalNotification(DatasourceInterface *pDatasource) {
   datasourceId_ = pDatasource->GetId();
}

/**
 * Destructor
 */
DatasourceRemovalNotification::~DatasourceRemovalNotification() {}

/**
 * Elimina el datasource de la lista del observer
 * @param[in] pObserver clase que posee la lista.
 * @return bool True si se pudo remover la fuente de datos de la lista
 */
bool DatasourceRemovalNotification::ApplyNotification(ObserverInterface* pObserver) {
   // Es necesario realizar el casteo hace un viewcontext, ya que es necesario
   // aplicar la notificacion sobre una instancia de viewcontext y no sobre cualquier
   // obeserver
   ViewcontextInterface* pviewcontext = dynamic_cast<ViewcontextInterface *>(pObserver);
   if (pviewcontext == NULL) {
      return false;
   }
   LayerInterface *player = pviewcontext->GetAssociatedLayer(datasourceId_);
   if (player != NULL)
      pviewcontext->RemoveLayer(player->GetId());
   return true;
}

} /** namespace suri */
