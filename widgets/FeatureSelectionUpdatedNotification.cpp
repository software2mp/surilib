/*! \file FeatureSelectionUpdatedNotification.cpp */
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
#include "FeatureSelectionUpdatedNotification.h"
#include "FeatureSelectionObserver.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
FeatureSelectionUpdatedNotification::FeatureSelectionUpdatedNotification() {
}

/**
 * Destructor
 */
FeatureSelectionUpdatedNotification::~FeatureSelectionUpdatedNotification() {
}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool FeatureSelectionUpdatedNotification::ApplyNotification(ObserverInterface* pObserver) {
   FeatureSelectionObserver* pobserver = dynamic_cast<FeatureSelectionObserver*>(pObserver);
   if (pobserver) {
      pobserver->SelectionUpdated();
   }
   return true;
}

} /** namespace suri */
