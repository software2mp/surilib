/*! \file RmseChangeNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RMSECHANGENOTIFICATION_H_
#define RMSECHANGENOTIFICATION_H_

// Includes Estandar
#include "suri/NotificationInterface.h"
#include "suri/ObserverInterface.h"
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

class RmseChangeNotification : public suri::NotificationInterface {
public:
   RmseChangeNotification(double Rmse);
   virtual ~RmseChangeNotification();
   virtual bool ApplyNotification(ObserverInterface* pObserver);
private:
   double rmse_;
};

} /** namespace suri */

#endif /* RMSECHANGENOTIFICATION_H_ */
