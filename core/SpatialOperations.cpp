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

// Includes standard
// Includes Suri
#include "SpatialOperations.h"
#include "suri/CoordinatesTransformation.h"
#include "logmacros.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/**
 * Calcula la interseccion de los extendst de los elementos y
 * @param[in] pFirstElement primer elemento a intersectar
 * @param[in] pSecondElement segundo elemento a intersectar
 * @param[out] Intersection subset con la interseccion de los extends de los elementos. Esta
 * en el sistema de referencia del primer elemnento.
 * @return informa si la operacion fue exitosa
 */
bool IntersectExtents(Element* pFirstElement, Element* pSecondElement,
                      Extent &Intersection) {
   Subset firstsubset;
   pFirstElement->GetElementExtent(firstsubset);
   Subset secondsubset;
   pSecondElement->GetElementExtent(secondsubset);
   ParameterCollection params;
     params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                  pSecondElement->GetSpatialReference().c_str());
     params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                  pFirstElement->GetSpatialReference().c_str());
     TransformationFactory* pfactory =
           TransformationFactoryBuilder::Build();
     CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                              params);
     TransformationFactoryBuilder::Release(pfactory);

   if (pct == NULL) {
      REPORT_DEBUG("D:Error al crear CoordinatesTransform.");
      return false;
   }

   pct->Transform(secondsubset, _SPATIAL_OPERATIONS_EXTENT_POINTS_);

   // Verifico que el vector este contenido en el extend transformado
   Intersection = Intersect(Extent(secondsubset), Extent(firstsubset));
   delete pct;
   return true;
}
}

