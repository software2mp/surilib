/** \file Polygon.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef POLYGON_H_
#define POLYGON_H_

// Includes standard
#include <vector>
#include <string>

// Include suri
#include "Geometry.h"
#include "Ring.h"

namespace suri {

/**
 * Geometry de tipo poligono. Un poligono esta compuesto por un anillo.
 * Para que el anillo sea valido debe estar cerrado y tener al menos 4 puntos.
 */
class Polygon : public Geometry {
public:
   /** Ctor */
   explicit Polygon(const std::string &SpatialReference = "");
   /** Ctor */
   explicit Polygon(const Polygon &APolygon);
   /** Dtor */
   virtual ~Polygon();

   /** Indica si la geometria es valida */
   virtual bool IsValid() const;
   /** Retorna una copia de la geometria. Tambie copia subgeometrias */
   virtual Geometry* Clone() const;
   /** Configura el srs de la geometria */
   virtual void AssignSpatialReference(const std::string &SpatialReference);

   /** Indica si la geometria esta cerrada, solo valida si es poligono */
   virtual bool IsClosed() const;
   /** Fuerza a que se cierre la geometria. Solo para poligonos */
   virtual bool CloseRings();

   /** Agrega un anillo al vector */
   bool AppendRing(Ring* pRing);
   /** Elimina un anillo del vector */
   bool RemoveRing(SuriObject::UuidType RingId);
   /** Retorla listado de anillos en vector */
   std::vector<SuriObject::UuidType> GetRings() const;
   /** Retorna la linea con id indicado */
   Ring* GetRing(SuriObject::UuidType RingId);
   /** Retorna la linea con id indicado */
   const Ring* GetRing(SuriObject::UuidType RingId) const;

private:
   typedef std::vector<Ring*>::const_iterator RingIterator;

   /** Busca un punto a partir del id */
   RingIterator FindRing(SuriObject::UuidType RingId) const;
   /** Valida el anillo */
   bool ValidateRing(const Ring* pPoint);

   /** Vector con puntos */
   std::vector<Ring*> ringVector_;

   static const int MINIMUN_RING_POINTCOUNT = 3;
};

} /* namespace suri */
#endif /* POLYGON_H_ */
