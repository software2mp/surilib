/** \file Ring.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef RING_H_
#define RING_H_

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "Line.h"

namespace suri {

/**
 * Clase que representa un anillo. El primer punto y el ultimo son iguales.
 * Para ser valido necesita tener 3 puntos distintos.
 */
class Ring : public Line {
public:
   /** Ctor */
   explicit Ring(const std::string &SpatialReference = "");
   /** Ctor */
   explicit Ring(const Ring &ARing);
   /** Dtor */
   virtual ~Ring();

   /** Indica si la geometria es valida */
   virtual bool IsValid() const;
   /** Retorna una copia de la geometria. Tambie copia subgeometrias */
   virtual Geometry* Clone() const;
   /** Indica si la geometria esta cerrada, solo valida si es poligono */
   virtual bool IsClosed() const;

   /** Retorla listado de puntos en vector */
   std::vector<SuriObject::UuidType> GetPoints() const;
   /** Retorna la cantidad de puntos en el vector */
   int GetSize() const;

private:
   static const int MINIMUN_LINE_POINTCOUNT = 4;
};

} /* namespace suri */
#endif /* RING_H_ */
