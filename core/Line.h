/** \file Line.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef LINE_H_
#define LINE_H_

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "Geometry.h"
#include "Point.h"

namespace suri {

/**
 * Geometry de tipo linea.
 * Para que sea valida debe contener por lo menos dos puntos.
 */
class Line : public Geometry {
public:
   /** Ctor */
   explicit Line(const std::string &SpatialReference = "");
   /** Ctor */
   explicit Line(const Line &ALine);
   /** Dtor */
   virtual ~Line();

   /** Indica si la geometria es valida */
   virtual bool IsValid() const;
   /** Retorna una copia de la geometria. Tambie copia subgeometrias */
   virtual Geometry* Clone() const;
   /** Configura el srs en el que estan los puntos de la geometria */
   virtual void AssignSpatialReference(const std::string &SpatialReference);

   /** Indica si la geometria esta cerrada, solo valida si es poligono */
   virtual bool IsClosed() const;

   /** Agrega un punto al final de la linea */
   virtual bool AppendPoint(Point* pPoint);
   /** Agrega un punto en la posicion indicada */
   virtual bool InsertPointBefore(Point* pPoint,
                  const SuriObject::UuidType &ReferencePointId);
   /** Elimina un punto del vector */
   virtual bool DeletePoint(const SuriObject::UuidType &PointId);
   /** Saca un punto del vector sin eliminarlo */
   virtual Point* RemovePoint(const SuriObject::UuidType &PointId);
   /** Cambia de posicion un punto en el vector. */
   virtual bool MovePoint(const SuriObject::UuidType &PointId,
                                             Coordinates NewPosition);
   /** Retorla listado de puntos en vector */
   virtual std::vector<SuriObject::UuidType> GetPoints() const;
   /** Retorna punto de linea con id correspondiente */
   virtual const Point* GetPoint(const SuriObject::UuidType &PointId) const;
   /** Retorna punto de linea con id correspondiente */
   virtual Point* GetPoint(const SuriObject::UuidType &PointId);
   /** Retorna la cantidad de puntos en el vector */
   virtual int GetSize() const;

protected:
   typedef std::vector<Point*>::const_iterator ConstPointIterator;
   typedef std::vector<Point*>::iterator PointIterator;

   /** Busca un punto a partir del id */
   ConstPointIterator FindPoint(const SuriObject::UuidType &PointId) const;
   /** Busca un punto a partir del id */
   PointIterator FindPoint(const SuriObject::UuidType &PointId);
   /** Valida el punto */
   bool ValidatePoint(const Point* pPoint);

   /** Vector con puntos */
   std::vector<Point*> pointVector_;
private:
   static const int MINIMUN_LINE_POINTCOUNT = 2;
};

} /* namespace suri */
#endif /* LINE_H_ */
