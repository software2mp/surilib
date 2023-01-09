/** \file CanAppendRowNotification.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "CanAppendRowNotification.h"
#include "suri/TableEditionTask.h"

namespace suri {

/**
 * Ctor.
 */
CanAppendRowNotification::CanAppendRowNotification() :
      canAppendRow_(true) {
}

/**
 * Dtor.
 */
CanAppendRowNotification::~CanAppendRowNotification() {
}

/**
 * Obtiene el valor para saber si se puede agregar o no una nueva fila.
 */
bool CanAppendRowNotification::CanAppendRow() {
   return canAppendRow_;
}

/**
 * Establece el valor para saber si se puede agregar o no una nueva fila.
 */
void CanAppendRowNotification::CanAppendRow(bool CanAppendRow) {
   canAppendRow_ = CanAppendRow;
}

/**
 * Metodo que ejecuta una accion sobre el observador a ser notificado.
 */
bool CanAppendRowNotification::ApplyNotification(ObserverInterface* pObserver) {
   TableEditionTask* peditionobserver = dynamic_cast<TableEditionTask*>(pObserver);
   if (peditionobserver) {
      CanAppendRow(!peditionobserver->IsEditingFeature());
   }
   return true;
}

}  // namespace suri
