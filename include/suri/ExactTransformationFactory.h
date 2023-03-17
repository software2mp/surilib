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
