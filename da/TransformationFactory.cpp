/** \file TransformationFactory.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/TransformationFactory.h"

namespace suri {

const std::string TransformationFactory::kExact = "Exact";
const std::string TransformationFactory::kPolynomial = "Polynomial";
const std::string TransformationFactory::kTriangulation = "Triangulation";

const std::string TransformationFactory::kParamWktIn = "SRWktIn";
const std::string TransformationFactory::kParamWktOut = "SRWktOut";
const std::string TransformationFactory::kParamGcps = "Gcps";
const std::string TransformationFactory::kParamOrder = "Order";

/**
 * Destructor (elimina al sucesor)
 *
 * \todo (danieltesta-#2808) stub... implementar!
 */
TransformationFactory::~TransformationFactory() {
}

/**
 * Constructor protegido para evitar que se pueda crear un TransformFactory
 * pero que al mismo tiempo pueda ser utilizado por las clases que heredan para
 * configurar al sucesor.
 *
 * \todo (danieltesta-#2808) stub... implementar! configurar sucesor.
 */
TransformationFactory::TransformationFactory(TransformationFactory* pSuccessor) {
   SetSuccessor(pSuccessor);
}

}  // namespace suri
