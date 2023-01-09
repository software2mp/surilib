/*! \file DatasourceOrderChangeNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DATASOURCEORDERCHANGENOTIFICATION_H_
#define DATASOURCEORDERCHANGENOTIFICATION_H_

// Includes Estandar

// Includes Suri
#include "suri/NotificationInterface.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Clase que representa una notificacion de cambia de orden sobre la lista de fuente de datos
 */
class DatasourceOrderChangeNotification : public suri::NotificationInterface {
public:
   /** Constructor */
   DatasourceOrderChangeNotification(
         DatasourceInterface *pDatasource, DatasourceInterface *pDatasourceDestination = NULL);
   /** Destructor */
   virtual ~DatasourceOrderChangeNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   /** Constructor de copia */
   DatasourceOrderChangeNotification(const DatasourceOrderChangeNotification &);

   DatasourceInterface* pDatasource_;                    /*! Puntero a Datasource */
   DatasourceInterface* pDatasourceDestination_;         /*! Puntero a Datasource de destino */
};

} /** namespace suri */

#endif /* DATASOURCEORDERCHANGENOTIFICATION_H_ */
