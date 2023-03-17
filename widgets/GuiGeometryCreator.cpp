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

// Includes suri
#include "GuiGeometryCreator.h"
#include "SimpleGeometryEditor.h"
#include "Ring.h"
#include "Polygon.h"
#include "Point.h"
#include "suri/VectorStyle.h"
#include "logmacros.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/Configuration.h"

namespace suri {

/** Ctor */
GuiGeometryCreator::GuiGeometryCreator() {
   pWorld_ = NULL;
   pGeometryEditor_ = NULL;
   pEditedGeometry_ = NULL;
   phantomPoint_ = SuriObject::NullUuid;
   // Punto de edicion.
   pRealPointStyle_ = VectorStyle::Create("VECTORSTYLE[1,\"Punto real\",SYMBOL[\"Suri-symbol\",2,COLOR[233,183,10,255],SIZE[2]]]");
   pPhantomPointStyle_ = VectorStyle::Create("VECTORSTYLE[1,\"Punto fantasma\",SYMBOL[\"Suri-symbol\",7,COLOR[233,183,10,255],SIZE[2]]]");
   pEditedGeometryStyle_ = VectorStyle::Create("VECTORSTYLE[2,\"Geometria en edicion\",BRUSH[\"Suri-brush\",0,COLOR[0,0,0,255]],PEN[\"Suri-pen\",1,COLOR[245,89,44,255],WIDTH[1]]]");
}

/** Dtor */
GuiGeometryCreator::~GuiGeometryCreator() {
   End();
   VectorStyle::Destroy(pRealPointStyle_);
   VectorStyle::Destroy(pPhantomPointStyle_);
   VectorStyle::Destroy(pEditedGeometryStyle_);
}

/**
 * Inicia la creacion de una geometria
 * @param[in] Type tipo de geometria que se desea crear
 * @param[in] GeometrySrs srs de la geometria a crear
 * @param[in] pWorld mundo donde quiero editar la geometria
 * @return bool que indica si pudo configurar la creacion
 **/
bool GuiGeometryCreator::Start(GeometryType Type,
                  const std::string& GeometrySrs, World* pWorld) {
   if (IsEditing() || !pWorld)
      return false;

   SetWorld(pWorld);
   geometrySpatialReference_ = GeometrySrs;
   pGeometryType_ = Type;
   return true;
}

/**
 * Finaliza la edicion de una geometria. Si se llama a este metodo sin
 * haber llamado a GetCreatedGeometry() ocurre memoryleak.
 * @return bool que indica si finalizo la edicion correctamente.
 */
bool GuiGeometryCreator::End() {
   if (!IsEditing() || !DeletePhantomPoint())
      return false;
   delete pGeometryEditor_;
   pGeometryEditor_  = NULL;
   pEditedGeometry_ = NULL;
   pWorld_ = NULL;
   return true;
}

/**
 * Indica si hay una geometria creandose
 * @return bool que indica si se esta creando una geometria
 */
bool GuiGeometryCreator::IsEditing() const {
   return pWorld_ != NULL;
}

/**
 * Configura el mundo donde se crea la geometria
 * @param[in] pWorld mundo que quiere configurar
 */
void GuiGeometryCreator::SetWorld(World* pWorld) {
   pWorld_ = pWorld;
}
/**
 * Retorna el mundo donde se crea la geometria
 * @return mundo
 */
World* GuiGeometryCreator::GetWorld() const {
   return pWorld_;
}

/**
 * Retorna la geometria creada
 * @return geometria creada
 * @return NULL si no hay geometria en edicion o todavia no se agrego el primer
 * punto.
 */
Geometry* GuiGeometryCreator::GetCreatedGeometry() {
   return pEditedGeometry_;
}

/**
 * Agrega un punto al final de la geometria.
 * @param[in] PointCoordinate coordenada del punto que quiero configurar.
 * Las coordenadas estan en pixel-linea del viewport.
 * @return bool que indica si pudo agregar el punto.
 */
bool GuiGeometryCreator::AppendPoint(const Coordinates &PointCoordinate) {
   if (!IsEditing())
      return false;

   Coordinates geometrycoordinate;
   pWorld_->Transform(PointCoordinate, geometrycoordinate);
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                pWorld_->GetSpatialReference());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                geometrySpatialReference_);

   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                     params);
   TransformationFactoryBuilder::Release(pfactory);

   if (!pct ||pct->Transform(geometrycoordinate) < 1) {
      if (pct)
         delete pct;
      return false;
   }
   delete pct;
   if (pGeometryEditor_) {
      SuriObject::UuidType pointid = pGeometryEditor_->
                          InsertPointBefore(geometrycoordinate, phantomPoint_);
      pGeometryEditor_->SetStyle(pointid, pRealPointStyle_->Clone());
      return pointid != SuriObject::NullUuid;
   } else {
      return CreateGeometry(geometrycoordinate, geometrySpatialReference_);
   }
}

/**
 * Elimina el ultimo punto que se agrego a la geometria
 * @return bool que indica si pudo eliminar el punto
 */
bool GuiGeometryCreator::RemoveLastPoint() {
   if (!IsEditing() || !pGeometryEditor_)
      return false;

   SuriObject::UuidType lastpoint = GetLastPointId();
   if (lastpoint != SuriObject::NullUuid)
      return pGeometryEditor_->DeletePoint(lastpoint);

   return false;
}

