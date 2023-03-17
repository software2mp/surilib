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

// Includes standard
#include <set>

// Includes suri
#include "GuiGeometryEditor.h"
#include "SimpleGeometryEditor.h"
#include "logmacros.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/Configuration.h"

namespace suri {


/** Ctor */
GuiGeometryEditor::GuiGeometryEditor() {
   selectedPoint_ = SuriObject::NullUuid;
   pGeometryEditor_ = NULL;
   pRealPointStyle_ = VectorStyle::Create(Configuration::GetParameter("geo_edition_style",DEFAULT_GEO_EDITION_STYLE));//VectorStyle::Create("VECTORSTYLE[1,\"Punto real\",SYMBOL[\"Suri-symbol\",2,COLOR[233,183,10,255],SIZE[2]]]");
   pPhantomPointStyle_ = VectorStyle::Create(Configuration::GetParameter("geo_edition_style",DEFAULT_GEO_EDITION_STYLE));//VectorStyle::Create("VECTORSTYLE[1,\"Punto fantasma\",SYMBOL[\"Suri-symbol\",7,COLOR[233,183,10,255],SIZE[2]]]");
   pSelectedPointStyle_ = VectorStyle::Create(Configuration::GetParameter("geo_edition_style",DEFAULT_GEO_EDITION_STYLE));//VectorStyle::Create("VECTORSTYLE[1,\"Punto seleccionado\",SYMBOL[\"Suri-symbol\",2,COLOR[217,0,0,255],SIZE[2]]]");
   pEditedGeometryStyle_ = VectorStyle::Create(Configuration::GetParameter("geo_edition_style",DEFAULT_GEO_EDITION_STYLE));//VectorStyle::Create("VECTORSTYLE[2,\"Geometria en edicion\",BRUSH[\"Suri-brush\",0,COLOR[0,0,0,255]],PEN[\"Suri-pen\",1,COLOR[245,89,44,255],WIDTH[1]]]");

   pWorld_ = NULL;
}

/** Dtor */
GuiGeometryEditor::~GuiGeometryEditor() {
   End();
   VectorStyle::Destroy(pRealPointStyle_);
   VectorStyle::Destroy(pPhantomPointStyle_);
   VectorStyle::Destroy(pSelectedPointStyle_);
   VectorStyle::Destroy(pEditedGeometryStyle_);
}

/**
 * Inicia la edicion de una geometria. Se pisa el estilo de la geometria.
 * @param[in] pEditedGeometry geometria que quiero editar
 * @return bool que indica si pudo iniciar edicion
 */
bool GuiGeometryEditor::Start(Geometry* pEditedGeometry, World* pWorld) {
   if (IsEditing() || !pEditedGeometry || !pWorld)
      return false;
   pGeometryEditor_ = SimpleGeometryEditor::InitializeEditor(pEditedGeometry);
   if (!pGeometryEditor_)
      return false;
   if (!UpdateWorld(pWorld)) {
      delete pGeometryEditor_;
      pGeometryEditor_ = NULL;
      return false;
   }

   std::vector<SuriObject::UuidType> points = pGeometryEditor_->GetPoints();
   if (points.size() > 0)
      SelectPoint(*points.begin());

   pGeometryEditor_->SetStyle(pEditedGeometryStyle_->Clone());

   hasChanged_ = false;

   return CreatePhantomPoints();
}

/**
 * Finaliza la edicion de una geometria
 * @return bool que indica si pudo finalizar edicion correctamente.
 */
bool GuiGeometryEditor::End() {
   if (!IsEditing() || !DeletePhantomPoints())
      return false;
   delete pGeometryEditor_;
   pGeometryEditor_  = NULL;
   phantomPoints_.clear();
   selectedPoint_ = SuriObject::NullUuid;
   pWorld_ = NULL;
   return true;
}

/**
 * Indica si hay una geometria en edicion
 * @return bool que indica si hay geometria en edicion
 */
bool GuiGeometryEditor::IsEditing() const {
   return pGeometryEditor_ != NULL;
}

/**
 * Actualiza el mundo donde se edita la geometria
 * @param[in] pWorld mundo que estoy editando
 * @return bool que indica si pudo actualizar el mundo
 */
bool GuiGeometryEditor::UpdateWorld(World* pWorld) {
   if (!IsEditing() || !pWorld)
      return false;
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                pWorld->GetSpatialReference());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                pGeometryEditor_->GetSpatialReference());
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(
         TransformationFactory::kExact, params);
   TransformationFactoryBuilder::Release(pfactory);
   if (!pct) {
      REPORT_DEBUG("D: El sistema de coordenadas del mundo no es compatible con la geometria");
      return false;
   }
   delete pct;
   pWorld_ = pWorld;
   UpdatePhantomPoints();
   return true;
}

/** Retorna el mundo donde se edita la geometria */
World* GuiGeometryEditor::GetWorld() const {
   return pWorld_;
}

/** Indica si se modifico la geometria */
bool GuiGeometryEditor::HasChanged() const {
   return hasChanged_;
}

/**
 * Retorna el conjunto de puntos de la geometria editada
 * que estan dentro de SearchSubset
 * @param[in] SearchSubset subset donde quiero buscar los puntos
 */
