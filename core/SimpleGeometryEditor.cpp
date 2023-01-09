/** \file GeometryEditor.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes suri
#include "SimpleGeometryEditor.h"
#include "Point.h"
#include "Line.h"
#include "Polygon.h"
#include "suri/VectorStyle.h"
#include "logmacros.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"

namespace suri {

/** Ctor */
SimpleGeometryEditor::SimpleGeometryEditor(Ring* pRing) {
   pEditedLine_ = pRing;
   type_ = SimpleRing;
}

/** Ctor */
SimpleGeometryEditor::SimpleGeometryEditor(Line* pLine) {
   pEditedLine_ = pLine;
   type_ = SimpleLine;
}

/** Ctor */
SimpleGeometryEditor::SimpleGeometryEditor(Point* pPoint) {
   pEditedLine_ = new Line(pPoint->GetSpatialReference());
   pEditedLine_->AppendPoint(pPoint);
   type_ = SimplePoint;
}

/** Dtor */
SimpleGeometryEditor::~SimpleGeometryEditor() {
   if (type_ == SimplePoint) {
      std::vector<SuriObject::UuidType> ids = pEditedLine_->GetPoints();
      // Saco el punto para que no se elimine al destruir linea.
      pEditedLine_->RemovePoint(*ids.begin());
      delete pEditedLine_;
   }
}

/**
 * Retorna los puntos en la geometria
 * @return vector con el id de los puntos en la geometria
 */
std::vector<SuriObject::UuidType> SimpleGeometryEditor::GetPoints() const {
   return pEditedLine_->GetPoints();
}

/**
 * Retorna la coordenada del punto con id indicado
 * @param[in] PointId id del punto
 * @param[out] PointCoordinate coordenada del punto
 * @return bool que indica si encontro el punto
 */
bool SimpleGeometryEditor::GetPointCoordinate(
                           const SuriObject::UuidType &PointId,
                                 Coordinates &PointCoordiante) const {
   const Point* ppoint = pEditedLine_->GetPoint(PointId);
   if (ppoint)
      PointCoordiante = ppoint->GetCoordinate();
   return ppoint != NULL;
}

/**
 * Retorna los puntos contenidos en el subset
 * @param[in] Subset donde quiero buscar
 * @param[in] SubsetSrs sistema de referencia en que estan lo puntos
 * del subset
 * @return set con los ids de los puntos contenidos en el vector.
 */
std::set<SuriObject::UuidType> SimpleGeometryEditor::FindPoints(
                                    const Subset &SearchSubset,
                                    const std::string &SubsetSrs) const {
   std::set<SuriObject::UuidType> result;
   ParameterCollection params;
   params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                pEditedLine_->GetSpatialReference());
   params.AddValue<std::string>(TransformationFactory::kParamWktOut, SubsetSrs);
   TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
   CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                     params);
   TransformationFactoryBuilder::Release(pfactory);
   if (!pct) {
      REPORT_DEBUG("D: Sistema de referencia de subset incompatible con la geometria");
      return result;
   }

   std::vector<SuriObject::UuidType> points = GetPoints();
   std::vector<SuriObject::UuidType>::iterator it = points.begin();
   Coordinates coordinate;
   while (it != points.end()) {
      if (GetPointCoordinate(*it, coordinate) &&
                  pct->Transform(coordinate) &&
                        SearchSubset.IsInside(coordinate))
         result.insert(*it);
      ++it;
   }

   delete pct;

   return result;
}

/**
 * Mueve un punto en la geoemtria
 * @param[in] PointId id del punto que quiero mover
 * @param[in] NewCoordinate Nueva coordenada del punto
 * @return bool que indica si pudo mover el punto.
 */
bool SimpleGeometryEditor::MovePoint(const SuriObject::UuidType &PointId,
                                          const Coordinates &NewCoordinate) {
   return pEditedLine_->MovePoint(PointId, NewCoordinate);
}

