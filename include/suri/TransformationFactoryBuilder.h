/** \file TransformationFactoryBuilder.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef TRANSFORMATIONFACTORYBUILDER_H_
#define TRANSFORMATIONFACTORYBUILDER_H_

#include <cstddef>

#include "suri/TransformationFactory.h"
#include "suri/ExactTransformationFactory.h"
#include "suri/PolynomialTransformationFactory.h"

namespace suri {

/**
 * Es la clase (auxiliar) donde se declara el metodo "Build" para la construccion
 * de la cadena de factorias (TransformationFactory). Tambien provee un metodo
 * "Release" para liberar la memoria.
 */
class TransformationFactoryBuilder {
public:
   /**
    * Destructor
    */
   virtual ~TransformationFactoryBuilder() { }

   /**
    * Construye la cadena de reponsabilidad con todas las TransformationFactory disponibles.
    */
   static TransformationFactory* Build();

   /**
    * Libera la memoria reservada por la cadena (metodo utilizado por el codigo cliente).
    */
   static void Release(TransformationFactory*& Factory);

private:
   /**
    * Constructor privado. Esta clase no debe ser instanciable.
    */
   TransformationFactoryBuilder() { }
};

}  // namespace suri

#endif  // TRANSFORMATIONFACTORYBUILDER_H_
