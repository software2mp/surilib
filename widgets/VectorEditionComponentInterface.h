/*! \file VectorEditionComponentInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef VECTOREDITIONCOMPONENTINTERFACE_H_
#define VECTOREDITIONCOMPONENTINTERFACE_H_

// Includes Estandar
// Includes Suri
// Includes Wx
// Defines
// forwards

namespace suri {

/**
 * Interface que representa el comportamiento esperado de un componente que
 * participa de la edicion vectorial. Este tipo de componente debe reaccionar
 * frente al inicio/finalizacion de la edicion vectorial.
 */
class VectorEditionComponentInterface {
   /** ctor copia **/
   VectorEditionComponentInterface(const VectorEditionComponentInterface&);

public:
   /** ctor **/
   VectorEditionComponentInterface() {
   }
   virtual ~VectorEditionComponentInterface() {
   }
   /** Metodo que se llama cuando se inicializa la edicion vectorial. **/
   virtual void OnVectorEditionStarted()=0;
   /**  Metodo que se llama cuando finaliza la edicion vectorial.*/
   virtual void OnVectorEditionFinished()=0;
};

} /** namespace suri */

#endif /* VECTOREDITIONCOMPONENTINTERFACE_H_ */
