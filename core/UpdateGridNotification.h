/*! \file UpdateGridNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef UPDATEGRIDNOTIFICATION_H_
#define UPDATEGRIDNOTIFICATION_H_


// Includes Estandar
// Includes Suri
#include "suri/NotificationInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {
/*!
 * Clase que representa una notificacion
 * sobre la actualizacion de la tabla en si
 */
class UpdateGridNotification : public suri::NotificationInterface {
public:
   UpdateGridNotification();
   virtual ~UpdateGridNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);
};

}  /** namespace suri **/
#endif /* UPDATEGRIDNOTIFICATION_H_ */
