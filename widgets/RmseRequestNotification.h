/*! \file RmseRequestNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RMSEREQUESTNOTIFICATION_H_
#define RMSEREQUESTNOTIFICATION_H_

// Includes Estandar

// Includes Suri
#include "suri/NotificationInterface.h"
#include "ModelOrderWidget.h"
// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Clase que representa una notificacion de solicitud para obtener
 * el valor de rmse que posee la transformacion
 */
class RmseRequestNotification : public suri::NotificationInterface {
   /** Constructor copia */
   RmseRequestNotification(const RmseRequestNotification&);

public:
   /** Constructor **/
   explicit RmseRequestNotification(ModelOrderWidget *pOrderWidget);
   /** Destructor **/
   virtual ~RmseRequestNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   /** Referencia al widget que se encarga de mostrar el orden
    * y error de la transformacion que se esta utilizando**/
   ModelOrderWidget *pOrderWidget_;
};

} /** namespace suri */

#endif /* RMSEREQUESTNOTIFICATION_H_ */