/**
 * Mueve el punto fantasma a la posicion indicada.
 * @param[in] NewCoordinate nueva coordenada del punto fantasma. Las coordenadas
 * estan en pixel-linea del viewport.
 * @return bool que indica si pudo mover el punto.
 */
bool GuiGeometryCreator::MovePhantomPoint(const Coordinates &NewCoordinate) {
   if (!IsEditing() || !pGeometryEditor_ || phantomPoint_ == SuriObject::NullUuid)
      return false;
   Coordinates geometrycoordinate;
   pWorld_->Transform(NewCoordinate, geometrycoordinate);
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                pWorld_->GetSpatialReference());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                pGeometryEditor_->GetSpatialReference());

   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                     params);
   TransformationFactoryBuilder::Release(pfactory);
   if (!pct || pct->Transform(geometrycoordinate) < 1) {
      if (pct)
         delete pct;
      return false;
   }
   delete pct;
   return pGeometryEditor_->MovePoint(phantomPoint_, geometrycoordinate);
}

/**
 * Posiciona el punto fantasma sobre el ultimo punto agregado
 * @return bool que indica si pudo esconder el punto fantasma
 */
bool GuiGeometryCreator::HidePhantomPoint() {
   if (!IsEditing() || !pGeometryEditor_ || phantomPoint_ == SuriObject::NullUuid)
      return false;
   Coordinates coord;
   return pGeometryEditor_->GetPointCoordinate(GetLastPointId(), coord) &&
                              pGeometryEditor_->MovePoint(phantomPoint_, coord);
}

/**
 * Crea una geometria del tipo correcto. Agrega el primer punto a la geometria.
 * @param[in] Coord coordenada del primer punto en la geometria
 * @return bool que indica si pudo crear la geometria
 */
bool GuiGeometryCreator::CreateGeometry(const Coordinates &Coord,
                                       const std::string &SpatialReference) {
   if (!IsEditing())
      return false;

   pEditedGeometry_ = NULL;
   suri::Polygon* ppolygon = NULL;
   switch (pGeometryType_) {
      case Polygon:
         ppolygon = new suri::Polygon(SpatialReference);
         ppolygon->AppendRing(new suri::Ring(SpatialReference));
         ppolygon->SetStyle(pRealPointStyle_->Clone());
         pEditedGeometry_ = ppolygon;
         break;
      case Line:
         pEditedGeometry_ = new suri::Line(SpatialReference);
         break;
      case Point:
         pEditedGeometry_ = new suri::Point(Coord, SpatialReference);
         pEditedGeometry_->SetStyle(pRealPointStyle_->Clone());
         break;
   }

   pGeometryEditor_ = SimpleGeometryEditor::InitializeEditor(pEditedGeometry_);
   if (!pGeometryEditor_) {
      delete pEditedGeometry_;
      pEditedGeometry_ = NULL;
   }

   pGeometryEditor_->SetStyle(pEditedGeometryStyle_->Clone());

   if (pGeometryEditor_ && pGeometryType_ != Point) {
      SuriObject::UuidType pointid = pGeometryEditor_->AppendPoint(Coord);
      pGeometryEditor_->SetStyle(pointid, pRealPointStyle_->Clone());
      AddPhantomPoint(Coord);
   }

   return pGeometryEditor_ != NULL;
}

/**
 * Agrega el punto fantasma a la geometria en edicion
 * @param[in] Coord coordenada inicial del punto
 * @return bool que indica si pudo agregar el punto fantasma.
 */
bool GuiGeometryCreator::AddPhantomPoint(const Coordinates &Coord) {
   if (pGeometryType_ == Point || !pGeometryEditor_)
      return false;
   phantomPoint_ = pGeometryEditor_->AppendPoint(Coord);
   pGeometryEditor_->SetStyle(phantomPoint_, pPhantomPointStyle_->Clone());
   return true;
}

/**
 * Elimina el punto fantasma de la geometria en edicion
 * @return bool que indica si pudo eliminar el punto fantasma
 */
bool GuiGeometryCreator::DeletePhantomPoint() {
   if (!pGeometryEditor_)
      return true;

   if (pGeometryType_ == Point || pGeometryEditor_->DeletePoint(phantomPoint_)) {
      phantomPoint_ = SuriObject::NullUuid;
      return true;
   }
   return false;
}

/**
 * Retorna el id del ultimo punto real en la geometria
 * @return id del ultimo punto real en la geometria
 */
SuriObject::UuidType GuiGeometryCreator::GetLastPointId() {
   if (!pGeometryEditor_)
      return SuriObject::NullUuid;

   std::vector<SuriObject::UuidType> ids = pGeometryEditor_->GetPoints();
   std::vector<SuriObject::UuidType>::iterator it = ids.begin();
   SuriObject::UuidType lastrealpoint = SuriObject::NullUuid;
   for (; it != ids.end() && *it != phantomPoint_; ++it)
      lastrealpoint = *it;

   return (it != ids.end()) ? lastrealpoint : SuriObject::NullUuid;
}

} /* namespace suri */
