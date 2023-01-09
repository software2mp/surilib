/*! \file RegisterEditionStartedNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef REGISTEREDITIONSTARTEDNOTIFICATION_H_
#define REGISTEREDITIONSTARTEDNOTIFICATION_H_

// Includes Estandar
// Includes Suri
#include "suri/NotificationInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 *  Clase que representa una notificacion de inicio de edicion
 *  de un registro
 */
class RegisterEditionStartedNotification : public NotificationInterface {
public:
   RegisterEditionStartedNotification();
   virtual ~RegisterEditionStartedNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);
};

} /** namespace suri */

#endif /* REGISTEREDITIONSTARTEDNOTIFICATION_H_ */
