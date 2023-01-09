/** \file GuiGeometryEditor.h */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GUIGEOMETRYEDITOR_H_
#define GUIGEOMETRYEDITOR_H_

// Includes standard
#include <set>

// Includes suri
#include "suri/Coordinates.h"
#include "suri/VectorStyle.h"
#include "Geometry.h"
#include "suri/Subset.h"
#include "suri/World.h"
#include "suri/GuiGeometryEditorInterface.h"

namespace suri {

// fordwards
class SimpleGeometryEditor;

/**
 * Clase que permite editar una geometria graficamente.
 * Las coordenadas que recibe deben estar en pixel-linea del viewport.
 */
class GuiGeometryEditor : public GuiGeometryEditorInterface {
public:
   /** Ctor */
   GuiGeometryEditor();
   /** Dtor */
   virtual ~GuiGeometryEditor();

   /** Inicia la edicion de una geometria */
   bool Start(Geometry* pEditedGeometry, World* pWorld);
   /** Finaliza la edicion de una geometria */
   bool End();
   /** Indica si hay una geometria en edicion */
   bool IsEditing() const;
   /** Actualiza el mundo donde se edita la geometria */
   bool UpdateWorld(World* pWorld);
   /** Retorna el mundo donde se edita la geometria */
   World* GetWorld() const;
   /** Indica si se modifico la geometria */
   bool HasChanged() const;

   /** Selecciona un punto dentro del subset */
   std::set<SuriObject::UuidType> FindPoints(const Subset &SearchSubset);
   /** Selecciona un punto con el id indicado */
   bool SelectPoint(const SuriObject::UuidType &PointId);
   /** Cambia las coordenadas del punto seleccionado. */
   bool MoveSelectedPoint(const Coordinates &NewCoordinate);
   /** Elimina el punto seleccionado de la geometria */
   bool DeleteSelectedPoint();
   /** Retorna el id del punto seleccionado */
   SuriObject::UuidType GetSelectedPoint();

private:
   /** Agrega los puntos fantasmas a la geometria */
   bool CreatePhantomPoints();
   /** Elimina los puntos fantasmas de la geometria */
   bool DeletePhantomPoints();
   /** Actualiza la posicion de los puntos fantasmas en la geometria */
   bool UpdatePhantomPoints();
   /** Actualiza el estilo de los puntos en la geometria */
   bool UpdatePointStyles();
   /** Retorna el id del punto siguiente en la geometria editada*/
   SuriObject::UuidType FindNextPoint(const SuriObject::UuidType &PointId);
   /** Retorna el id del punto real siguiente en la geometria editada*/
   SuriObject::UuidType FindNextRealPoint(const SuriObject::UuidType &PointId);
   /** Agrega un punto fantasma a geometria antes del punto indicado */
   bool AddPhantomPoint(const Coordinates &Coord,
                       const SuriObject::UuidType &ReferencePointId);
   /** Tranforma coordenadas desde pixel-linea a srs de geometria */
   bool ViewportToGeometryCoordinateTranform(const Coordinates &ViewportCoordinate,
                                                  Coordinates &GeometryCoordinate);
   /** Indica si un punto es fantasma */
   bool IsPhantomPoint(const SuriObject::UuidType &PointId);

   /** Id del punto seleccionado */
   SuriObject::UuidType selectedPoint_;
   /** Id de los puntos fantasmas */
   std::set<SuriObject::UuidType> phantomPoints_;
   /** Editor que permite modificar la geoemtria */
   SimpleGeometryEditor* pGeometryEditor_;
   /** Mundo donde se esta editando el vector */
   World* pWorld_;
   /** Indica si se modifico la geometria */
   bool hasChanged_;

   /** Estilo de los puntos de geometria en edicion */
   VectorStyle* pEditedGeometryStyle_;
   VectorStyle* pRealPointStyle_;
   VectorStyle* pPhantomPointStyle_;
   VectorStyle* pSelectedPointStyle_;
};

} /* namespace suri */
#endif /* GUIGEOMETRYEDITOR_H_ */
