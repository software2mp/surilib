/** \file StateChangeNotification.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef STATECHANGENOTIFICATION_H_
#define STATECHANGENOTIFICATION_H_

// Includes standard
#include <string>

// Includes sopi
#include "suri/NotificationInterface.h"

namespace suri {

/**
 * Notificacion generada cuando se quiere modificar la GUI de la aplicacion.
 */
class StateChangeNotification : public NotificationInterface {
public:
   /** Ctor */
   StateChangeNotification(const std::string EventId,
                                    const std::string SourceId);
   /** Dtor */
   virtual ~StateChangeNotification();

   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

   std::string GetEventId() { return eventId_; }

   std::string GetSourceId() { return sourceId_; }

private:
   /** Id del evento que modifico el estado de la aplicacion */
   std::string eventId_;
   /** Id del modulo que genero el evento. Puede ser un toolbar, command, etc */
   std::string sourceId_;
};

} /* namespace suri */

#endif /* STATECHANGENOTIFICATION_H_ */
