/*! \file DatasourceBlockedNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DATASOURCEBLOCKEDNOTIFICATION_H_
#define DATASOURCEBLOCKEDNOTIFICATION_H_


// Includes Estandar
// Includes Suri
#include "suri/SuriObject.h"
#include "suri/NotificationInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase que representa una notificacion de bloqueo de fuente de datos **/
class DatasourceBlockedNotification : public suri::NotificationInterface {
public:
   /** Constructor **/
   explicit DatasourceBlockedNotification(const SuriObject::UuidType& DatasourceId);
   /** Destructor */
   virtual ~DatasourceBlockedNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   SuriObject::UuidType datasourceId_;
};

}  /** namespace suri **/

#endif /* DATASOURCEBLOCKEDNOTIFICATION_H_ */
