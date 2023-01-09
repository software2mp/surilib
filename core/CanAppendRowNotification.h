/** \file CanAppendRowNotification.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef CANAPPENDROWNOTIFICATION_H_
#define CANAPPENDROWNOTIFICATION_H_

#include "suri/NotificationInterface.h"
#include "suri/ObserverInterface.h"

namespace suri {

/**
 * Notificacion para verificar si es posible agregar una fila nueva.
 */
class CanAppendRowNotification : public suri::NotificationInterface {
public:
   /**
    * Ctor.
    */
   CanAppendRowNotification();

   /**
    * Dtor.
    */
   virtual ~CanAppendRowNotification();

   /**
    * Obtiene el valor para saber si se puede agregar o no una nueva fila.
    */
   bool CanAppendRow();

   /**
    * Establece el valor para saber si se puede agregar o no una nueva fila.
    */
   void CanAppendRow(bool CanAppendRow);

   /**
    * Metodo que ejecuta una accion sobre el observador a ser notificado.
    */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   bool canAppendRow_;  // Valor para saber si se puede agregar o no una nueva fila
};

}  // namespace suri

#endif  // CANAPPENDROWNOTIFICATION_H_
