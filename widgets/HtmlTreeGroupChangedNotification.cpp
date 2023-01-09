/** \file HtmlTreeGroupChangedNotification.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "HtmlTreeGroupChangedNotification.h"
#include "suri/HtmlTreeWidget.h"

namespace suri {

HtmlTreeGroupChangedNotification::HtmlTreeGroupChangedNotification(
                                 GroupManagerInterface* pGroupManager) {
   pGroupManager_ = pGroupManager;
}

HtmlTreeGroupChangedNotification::~HtmlTreeGroupChangedNotification() {
}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool HtmlTreeGroupChangedNotification::ApplyNotification(
                                          ObserverInterface* pObserver) {
   // Es necesario realizar el casteo hacia un HtmlTreeWidget, ya que es necesario
   // aplicar la notificacion sobre una instancia de HtmlTreeWidget y no sobre
   // cualquier obeserver
   HtmlTreeWidget* ptreewidget = dynamic_cast<HtmlTreeWidget*>(pObserver);

   // Temporal hasta que se pueda verificar/actualizar el codigo para que
   // todas las notificaciones lleguen con ViewContextId.
   // Ticket #5741, Fix.

   if (ptreewidget != NULL) {
      SuriObject::UuidType viewcontextid = GetViewcontextId();
      if (viewcontextid.empty()) {
         ptreewidget->UpgradeControlContent();
         ptreewidget->ForceRefresh();
      } else if (ptreewidget->GetViewcontextId().compare(GetViewcontextId()) == 0) {
         ptreewidget->UpgradeControlContent();
         ptreewidget->ForceRefresh();
      }
   }

   // Codigo que deberia quedar cuando lleguen todas las notificaciones
   // con ViewContextId (NO BORRAR CODIGO)
   // if (ptreewidget != NULL && ptreewidget->GetViewcontextId().compare(GetViewcontextId()) == 0)
   // ptreewidget->UpgradeControlContent();

   return (ptreewidget != NULL);
}

} /* namespace suri */
