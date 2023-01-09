/*! \file Subject.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SUBJECT_H_
#define SUBJECT_H_

// Includes Estandar
#include <map>
#include <utility>

// Includes Suri
#include "ObserverInterface.h"
#include "NotificationInterface.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/**
 * Un sujeto que puede ser observado por observadores para que estos puedan recibir notificaciones
 */
class Subject {
public:
   /** Destructor */
   virtual ~Subject();
   /** Registra un Observer para que pueda recibir notificaciones */
   int Attach(ObserverInterface* pObserver);
   /** Quita un Observer para que pueda recibir notificaciones */
   bool Detach(int ObserverId);
   /** Notifica a todos los observadores sobre una modificacion del sujeto */
   void Notify(NotificationInterface* pNotification);

   static const int InvalidCount = -1; /*! Valor si hay errores en Attach/Detach de Observers */

protected:
   /** Constructor */
   Subject();

private:
   typedef std::map<int, ObserverInterface*> SubjectHashmapType; /*! Typedef para el mapa */
   typedef std::pair<int, ObserverInterface*> SubjectPairType; /*! Typedef para el par (del mapa) */
   typedef std::pair<SubjectHashmapType::iterator, bool> SubjectAddResultPairType; /*! Resultado */
   SubjectHashmapType observers_; /*! Mapa para esta implementacion de Subject */
   int attachCount_; /*! Contador de llamadas al metodo Attach*/
};

} /** namespace suri */

#endif /* SUBJECT_H_ */
