/** \file ExactTransformationFactory.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef EXACTTRANSFORMATIONFACTORY_H_
#define EXACTTRANSFORMATIONFACTORY_H_

#include <cstddef>
#include <vector>
#include <string>

#include <ogr_spatialref.h>

#include "suri/TransformationFactory.h"
#include "suri/ParameterCollection.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ExactCoordinatesTransformation.h"

// Se necesita este include para SpatialReference
// (SACAR CUANDO SE PUEDA ARREGLAR LA REFERENCIA)
#include "suri/CoordinatesTransform.h"

#include "suri/Coordinates.h"
#include "suri/Subset.h"
#include "SpatialReference.h"
#include "suri/Wkt.h"

namespace suri {

/**
 * Implementa el metodo de creacion "Create" para devolver una instancia en particular de
 * CoordinatesTransformation (transformacion exacta). De no ser el responsable,
 * de la creacion de la transformacion, le pasa el pedido a su sucesor
 * (si este existe).
 */
class ExactTransformationFactory : public TransformationFactory {
public:
   /**
    * Construye una nueva instancia especificando al sucesor.
    */
   explicit ExactTransformationFactory(TransformationFactory* pSuccessor);

   /**
    * Destructor (elimina al sucesor)
    */
   virtual ~ExactTransformationFactory();

   /**
    * Implementa el metodo de la clase base para la creacion de una instancia de
    * ExactCoordinatesTransformation.
    */
   virtual CoordinatesTransformation* Create(const std::string& Type, const suri::ParameterCollection& ParamsP);

private:
   /**
    * Crea el WKT de salida.
    */
   std::string GetWktOut(std::string& WktIn);
};

}  // namespace suri

#endif  // EXACTTRANSFORMATIONFACTORY_H_
