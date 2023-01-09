/** \file ObjectAditionGroupNotification.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "ObjectAditionGroupNotification.h"
#include "SuriObjectGroupManager.h"

namespace suri {

/**
 * Ctor.
 */
ObjectAditionGroupNotification::ObjectAditionGroupNotification(
      SuriObject::UuidType ObjectId, SuriObject::UuidType Predecessor, 
      ViewcontextInterface::ViewcontextType CType) : objectId_(ObjectId), 
		predecessor_(Predecessor), ctype_(CType) {
}

/**
 * Dtor.
 */
ObjectAditionGroupNotification::~ObjectAditionGroupNotification() {

}

/** Metodo que ejecuta una accion sobre el observador a ser notificado */
bool ObjectAditionGroupNotification::ApplyNotification(
                                       ObserverInterface* pObserver) {
   // Es necesario realizar el casteo hacia un SuriObjectGroupManager
   // para no aplicar el cambio sobre cualquier observer
   SuriObjectGroupManager* pgroupmanager =
                  dynamic_cast<SuriObjectGroupManager*>(pObserver);
   bool success = false;
   if (pgroupmanager) {
      MutableWorkGroupInterface* pworkgroup =
                                pgroupmanager->GetRootMutableWorkGroup();
      NodePath spath = pworkgroup->SearchContent(objectId_);
      if (!spath.IsValid()) {
			int parentindex = pworkgroup->GetRootSize();
			if (predecessor_.compare(SuriObject::NullUuid) != 0) {
				parentindex = 0;
				TreeIterator iterator = pworkgroup->GetIterator(pworkgroup->GetRootPath());
				TreeNodeInterface* pnode = iterator.GetCurrent();
				int index = 0;
				while (pnode) {
					if (pnode->GetContent().compare(predecessor_) == 0) {
						parentindex = index > 0 ? index - 1 : index;
						break;
					}
					iterator.NextNode();
					pnode = iterator.GetCurrent();
					++index;
				}
			}
         pworkgroup->InsertObject(objectId_, pworkgroup->GetRootPath(),
                                  parentindex, GetContextType());
      }
      success = true;
   }
   return success;
}

} /* namespace suri */
