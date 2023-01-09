/** \file TaskInterface.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TASKINTERFACE_H_
#define TASKINTERFACE_H_

// Includes suri
#include "suri/SuriObject.h"

namespace suri {

/**
 * Interfaz que deben implementar todas las tareas.
 * Las tareas permiten organizar el trabajo realizado por el usuario.
 */
class TaskInterface : public SuriObject {
public:
   /** Dtor */
   virtual ~TaskInterface() {}
   /** Comienza la tarea */
   virtual bool Start() = 0;
   /** Finaliza la tarea. Intenta guardar los cambios. */
   virtual bool End() = 0;
   /** Cancela la tarea sin guardar */
   virtual bool Cancel() = 0;
   /** Indica si la tarea esta activa */
   virtual bool IsActive() const = 0;
};

} /* namespace suri */
#endif /* TASKINTERFACE_H_ */
