/*! \file Extent.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/Extent.h"

// Includes estandar
#include <algorithm>

// Includes Suri

// Includes Wx

// Includes App

// Defines

/** namespace suri */
namespace suri {
/**
 * Constructor
 * Recibe un subset y asigna a la propiedad min_ la coordenada formada por el
 * minimo entre ul_.x_ y lr_.x_, y el minimo entre ul_.y_ y lr_.y_; asigna a la
 * propiedad max_ la coordenada formada por el maximo entre ul_.x_ y lr_.x_, y
 * el maximo entre ul_.y_ y lr_.y_;
 * @param[in] InputSubset subset de entrada
 */
Extent::Extent(const Subset &InputSubset) {
   min_ = Coordinates(std::min(InputSubset.ul_.x_, InputSubset.lr_.x_),
                      std::min(InputSubset.ul_.y_, InputSubset.lr_.y_));
   max_ = Coordinates(std::max(InputSubset.ul_.x_, InputSubset.lr_.x_),
                      std::max(InputSubset.ul_.y_, InputSubset.lr_.y_));
}

/**
 * Constructor
 * Recibe valores que representan las esquinas de un extent.
 * Asigna los valores a las propiedades min_ y max_
 * @param[in] MinX valor minimo para el valor que representa el ancho del extent
 * @param[in] MinY valor minimo para el valor que representa el alto del extent
 * @param[in] MaxX valor maximo para el valor que representa el ancho del extent
 * @param[in] MaxY valor maximo para el valor que representa el alto del extent
 */
Extent::Extent(double MinX, double MinY, double MaxX, double MaxY) {
   min_ = Coordinates(std::min(MinX, MaxX), std::min(MinY, MaxY));
   max_ = Coordinates(std::max(MinX, MaxX), std::max(MinY, MaxY));
}

/**
 * Destructor
 */
Extent::~Extent() {
}

/**
 * Calcula la interseccion de dos Extents
 * @param[in] Extent1 extent
 * @param[in] Extent2 extent
 * @return extent con la interseccion de los extents recibidos.
 */
Extent Intersect(const Extent &Extent1, const Extent &Extent2) {
   Subset subset1(Extent1.min_, Extent1.max_);
   Subset subset2(Extent2.min_, Extent2.max_);

   Subset subsetretorno = subset1.Intersect(subset2);

   return Extent(subsetretorno);
}

/** Operador igualdad */
bool operator==(const Extent &Extent1, const Extent &Extent2) {
   Subset subset1(Extent1.min_, Extent1.max_);
   Subset subset2(Extent2.min_, Extent2.max_);
   return subset1 == subset2;
}
}
