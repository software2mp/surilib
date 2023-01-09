/*! \file DatasourceOrderChangeNotification.cpp */
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
#include "suri/DatasourceInterface.h"
#include "DatasourceOrderChangeNotification.h"
#include "DefaultViewcontext.h"
#include "suri/LayerInterface.h"
#include "suri/messages.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
DatasourceOrderChangeNotification::DatasourceOrderChangeNotification(
      DatasourceInterface *pDatasource, DatasourceInterface *pDatasourceDestination) :
      pDatasource_(pDatasource), pDatasourceDestination_(pDatasourceDestination) {
}

/**
 * Destructor
 */
DatasourceOrderChangeNotification::~DatasourceOrderChangeNotification() {
}

/**
 * Metodo que ejecuta una accion sobre el observador a ser notificado
 */
bool DatasourceOrderChangeNotification::ApplyNotification(ObserverInterface* pObserver) {
   // Es necesario realizar el casteo hace un viewcontext, ya que es necesario
   // aplicar la notificacion sobre una instancia de viewcontext y no sobre cualquier
   // obeserver
   ViewcontextInterface* pviewcontext = dynamic_cast<ViewcontextInterface*>(pObserver);
   if (pviewcontext == NULL) {
      return false;
   }
   LayerInterface *player = pviewcontext->GetAssociatedLayer(pDatasource_->GetId());
   if (!player) {
      return false;
   }
   if (pDatasourceDestination_ != NULL) {
      LayerInterface *playerdestination = pviewcontext->GetAssociatedLayer(
            pDatasourceDestination_->GetId());
      if (!playerdestination)
         return false;
      pviewcontext->OrderChangeLayer(player, playerdestination);
   } else {
      pviewcontext->MoveLayerToEnd(player);
   }
   return true;
}

} /** namespace suri */
