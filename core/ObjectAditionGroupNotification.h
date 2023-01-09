/** \file ObjectAditionGroupNotification.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef OBJECTADITIONGROUPNOTIFICATION_H_
#define OBJECTADITIONGROUPNOTIFICATION_H_

// Includes standard
// Includes Suri
#include "suri/NotificationInterface.h"
#include "suri/SuriObject.h"
#include "suri/ViewcontextInterface.h"
// Includes Wx
// Includes App
// Defines

namespace suri {

/**
 * Notificacion sobre el agregado de un datasource a la lista unica de fuentes de datos
 */
class ObjectAditionGroupNotification : public suri::NotificationInterface {
public:
   /** Constructor */
   ObjectAditionGroupNotification(SuriObject::UuidType ObjectId, 
	 										SuriObject::UuidType Predecessor = SuriObject::NullUuid,
                                  ViewcontextInterface::ViewcontextType ctype = ViewcontextInterface::V2D);

   /** Destructor */
   virtual ~ObjectAditionGroupNotification();

   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

   /** */
   ViewcontextInterface::ViewcontextType GetContextType() { return ctype_; }

private:
   SuriObject::UuidType objectId_; /*! Id que quiero agregar al grupo */
	SuriObject::UuidType predecessor_;
   ViewcontextInterface::ViewcontextType ctype_;
};


} /* namespace suri */
#endif /* OBJECTADITIONGROUPNOTIFICATION_H_ */
