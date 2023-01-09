/*! \file DatasourceAddtitionNotification.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef DATASOURCEADDTITIONNOTIFICATION_H_
#define DATASOURCEADDTITIONNOTIFICATION_H_

// Includes Estandar

// Includes Suri
#include "suri/NotificationInterface.h"
#include "suri/ObserverInterface.h"
#include "suri/DatasourceInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Notificacion sobre el agregado de un datasource a la lista unica de fuentes de datos
 */
class DatasourceAddtitionNotification : public suri::NotificationInterface {
public:
   /** Constructor */
	DatasourceAddtitionNotification(DatasourceInterface* pDatasource,
			const suri::SuriObject::UuidType& ViewcontextId =
					SuriObject::NullUuid, bool Update = true);
   /** Destructor */
   virtual ~DatasourceAddtitionNotification();
   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);
   /** Devuelve el resultado de aplicar la notificacion **/
   bool GetApplyResult() const;

   /** */
   suri::SuriObject::UuidType GetViewcontextId() { return viewcontextId_; }

private:
   DatasourceInterface* pDatasource_;     /*! Fuente de datos a ser agregado */
   bool update_;                          /*! Flag de update_ */
   suri::SuriObject::UuidType viewcontextId_; /*! Identifica el contexto */
   /** bool que guarda el resultado **/
   bool applyOk_;
};

} /** namespace suri */

#endif /* DATASOURCEADDTITIONNOTIFICATION_H_ */
