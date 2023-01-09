/** \file GeometryTranslator.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOMETRYTRANSLATOR_H_
#define GEOMETRYTRANSLATOR_H_

// Includes standard
#include <string>

// fordwards
class OGRGeometry;
class OGRPoint;
class OGRLineString;
class OGRPolygon;

namespace suri {

// fordwards
class Geometry;
class Point;
class Line;
class Polygon;

/**
 * Clase que transforma entre Geometry y WKT y viceversa.
 * Al tranformar a wkt se pierde el estilo de la geometria.
 */
class GeometryTranslator {
   /** Ctor */
   GeometryTranslator();

public:
   /** Dtor */
   virtual ~GeometryTranslator();

   /** Tranforma de geometry ogr a wkt */
   static std::string ExportToWkt(OGRGeometry* pOgrGeometry);
   /** Tranforma de geometry a wkt */
   static std::string ExportToWkt(Geometry* pGeometry);
   /** Tranforma de geometry a wkt */
   static Geometry* ImportFromWkt(const std::string &GeometryWkt,
                                        const std::string& GeometrySrs);

   /** Retorna el wkt de una geometria de tipo punto */
   static OGRPoint* GetOgrPoint(const Point* pPoint);
   /** Retorna el wkt de una geometria de tipo linea */
   static OGRLineString* GetOgrLine(const Line* pLine);
   /** Retorna el wkt de una geometria de tipo poligono */
   static OGRPolygon* GetOgrPolygon(const Polygon* pPolygon);

   /** Retorna una geometria de tipo punto */
   static Point* GetPoint(const OGRPoint* pOgrPoint);
   /** Retorna una geometria de tipo linea */
   static Line* GetLine(const OGRLineString* pOgrLine);
   /** Retorna una geometria de tipo poligono */
   static Polygon* GetPolygon(const OGRPolygon* pOgrPolygon);

private:
   /** Configura un OGRLineString con puntos en Line */
   static void CopyLinePointsToOGRLine(const Line* pLine,
                                                   OGRLineString* pOgrLine);
   static void CopyOGRLinePointsToLine(const OGRLineString* pOgrLine,
                                                   Line* pLine);
};

} /* namespace suri */
#endif /* GEOMETRYTRANSLATOR_H_ */
