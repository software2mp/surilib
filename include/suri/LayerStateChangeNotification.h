/*! \file LayerStateChangeNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LAYERSTATECHANGENOTIFICATION_H_
#define LAYERSTATECHANGENOTIFICATION_H_

// Includes Estandar
// Includes Suri
#include "suri/NotificationInterface.h"
#include "suri/SuriObject.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase que representa una notificacion sobre el cambio de estado de una capa **/
class LayerStateChangeNotification : public suri::NotificationInterface {
public:
   /** Constructor **/
   LayerStateChangeNotification();
   /** Destructor **/
   virtual ~LayerStateChangeNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   SuriObject::UuidType datasourceId_;
};

} /** namespace suri */

#endif /* LAYERSTATECHANGENOTIFICATION_H_ */
