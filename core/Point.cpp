/** \file Point.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>

// Includes suri
#include "Point.h"

namespace suri {

/** Ctor */
Point::Point() {
}

/** Ctor */
Point::Point(Coordinates Coord, const std::string &SpatialReference) :
                                                Geometry(SpatialReference) {
   SetCoordinate(Coord);
}

/** Ctor */
Point::Point(const Point &APoint) : Geometry(APoint) {
   this->coordinate_ = APoint.coordinate_;
}

/** Dtor */
Point::~Point() {
}

/**
 * Indica si la geometria es valida
 * @return siempre true
 */
bool Point::IsValid() const {
   return true;
}

/**
 * Retorna una copia de la geometria. Tambien copia subgeometrias
 * @return copia de la geometria
 */
Geometry* Point::Clone() const {
   return new Point(*this);
}

/**
 * Configura la coordenada asociada al punto
 * @param[in] Coord nueva coordenada asignada al punto
 */
void Point::SetCoordinate(Coordinates Coord) {
   coordinate_ = Coord;
}

/**
 * Retorna la coordenada asociada al punto
 * @return coordenada del punto
 */
Coordinates Point::GetCoordinate() const {
   return coordinate_;
}

} /* namespace suri */
