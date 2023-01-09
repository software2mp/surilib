/** \file HtmlTreeGroupChangedNotification.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef HTMLTREEGROUPCHANGEDNOTIFICATION_H_
#define HTMLTREEGROUPCHANGEDNOTIFICATION_H_

#include "suri/NotificationInterface.h"
#include "suri/GroupManagerInterface.h"

namespace suri {

class HtmlTreeGroupChangedNotification : public suri::NotificationInterface {
public:
   /** Ctor */
   explicit HtmlTreeGroupChangedNotification(GroupManagerInterface* pGroupManager);
   /** Dtor */
   virtual ~HtmlTreeGroupChangedNotification();

   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

   /** Setea el context id*/
   void SetViewcontextId(const SuriObject::UuidType& ViewcontextId) {
	   viewcontextId_ = ViewcontextId;
   }

   /** Devuelve el context id*/
   SuriObject::UuidType GetViewcontextId() {
	   return viewcontextId_;
   }

private:
   /** GroupManager que genero la notificacion */
   GroupManagerInterface* pGroupManager_;
   SuriObject::UuidType viewcontextId_;
};

} /* namespace suri */
#endif /* HTMLTREEGROUPCHANGEDNOTIFICATION_H_ */
