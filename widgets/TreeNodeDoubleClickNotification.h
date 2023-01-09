/** \file TreeNodeDoubleClickNotification.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TREENODEDOUBLECLICKNOTIFICATION_H_
#define TREENODEDOUBLECLICKNOTIFICATION_H_

// Includes suri
#include "suri/NotificationInterface.h"
#include "suri/NodePath.h"

namespace suri {

class TreeNodeDoubleClickNotification : public NotificationInterface {
public:
   /** Ctor */
   explicit TreeNodeDoubleClickNotification(const NodePath &EventNodePath);
   /** Dtor */
   virtual ~TreeNodeDoubleClickNotification();

   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   /** Path del nodo sobre el que se hizo dobre click */
   NodePath eventNodePath_;
};

} /* namespace suri */
#endif /* TREENODEDOUBLECLICKNOTIFICATION_H_ */
