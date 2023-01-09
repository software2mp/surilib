/** \file StateChangeNotification.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>

// Includes suri
#include "suri/StateChangeNotification.h"
#include "suri/GuiStateChangeMediator.h"

namespace suri {

/** Ctor */
StateChangeNotification::StateChangeNotification(const std::string EventId,
                                                   const std::string SourceId) {
   eventId_ = EventId;
   sourceId_ = SourceId;
}

/** Dtor */
StateChangeNotification::~StateChangeNotification() {
}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool StateChangeNotification::ApplyNotification(ObserverInterface* pObserver) {
   GuiStateChangeMediator* pmediator = dynamic_cast<GuiStateChangeMediator*>(pObserver);
   if (pmediator)
      pmediator->StateChangeEvent(eventId_, sourceId_);
   else
      pObserver->Update(this);

   return pmediator != NULL;
}

} /* namespace suri */
