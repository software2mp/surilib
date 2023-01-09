/*! \file Extent.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
#ifndef EXTENT_H_
#define EXTENT_H_

// Includes estandar

// Includes Suri
#include "suri/Subset.h"
#include "suri/Coordinates.h"

// Includes Wx
// Includes App
// Defines

/** namespace suri */
namespace suri {
/** Clase que representa una extension a partir de un subset. */
/**
 * Clase que recibe un subset y transforma las coordenadas UL y LR en coordenadas
 * minimo y maximo.
 */
class Extent {
public:
   /** Ctor. */
   Extent(const Subset &InputSubset);
   /** Ctor. */
   Extent(double MinX, double MinY, double MaxX, double MaxY);
   /** Dtor. */
   virtual ~Extent();
   Coordinates min_; /*! Coordenada con los valores minimos para x e y */
   Coordinates max_; /*! Coordenada con los valores minimos para x e y */
};

/** Calcula la interseccion de dos Subset */
Extent Intersect(const Extent &Extent1, const Extent &Extent2);
/** Operador igualdad */
bool operator==(const Extent &Extent1, const Extent &Extent2);
}

#endif /* EXTENT_H_ */
