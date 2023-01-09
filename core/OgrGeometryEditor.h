/*! \file OgrGeometryEditor.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */
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
