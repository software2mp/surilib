/** \file NewRowNotification.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef NEWROWNOTIFICATION_H_
#define NEWROWNOTIFICATION_H_

// Includes suri
#include "suri/NotificationInterface.h"

namespace suri {

/**
 * Le notifica a VectorDataLayer, el identificador de la fila
 * que se creo. Hasta que no se le asigne una geometria no se podran
 * agregar nuevas filas.
 * Cuando se cree un feature usando VectorDataLayer::CreateFeature, 
 * la geometria se asignara a esta fila.
 */
class NewRowNotification : public  suri::NotificationInterface {
public:
   /** Ctor */
   explicit NewRowNotification(long FeatureId);
   /** Dtor */
   virtual ~NewRowNotification();

   /** Metodo que ejecuta una accion sobre el observador a ser notificado */
   virtual bool ApplyNotification(ObserverInterface* pObserver);

private:
   /** Id de la fila creada en la tabla */
   long featureId_;
};

} /* namespace suri */
#endif /* NEWROWNOTIFICATION_H_ */
