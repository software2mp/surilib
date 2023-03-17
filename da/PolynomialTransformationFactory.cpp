/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
