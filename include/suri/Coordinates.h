/*! \file Coordinates.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef COORDINATES_H_
#define COORDINATES_H_

#include <vector>

/** namespace suri */
namespace suri {
/** Clase con coordenadas X, Y, Z */
/**
 *  Esta clase contiene las coordenadas de un punto 2D/3D
 *  Permite comparar si dos coordenadas son iguales, y transformar una coordenada
 *  3D en 2D (hace Z=0.0).
 */
class Coordinates {
public:

   /** Constructor */
   Coordinates(const double &X = 0.0, const double &Y = 0.0, const double &Z = 0.0);

   /** Destructor */
   ~Coordinates();

   /** Proyecta la coordenada en el plano x-y (hace z=0.0) */
   Coordinates Make2D() const;

   /** Compara la coordenada con otra */
   bool IsEqual(const Coordinates &Coord) const;

   /** Compara la coordenada con otra despreciando la coordenada vertical (z) */
   bool IsEqual2D(const Coordinates &Coord) const;

   double x_; /*! Coordenada X */
   double y_; /*! Coordenada Y */
   double z_; /*! Coordenada Z */
protected:
private:
};

/** Operador igualdad */
bool operator==(const Coordinates &Lhs, const Coordinates &Rhs);

/** Operador desigualdad */
bool operator!=(const Coordinates &Lhs, const Coordinates &Rhs);

/** Lista de coordenadas */
typedef std::vector<Coordinates> CoordinateListType;
} /*namespace suri*/

#endif /*COORDINATES_H_*/
