/*! \file ModelOrderChangeNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef MODELORDERCHANGENOTIFICATION_H_
#define MODELORDERCHANGENOTIFICATION_H_

// Includes Estandar
// Includes Suri
#include "suri/NotificationInterface.h"
#include "suri/ObserverInterface.h"
#include "TablePart.h"
// Includes Wx
// Defines
// forwards

namespace suri {
// forwards

class ModelOrderWidget;

/** Clase que representa una notification de cambio de orden del modelo de
 * transformacion
 **/
class ModelOrderChangeNotification : public suri::NotificationInterface {
   /** Constructor copia **/
   ModelOrderChangeNotification(const ModelOrderChangeNotification&);

public:
   /** Constructor **/
   ModelOrderChangeNotification(ModelOrderWidget *pOrderWidget, int Order);
   /** Destructor **/
   virtual ~ModelOrderChangeNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);
   /** Metodo que devuelve el valor RMSE por aproximacion polinomica **/
   double GetRmse();

private:
   int order_;
   double rmse_;
   ModelOrderWidget *pOrderWidget_;
};

} /** namespace suri */

#endif /* MODELORDERCHANGENOTIFICATION_H_ */
