/*! \file FeatureSelectionObserver.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef FEATURESELECTIONOBSERVER_H_
#define FEATURESELECTIONOBSERVER_H_

// Includes Estandar
#include <set>

// Includes Suri
#include "suri/ObserverInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/*!
 * Clase abstracta que representa una clase que observa el cambio sobre la seleccion.
 */
class FeatureSelectionObserver : public suri::ObserverInterface {
public:
   /** ctor **/
   FeatureSelectionObserver();
   /** dtor **/
   virtual ~FeatureSelectionObserver();
   /** Aplica la notificacion de actualizacion de seleccion sobre el observer */
   virtual void Update(NotificationInterface* pNotification);
   /** Metodo que es llamado cuando se actualiza la seleccion. */
   virtual void SelectionUpdated() = 0;
};

} /** namespace suri */

#endif /* FEATURESELECTIONOBSERVER_H_ */
