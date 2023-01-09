/*! \file ViewerBlockedNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWBERBLOCKEDNOTIFICATION_H_
#define VIEWBERBLOCKEDNOTIFICATION_H_


// Includes Estandar
// Includes Suri
#include "suri/NotificationInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {
/** Notificacion de visualizador bloqueado. **/
class ViewerBlockedNotification : public suri::NotificationInterface {
public:
   ViewerBlockedNotification();
   virtual ~ViewerBlockedNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);
};

} /** namespace suri */

#endif /* VIEWBERBLOCKEDNOTIFICATION_H_ */
