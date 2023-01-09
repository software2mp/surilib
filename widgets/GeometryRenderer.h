/** \file GeometryRenderer.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOMETRYRENDERER_H_
#define GEOMETRYRENDERER_H_

// Inclues standard
#include <vector>
#include <string>

// Includes suri
#include "suri/VectorStyle.h"

// fordwards
class wxDC;
class wxPoint;

namespace suri {

class Mask;
class Geometry;
class World;
class GeometryCollection;

/**
 * Clase que renderiza un listado de gemetrias sobre un wxDC.
 */
class GeometryRenderer {
   /**
    * Estructura que se usa para guardar informacion de renderizacion
    */
   class GeometryData {
   public:
      /** Ctor */
      GeometryData(int PointCount, VectorStyle* pStyle);
      /** Dtor */
      ~GeometryData();

      wxPoint* points_; /** Vector con puntos a renderizar */
      int pointCount_; /** Cantidad de puntos en points_ */
      VectorStyle* pStyle_; /** Estilo de los puntos */
   };

public:
   /** Ctor */
   GeometryRenderer(wxDC* pDC, const World* pWorldWindow);
   /** Dtor */
   virtual ~GeometryRenderer();
   /** Renderiza las geometrias sobre wl wxDC */
   void Render(GeometryCollection* pCollection,
                               const Mask* pMask = NULL);
   /** Renderiza las geometrias en vector sobre wl wxDC */
   void Render(std::vector<Geometry*> &Geometries,
                                const VectorStyle* pDefaultStyle,
                                const Mask* pMask = NULL);

private:
   /** Pinta */
   bool Paint(std::vector<GeometryData*> &Polygons, const Mask* &pMask);
   /** Traza */
   bool Trace(std::vector<GeometryData*> &Lines, const Mask* &pMask);
   /** Pone marcas */
   bool Mark(std::vector<GeometryData*> &Points, const Mask* &pMask);

   /** DC wx sobre el que se renderizan los vectores */
   wxDC* pDC_;
   /** Mundo asociado al dc donde quiero renderizar */
   const World* pWorldWindow_;
};

} /* namespace suri */

#endif /* GEOMETRYRENDERER_H_ */
