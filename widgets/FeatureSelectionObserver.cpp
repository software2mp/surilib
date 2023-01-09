/*! \file FeatureSelectionObserver.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "FeatureSelectionObserver.h"

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

FeatureSelectionObserver::FeatureSelectionObserver() {
}

FeatureSelectionObserver::~FeatureSelectionObserver() {
}

/** Aplica la notificacion de actualizacion de seleccion sobre el observer
 *  @param pNotification notificacion a ser aplicada por el observer
 */
void FeatureSelectionObserver::Update(NotificationInterface* pNotification) {
   pNotification->ApplyNotification(this);
}
} /** namespace suri */