/**
 * Elimina un punto de la geometria
 * @param[in] id del punto
 * @return bool que indica si pudo eliminar el punto.
 */
bool SimpleGeometryEditor::DeletePoint(const SuriObject::UuidType &PointId) {
   return CanChangeLineSize() ? pEditedLine_->DeletePoint(PointId) : false;
}

/**
 * Agrega un punto a la geoemtria
 * @param[in] PointCoordinate coordenada del punto que quiero agrgar
 * @param[in] Posicion del punto en la geometria
 * @return id del punto que se agrego
 * @return SuriObject::NullUuid en caso de error.
 */
SuriObject::UuidType SimpleGeometryEditor::InsertPointBefore(
                              const Coordinates &PointCoordinate,
                              const SuriObject::UuidType &ReferencePointId) {
   if (!CanChangeLineSize())
      return SuriObject::NullUuid;

   Point* ppoint = new Point(PointCoordinate, pEditedLine_->GetSpatialReference());
   if (!pEditedLine_->InsertPointBefore(ppoint, ReferencePointId)) {
      delete ppoint;
      return SuriObject::NullUuid;
   }

   return ppoint->GetId();
}

/**
 * Agrega un punto al final de la geometria
 * @param[in] Coordenada del punto que se quiere agregar
 * @return id del punto que se agrego.
 * @return SuriObject::NullUuid en caso de error.
 */
SuriObject::UuidType SimpleGeometryEditor::AppendPoint(
                                 const Coordinates &PointCoordinate) {
   if (!CanChangeLineSize())
      return SuriObject::NullUuid;
   Point* point = new Point(PointCoordinate, pEditedLine_->GetSpatialReference());
   pEditedLine_->AppendPoint(point);
   return point->GetId();
}

/**
 * Configura estilo de la geometria completa
 * @param[in] pNewStyle estilo que quiero configurar
 * @return siempre true
 */
bool SimpleGeometryEditor::SetStyle(VectorStyle* pNewStyle) {
   pEditedLine_->SetStyle(pNewStyle);
   return true;
}

/**
 * Configura el estilo de un punto en la geometria
 * @param[in] PointId id del punto que quiero configurar
 * @param[in] pNewStyle estilo que quiero configurar
 * @return bool que indica si pudo configurar el punto.
 */
bool SimpleGeometryEditor::SetStyle(const SuriObject::UuidType &PointId,
                                                    VectorStyle* pNewStyle) {
   Point* ppoint = pEditedLine_->GetPoint(PointId);
   if (ppoint)
      ppoint->SetStyle(pNewStyle);
   return ppoint != NULL;
}

/**
 * Retorna el estilo que corresponde al la geometria editada
 * @param[in] PointId id del punto del que quiero el estilo
 * @param[in] Valores por default que debe tener el estilo.
 * @return Estilo formado a partir del estilo del punto, geometria y el default.
 * \attention el puntero retornado es responsabilidad del invocante.
 */
VectorStyle* SimpleGeometryEditor::GetStyle(const VectorStyle* pDefaultStyle,
                                          const SuriObject::UuidType &PointId) {
   VectorStyle* pnewstyle = NULL;
   const Point* ppoint = pEditedLine_->GetPoint(PointId);

   if (ppoint && ppoint->GetStyle() != NULL)
      pnewstyle = ppoint->GetStyle()->Clone("POINTSTYLE");

   if (!ppoint && pEditedLine_->GetStyle() != NULL)
      pnewstyle = pEditedLine_->GetStyle()->Clone("POINTSTYLE");

   if (!pnewstyle && pDefaultStyle != NULL)
         pnewstyle = pDefaultStyle->Clone("POINTSTYLE");
   else if (pnewstyle && pDefaultStyle != NULL) {
      if (!pnewstyle->GetBrush() && pDefaultStyle->GetBrush())
         pnewstyle->SetBrush(pDefaultStyle->GetBrush());
      if (!pnewstyle->GetPen() && pDefaultStyle->GetPen())
               pnewstyle->SetPen(pDefaultStyle->GetPen());
   }

   return pnewstyle;
}

