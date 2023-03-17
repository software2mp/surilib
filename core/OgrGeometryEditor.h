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

#ifndef OGRGEOMETRYEDITOR_H_
#define OGRGEOMETRYEDITOR_H_

// Includes standard

// Includes Suri
#include "suri/Subset.h"
#include "suri/Vector.h"
// Includes Wx
#include "wx/wx.h"
// Includes App

// Includes OGR

// forwards
class OGRGeometry;
class OGRLineString;
class OGRLinearRing;
class OGRPoint;
class OGRPolygon;
class OGRSpatialReference;

/** namespace suri */
namespace suri {
/** Clase abstracta con metodos compunes a todas las geometrias */
/**
 * Permite editar una geometria, los tipos de geometrias permitidos son
 * punto, linea y poligono.
 * Tiene un constructor que crea la geometria y otro que la abre. Si el tipo
 * de geometria que se abre no se conoce la guarda de todas formas pero no
 * permite agregar puntos.
 */
class OgrGeometryEditor {
public:
   /** ctor */
   OgrGeometryEditor(const std::string &SpatialReference, Vector::VectorType GeometryType);
   /** Ctor. */
   OgrGeometryEditor(OGRGeometry* Geometry);
   /** dtor */
   virtual ~OgrGeometryEditor();
   /** Retorna el tipo de geometria */
   virtual Vector::VectorType GetGeometryType();
   /** Retorna la referencia espacial de la geometria */
   virtual std::string GetSpatialReference();
   /** Informa si la referencia espacial es valida. */
   virtual bool IsValid();
   /** Retorna la geometria en formato ORG */
   virtual OGRGeometry* GetOgrGeometry();
   /** Agrega un punto a la geometria */
   virtual bool AddPoint(const Coordinates &Point, int Position = -1);
   /** MOdifica un punto a la geometria */
   virtual bool SetPoint(const Coordinates &Point, int Position = -1);
   /** Obtiene las coordenadas de un punto de la geometria */
   virtual bool GetPoint(Coordinates &Point, int Position);
   /** Elimina el punto en Posicion de la geometria */
   virtual bool DeletePoint(int Position);
   /** Retorna la cantidad de puntos en el feature abierto */
   virtual int GetPointCount();
   /** Indica la geometria asociada a partir de un wkt **/
   bool SetGeometryFromWkt(const std::string& Wkt, OGRSpatialReference* pSpatialRef = NULL);

private:
   /** Configura el tipo de geometry y los punteros correspondientes */
   void SetGeometry(OGRGeometry* pGeometry);
   Vector::VectorType vecType_; /*! tipo de vector que edita la geometria */
   std::string spatialReference_; /*! referencia espacial de todos los puntos */
   /* en la geometria */
   OGRLineString* pLineString_; /*! Geometria que se usa cuando tengo una */
   /* linea */
   OGRLinearRing* pLinearRing_; /*! Geometria que se usa cuando tengo un */
   /* poligono */
   OGRPoint* pPoint_; /*! Geometria que se usa cuando tengo puntos */
   OGRPolygon* pPoligon_; /*! Geometria que se usa cuando tengo poligono */
   OGRGeometry* pUndefined_; /*! Geometria que se usa cuando no se que tipo */
   /* de vector es. No permite agregar puntos */
};
}

#endif /*OGRGEOMETRYEDITOR_H_*/
