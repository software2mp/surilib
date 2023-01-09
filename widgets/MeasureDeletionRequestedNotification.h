/** \file MeasureDeletionRequestedNotification.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MEASUREDELETIONREQUESTEDNOTIFICATION_H_
#define MEASUREDELETIONREQUESTEDNOTIFICATION_H_

#include "suri/NotificationInterface.h"

namespace suri {

/**
 * Envia una solicitud al editor para eliminar una geometria (medida)
 */
class MeasureDeletionRequestedNotification : public NotificationInterface {
public:
   /**
    * Ctor.
    */
   MeasureDeletionRequestedNotification() { }

   /**
    * Dtor.
    */
   ~MeasureDeletionRequestedNotification() { }

   /**
    * Metodo que ejecuta una accion sobre el observador a ser notificado.
    */
   virtual bool ApplyNotification(ObserverInterface* pObserver) {
      if (pObserver != NULL)
         pObserver->Update(this);
      return true;
   }
};

}  // namespace suri

#endif  // MEASUREDELETIONREQUESTEDNOTIFICATION_H_
