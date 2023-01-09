/** \file NewRowNotification.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes suri
#include "NewRowNotification.h"
#include "suri/TableEditionTask.h"

namespace suri {

/** Ctor */
NewRowNotification::NewRowNotification(long FeatureId) {
   featureId_ = FeatureId;
}

/** Dtor */
NewRowNotification::~NewRowNotification () {
}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool NewRowNotification::ApplyNotification(ObserverInterface* pObserver) {
   VectorDataLayer* pdatalayerobserver = dynamic_cast<VectorDataLayer*>(pObserver);
   if (pdatalayerobserver)
      pdatalayerobserver->createdFeatureId_ = featureId_;

   TableEditionTask* peditionobserver = dynamic_cast<TableEditionTask*>(pObserver);
   if (peditionobserver) {
      if (!peditionobserver->EndFeatureEdition(true)) {
         peditionobserver->EndFeatureEdition(false);
         REPORT_DEBUG("D: No se pudo finalizar la edicion del feature editado.");
      }
      peditionobserver->StartFeatureEdition(featureId_);
   }

   return pdatalayerobserver != NULL || peditionobserver != NULL;
}

} /* namespace suri */
