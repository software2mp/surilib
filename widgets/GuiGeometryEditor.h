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
