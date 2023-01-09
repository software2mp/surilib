/*! \file UpdateProviderNotification.cpp */
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
#include "UpdateProviderNotification.h"
#include "wxGenericTableBase.h"

// Includes Wx

// Defines

// forwards

namespace suri {

UpdateProviderNotification::UpdateProviderNotification() {
   // TODO Auto-generated constructor stub

}

UpdateProviderNotification::~UpdateProviderNotification() {
   // TODO Auto-generated destructor stub
}


/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool UpdateProviderNotification::ApplyNotification(ObserverInterface* pObserver) {
   wxGenericTableBase* pgridobserver = dynamic_cast<wxGenericTableBase*>(pObserver);
   if (!pgridobserver)
      return false;
   pgridobserver->UpdateProvider();
   return true;
}

} /** namespace suri */
