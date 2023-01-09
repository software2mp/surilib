/*! \file HtmlListItemStateChangeNotification.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "HtmlListItemStateChangeNotification.h"
#include "SpatialSubsetSelectionPart.h"

// Includes Wx
// Defines
// forwards

namespace suri {

HtmlListItemStateChangeNotification::HtmlListItemStateChangeNotification(
      const SuriObject::UuidType& ItemId, bool NewState) :
      itemId_(ItemId), newState_(NewState) {
}

HtmlListItemStateChangeNotification::~HtmlListItemStateChangeNotification() {
}
/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool HtmlListItemStateChangeNotification::ApplyNotification(
      ObserverInterface* pObserver) {
   SpatialSubsetSelectionPart* psubsetpart =
         dynamic_cast<SpatialSubsetSelectionPart*>(pObserver);
   if (psubsetpart) {
      psubsetpart->UpdateSubsetWithLayer(itemId_);
     return true;
   }
   return false;
}

} /** namespace suri */
