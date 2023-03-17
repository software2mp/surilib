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