std::set<SuriObject::UuidType> GuiGeometryEditor::FindPoints(
                                          const Subset &SearchSubset) {

   Subset worldsubset;
   pWorld_->Transform(SearchSubset.lr_, worldsubset.lr_);
   pWorld_->Transform(SearchSubset.ul_, worldsubset.ul_);

   return pGeometryEditor_->FindPoints(worldsubset, pWorld_->GetSpatialReference());
}

/**
 * Selecciona un punto en la geometria editada
 * @param[in] PointId id del punto a seleccionar
 * @return bool que indica si pudo seleccionar el punto
 */
bool GuiGeometryEditor::SelectPoint(const SuriObject::UuidType &PointId) {
   if (selectedPoint_ == PointId)
      return true;
   selectedPoint_ = PointId;
   return UpdatePointStyles();
}

/**
 * Cambia las coordenadas del punto seleccionado. Si era un punto
 * fantasma lo hace real y agrega los puntos fantasma adyacentes.
 * @param[in] NewCoordinate nueva coordenada que quiero asignar al punto.
 * Esta en pixel y linea de viewport
 * @return bool que indica si pudo mover el punto
 */
bool GuiGeometryEditor::MoveSelectedPoint(const Coordinates &NewCoordinate) {
   Coordinates pointcoordinate;
   if (!ViewportToGeometryCoordinateTranform(NewCoordinate, pointcoordinate) ||
                 !pGeometryEditor_->MovePoint(selectedPoint_, pointcoordinate))
      return false;

   std::set<SuriObject::UuidType>::iterator it = phantomPoints_.find(selectedPoint_);
   if (it != phantomPoints_.end()) {
      phantomPoints_.erase(it);
      // Las coordenadas se calculan en UpdatePhantomPoints
      AddPhantomPoint(Coordinates(0,0), FindNextRealPoint(selectedPoint_));
      AddPhantomPoint(Coordinates(0,0), selectedPoint_);
   }

   hasChanged_ = true;

   return UpdatePhantomPoints() && UpdatePointStyles();
}

/**
 * Elimina el punto seleccionado de la geometria
 * @return bool que indica si pudo eliminar el punto.
 */
bool GuiGeometryEditor::DeleteSelectedPoint() {
   if (IsPhantomPoint(selectedPoint_))
      return false;

   SuriObject::UuidType nextphantompointid = FindNextPoint(selectedPoint_);
   std::set<SuriObject::UuidType>::iterator it = phantomPoints_.find(nextphantompointid);
   if (nextphantompointid == SuriObject::NullUuid || it == phantomPoints_.end())
      return false;

   pGeometryEditor_->DeletePoint(nextphantompointid);
   pGeometryEditor_->DeletePoint(selectedPoint_);
   phantomPoints_.erase(*it);

   hasChanged_ = true;

   return UpdatePhantomPoints();
}

/**
 * Retorna el id del punto seleccionado
 * @return id del punto seleccionado
 */
SuriObject::UuidType GuiGeometryEditor::GetSelectedPoint() {
   return selectedPoint_;
}

/**
 * Agrega los puntos fantasmas a la geometria
 * @return bool que indica si pudo crear los puntos.
 */
bool GuiGeometryEditor::CreatePhantomPoints() {
   if (!phantomPoints_.empty())
      return false;

   Coordinates previewspoint;
   Coordinates currentpoint;
   std::vector<SuriObject::UuidType> points = pGeometryEditor_->GetPoints();
   std::vector<SuriObject::UuidType>::iterator it = points.begin();
   for (++it; it != points.end(); ++it)
      // Las coordenadas se calculan en UpdatePhantomPoints
      AddPhantomPoint(Coordinates(0,0), *it);

   return UpdatePhantomPoints() && UpdatePointStyles();
}

/**
 * Elimina los puntos fantasmas de la geometria
 * @return bool que indica si pudo eliminar los puntos
 */
bool GuiGeometryEditor::DeletePhantomPoints() {
   std::set<SuriObject::UuidType>::iterator it = phantomPoints_.begin();
   for (; it != phantomPoints_.end(); ++it)
      pGeometryEditor_->DeletePoint(*it);
   phantomPoints_.clear();
   return true;
}

/**
 * Actualiza la posicion de los puntos fantasmas en la geometria
 * @return bool que indica si pudo actualizar los puntos
 */
