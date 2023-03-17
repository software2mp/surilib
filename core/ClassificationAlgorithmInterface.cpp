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

#include <string>

#include "ClassificationAlgorithmInterface.h"
#include "suri/AbstractFactory.h"

#define NDV_PIXEL_VALUE 255

namespace suri {

/** Macro de la factoria que crea metodos de implementacion */
HAS_FACTORY_IMPLEMENTATION_COMMON(ClassificationAlgorithmInterface, 0)

/** Macro de la factoria que crea metodos de implementacion */
HAS_FACTORY_IMPLEMENTATION(ClassificationAlgorithmInterface, 1,
                           DeclareParameters_1(const std::string&))

/** Ctor */
ClassificationAlgorithmInterface::ClassificationAlgorithmInterface() :
      ndvPixelValue_(NDV_PIXEL_VALUE) {

}

/** Dtor */
ClassificationAlgorithmInterface::~ClassificationAlgorithmInterface() {

}

/**
 * Configura el valor que el algoritmo asigna a los pixels con valor no valido.
 */
void ClassificationAlgorithmInterface::SetNDVPixelValue(int NDVPixelValue) {
   ndvPixelValue_ = NDVPixelValue;
}

/**
 * Retorna el valor que el algoritmo asigna a los pixels valor no valido.
 */
int ClassificationAlgorithmInterface::GetNDVPixelValue() const {
   return ndvPixelValue_;
}

}  // namespace suri
