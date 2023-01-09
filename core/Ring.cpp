/** \file Ring.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "Ring.h"

namespace suri {

/** Ctor */
Ring::Ring(const std::string &SpatialReference) : Line(SpatialReference) {
}

/** Ctor */
Ring::Ring(const Ring &ARing) : Line(ARing) {
}

/** Dtor */
Ring::~Ring () {
}

/**
 * Indica si la geometria es valida
 * @return bool que indica si el anillo es valido
 */
bool Ring::IsValid() const {
   return GetSize() >= MINIMUN_LINE_POINTCOUNT;
}

/**
 * Retorna una copia de la geometria. Tambien copia subgeometrias
 * @return copia del anillo.
 */
Geometry* Ring::Clone() const {
   return new Ring(*this);
}

/**
 * Indica si la geometria esta cerrada, solo valida si es poligono
 * @return siempre true
 */
bool Ring::IsClosed() const {
   return true;
}

/**
 * Retorna listado de puntos en vector
 * @return vector con el id de los puntos en vector. El primer
 * se repite al final ya que geometria esta cerrada.
 */
std::vector<SuriObject::UuidType> Ring::GetPoints() const {
   std::vector<SuriObject::UuidType> points = Line::GetPoints();
   if (points.size() > 1)
      points.push_back(points[0]);
   return points;
}

/**
 * Retorna la cantidad de puntos en el vector
 * @return Cantidad de puntos. Incluye el punto final.
 */
int Ring::GetSize() const {
   return Line::GetSize() + 1;
}

} /* namespace suri */
