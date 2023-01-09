/** \file SourceSrsChangeNotification.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SOURCESRSCHANGENOTIFICATION_H_
#define SOURCESRSCHANGENOTIFICATION_H_

// Includes standard
#include <string>
// Includes Suri
#include "suri/NotificationInterface.h"

namespace suri {

/**
 * Notification de cambio de sistema de referencia del origen de los puntos
 * de control
 **/
class SourceSrsChangeNotification  : public suri::NotificationInterface{
public:
   /** Ctor */
   SourceSrsChangeNotification(const std::string &SourceSrs);
   /** Dtor */
   virtual ~SourceSrsChangeNotification();

   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   std::string sourceSrs_;
};

} /* namespace suri */
#endif /* SOURCESRSCHANGENOTIFICATION_H_ */
