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

// Includes Estandar
#include <math.h>
#include <limits>
#include <algorithm>

// Includes Suri
#include "suri/SnapTool.h"
#include "suri/World.h"
#include "suri/RasterSpatialModel.h"
#include "suri/Table.h"
#include "suri/VectorEditionTask.h"
#include "suri/AuxiliaryFunctions.h"
#include "SnapInterface.h"

namespace suri {

/** Constructor */
SnapTool::SnapTool(Table* pTable, World* pWorld, std::string RasterModel) : pSnap_(NULL),
      unit_(Map), tolerance_(0), pWorld_(pWorld), pTable_(pTable), rastermodel_(RasterModel) {
   char* csrs = (char*)pWorld_->GetSpatialReference().c_str();
   srs_.importFromWkt(&csrs);
}

/** Destructor */
SnapTool::~SnapTool() {
   for (size_t i = 0; i < segmentbuffers_.size(); ++i)
      delete segmentbuffers_[i];
   for (size_t i = 0; i < segmentgeometries_.size(); ++i)
      delete segmentgeometries_[i];
   for (size_t i = 0; i < vertexgeometries_.size(); ++i)
      delete vertexgeometries_[i];
   for (size_t i = 0; i < vertexbuffers_.size(); ++i)
      delete vertexbuffers_[i];
}

/** Setea el modo de snap */
void SnapTool::SetMode(SnapInterface *pSnap) {
   if (pSnap_)
      delete pSnap_;
   pSnap->SetWorld(pWorld_);
   pSnap_ = pSnap;
}

/** Setea la unidad de snap */
void SnapTool::SetUnit(UnitEnum Unit) {
   unit_ = Unit;
}

/** Setea la tolerancia de snap */
void SnapTool::SetTolerance(int Tolerance) {
   tolerance_ = Tolerance;
}

/**
 * Verifica si el punto fantasma que quiere agregarse se encuentra dentro del area de tolerancia
 * param[in] ViewportPosition posicion del punto fantasma en el visualizador
 */
void SnapTool::SnapPoint(Coordinates &ViewportPosition) {
   if (segmentbuffers_.size() < 1 && vertexbuffers_.size() < 1)
      return;
   // Obtengo el punto que se quiere crear
   Coordinates phanthomcoord;
   pWorld_->Transform(ViewportPosition, phanthomcoord);
   OGRPoint* phantompoint = new OGRPoint(phanthomcoord.x_, phanthomcoord.y_);
   phantompoint->assignSpatialReference(&srs_);
   pSnap_->DoSnap(ViewportPosition, phantompoint);
   delete phantompoint;
}

/** Calcula la distancia correspondiente de acuerdo a la unidad configurada */
double SnapTool::CalculateBufferDistance() {
   if (unit_ == Map) {
      return tolerance_;
   } else if (unit_ == Pixel) {
      RasterSpatialModel::Parameters params = RasterSpatialModel::WktToParameters(rastermodel_);
      double distance = params.pixelSizes_[0] * tolerance_;
      return distance;
   } else {
      return 0;
   }
}

/** Crea los buffers de las geometrias de acuerdo al modo seleccionado */
bool SnapTool::CreateBufferGeometries() {
   segmentbuffers_.clear();
   segmentgeometries_.clear();
   vertexgeometries_.clear();
   vertexbuffers_.clear();
   std::string type = pSnap_->GetStrategyType();
   if (type.compare("Segment") == 0 || type.compare("VertexSegment") == 0)
      CreateSegmentBuffers();
   if (type.compare("Vertex") == 0 || type.compare("VertexSegment") == 0)
      CreateVertexBuffers();
   pSnap_->SetVertexGeometries(vertexgeometries_);
   pSnap_->SetVertexBufferGeometries(vertexbuffers_);
   pSnap_->SetSegmentGeometries(segmentgeometries_);
   pSnap_->SetSegmentBufferGeometries(segmentbuffers_);
   return true;
}

/** Crea las geometrias existentes en la capa con OGR y sus buffers correspondientes */
void SnapTool::CreateSegmentBuffers() {
   int column = pTable_->GetColumnByName(VectorEditionTask::GEOMETRY_COLUMN_NAME);
   int rows = pTable_->GetRows();
   double bufferdistance = CalculateBufferDistance();

   for (int row = 0; row < rows; ++row) {
      std::string geomwkt = "";
      pTable_->GetCellValue(column, row, geomwkt);
      if (geomwkt.empty())
         continue;
      char* pszpolywkt = (char*)geomwkt.c_str();
      OGRGeometry* geometry = NULL;
      OGRGeometryFactory::createFromWkt(&pszpolywkt, &srs_, &geometry);
      OGRGeometry* buffergeometry = geometry->Buffer(bufferdistance);
      segmentgeometries_.push_back(geometry);
      segmentbuffers_.push_back(buffergeometry);
   }
}

/**
 * Crea las geometrias existentes en la capa con OGR y los buffers correspondientes
 * a sus vertices
 */
void SnapTool::CreateVertexBuffers() {
   int column = pTable_->GetColumnByName(VectorEditionTask::GEOMETRY_COLUMN_NAME);
   int rows = pTable_->GetRows();
   double bufferdistance = CalculateBufferDistance();

   for (int row = 0; row < rows; ++row) {
      std::string geomwkt = "";
      pTable_->GetCellValue(column, row, geomwkt);
      if (geomwkt.empty())
         continue;
      char* pszpolywkt = (char*)geomwkt.c_str();
      OGRGeometry* geometry = NULL;
      OGRGeometryFactory::createFromWkt(&pszpolywkt, &srs_, &geometry);
      segmentgeometries_.push_back(geometry);
      int lenix = GetCount(geometry);
      for (int ix = 0; ix < lenix; ++ix) {
         OGRPoint* point = GetPoint(ix, geometry);
         vertexgeometries_.push_back(point);
         OGRGeometry* buffergeometry = point->Buffer(bufferdistance);
         vertexbuffers_.push_back(buffergeometry);
      }
   }
}

/** Agrega una geometria en los vectores correspondientes al modo configurado en la herramienta */
void SnapTool::AddGeometry() {
   int column = pTable_->GetColumnByName(VectorEditionTask::GEOMETRY_COLUMN_NAME);
   int row = pTable_->GetRows() - 1;
   double bufferdistance = CalculateBufferDistance();

   std::string geomwkt = "";
   pTable_->GetCellValue(column, row, geomwkt);
   if (geomwkt.empty())
      return;
   char* pszpolywkt = (char*) geomwkt.c_str();
   OGRGeometry* geometry = NULL;
   OGRGeometryFactory::createFromWkt(&pszpolywkt, &srs_, &geometry);
   segmentgeometries_.push_back(geometry);

   std::string type = pSnap_->GetStrategyType();
   if (type.compare("Segment") == 0 || type.compare("VertexSegment") == 0) {
      OGRGeometry* buffergeometry = geometry->Buffer(bufferdistance);
      segmentbuffers_.push_back(buffergeometry);
   }
   if (type.compare("Vertex") == 0 || type.compare("VertexSegment") == 0) {
      int lenix = GetCount(geometry);
      for (int ix = 0; ix < lenix; ++ix) {
         OGRPoint* point = GetPoint(ix, geometry);
         vertexgeometries_.push_back(point);
         OGRGeometry* buffergeometry = point->Buffer(bufferdistance);
         vertexbuffers_.push_back(buffergeometry);
      }
   }

   pSnap_->SetVertexGeometries(vertexgeometries_);
   pSnap_->SetVertexBufferGeometries(vertexbuffers_);
   pSnap_->SetSegmentGeometries(segmentgeometries_);
   pSnap_->SetSegmentBufferGeometries(segmentbuffers_);
}

/**
 * Retorna la cantidad de puntos por las que esta conformada una geometria
 * @param[in] Geometry geometria (puede ser punto, linea o poligono)
 * @return count cantidad de puntos por las que esta conformada la geometria
 */
int SnapTool::GetCount(OGRGeometry* Geometry) {
   OGRwkbGeometryType type = Geometry->getGeometryType();
   OGRLinearRing* pring; // Este puntero no debe ser borrado ya que forma parte de la geometria
   int count = 1;
   switch (type) {
      case wkbPolygon:
      {
         pring = static_cast<OGRPolygon*>(Geometry)->getExteriorRing();
         count = pring->getNumPoints() - 1;
         break;
      }
      case wkbLineString:
      {
         count = static_cast<OGRLineString*>(Geometry)->getNumPoints();
         break;
      }
      default:
         break;
   }
   return count;
}

/**
 * Retorna el punto especificado correspondiente a una geometria
 * @param[in] Number numero de punto de la geometria
 * @param[in] Geometry geometria (puede ser punto, linea o poligono)
 * @return point punto especificado de la geometria
 */
OGRPoint* SnapTool::GetPoint(int Number, OGRGeometry* Geometry) {
   OGRPoint* point = new OGRPoint();
   OGRwkbGeometryType type = Geometry->getGeometryType();
   OGRLinearRing* pring; // Este puntero no debe ser borrado ya que forma parte de la geometria
   switch (type) {
      case wkbPolygon: {
         pring = static_cast<OGRPolygon*>(Geometry)->getExteriorRing();
         pring->getPoint(Number, point);
         break;
      }
      case wkbLineString: {
         static_cast<OGRLineString*>(Geometry)->getPoint(Number, point);
         break;
      }
      default: {
         point = static_cast<OGRPoint*>(Geometry);
         break;
      }
   }
   return point;
}

} /** namespace suri */
