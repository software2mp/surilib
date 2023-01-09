/*! \file SnapInterface.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2015-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SNAPINTERFACE_H_
#define SNAPINTERFACE_H_

// Includes Estandar
#include <vector>
#include <string>

// Includes Gdal
#include "ogr_geometry.h"

namespace suri {

// forwards
class World;
class Coordinates;

class SnapInterface {
public:
   /** Destructor */
   virtual ~SnapInterface();
   /** Modifica las coordenadas de un punto de acuerdo a la estrategia definida */
   virtual bool DoSnap(Coordinates &ViewportPosition, OGRPoint* PhantomPoint)=0;
   /** Retorna el tipo de estrategia que utiliza el snapping */
   virtual std::string GetStrategyType()=0;
   /** Configura el mundo que se utilizara */
   virtual void SetWorld(World* pWorld);
   /** Configura el vector con las geometrias correspondientes a los segmentos */
   void SetSegmentGeometries(std::vector<OGRGeometry*> SegmentGeometries);
   /** Configura el vector con las geometrias correspondientes a los vertices */
   void SetVertexGeometries(std::vector<OGRGeometry*> VertexGeometries);
   /** Configura el vector con las geometrias correspondientes a los buffers de los vertices */
   void SetVertexBufferGeometries(std::vector<OGRGeometry*> VertexBuffers);
   /** Configura el vector con las geometrias correspondientes a los buffers de los segmentos */
   void SetSegmentBufferGeometries(std::vector<OGRGeometry*> SegmentBuffers);

protected:
   /** Puntero al mundo */
   World* pWorld_;
   /** Vector con geometrias */
   std::vector<OGRGeometry*> segmentbuffers_, vertexbuffers_, segmentgeometries_, vertexgeometries_;
};

} /** namespace suri */

#endif /* SNAPINTERFACE_H_ */
