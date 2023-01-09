/*! \file DatasourceRemovalNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DATASOURCEREMOVALNOTIFICATION_H_
#define DATASOURCEREMOVALNOTIFICATION_H_

// Includes Estandar

// Includes Suri
#include "suri/NotificationInterface.h"
#include "suri/SuriObject.h"
#include "suri/ObserverInterface.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {


/**
 * Notificacion sobre la eliminacion de un datasource a la lista unica de fuentes de datos
 */
class DatasourceRemovalNotification : public suri::NotificationInterface {
public:
   /** Constructor */
   explicit DatasourceRemovalNotification(DatasourceInterface* pDatasource);
   /** Destructor */
   virtual ~DatasourceRemovalNotification();
   /** Elimina el datasource de la lista del observer */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   SuriObject::UuidType datasourceId_;          /*! Id del datasource */
};

} /** namespace suri */

#endif /* DATASOURCEREMOVALNOTIFICATION_H_ */
