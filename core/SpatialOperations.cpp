/*! \file SpatialOperations.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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