bool GuiGeometryEditor::UpdatePhantomPoints() {
   std::vector<SuriObject::UuidType> points = pGeometryEditor_->GetPoints();

   Coordinates previewsrealpoint;
   Coordinates currentpoint;
   SuriObject::UuidType previewsphantompoint = SuriObject::NullUuid;

   // La posicion de los puntos fantasmas se debe calcular en el
   // srs del mundo.
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                pWorld_->GetSpatialReference());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                pGeometryEditor_->GetSpatialReference());
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(
         TransformationFactory::kExact, params);
   TransformationFactoryBuilder::Release(pfactory);
   if (!pct) {
      REPORT_DEBUG("D: No se pudo tranformar punto a srs de geometria");
      return false;
   }

   std::vector<SuriObject::UuidType>::iterator it = points.begin();
   for (; it != points.end(); ++it) {
      if (!IsPhantomPoint(*it)) {
         pGeometryEditor_->GetPointCoordinate(*it, currentpoint);
         pct->Transform(currentpoint, true);
         if (previewsphantompoint != SuriObject::NullUuid) {
            Coordinates phantompointcoordinate((previewsrealpoint.x_ + currentpoint.x_) / 2,
                                               (previewsrealpoint.y_ + currentpoint.y_) / 2);
            pct->Transform(phantompointcoordinate, false);
            pGeometryEditor_->MovePoint(previewsphantompoint, phantompointcoordinate);
         }
         previewsrealpoint = currentpoint;
      } else {
         previewsphantompoint = *it;
      }
   }
   delete pct;
   return true;
}

/**
 * Actualiza el estilo de los puntos en la geometria
 * @return siempre true
 */
bool GuiGeometryEditor::UpdatePointStyles() {
   std::vector<SuriObject::UuidType> points = pGeometryEditor_->GetPoints();
   std::vector<SuriObject::UuidType>::iterator it = points.begin();
   for (; it != points.end(); ++it) {
      if (*it == selectedPoint_)
         pGeometryEditor_->SetStyle(*it, pSelectedPointStyle_->Clone());
      else if (IsPhantomPoint(*it))
         pGeometryEditor_->SetStyle(*it, pPhantomPointStyle_->Clone());
      else
         pGeometryEditor_->SetStyle(*it, pRealPointStyle_->Clone());
   }

   return true;
}

/**
 * Retorna el id del punto siguiente a PointId en la geometria editada
 * @param[in] PointId id del punto de referencia
 * @return id del punto siguiente en la geometria
 * @return NULL si el el ultimo punto o no lo encontro
 */
SuriObject::UuidType GuiGeometryEditor::FindNextPoint(
                              const SuriObject::UuidType &PointId) {
   std::vector<SuriObject::UuidType> points = pGeometryEditor_->GetPoints();
   std::vector<SuriObject::UuidType>::iterator it = points.begin();
   for (; it != points.end() && *it != PointId; ++it);
   ++it;
   return it != points.end() ? *it : SuriObject::NullUuid;
}

/**
 * Retorna el id del punto real siguiente en la geometria editada
 * @param[in] PointId id del punto de referencia
 * @return id del punto real siguiente en la geometria
 * @return NULL si el el ultimo punto o no lo encontro
 */
SuriObject::UuidType GuiGeometryEditor::FindNextRealPoint(
                                 const SuriObject::UuidType &PointId) {
   SuriObject::UuidType nextpoint = FindNextPoint(PointId);
   while (nextpoint != SuriObject::NullUuid && nextpoint != PointId && 
                                               IsPhantomPoint(nextpoint))
      nextpoint = FindNextPoint(PointId);
   return nextpoint;
}

/**
 * Agrega un punto fantasma a geometria antes del punto indicado
 * @param[in] Coord coordenadas del punto fantasma que quiero agregar.
 * @param[in] ReferencePointId punto de referencia antes del cual quiero
 * agregar el punto
 * @return bool que indica si tuvo exito al agregar el punto
 */
bool GuiGeometryEditor::AddPhantomPoint(const Coordinates &Coord,
                            const SuriObject::UuidType &ReferencePointId) {
   std::vector<SuriObject::UuidType> points = pGeometryEditor_->GetPoints();
   SuriObject::UuidType newpointid = SuriObject::NullUuid;
   if (ReferencePointId != *points.begin())
      newpointid = pGeometryEditor_->InsertPointBefore(Coord, ReferencePointId);
   else
      newpointid = pGeometryEditor_->AppendPoint(Coord);
   if (newpointid == SuriObject::NullUuid)
      return false;
   phantomPoints_.insert(newpointid);
   return true;
}

/**
 * Tranforma coordenadas desde pixel-linea a srs de geometria
 * @param[in] ViewportCoordinate coordenada en srs de viewport
 * @param[out] GeometryCoordinate coordenada en srs de geometria
 * @return bool que indica si pudo transformar coordenada.
 */
bool GuiGeometryEditor::ViewportToGeometryCoordinateTranform(
                                       const Coordinates &ViewportCoordinate,
                                             Coordinates &GeometryCoordinate) {
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                pWorld_->GetSpatialReference());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                pGeometryEditor_->GetSpatialReference());
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(
         TransformationFactory::kExact, params);
   TransformationFactoryBuilder::Release(pfactory);
   if (!pct) {
      REPORT_DEBUG("D: No se pudo tranformar punto a srs de geometria");
      return false;
   }

   pWorld_->Transform(ViewportCoordinate, GeometryCoordinate);
   pct->Transform(GeometryCoordinate);
   delete pct;
   return true;
}

/**
 * Indica si un punto es fantasma
 * @return bool que indica si el punto es fantasma
 */
bool GuiGeometryEditor::IsPhantomPoint(const SuriObject::UuidType &PointId) {
   return phantomPoints_.find(PointId) != phantomPoints_.end();
}

} /* namespace suri */
