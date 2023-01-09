/*! \file Subject.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <map>
#include <utility>

// Includes Suri
#include "suri/Subject.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
Subject::Subject() :
      observers_(), attachCount_(InvalidCount) {
}

/**
 * Destructor
 */
Subject::~Subject() {
}

/**
 * Registra un Observer para que pueda recibir notificaciones
 */
int Subject::Attach(ObserverInterface* pObserver) {
   attachCount_++;
   SubjectAddResultPairType result =
         observers_.insert(std::make_pair(attachCount_, pObserver));
   if (result.second == false) {
      return Subject::InvalidCount;
   }
   return attachCount_;
}

/**
 * Quita un Observer para que pueda recibir notificaciones
 */
bool Subject::Detach(int ObserverId) {
   int erased = observers_.erase(ObserverId);
   return (erased > 0);
}

/**
 * Notifica a todos los observadores sobre una modificacion del sujeto
 */
void Subject::Notify(NotificationInterface* pNotification) {
   SubjectHashmapType::const_iterator it = observers_.begin();
   for (; it != observers_.end(); it++) {
      it->second->Update(pNotification);
   }
}

} /** namespace suri */
