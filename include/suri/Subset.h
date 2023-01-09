/*! \file Subset.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SUBSET_H_
#define SUBSET_H_

// Includes suri
#include <ostream>
#include "Coordinates.h"

/** namespace suri */
#include <suri/Serializable.h>

namespace suri {
/** Clase que guarda coordenadas limites de un rectangulo */
/**
 *  Al estar definido por las coordenadas UpperLeft y LowerRight
 * puede inferirse de esta informacion el sentido de los ejes del
 * sistema coordenado en el que esta expresado el Subset.
 * \see Dimension
 */
class Subset : public Serializable{
public:
   /** ctor */
   Subset() : ul_(0, 0, 0), lr_(0, 0, 0) {
   }
   /** ctor */
   Subset(double Ulx, double Uly, double Lrx, double Lry);
   /** ctor */
   Subset(const Coordinates &UL, const Coordinates &LR);
   /** Verifica si el punto cae adentro */
   bool IsInside(const Coordinates &Point) const;
   /** Encuentra el punto mas cercano dentro del subset */
   Coordinates NearestPoint(const Coordinates &Point);
   /** Verfica que sea un subset valido */
   bool IsValid() const;
   /** Obtiene el centro */
   Coordinates GetCenter() const;

   /** Calcula la interseccion de dos Subset */
   Subset Intersect(const Subset &other) const;
   /**
    * Extrae un vector de coordenadas con una determinada distancia
    * entre cada una.
    */
   static std::vector<Coordinates> ExtractCoordinates(const Subset& SubsetP,
                                                      int PointsCountP, int DeltaP);

   friend std::ostream & operator<<(std::ostream &os, const Subset& Asubset);

   wxXmlNode* SerializeXml() const;

   static Subset* DeserializeXml(wxXmlNode* pRoot);
   /** Devuelve el subset desplazado a la coordenada 0,0 **/
   Subset ToOrigin();

   Coordinates ul_; /*! coordenada de vertice superior izquierdo */
   Coordinates lr_; /*! coordenada de vertice inferior derecho */
};

/** Operador igualdad */
bool operator==(const Subset &Lhs, const Subset &Rhs);

/** Operador desigualdad */
bool operator!=(const Subset &Lhs, const Subset &Rhs);

/** Calcula la interseccion de dos Subset */
Subset Intersect(const Subset& Subset1, const Subset &Subset2);

/** operator suma de una coordenada a un subset (corrimiento) */
Subset operator+(const Subset &Lhs, const Coordinates &Rhs);
/** operador de suma de un entero a un subset (lo hace crecer) */
Subset operator+(const Subset &Lhs, double Buffer);

/** Retorna un Subset que contiene a Subset1 y Subset2 */
Subset Join(const Subset &Subset1, const Subset &Subset2);

/** Obtiene el subset representado por un stream **/
std::ostream & operator<<(std::ostream &os, const Subset& Asubset);

}

#endif /*SUBSET_H_*/
