/** \file TreeNodeDoubleClickNotification.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "TreeNodeDoubleClickNotification.h"
#include "SpatialSubsetSelectionPart.h"

namespace suri {

/** Ctor */
TreeNodeDoubleClickNotification::TreeNodeDoubleClickNotification(
                                       const NodePath &EventNodePath) :
                                          eventNodePath_(EventNodePath) {

}

/** Dtor */
TreeNodeDoubleClickNotification::~TreeNodeDoubleClickNotification() {
}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool TreeNodeDoubleClickNotification::ApplyNotification(
                                          ObserverInterface* pObserver) {
   SpatialSubsetSelectionPart* psubsetpart =
         dynamic_cast<SpatialSubsetSelectionPart*>(pObserver);
   if (psubsetpart) {
      SuriObject::UuidType datasourceid = eventNodePath_.GetLastPathNode()->GetContent();
      psubsetpart->UpdateSubsetWithDatasource(datasourceid);
   }
   return psubsetpart != NULL;
}

} /* namespace suri */
