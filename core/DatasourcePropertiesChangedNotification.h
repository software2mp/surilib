/*! \file DatasourcePropertiesChangedNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DATASOURCEPROPERTIESCHANGEDNOTIFICATION_H_
#define DATASOURCEPROPERTIESCHANGEDNOTIFICATION_H_

// Includes Estandar
#include <string>

// Includes Suri
#include "suri/NotificationInterface.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Notificacion de cambio en las propiedades
 */
class DatasourcePropertiesChangedNotification : public suri::NotificationInterface {
public:
   /** Enumerado del tipo de propiedad */
   typedef enum {
      Name = 0, Copyright = 1, Description = 2, Url = 3, Icon = 4
   } PropertiesEnum;
   /** Constructor */
   DatasourcePropertiesChangedNotification(DatasourceInterface *pDatasource,
                                           const std::string &Value,
                                           PropertiesEnum Property = Name);
   /** Destructor */
   virtual ~DatasourcePropertiesChangedNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   DatasourceInterface* pDatasource_;        /*! Puntero al datasource */
   PropertiesEnum property_;                 /*! Propiedad */
   std::string value_;                       /*! Valor */
};

} /** namespace suri */

#endif /* DATASOURCEPROPERTIESCHANGEDNOTIFICATION_H_ */
