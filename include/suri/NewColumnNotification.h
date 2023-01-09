/*! \file NewColumnNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef NEWCOLUMNNOTIFICATION_H_
#define NEWCOLUMNNOTIFICATION_H_


// Includes Estandar
// Includes Suri
#include "NotificationInterface.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Clase que representa una notificacion de agregad de columna **/
class NewColumnNotification : public suri::NotificationInterface {
   /** ctor copia **/
   NewColumnNotification(const NewColumnNotification&);

public:
   /** Constructor */
   explicit NewColumnNotification(int ColId);
   /** Destructor **/
   virtual ~NewColumnNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   int colId_;
};

} /** namespace suri */

#endif /* NEWCOLUMNNOTIFICATION_H_ */
