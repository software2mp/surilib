/*! \file ViewerUnblockedNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWERUNBLOCKEDNOTIFICATION_H_
#define VIEWERUNBLOCKEDNOTIFICATION_H_

// Includes Estandar
// Includes Suri
#include "suri/NotificationInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

class ViewerUnblockedNotification : public suri::NotificationInterface {
public:
   ViewerUnblockedNotification();
   virtual ~ViewerUnblockedNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);
};

} /** namespace suri */

#endif /* VIEWERUNBLOCKEDNOTIFICATION_H_ */
