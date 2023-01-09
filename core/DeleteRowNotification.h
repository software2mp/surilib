/** \file DeleteRowNotification.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DELETEROWNOTIFICATION_H_
#define DELETEROWNOTIFICATION_H_

#include "suri/NotificationInterface.h"
#include "suri/ParameterCollection.h"


namespace suri {

/**
 * Notificacionp para el borrado de una fila.
 */
class DeleteRowNotification : public NotificationInterface {
public:
   /**
    * Ctor.
    */
   DeleteRowNotification();

   /**
    * Dtor.
    */
   virtual ~DeleteRowNotification();

   /**
    * Metodo que ejecuta una accion sobre el observador a ser notificado
    */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

   /**
    * Obtiene el dato extra pasado a la instancia
    */
   BaseParameter* GetTag();

   /**
    * Establece el dato extra para la instancia
    */
   void SetTag(BaseParameter* pTag);

private:
   BaseParameter* pTag_;
};

}  // namespace suri

#endif  // DELETEROWNOTIFICATION_H_
