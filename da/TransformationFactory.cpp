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
