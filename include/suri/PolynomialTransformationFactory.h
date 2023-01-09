/** \file PolynomialTransformationFactory.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef POLYNOMIALTRANSFORMATIONFACTORY_H_
#define POLYNOMIALTRANSFORMATIONFACTORY_H_

#include <cstddef>
#include <vector>
#include <string>

#include <ogr_spatialref.h>

#include "suri/TransformationFactory.h"
#include "suri/ParameterCollection.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/PolynomialCoordinatesTransformation.h"

#include "suri/Coordinates.h"
#include "suri/Subset.h"

namespace suri {

/**
 * Implementa el metodo de creacion "Create" para devolver una instancia en particular de
 * CoordinatesTransformation (transformacion polinomica). De no ser el responsable,
 * de la creacion de la transformacion, le pasa el pedido a su sucesor
 * (si este existe).
 */
class PolynomialTransformationFactory : public TransformationFactory {
public:
   /**
    * Construye una nueva instancia especificando al sucesor.
    */
   explicit PolynomialTransformationFactory(TransformationFactory* pSuccessor);

   /**
    * Destructor (elimina al sucesor)
    */
   virtual ~PolynomialTransformationFactory();

   /**
    * Implementa el metodo de la clase base para la creacion de una instancia de
    * PolynomialCoordinatesTransformation.
    */
   virtual CoordinatesTransformation* Create(const std::string& Type,
                                             const suri::ParameterCollection& ParamsP);

};

}  // namespace suri

#endif  // POLYNOMIALTRANSFORMATIONFACTORY_H_
