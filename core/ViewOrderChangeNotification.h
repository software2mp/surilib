/*! \file ViewOrderChangeNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VIEWORDERCHANGENOTIFICATION_H_
#define VIEWORDERCHANGENOTIFICATION_H_

// Includes Estandar

// Includes Suri
#include "suri/NotificationInterface.h"
#include "suri/Element.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Clase que notifica a un observador que se ha cambiado el orden en la vista.
 */
class ViewOrderChangeNotification : public suri::NotificationInterface {
public:
   /** Constructor */
   ViewOrderChangeNotification(Element* pElement, Element* pElementDest, const int &Movetype);
   /** Destructor */
   virtual ~ViewOrderChangeNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   /** Constructor de Copia */
   ViewOrderChangeNotification(const ViewOrderChangeNotification &);
   /** Tipo de Movimiento: "Mover despues de" */
   static const int MoveAfter = 0;
   /** Tipo de Movimiento: "Mover al final" */
   static const int MoveUnder = 1;
   /** Tipo de Movimiento: "Mover antes de" */
   static const int MoveBefore = 2;
   /** Tipo de movimiento indicado*/
   int moveType_;
   /** Movimiento sobre el cual se mueve */
   Element* pElement_;
   /** Elemento que esta en la posicion donde se desea mover el element */
   Element* pElementDest_;
};

} /** namespace suri */

#endif /* VIEWORDERCHANGENOTIFICATION_H_ */
