/** \file SuriObjectOrderChangeGroupNotification.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SURIOBJECTORDERCHANGEGROUPNOTIFICATION_H_
#define SURIOBJECTORDERCHANGEGROUPNOTIFICATION_H_

#include "suri/NotificationInterface.h"
#include "suri/SuriObject.h"

namespace suri {

/**
 * Evento generado cuando se produce un cambio sobre la lista de capas,
 * se propaga el cambio al arbol para que queden sincronizados.
 */
class SuriObjectOrderChangeGroupNotification : public suri::NotificationInterface {
public:
   /** Ctor */
   SuriObjectOrderChangeGroupNotification(SuriObject::UuidType ObjectId,
            SuriObject::UuidType DestinationObjectId  = SuriObject::NullUuid);
   /** Dtor */
   virtual ~SuriObjectOrderChangeGroupNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   /*! Id de objecto que se movio */
   SuriObject::UuidType objectId_;
   /*! Id de objecto en posicion donde se movio */
   SuriObject::UuidType destinationObjectId_;
};

} /* namespace suri */
#endif /* SURIOBJECTORDERCHANGEGROUPNOTIFICATION_H_ */
