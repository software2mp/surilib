/** \file Point.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef POINT_H_
#define POINT_H_

// Includes standard
#include <string>

// Includes suri
#include "Geometry.h"
#include "suri/Coordinates.h"

namespace suri {

/**
 * Clase que representa un punto.
 */
class Point : public Geometry {
public:
   /** Ctor */
   Point();
   /** Ctor */
   explicit Point(const Point &APoint);
   /** Ctor */
   Point(Coordinates Coord, const std::string &SpatialReference = "");
   /** Dtor */
   virtual ~Point();

   /** Indica si la geometria es valida */
   virtual bool IsValid() const;
   /** Retorna una copia de la geometria. Tambie copia subgeometrias */
   virtual Geometry* Clone() const;
   /** Configura la coordenada asociada al punto */
   virtual void SetCoordinate(Coordinates Coord);
   /** Retorna la coordenada asociada al punto */
   virtual Coordinates GetCoordinate() const;

private:
   /** Coordenadas del punto */
   Coordinates coordinate_;
};

} /* namespace suri */
#endif /* POINT_H_ */
