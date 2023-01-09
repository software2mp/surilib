/** \file GeometryEditor.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GEOMETRYEDITOR_H_
#define GEOMETRYEDITOR_H_

// Includes standard
#include <vector>
#include <set>
#include <string>

// Includes suri
#include "Geometry.h"
#include "suri/SuriObject.h"
#include "suri/Subset.h"
#include "suri/Coordinates.h"
#include "suri/RasterSpatialModel.h"

namespace suri {

// fordwards
class Point;
class Line;
class Ring;

/**
 * Facade que permite editar una geometria.
 * Para simplificar la edicion, trata a la geomtria como un listado de puntos.
 * Dependiendo del tipo concreto de geometria estan soportadas las operaciones de
 * agregar/remover puntos.
 */
class SimpleGeometryEditor {
   /** Ctor */
   explicit SimpleGeometryEditor(Ring* pRing);
   /** Ctor */
   explicit SimpleGeometryEditor(Line* pLine);
   /** Ctor */
   explicit SimpleGeometryEditor(Point* pPoint);

public:
   enum GeometryType {SimplePoint = 0, SimpleLine, SimpleRing};

   /** Dtor */
   virtual ~SimpleGeometryEditor();

   /** Retorna los puntos en la geometria */
   std::vector<SuriObject::UuidType> GetPoints() const;
   /** Retorna la coordenada del punto con id indicado */
   bool GetPointCoordinate(const SuriObject::UuidType &PointId,
                                Coordinates &PointCoordinate) const;
   /** Retorna los puntos contenidos en el subset */
   std::set<SuriObject::UuidType> FindPoints(const Subset &SearchSubset,
                                      const std::string &SubsetSrs) const;

   /** Mueve un punto en la geoemtria */
   bool MovePoint(const SuriObject::UuidType &PointId,
                                 const Coordinates &NewCoordinate);
   /** Elimina un punto de la geometria */
   bool DeletePoint(const SuriObject::UuidType &PointId);
   /** Agrega un punto antes del punto indicado */
   SuriObject::UuidType InsertPointBefore(const Coordinates &PointCoordinate,
                                 const SuriObject::UuidType &ReferencePointId);
   /** Agrega un punto al final de la geometria */
   SuriObject::UuidType AppendPoint(const Coordinates &PointCoordinate);

   /** Configura estilo de la geometria completa */
   bool SetStyle(VectorStyle* pNewStyle);
   /** Configura el estilo de un punto en la geometria */
   bool SetStyle(const SuriObject::UuidType &PointId, VectorStyle* pNewStyle);
   /** Retorna el estilo que corresponde al la geometria indicada */
   VectorStyle* GetStyle(const VectorStyle* pDefaultStyle,
              const SuriObject::UuidType &PointId = SuriObject::NullUuid);
   /** Retorna el tipo de la geometria editada */
   GeometryType GetType();
   /** Retorna el srs de la geometria editada */
   std::string GetSpatialReference();

   /** Asigna un srs a la geometria. Transforma coordenadas para que esten en srs */
   bool AddSpatialReference(RasterSpatialModel* pRasterSpatialModel,
                                         const std::string &SpatialModelSrs);

   /** 
    * Inicializa un editor con la geometria que recibe por parametro 
    * Si la geometria es un poligono, trabaja con el primer anillo.
    * Si la geometria es un punto, no permite agregar/remover puntos.
    */
   static SimpleGeometryEditor* InitializeEditor(Geometry* pGeometry);

private:
   /** Indica si se puede modificar la longitud de la lista */
   bool CanChangeLineSize();
   /** Retorna la posicion de un punto en la linea */
   int GetPointPosition(const SuriObject::UuidType &PointId);
   /** Linea en edicion */
   Line* pEditedLine_;
   /** Tipo de geometria editado */
   GeometryType type_;
};

} /* namespace suri */

#endif /* GEOMETRYEDITOR_H_ */
