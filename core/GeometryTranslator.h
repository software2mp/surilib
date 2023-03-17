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
