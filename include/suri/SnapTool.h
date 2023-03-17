/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

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
