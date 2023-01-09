/** \file TransformationFactoryBuilder.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/TransformationFactoryBuilder.h"

namespace suri {

/**
 * Construye la cadena de reponsabilidad con todas las TransformationFactory disponibles.
 *
 * @return Una cadena de factorias.
 *
 * \todo (danieltesta-#2808) stub... implementar!
 */
TransformationFactory* TransformationFactoryBuilder::Build() {
   TransformationFactory* polynomial = new PolynomialTransformationFactory(NULL);
   TransformationFactory* exact = new ExactTransformationFactory(polynomial);
   return exact;
}

/**
 * Libera la memoria reservada por la cadena (metodo utilizado por el codigo cliente).
 *
 * @param[in] factory Cadena de factorias para liberar.
 *
 * \todo (danieltesta-#2808) stub... implementar!
 */
void TransformationFactoryBuilder::Release(TransformationFactory*& Factory) {
   if (Factory != NULL) {
      delete Factory;
      Factory = NULL;
   }
}

} // namespace suri
