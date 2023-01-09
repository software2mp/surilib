/*! \file ViewOrderChangeNotification.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar

// Includes Suri
#include "ViewOrderChangeNotification.h"
#include "LayerTreeEventHandler.h"
#include "suri/Element.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Constructor
 */
ViewOrderChangeNotification::ViewOrderChangeNotification(Element *pElement, Element *pElementDest,
                                                         const int &Movetype) :
      moveType_(Movetype), pElement_(pElement), pElementDest_(pElementDest) {
}

/**
 * Destructor
 */
ViewOrderChangeNotification::~ViewOrderChangeNotification() {
}

/**
 *  Indica al handler asociado que mueva la lista
 *  @param[in] Observer que es notificado el cambia de orden
 *  @return true en caso de poder cambiar el orden, false en caso contrario
 */
bool ViewOrderChangeNotification::ApplyNotification(ObserverInterface* pObserver) {
   LayerTreeEventHandler *phandler = dynamic_cast<LayerTreeEventHandler *>(pObserver);
   if (phandler == NULL)
      return false;
   switch (moveType_) {
      case MoveBefore:
         phandler->MoveNodeBefore(pElement_, pElementDest_);
         break;
      case MoveAfter:
         phandler->MoveNodeAfter(pElement_, pElementDest_);
         break;
      case MoveUnder:
         phandler->MoveNodeUnder(pElement_, pElementDest_);
         break;
      default:
         return false;
   }
   return true;
}

} /** namespace suri */
