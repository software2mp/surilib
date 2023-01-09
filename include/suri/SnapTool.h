/*! \file SnapTool.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2014-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef SNAPTOOL_H_
#define SNAPTOOL_H_

// Includes Estandar
#include <vector>
#include <string>

// Includes OGR
#include "ogr_geometry.h"

namespace suri {

// forwards
class World;
class Table;
class Coordinates;
class SnapInterface;

/**
 * Herramienta de snapping vectorial que permite ajustar los objetos en edicion a otros objetos
 * de acuerdo al modo, unidad y tolerancia configurados a traves de SnapPart
 */
class SnapTool {
public:
   /** Enumerado del modo */
   typedef enum {
      Map = 0, Pixel = 1
   } UnitEnum;
   /** Constructor */
   SnapTool(Table* pTable, World* pWorld, std::string RasterModel);
   /** Destructor */
   virtual ~SnapTool();
   /** Setea el modo de snap */
   void SetMode(SnapInterface *pSnap);
   /** Setea la unidad de snap */
   void SetUnit(UnitEnum Unit);
   /** Setea la tolerancia de snap */
   void SetTolerance(int Tolerance);
   /**
    * Verifica si el punto fantasma que quiere agregarse se encuentra dentro del area de tolerancia
    */
   void SnapPoint(Coordinates &ViewportPosition);
   /** Crea los buffers de las geometrias de acuerdo al modo seleccionado */
   bool CreateBufferGeometries();
   /**
    * Agrega una geometria en los vectores correspondientes al modo configurado en la herramienta
    */
   void AddGeometry();

private:
   /** Crea las geometrias existentes en la capa con OGR y sus buffers correspondientes */
   void CreateSegmentBuffers();
   /**
    * Crea las geometrias existentes en la capa con OGR y los buffers correspondientes
    * a sus vertices
    */
   void CreateVertexBuffers();
   /** Calcula la distancia correspondiente de acuerdo a la unidad configurada */
   double CalculateBufferDistance();
   /** Retorna la cantidad de puntos por las que esta conformada una geometria */
   int GetCount(OGRGeometry* Geometry);
   /** Retorna el punto especificado correspondiente a una geometria */
   OGRPoint* GetPoint(int Number, OGRGeometry* Geometry);
   SnapInterface *pSnap_;
   /** Unidad para snap */
   UnitEnum unit_;
   /** Tolerancia de la unidad seleccionada */
   int tolerance_;
   /** Puntero al mundo del raster */
   World* pWorld_;
   /** Puntero a la tabla con las geometrias */
   Table* pTable_;
   /** Modelo raster */
   std::string rastermodel_;
   /** Vector con buffers de vertices y segmentos */
   std::vector<OGRGeometry*> segmentbuffers_, vertexbuffers_, segmentgeometries_, vertexgeometries_;
   /** Sistema de referencia de la capa */
   OGRSpatialReference srs_;
};

} /** namespace suri */

#endif /* SNAPTOOL_H_ */
