/** \file SuriObjectOrderChangeGroupNotification.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "SuriObjectOrderChangeGroupNotification.h"
#include "SuriObjectGroupManager.h"
#include "TreeNodeAuxiliaryFunctions.h"

namespace suri {

SuriObjectOrderChangeGroupNotification::SuriObjectOrderChangeGroupNotification(
                                          SuriObject::UuidType ObjectId,
                                          SuriObject::UuidType DestinationObjectId) {
   objectId_ = ObjectId;
   destinationObjectId_ = DestinationObjectId;
}

SuriObjectOrderChangeGroupNotification::~SuriObjectOrderChangeGroupNotification() {
}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool SuriObjectOrderChangeGroupNotification::ApplyNotification(
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
      if (!nodepath.IsValid())
         return false;

      // Si no hay nodo destino trato de moverlo al final del arbol.
      TreeNodeInterface* pdestinationgroup = pworkgroup->GetRoot();
      int destinationindex = pdestinationgroup->GetSize();

      // Busco nodo destino
      if (destinationObjectId_ != SuriObject::NullUuid) {
         TreeNodeInterface* pdestinationnode = pworkgroup->SearchContent(
                                      destinationObjectId_).GetLastPathNode();
         if (!pdestinationnode)
            return false;
         pdestinationgroup = pdestinationnode->GetParent();
         destinationindex = GetIndexOfChild(pdestinationgroup, pdestinationnode);
      }

      success = pworkgroup->MoveNode(nodepath,
                                  pworkgroup->GetPathToRoot(pdestinationgroup),
                                  destinationindex).IsValid();
   }
   return success;
}

} /* namespace suri */
