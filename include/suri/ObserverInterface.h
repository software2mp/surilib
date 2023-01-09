/*! \file ObserverInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef OBSERVERINTERFACE_H_
#define OBSERVERINTERFACE_H_

// Includes Estandar

// Includes Suri
#include "NotificationInterface.h"

// Includes Wx

// Defines

namespace suri {

// forwards
class Subject;

/**
 * Interfaz de Observador
 * En correspondencia con el patron Observer, una clase debe heredar de ObserverInterface
 * e implementar el metodo Update para recibir notificaciones de un Subject al cual se
 * suscribio mediante el metodo Attach
 */
class ObserverInterface {
public:
   /** Destructor */
   virtual ~ObserverInterface() {}
   /** Metodo a ser redefinido para recibir Notificaciones de un Sujeto (Observable) */
   virtual void Update(NotificationInterface* pNotification)=0;

protected:
   /** Constructor protegido */
   ObserverInterface() {}
};

} /** namespace suri */

#endif /* OBSERVERINTERFACE_H_ */
