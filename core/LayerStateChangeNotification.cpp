/*! \file LayerStateChangeNotification.cpp */
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
#include "suri/LayerStateChangeNotification.h"
#include "suri/HtmlTreeWidget.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/** Constructor **/
LayerStateChangeNotification::LayerStateChangeNotification() {
}
/** Destructor **/
LayerStateChangeNotification::~LayerStateChangeNotification() {
}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool LayerStateChangeNotification::ApplyNotification(ObserverInterface* pObserver) {
   // Es necesario realizar el casteo hacia un HtmlTreeWidget, ya que es necesario
   // aplicar la notificacion sobre una instancia de HtmlTreeWidget y no sobre
   // cualquier obeserver
   HtmlTreeWidget* ptreewidget = dynamic_cast<HtmlTreeWidget*>(pObserver);
   if (ptreewidget != NULL)
      ptreewidget->UpgradeControlContent();
   return true;
}
} /** namespace suri */
