/*! \file DatasourceUnblockedNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DATASOURCEUNBLOCKEDNOTIFICATION_H_
#define DATASOURCEUNBLOCKEDNOTIFICATION_H_


// Includes Estandar
// Includes Suri
#include "suri/NotificationInterface.h"
#include "suri/SuriObject.h"

// Includes Wx
// Defines
// forwards
namespace suri {

/** Clase que representa una notificacion de desbloqueo de fuente de datos **/
class DatasourceUnblockedNotification : public suri::NotificationInterface {
public:
   /** Constructor **/
   explicit DatasourceUnblockedNotification(const SuriObject::UuidType& DatasourceId);
   /** Destructor **/
   virtual ~DatasourceUnblockedNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   SuriObject::UuidType datasourceId_;

};
}   /** namespace suri**/
#endif /* DATASOURCEUNBLOCKEDNOTIFICATION_H_ */
