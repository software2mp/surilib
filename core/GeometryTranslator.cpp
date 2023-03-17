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

// Includes standard
#include <string>

// Includes suri
#include "GeometryTranslator.h"
#include "Polygon.h"
#include "Line.h"
#include "Point.h"

// Includes gdal
#include "ogr_geometry.h"

namespace suri {

/** Ctor */
GeometryTranslator::GeometryTranslator () {
}

/** Dtor */
GeometryTranslator::~GeometryTranslator () {
}

/**
 * Tranforma de geometry ogr a wkt
 * @param[in] pGeometry geometria que quiero exportar
 * @return wkt de geometria
 * @return "" si no pudo generar wkt
 */
std::string GeometryTranslator::ExportToWkt(OGRGeometry* pOgrGeometry) {
   if (!pOgrGeometry)
      return "";
   char* pwkt = NULL;
   if(pOgrGeometry->exportToWkt(&pwkt) != OGRERR_NONE)
      return "";
   std::string returnwkt = pwkt;
   delete pwkt;
   return returnwkt;
}

/**
 * Tranforma de geometry a wkt
 * @param[in] pGeometry geometria que quiero exportar
 * @return wkt de geometria
 * @return "" si no pudo generar wkt
 */
std::string GeometryTranslator::ExportToWkt(Geometry* pGeometry) {
   std::string wkt = "";
   OGRGeometry* pogrgeometry = GetOgrPoint(dynamic_cast<Point*>(pGeometry));
   pogrgeometry = pogrgeometry ? pogrgeometry : GetOgrLine(
                                       dynamic_cast<Line*>(pGeometry));
   pogrgeometry = pogrgeometry ? pogrgeometry : GetOgrPolygon(
                                       dynamic_cast<Polygon*>(pGeometry));
   if (!pogrgeometry)
      return wkt;

   char* pwkt;
   if(pogrgeometry->exportToWkt(&pwkt) == OGRERR_NONE)
      wkt = std::string(pwkt);

   OGRFree(pwkt);
   OGRGeometryFactory::destroyGeometry(pogrgeometry);

   return wkt;
}

/**
 * Genera Geometry a partir de wkt
 * @param[in] GeometryWkt wkt de la geometria
 * @return geometria inicializada con wkt
 * @return NULL si no pudo inicializar la geoemtria.
 */
Geometry* GeometryTranslator::ImportFromWkt(const std::string &GeometryWkt,
                                             const std::string& GeometrySrs) {
   Geometry* pgeometry = NULL;

   char* pwkt = (char*)GeometryWkt.c_str();
   OGRGeometry* pogrgeom = NULL;
   if (OGRGeometryFactory::createFromWkt(&pwkt, NULL, &pogrgeom) != OGRERR_NONE)
      return NULL;

   switch (pogrgeom->getGeometryType()) {
      case wkbPoint :
         pgeometry = GetPoint(dynamic_cast<OGRPoint*>(pogrgeom));
         break;
      case wkbLineString :
         pgeometry = GetLine(dynamic_cast<OGRLineString*>(pogrgeom));
         break;
      case wkbPolygon :
         pgeometry = GetPolygon(dynamic_cast<OGRPolygon*>(pogrgeom));
         break;
      default :
         break;
   }

   if (pgeometry)
      pgeometry->AssignSpatialReference(GeometrySrs);

   OGRGeometryFactory::destroyGeometry(pogrgeom);
   return pgeometry;
}

/**
 * Genera OGRPoint a partir de una geometria de tipo punto(de biblioteca)
 * @param[in] pPoint punto de biblioteca
 * @return punto ogr
 * @return NULL si no pudo inicializar punto
 */
OGRPoint* GeometryTranslator::GetOgrPoint(const Point* pPoint) {
   if (!pPoint)
      return NULL;

   Coordinates coord = pPoint->GetCoordinate();

   OGRPoint* pogrpoint = new OGRPoint();
   pogrpoint->setX(coord.x_);
   pogrpoint->setY(coord.y_);

   return pogrpoint;
}

/**
 * Genera OGRLineString a partir de una geometria de tipo linea(de biblioteca)
 * @param[in] pLine linea de biblioteca
 * @return linea ogr
 * @return NULL si no pudo inicializar linea
 */
OGRLineString* GeometryTranslator::GetOgrLine(const Line* pLine) {
   if (!pLine)
      return NULL;

   OGRLineString* pogrline = new OGRLineString();
   CopyLinePointsToOGRLine(pLine, pogrline);

   return pogrline;
}

/**
 * Genera OGRPolygon a partir de una geometria de tipo poligono(de biblioteca)
 * @param[in] pPolygon poligono de biblioteca
 * @return poligono ogr.
 * @return NULL si no pudo inicializar poligono.
 */
OGRPolygon* GeometryTranslator::GetOgrPolygon(const Polygon *pPolygon) {
   if (!pPolygon)
      return NULL;

   std::vector<SuriObject::UuidType> rings = pPolygon->GetRings();
   if (rings.size() == 0)
      return NULL;
   const Line* pline = pPolygon->GetRing(rings[0]);

   OGRPolygon* pogrpolygon = new OGRPolygon();
   OGRLinearRing* pogrring = new OGRLinearRing();
   CopyLinePointsToOGRLine(pline, pogrring);
   pogrpolygon->addRing(pogrring);

   return pogrpolygon;
}

/**
 * Genera una geometria de tipo punto(de biblioteca) a partir de OGRPoint
 * @param[in] punto ogr
 * @return Point con la misma coordenada
 */
Point* GeometryTranslator::GetPoint(const OGRPoint* pOgrPoint) {
   Point* ppoint = new Point();
   Coordinates coordiante(pOgrPoint->getX(), pOgrPoint->getY());
   ppoint->SetCoordinate(coordiante);
   return ppoint;
}

/**
 * Retorna una geometria de tipo linea(de biblioteca) a partir de pOgrLine
 * @param[in] pOgrLine linea ogr
 * @return Line configurado con punto en linea ogr.
 */
Line* GeometryTranslator::GetLine(const OGRLineString* pOgrLine) {
   Line* pline = new Line();
   CopyOGRLinePointsToLine(pOgrLine, pline);
   return pline;
}

/**
 * Retorna una geometria de tipo poligono(de biblioteca) a partir pOgrPolygon
 * @param[in] pOgrPolygon poligono ogr
 * @return Polygon configurado con anillos en pOgrPolygon
 */
Polygon* GeometryTranslator::GetPolygon(const OGRPolygon* pOgrPolygon) {
   Polygon* ppolygon = new Polygon();
   const OGRLinearRing* pogrring = pOgrPolygon->getExteriorRing();
   Ring* pring = new Ring();
   CopyOGRLinePointsToLine(pogrring, pring);
   ppolygon->AppendRing(pring);
   return ppolygon;
}

/**
 * Configura pOgrLine con puntos en pLine
 * @param[in] pLine linea con puntos que quiero agregar
 * @param[out] pOgrLine linea ogr configurada con puntos.
 */
void GeometryTranslator::CopyLinePointsToOGRLine(const Line* pLine,
                                                      OGRLineString* pOgrLine) {
   std::vector<SuriObject::UuidType> points = pLine->GetPoints();
   std::vector<SuriObject::UuidType>::iterator it = points.begin();
   while (it != points.end()) {
      OGRPoint* pogrpoint = GetOgrPoint(pLine->GetPoint(*it));
      pOgrLine->addPoint(pogrpoint);
      ++it;
   }
}

/**
 * Configura pLine con puntos en pOgrLine
 * @param[in] pLine linea con puntos que quiero agregar
 * @param[out] pOgrLine linea ogr configurada con puntos.
 */
void GeometryTranslator::CopyOGRLinePointsToLine(
                              const OGRLineString* pOgrLine, Line* pLine) {
   OGRPoint* pogrpoint = new OGRPoint();
   int pointcount = pOgrLine->get_IsClosed() ? pOgrLine->getNumPoints() - 1 :
                                                      pOgrLine->getNumPoints();
   for (int i = 0; i < pointcount; i++) {
      pOgrLine->getPoint(i, pogrpoint);
      Point* ppoint = GetPoint(pogrpoint);
      pLine->AppendPoint(ppoint);
   }
   OGRGeometryFactory::destroyGeometry(pogrpoint);
}

} /* namespace suri */