/** Retorna el tipo de geometria editada */
SimpleGeometryEditor::GeometryType SimpleGeometryEditor::GetType() {
   return type_;
}

/**
 * Retorna el srs de la geometria editada
 * @return srs de la geometria editada
 */
std::string SimpleGeometryEditor::GetSpatialReference() {
   return pEditedLine_->GetSpatialReference();
}

/**
 * Inicializa un editor con la geometria que recibe por parametro
 * @param[in] pGeometry Geoemtria que quiero editar.
 * @return puntero al editor de geometria.
 */
SimpleGeometryEditor* SimpleGeometryEditor::InitializeEditor(
                                                      Geometry* pGeometry) {
   SimpleGeometryEditor* peditor = NULL;

   Polygon* ppolygon = dynamic_cast<Polygon*>(pGeometry);
   if (ppolygon) {
      std::vector<SuriObject::UuidType> rings = ppolygon->GetRings();
      Ring* pring = (rings.size() > 0) ? ppolygon->GetRing(rings[0]) : NULL;
      peditor = pring ? new SimpleGeometryEditor(pring) : NULL;
   }

   if (!peditor) {
      Line* pline = dynamic_cast<Line*>(pGeometry);
      peditor = pline ? new SimpleGeometryEditor(pline) : NULL;
   }

   if (!peditor) {
      Point* ppoint = dynamic_cast<Point*>(pGeometry);
      peditor = ppoint ? new SimpleGeometryEditor(ppoint) : NULL;
   }

   return peditor;
}

/** Indica si se puede modificar la longitud de la lista */
bool SimpleGeometryEditor::CanChangeLineSize() {
   // En caso que la linea sea interna, no tiene sentido que se puedan agregar
   // eliminar puntos ya que los cambios no se propagaran a geometria original.
   return type_ != SimplePoint;
}

/**
 * Retorna la posicion de un punto en la linea
 * @param[in] PointId id del punto
 * @return posicion del punto el la linea
 * @return -1 si no lo encontro.
 */
int SimpleGeometryEditor::GetPointPosition(const SuriObject::UuidType &PointId) {
   int pointposition = -1;
   std::vector<SuriObject::UuidType> points = pEditedLine_->GetPoints();
   std::vector<SuriObject::UuidType>::iterator it = points.begin();
   for (int i = 0; it != points.end() && pointposition < 0; i++) {
      if (PointId == *it)
         pointposition = i;
      ++it;
   }
   return pointposition;
}

/**
 * Le configura un srs a la geometria en edicion.
 * Transforma coordenadas para que esten en srs
 * La geometria no debe tener un srs pre-existente
 * @param[in] pRasterSpatialModel raster model que se usa para transformar
 * las coordenadas al srs.
 * @param[in] SpatialModelSrs nuevo sistema de referencia
 * @return bool que indica si tuvo exito.
 */
bool SimpleGeometryEditor::AddSpatialReference(RasterSpatialModel* pRasterSpatialModel,
                                                  const std::string &SpatialModelSrs) {
   if (!pEditedLine_->GetSpatialReference().empty())
      return false;

   std::vector<SuriObject::UuidType> points = pEditedLine_->GetPoints();
   std::vector<SuriObject::UuidType>::iterator it = points.begin();
   for (; it != points.end(); ++it) {
      Point* ppoint = pEditedLine_->GetPoint(*it);
      Coordinates coord = ppoint->GetCoordinate();
      pRasterSpatialModel->Transform(coord);
      ppoint->SetCoordinate(coord);
   }
   pEditedLine_->AssignSpatialReference(SpatialModelSrs);

   return true;
}

} /* namespace suri */
