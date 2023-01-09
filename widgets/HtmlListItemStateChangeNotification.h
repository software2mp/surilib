/*! \file HtmlListItemStateChangeNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef HTMLLISTITEMSTATECHANGENOTIFICATION_H_
#define HTMLLISTITEMSTATECHANGENOTIFICATION_H_

// Includes Estandar
// Includes Suri
#include "suri/NotificationInterface.h"
#include "suri/SuriObject.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Notificacion que es emitida cuando es cambiado el estado de un item. **/
class HtmlListItemStateChangeNotification : public NotificationInterface {
public:
   /** ctor **/
   HtmlListItemStateChangeNotification(const SuriObject::UuidType& ItemId,
                                       bool NewState);
   /** dtor **/
   virtual ~HtmlListItemStateChangeNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   SuriObject::UuidType itemId_;
   bool newState_;
};

} /** namespace suri */

#endif /* HTMLLISTITEMSTATECHANGENOTIFICATION_H_ */
