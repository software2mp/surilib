/** \file PolynomialTransformationFactory.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/PolynomialTransformationFactory.h"

namespace suri {

/**
 * Construye una nueva instancia especificando al sucesor.
 */
PolynomialTransformationFactory::PolynomialTransformationFactory(
      TransformationFactory* pSuccessor) : TransformationFactory(pSuccessor) {

}

/**
 * Destructor (elimina al sucesor)
 */
PolynomialTransformationFactory::~PolynomialTransformationFactory() {
   TransformationFactory* psuccessor = GetSuccessor();
   if (psuccessor != NULL) {
      delete psuccessor;
      psuccessor = NULL;
   }
}

/**
 * Implementa el metodo de la clase base para la creacion de una instancia de
 * PolynomialCoordinatesTransformation.
 */
CoordinatesTransformation* PolynomialTransformationFactory::Create(
      const std::string& Type, const suri::ParameterCollection& ParamsP) {

   if (Type.compare(suri::TransformationFactory::kPolynomial) == 0) {
      suri::ParameterCollection& params = const_cast<suri::ParameterCollection&>(ParamsP);

      int order;
      params.GetValue<int>(suri::TransformationFactory::kParamOrder, order);

      std::vector<GroundControlPoint> gcps;
      params.GetValue<std::vector<GroundControlPoint> >(suri::TransformationFactory::kParamGcps, gcps);

      PolynomialCoordinatesTransformation* polynomial =
            new PolynomialCoordinatesTransformation(order, gcps);

      if (polynomial->Initialize())
         return polynomial;
   } else {
      TransformationFactory* psuccessor = GetSuccessor();
      if (psuccessor != NULL)
         return psuccessor->Create(Type, ParamsP);
   }

   return NULL;
}

}  // namespace suri
