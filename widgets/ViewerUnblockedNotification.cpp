/*! \file ViewerUnblockedNotification.cpp */
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
#include "ViewerUnblockedNotification.h"
#include "suri/PixelInfoTool.h"

// Includes Wx
// Defines
// forwards

namespace suri {

ViewerUnblockedNotification::ViewerUnblockedNotification() {
}

ViewerUnblockedNotification::~ViewerUnblockedNotification() {
}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool ViewerUnblockedNotification::ApplyNotification(ObserverInterface* pObserver) {
   PixelInfoTool* pptool = dynamic_cast<PixelInfoTool*>(pObserver);
   if (pptool) {
      pptool->EnableCoordinatesInfo();
   }
   return true;
}

} /** namespace suri */
