/*! \file DeleteColumNotification.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/DeleteColumNotification.h"
#include "wxGenericTableBase.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Ctor */
DeleteColumNotification::DeleteColumNotification(int ColId,
                                                 const std::string& ColumnName) :
                                                 colId_(ColId), columnName_(ColumnName) {
}

/** Dtor */
DeleteColumNotification::~DeleteColumNotification() {
}
/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool DeleteColumNotification::ApplyNotification(ObserverInterface* pObserver) {
   wxGenericTableBase* ptableobserver = dynamic_cast<wxGenericTableBase*>(pObserver);
   if (ptableobserver) {
      ptableobserver->ColumnDeleted(colId_, columnName_);
   }
   return true;
}
} /** namespace suri */
