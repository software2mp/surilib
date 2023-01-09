/*! \file Coordinates.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include "suri/Coordinates.h"

// Includes standard
#include <cmath>

// Includes suri
#include "suri/AuxiliaryFunctions.h"

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] X Coordenada X
 * @param[in] Y Coordenada Y
 * @param[in] Z Coordenada Z
 * @return instancia de la clase Coordinates
 */
Coordinates::Coordinates(const double &X, const double &Y, const double &Z) :
      x_(X), y_(Y), z_(Z) {
}

/** Destructor */
Coordinates::~Coordinates() {
}

/**
 * Proyecta la coordenada en el plano x-y (hace z=0.0)
 * @return Coordenadas en el plano x-y
 */
Coordinates Coordinates::Make2D() const {
   return Coordinates(x_, y_, 0.0);
}

/**
 * Compara la coordenada con otra (compara x,y,z)
 * @param[in] Coord Coordenada a comparar
 * @return True si son iguales
 */
bool Coordinates::IsEqual(const Coordinates &Coord) const {
   // Se aumenta la precision porque quiero distinguir centesimas de segundo
   return FLOAT_COMPARE_WITH_PRECISION(x_, Coord.x_, 0.000000001, 0.000000001)
         && FLOAT_COMPARE_WITH_PRECISION(y_, Coord.y_, 0.000000001, 0.000000001)
         && FLOAT_COMPARE_WITH_PRECISION(z_, Coord.z_, 0.000000001, 0.000000001);
}

/**
 * Compara la coordenada con otra despreciando la coordenada vertical (z)
 * @param[in] Coord Coordenada a comparar
 * @return True si son iguales en el plano x-y
 */
bool Coordinates::IsEqual2D(const Coordinates &Coord) const {
   return FLOAT_COMPARE_WITH_PRECISION(x_, Coord.x_, 0.000000001, 0.000000001)
         && FLOAT_COMPARE_WITH_PRECISION(y_, Coord.y_, 0.000000001, 0.000000001);
}
} /*namespace suri*/

