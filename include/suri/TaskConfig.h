/** \file TaskConfig.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TASKCONFIG_H_
#define TASKCONFIG_H_

#define TC_OK                 0
#define TC_ERROR              1
#define TC_UNEXPECTED_ERROR   2
#define TC_BASE_USER          1024

namespace suri {

/**
 * Define la interfaz para la configuracion principal y para las
 * configuraciones que pueden ser agragadas dinamicamente a la principal.
 */
class TaskConfig {
public:
   /** dtor */
   virtual ~TaskConfig() {}
   /**
    * Metodo a implementar por la configuracion concreta que contiene
    * el procedimiento de configuracion en si.
    */
   virtual int Configure() = 0;

   /**
    * Metodo a implementar por la configuracion concreta que contiene
    * el procedimiento para configurar el entorno una vez finalizada
    * la tarea.
    */
   virtual int Clear() = 0;

   /**
    * Verifica si la configuracion tiene lo que necesita para seguir adelante.
    */
   virtual int Verify() const = 0;
};

}  // namespace suri

#endif  // TASKCONFIG_H_
