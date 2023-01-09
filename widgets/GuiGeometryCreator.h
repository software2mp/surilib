/** \file GuiGeometryCreator.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GUIGEOMETRYCREATOR_H_
#define GUIGEOMETRYCREATOR_H_

// Includes standard
#include <string>

// Includes suri
#include "suri/World.h"
#include "Geometry.h"
#include "suri/Coordinates.h"
#include "suri/GuiGeometryCreatorInterface.h"

namespace suri {

// fordwards
class SimpleGeometryEditor;
class VectorStyle;

/**
 * Clase que permite crear una geometria graficamente. Dependiendo del tipo
 * de geometria creada, agrega un punto fantasma al final de la geometria
 * en edicion.
 * Las coordenadas que recibe deben estar en pixel-linea del viewport.
 */
class GuiGeometryCreator : public GuiGeometryCreatorInterface {
public:
   enum GeometryType {Polygon, Line, Point};

   /** Ctor */
   GuiGeometryCreator();
   /** Dtor */
   virtual ~GuiGeometryCreator();

   /** Inicia la creacion de una geometria */
   bool Start(GeometryType Type, const std::string& GeometrySrs, World* pWorld);
   /** Finaliza la edicion de una geometria */
   bool End();
   /** Indica si hay una geometria creandose */
   bool IsEditing() const;
   /** Configura el mundo donde se crea la geometria */
   void SetWorld(World* pWorld);
   /** Retorna el mundo donde se edita la geometria */
   World* GetWorld() const;

   /** Retorna la geometria creada */
   Geometry* GetCreatedGeometry();

   /** Agrega un punto al final de la geometria. */
   bool AppendPoint(const Coordinates &PointCoordinate);
   /** Elimina el ultimo punto que agregue a la geometria */
   bool RemoveLastPoint();
   /** Mueve el punto fantasma. */
   bool MovePhantomPoint(const Coordinates &NewCoordinate);
   /** Posiciona el punto fantasma sobre el ultimo punto agregado */
   bool HidePhantomPoint();

private:
   /** Crea una geometria de tipo correcto */
   bool CreateGeometry(const Coordinates &NewCoordinate,
                           const std::string &SpatialReference);
   /** Agrea el punto fantasma a la geometria en edicion */
   bool AddPhantomPoint(const Coordinates &Coord);
   /** Elimina el punto fantasma de la geometria en edicion */
   bool DeletePhantomPoint();
   /** Retorna el id del ultimo punto real en la geometria */
   SuriObject::UuidType GetLastPointId();

   /** Id del punto fantasma */
   SuriObject::UuidType phantomPoint_;
   /** Editor que permite modificar la geoemtria */
   SimpleGeometryEditor* pGeometryEditor_;
   /** Mundo donde se esta editando el vector */
   World* pWorld_;
   /** Tipo de geometria que estoy creando */
   GeometryType pGeometryType_;
   /** Puntero a la geometria creada */
   Geometry* pEditedGeometry_;

   /** Estilo de los puntos de geometria */
   VectorStyle* pEditedGeometryStyle_;
   VectorStyle* pRealPointStyle_;
   VectorStyle* pPhantomPointStyle_;
   std::string  geometrySpatialReference_;
};

} /* namespace suri */
#endif /* GUIGEOMETRYCREATOR_H_ */
