/*! \file UpdateProviderNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef UPDATEPROVIDERNOTIFICATION_H_
#define UPDATEPROVIDERNOTIFICATION_H_


// Includes Estandar

// Includes Suri
#include "suri/NotificationInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 * Clase que representa una notificacion sobre actualizacion del provider
 * de la tabla
 */
class UpdateProviderNotification : public suri::NotificationInterface {
public:
   UpdateProviderNotification();
   virtual ~UpdateProviderNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);
};

}  /** namespace suri */

#endif /* UPDATEPROVIDERNOTIFICATION_H_ */
