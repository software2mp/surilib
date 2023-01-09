/*! \file NotificationInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef NOTIFICATIONINTERFACE_H_
#define NOTIFICATIONINTERFACE_H_

// Includes Estandar

// Includes Suri

// Includes Wx

// Defines

namespace suri {

// forwards
class ObserverInterface;

/***
 * Interfaz que permite empaquetar notificaciones que envian los Subjects a sus Observers
 */
class NotificationInterface {
public:
   /** Constructor */
   NotificationInterface() {}
   /** Destructor */
   virtual ~NotificationInterface() {}
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver)=0;
};

} /** namespace suri */

#endif /* NOTIFICATIONINTERFACE_H_ */
