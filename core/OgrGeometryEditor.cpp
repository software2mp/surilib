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

#include "OgrGeometryEditor.h"

// Includes standard
// Includes Suri
#include "logmacros.h"
// Includes Wx
#include "wx/wx.h"
// Includes App
// Includes OGR
#include "ogr_geometry.h"
#include "ogr_spatialref.h"

/** namespace suri */
namespace suri {
/**
 * Constructor
 * @param[in] SpatialReference referencia espacial
 * @param[in] GeometryType tipo de geometria
 */
OgrGeometryEditor::OgrGeometryEditor(const std::string &SpatialReference,
                                     Vector::VectorType GeometryType) :
      vecType_(GeometryType), spatialReference_(SpatialReference), pLineString_(NULL),
      pLinearRing_(NULL), pPoint_(NULL), pPoligon_(NULL), pUndefined_(NULL) {
   // Inicializo punteros
   // Creo ogrgeometry que permite agregar puntos

   OGRSpatialReference *pspatialref = new OGRSpatialReference(SpatialReference.c_str());
   switch (vecType_) {
      case Vector::Point:
         pPoint_ =
               dynamic_cast<OGRPoint*>(OGRGeometryFactory::createGeometry(wkbPoint));
         pPoint_->assignSpatialReference(pspatialref);
         break;
      case Vector::Line:
         pLineString_ = dynamic_cast<OGRLineString*>(OGRGeometryFactory::createGeometry(
               wkbLineString));
         pLineString_->assignSpatialReference(pspatialref);
         break;
      case Vector::Polygon:
         // linearRing guarda el OGRLinearRing contenido en polygon para agregar puntos
         pLinearRing_ = dynamic_cast<OGRLinearRing*>(OGRGeometryFactory::createGeometry(
               wkbLinearRing));
         pLinearRing_->assignSpatialReference(pspatialref);
         pPoligon_ = dynamic_cast<OGRPolygon*>(OGRGeometryFactory::createGeometry(
               wkbPolygon));
         pPoligon_->assignSpatialReference(pspatialref);
         break;
      case Vector::GeometryCollection:
      case Vector::Undefined:
         break;
   }
   pspatialref->Release();
}

/** Indica la geometria asociada a partir de un wkt
 *  @param SpatialReference sistema de referencia espacial de la geometria
 *  @param wkt que describe la geometria
 *  @return puntero a la geometria creada, NULL en caso contrario.**/
bool OgrGeometryEditor::SetGeometryFromWkt(const std::string& Wkt,
                                           OGRSpatialReference* pSpatialRef) {
   char* pwkt = (char*) Wkt.c_str();
   OGRGeometry* pgeometry = NULL;
   if (OGRGeometryFactory::createFromWkt(&pwkt, pSpatialRef, &pgeometry) != OGRERR_NONE)
      return false;
   OGRSpatialReference* pspatialref = pgeometry->getSpatialReference();
   if (pspatialref) {
      char* ptempspatialref;
      pspatialref->exportToWkt(&ptempspatialref);
      spatialReference_ = wxString(ptempspatialref);
      OGRFree(ptempspatialref);
   }
   // Segun el tipo de geometria se guardan las clases necesarias
   SetGeometry(pgeometry);
   return true;
}

/**
 * Constructor
 * @param[in] pGeometry geometria sobre la que se edita
 */
OgrGeometryEditor::OgrGeometryEditor(OGRGeometry* pGeometry) :
      pLineString_(NULL), pLinearRing_(NULL), pPoint_(NULL), pPoligon_(NULL),
      pUndefined_(NULL) {
   char* ptempspatialref;
   pGeometry->getSpatialReference()->exportToWkt(&ptempspatialref);
   spatialReference_ = wxString(ptempspatialref);
   // Segun el tipo de geometria se guardan las clases necesarias
   SetGeometry(pGeometry);
}

/**
 * Destructor
 */
OgrGeometryEditor::~OgrGeometryEditor() {
}

/**
 * Retorna el tipo de geometria
 * @return tipo de vector que guarda la geometria
 */
Vector::VectorType OgrGeometryEditor::GetGeometryType() {
   return vecType_;
}

/**
 * Retorna la referencia espacial de la geometria
 * @return referencia espacial de la geometria
 */
std::string OgrGeometryEditor::GetSpatialReference() {
   return spatialReference_;
}

/**
 * Informa si la referencia espacial es valida.
 * @return bool que indica si la geometria esta en estado valido
 */
bool OgrGeometryEditor::IsValid() {
   bool ret = false;
   switch (vecType_) {
      case Vector::Point:
         ret = pPoint_->IsValid();
         break;
      case Vector::Line:
         ret = pLineString_->IsValid();
         break;
      case Vector::Polygon:
         ret = pLinearRing_->IsValid();
         break;
      case Vector::GeometryCollection:
      case Vector::Undefined:
         ret = pUndefined_->IsValid();
         break;
   }

   return ret;
}

/**
 * Retorna la geometria en formato OGRGeometry
 * @return geometria en formato OGRGeometry
 */
OGRGeometry* OgrGeometryEditor::GetOgrGeometry() {
   OGRGeometry* preturnvalue = pUndefined_;
   switch (vecType_) {
      case Vector::Point:
         preturnvalue = pPoint_;
         break;
      case Vector::Line:
         preturnvalue = pLineString_;
         break;
      case Vector::Polygon:
         pPoligon_->addRing(pLinearRing_);
         // El formato shapefile requiere que los anillos esten
         // cerrados. Ver TCK #2699
         pPoligon_->closeRings();
         preturnvalue = pPoligon_;
         break;
      case Vector::GeometryCollection:
      case Vector::Undefined:
         preturnvalue = pUndefined_;
         break;
   }
   return preturnvalue;
}

/**
 * Modifica el punto en la posicion indicada.
 * Si no se pasa Posicion se agrega al final.
 * En caso del punto se ignora la posicion.
 * @param[in] Position posicion dentro del vector donde se quiere agregar punto
 * @param[in] Point del nuevo punto en sistema de coordenadas del vector
 * @return bool que informa resultado de operacion
 */
bool OgrGeometryEditor::SetPoint(const Coordinates &Point, int Position) {
   switch (vecType_) {
      case Vector::Point:
         pPoint_->setX(Point.x_);
         pPoint_->setY(Point.y_);
         pPoint_->setZ(Point.z_);
         break;
      case Vector::Line:
         if (Position == -1) {
            Position = pLineString_->getNumPoints();
         }
         pLineString_->setPoint(Position, Point.x_, Point.y_, Point.z_);
         break;
      case Vector::Polygon:
         if (Position == -1) {
            Position = pLinearRing_->getNumPoints();
         }
         pLinearRing_->setPoint(Position, Point.x_, Point.y_, Point.z_);
         break;
      case Vector::GeometryCollection:
      case Vector::Undefined:
         return false;
         break;
   }
   return true;
}

/**
 * Agrega un punto a la geoemtria en la posicion indicada.
 * Si no se pasa Posicion se agrega al final.
 * En caso del punto se ignora la posicion.
 * @param[in] Position posicion dentro del vector donde se quiere agregar punto.
 * Si vale -1 o no se pasa significa al final del vector.
 * @param[in] Point del nuevo punto en sistema de coordenadas del vector
 * @return bool que informa resultado de operacion
 */
bool OgrGeometryEditor::AddPoint(const Coordinates &Point, int Position) {
   // Busco el ultimo punto
   int pointcount;
   Coordinates coordinate;
   pointcount = GetPointCount();

   // Si Position = -1 la seteo al final
   if (Position == -1) {
      Position = pointcount;
   }
   // Si Postion esta fuera de rango salgo
   if ((Position > pointcount) || (Position < 0)) {
      REPORT_DEBUG("D: Posicion fuera de rango");
      return false;
   }
   // Determino cual sera el nuevo punto final
   if (Position == pointcount) {
      coordinate = Point;
   } else {
      GetPoint(coordinate, pointcount - 1);
   }

   switch (vecType_) {
      case Vector::Point:
         REPORT_DEBUG("D: No se puede agregar otro punto a una geometria punto");
         return false;
         break;
      case Vector::Line:
         pLineString_->addPoint(coordinate.x_, coordinate.y_, coordinate.z_);
         break;
      case Vector::Polygon:
         pLinearRing_->addPoint(coordinate.x_, coordinate.y_, coordinate.z_);
         REPORT_DEBUG("D:ringlineal clockwise == %s",
                      pLinearRing_->isClockwise()==TRUE ? "TRUE" : "FALSE");
         break;
      case Vector::GeometryCollection:
      case Vector::Undefined:
         return false;
         break;
   }

   // Muevo los puntos hasta la Posicion indicada
   for (int i = pointcount - 1; i > Position; i--) {
      GetPoint(coordinate, i - 1);
      SetPoint(coordinate, i);
   }
   // Si el punto no esta al final lo guardo
   if (Position != pointcount) {
      SetPoint(Point, Position);
   }

   return true;
}

/**
 * Obtiene las coordenadas de un punto de la linea.
 * En caso del punto se ignora la posicion.
 * @param[in] Position posicion dentro del vector del punto
 * -1 retorna el ultimo punto
 * @param[out] Point del punto en sistema de coordenadas del vector
 * @return bool que informa resultado de operacion
 */
bool OgrGeometryEditor::GetPoint(Coordinates &Point, int Position) {
   int pointcount = GetPointCount();
   // Si Position = -1 la seteo al final
   if (Position == -1) {
      Position = pointcount - 1;
   }

   if ((Position >= pointcount) || (Position < 0)) {
      return false;
   }
   switch (vecType_) {
      case Vector::Point:
         Point.x_ = pPoint_->getX();
         Point.x_ = pPoint_->getY();
         Point.x_ = pPoint_->getZ();
         break;
      case Vector::Line:
         if (Position > pLineString_->getNumPoints()) {
            return false;
         }
         Point.x_ = pLineString_->getX(Position);
         Point.y_ = pLineString_->getY(Position);
         Point.z_ = pLineString_->getZ(Position);
         break;
      case Vector::Polygon:
         if (Position > pLinearRing_->getNumPoints()) {
            return false;
         }
         Point.x_ = pLinearRing_->getX(Position);
         Point.y_ = pLinearRing_->getY(Position);
         Point.z_ = pLinearRing_->getZ(Position);
         break;
      case Vector::GeometryCollection:
      case Vector::Undefined:
         return false;
         break;
   }
   return true;
}

/**
 * Elimina un punto de la linea.
 * @param[in] Position posicion del punto que se quiere eliminar
 * -1 elimina el ultimo
 * @return bool que informa resultado de operacion
 */
bool OgrGeometryEditor::DeletePoint(int Position) {
   // Busco el ultimo punto
   int pointcount;
   Coordinates coordinate;
   pointcount = GetPointCount();

   // Si Position = -1 la seteo al final
   if (Position == -1) {
      Position = pointcount - 1;
   }

   // Si Postion esta fuera de rango salgo
   if ((Position >= pointcount) || (Position < 0)) {
      REPORT_DEBUG("D: Posicion fuera de rango");
      return false;
   }

   OGRLineString* ppreviousgeometry = NULL;
   // Creo nueva geometria
   OGRSpatialReference *pspatialref = new OGRSpatialReference(
         spatialReference_.c_str());
   switch (vecType_) {
      case Vector::Point:
         pPoint_ =
               dynamic_cast<OGRPoint*>(OGRGeometryFactory::createGeometry(wkbPoint));
         pPoint_->assignSpatialReference(pspatialref);
         break;
      case Vector::Line:
         ppreviousgeometry = pLineString_;
         pLineString_ = dynamic_cast<OGRLineString*>(OGRGeometryFactory::createGeometry(
               wkbLineString));
         pLineString_->assignSpatialReference(pspatialref);
         break;
      case Vector::Polygon:
         ppreviousgeometry = pLinearRing_;
         // linearRing guarda el OGRLinearRing contenido en polygon para agregar puntos
         pLinearRing_ = dynamic_cast<OGRLinearRing*>(OGRGeometryFactory::createGeometry(
               wkbLinearRing));
         pLinearRing_->assignSpatialReference(pspatialref);
         pPoligon_ = dynamic_cast<OGRPolygon*>(OGRGeometryFactory::createGeometry(
               wkbPolygon));
         pPoligon_->assignSpatialReference(pspatialref);
         break;
      case Vector::GeometryCollection:
      case Vector::Undefined:
         pspatialref->Release();
         return false;
         break;
   }
   pspatialref->Release();

   // Muevo los puntos hasta la Posicion indicada
   int j = 0;
   for (int i = 0; i < pointcount; i++) {
      if (ppreviousgeometry && i != Position) {
         coordinate.x_ = ppreviousgeometry->getX(i);
         coordinate.y_ = ppreviousgeometry->getY(i);
         coordinate.z_ = ppreviousgeometry->getZ(i);
         SetPoint(coordinate, j);
         j++;
      }
   }

   return true;
}

/**
 * Retorna la cantidad de puntos en el feature abierto
 * @return cantidad de puntos en el feature abierto
 */
int OgrGeometryEditor::GetPointCount() {
   int count = 0;
   switch (vecType_) {
      case Vector::Point:
         if (!pPoint_->IsEmpty()) {
            count = 1;
         }
         break;
      case Vector::Line:
         count = pLineString_->getNumPoints();
         break;
      case Vector::Polygon:
         count = pLinearRing_->getNumPoints();
         break;
      case Vector::Undefined:
      case Vector::GeometryCollection:
         count = -1;
         break;
   }
   return count;
}

/**
 * Configura el tipo de geometry y los punteros correspondientes
 * @param pGeometry Geometria
 */
void OgrGeometryEditor::SetGeometry(OGRGeometry* pGeometry) {
   pPoint_ = dynamic_cast<OGRPoint*>(pGeometry);
   if (pPoint_) {
      vecType_ = Vector::Point;
   } else {
      pLineString_ = dynamic_cast<OGRLineString*>(pGeometry);
      if (pLineString_) {
         vecType_ = Vector::Line;
      } else {
         pPoligon_ = dynamic_cast<OGRPolygon*>(pGeometry);
         if (pPoligon_) {
            pLinearRing_ = pPoligon_->getExteriorRing();

            // El formato shapefile requiere que los anillos esten
            // cerrados. Ver TCK #2699
            if (pLinearRing_->getNumPoints() > 1
                  && pLinearRing_->get_IsClosed() == TRUE)
               pLinearRing_->setNumPoints(pLinearRing_->getNumPoints() - 1);

            // Se crea nuevo poligono para evitar que se agregen varios
            // linearRings al mismo poligono al obtener la geometria
            pPoligon_ = dynamic_cast<OGRPolygon*>(OGRGeometryFactory::createGeometry(
                  wkbPolygon));
            OGRSpatialReference *pspatialref = new OGRSpatialReference(
                  spatialReference_.c_str());
            pPoligon_->assignSpatialReference(pspatialref);
            pspatialref->Release();
            vecType_ = Vector::Polygon;
         } else {
            pUndefined_ = pGeometry;
            vecType_ = Vector::Undefined;
         }
      }
   }
}

}  /** namespace suri **/
