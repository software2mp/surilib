/*! \file ViewerBlockedNotification.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
// Includes Suri
#include "ViewerBlockedNotification.h"
#include "suri/PixelInfoTool.h"

// Includes Wx
// Defines
// forwards

namespace suri {

ViewerBlockedNotification::ViewerBlockedNotification() {
}

ViewerBlockedNotification::~ViewerBlockedNotification() {
}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool ViewerBlockedNotification::ApplyNotification(ObserverInterface* pObserver) {
   PixelInfoTool* pptool = dynamic_cast<PixelInfoTool*>(pObserver);
   if (pptool) {
      pptool->DisableCoordinatesInfo();
   }
   return true;
}
} /** namespace suri */
