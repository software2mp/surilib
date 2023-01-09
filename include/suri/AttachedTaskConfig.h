/** \file AttachedTaskConfig.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef ATTACHEDTASKCONFIG_H_
#define ATTACHEDTASKCONFIG_H_

#include <cstddef>

#include "suri/TaskConfig.h"

namespace suri {

/**
 * Contiene una referencia a un objeto de configuracion (el objeto al que se le
 * agrega una configuracion extra). Este mismo, puede ser un agregado a otra
 * configuracion o no (en este caso se estaria tratando con una configuracion
 * principal)
 */
class AttachedTaskConfig : public TaskConfig {
public:
   /**
    * Constructor.
    */
   explicit AttachedTaskConfig(TaskConfig* pTaskConfigP);

   /**
    * Destructor.
    */
   virtual ~AttachedTaskConfig();

   /**
    * Metodo a implementar por la configuracion concreta que se esta
    * agregando que contiene el procedimiento de configuracion en si.
    */
   virtual int Configure();

   /**
    * Metodo a implementar por la configuracion concreta que contiene
    * el procedimiento para configurar el entorno una vez finalizada
    * la tarea.
    */
   virtual int Clear();

   /**
    * Verifica si la configuracion tiene lo que necesita para seguir adelante.
    */
   virtual int Verify() const;

private:
   TaskConfig* pTaskConfig_;  // Configuracion a la que se le agrega responsabilidad.
};

}  // namespace suri

#endif  // ATTACHEDTASKCONFIG_H_
