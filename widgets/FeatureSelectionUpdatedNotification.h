/*! \file FeatureSelectionUpdatedNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FEATURESELECTIONUPDATEDNOTIFICATION_H_
#define FEATURESELECTIONUPDATEDNOTIFICATION_H_

// Includes Estandar
#include <set>

// Includes Suri
#include "suri/NotificationInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 * Notificacion encargada de comunicar un cambio en la seleccion desde la tabla.
 */
class FeatureSelectionUpdatedNotification : public suri::NotificationInterface {
public:
   /** Ctor */
   FeatureSelectionUpdatedNotification();
   /** Dtor */
   virtual ~FeatureSelectionUpdatedNotification();
   /** Actualiza la seleccion de features sobre el observer correspondiente */
   virtual bool ApplyNotification(ObserverInterface* pObserver);
};

} /** namespace suri */

#endif /* FEATURESELECTIONUPDATEDNOTIFICATION_H_ */
