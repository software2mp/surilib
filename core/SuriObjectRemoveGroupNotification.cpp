/** \file SuriObjectRemoveGroupNotification.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "SuriObjectRemoveGroupNotification.h"
#include "SuriObjectGroupManager.h"

namespace suri {

SuriObjectRemoveGroupNotification::SuriObjectRemoveGroupNotification(
                                             SuriObject::UuidType ObjectId) {
   objectId_ = ObjectId;
}

SuriObjectRemoveGroupNotification::~SuriObjectRemoveGroupNotification() {
}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool SuriObjectRemoveGroupNotification::ApplyNotification(
                                             ObserverInterface* pObserver) {
   // Es necesario realizar el casteo hacia un SuriObjectGroupManager
   // para no aplicar el cambio sobre cualquier observer
   SuriObjectGroupManager* pgroupmanager =
                  dynamic_cast<SuriObjectGroupManager*>(pObserver);
   bool success = false;
   if (pgroupmanager) {
      MutableWorkGroupInterface* pworkgroup =
                                pgroupmanager->GetRootMutableWorkGroup();
      NodePath nodepath = pworkgroup->SearchContent(objectId_);
      if (nodepath.IsValid()) {
         pworkgroup->Remove(nodepath);
         success = true;
      }
   }
   return success;
}

} /* namespace suri */
