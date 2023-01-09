/*! \file SegmentSnapStrategy.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SEGMENTSNAPSTRATEGY_H_
#define SEGMENTSNAPSTRATEGY_H_

// Includes Suri
#include "SnapInterface.h"

namespace suri {

// Modifica las coordenadas de un punto acoplandolo al punto mas cercano de la geometria
class SegmentSnapStrategy : public SnapInterface {
public:
   /** Constructor */
   SegmentSnapStrategy();
   /** Destructor */
   virtual ~SegmentSnapStrategy();
   /** Modifica las coordenadas de un punto acoplandolo al punto mas cercano de la geometria */
   virtual bool DoSnap(Coordinates &ViewportPosition, OGRPoint* PhantomPoint);
   /** Retorna el tipo de estrategia que utiliza el snapping */
   virtual std::string GetStrategyType();

private:
   /** Retorna la distancia entre un punto y una geometria */
   double GetDistance(OGRGeometry* Geometry, double X, double Y);
   /** Retorna un booleano que indica si el punto especificado se encuentra en el dominio */
   bool IsInDomain(double StartPoint, double EndPoint, double Point);
   /** Obtiene el punto de interseccion del punto fantasma con un segmento */
   void GetIntersectionPoint(OGRPoint* PhantomPoint, Coordinates NearVertex,
                             Coordinates OtherVertex, Coordinates &Intersection);
   /**
    * Retorna un valor booleano que indica si encontro una geometria dentro del area de tolerancia
    */
   bool FindNearestGeometryIndex(OGRPoint* PhantomPoint);
   /** Obtiene un vector con todos los vertices de la geometria mas cercana al punto fantasma */
   void GetGeometryVertex(OGRPoint* PhantomPoint, std::vector<OGRPoint*> &Vertex, int &Position);
   /** Indice correspondiente a la geometria mas cercana */
   int index_;
};

} /** namespace suri */

#endif /* SEGMENTSNAPSTRATEGY_H_ */
