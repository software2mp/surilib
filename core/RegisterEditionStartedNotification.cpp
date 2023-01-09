/*! \file RegisterEditionStartedNotification.cpp */
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
#include "RegisterEditionStartedNotification.h"

// Includes Wx

// Defines

// forwards

namespace suri {

RegisterEditionStartedNotification::RegisterEditionStartedNotification() {
   // TODO Auto-generated constructor stub

}

RegisterEditionStartedNotification::~RegisterEditionStartedNotification() {
   // TODO Auto-generated destructor stub
}

/**
 * Notifica al observer
 * @param pObserver
 * @return
 */
bool RegisterEditionStartedNotification::ApplyNotification(ObserverInterface* pObserver) {
   return true;
}
} /** namespace suri */
