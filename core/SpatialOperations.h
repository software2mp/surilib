/*! \file SpatialOperations.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SPATIALOPERATIONS_H_
#define SPATIALOPERATIONS_H_

// Includes standard
// Includes Suri
#include "suri/Element.h"
#include "suri/Extent.h"
// Includes Wx
// Includes App
// Defines
/** Cantidad de puntos a tener en cuenta en una transformacion de coordenadas. */
#define _SPATIAL_OPERATIONS_EXTENT_POINTS_ 100

/** namespace suri */
namespace suri {
/** Calcula la interseccion del subset de los elementos */
bool IntersectExtents(Element* pFirstElement, Element* pSecondElement,
                      Extent &Intersection);
}

#endif /* SPATIALOPERATIONS_H_ */
