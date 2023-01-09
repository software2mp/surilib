/** \file SuriObjectRemoveGroupNotification.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SURIOBJECTREMOVEGROUPNOTIFICATION_H_
#define SURIOBJECTREMOVEGROUPNOTIFICATION_H_

#include "suri/NotificationInterface.h"
#include "suri/SuriObject.h"

namespace suri {

class SuriObjectRemoveGroupNotification : public suri::NotificationInterface {
public:
   /** Ctor */
   explicit SuriObjectRemoveGroupNotification(SuriObject::UuidType ObjectId);
   /** Dtor*/
   virtual ~SuriObjectRemoveGroupNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   SuriObject::UuidType objectId_; /*! Id que quiero agregar al grupo */
};

} /* namespace suri */
#endif /* SURIOBJECTREMOVEGROUPNOTIFICATION_H_ */
