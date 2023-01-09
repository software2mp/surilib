/*! \file NewColumnNotification.cpp */
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
#include "suri/NewColumnNotification.h"
#include "wxGenericTableBase.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor */
NewColumnNotification::NewColumnNotification(int ColId) :
      colId_(ColId) {
}

/** Destructor **/
NewColumnNotification::~NewColumnNotification() {
}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool NewColumnNotification::ApplyNotification(ObserverInterface* pObserver) {
   wxGenericTableBase* ptableobserver = dynamic_cast<wxGenericTableBase*>(pObserver);
   if (ptableobserver) {
      ptableobserver->ColumnAdded(colId_);
   }
   return true;
}
} /** namespace suri */
