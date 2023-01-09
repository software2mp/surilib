/** \file GeometryRenderer.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <string>
#include <vector>

// Includes suri
#include "GeometryRenderer.h"
#include "Geometry.h"
#include "Mask.h"
#include "SimpleGeometryEditor.h"
#include "suri/VectorStyle.h"
#include "suri/AuxiliaryFunctions.h"
#include "VectorRenderer.h"
#include "suri/World.h"
#include "GeometryCollection.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "suri/Configuration.h"

// Includes wx
#include "wx/wx.h"

namespace suri {

/** Ctor */
GeometryRenderer::GeometryData::GeometryData(int PointCount, VectorStyle* pStyle) {
   pointCount_ = PointCount;
   pStyle_ = pStyle;
   points_ = new wxPoint[pointCount_];
}

/** Dtor */
GeometryRenderer::GeometryData::~GeometryData() {
   delete[] points_;
   VectorStyle::Destroy(pStyle_);
}

/**
 * Ctor
 * @param[in] pDC wxDC sobre el que voy a renderizar
 * @param[in] pWorldWindow mundo asociado a pDC.
 */
GeometryRenderer::GeometryRenderer(wxDC* pDC,
                              const World* pWorldWindow) {
   pDC_ = pDC;
   pWorldWindow_ = pWorldWindow;
}

/** Dtor */
GeometryRenderer::~GeometryRenderer() {
}

/** 
 * Renderiza la coleccion de geometrias sobre wxDC
 * @param[in] pCollection coleccion de geometrias a renderizar
 * @param[in] pMask mascara que aplico sobre geometrias
 */
void GeometryRenderer::Render(GeometryCollection* pCollection,
                                                   const Mask* pMask) {
   if (!pCollection)
      return;
   std::vector<Geometry*> geometries;

   std::vector<SuriObject::UuidType> ids = pCollection->GetGeometries();
   std::vector<SuriObject::UuidType>::iterator it = ids.begin();
   for (; it != ids.end(); ++it)
      geometries.push_back(pCollection->GetGeometry(*it));

   Render(geometries, pCollection->GetStyle(), pMask);
}

/**
 * Renderiza las geometrias sobre el wxDC. Debido al costo que tiene modificar
 * el estilo del wxDC se envian a renderizar, primero las areas, despues las
 * lineas y por ultimo los puntos.
 * @param[in] Geoemtries geometrias a renderizar. Todas las geometrias
 * deben tener el mismo srs.
 * @param[in] pDefaultStyle estilo default que se le asigna a las geometrias
 * @param[in] pMask mascara que aplico sobre geometrias
 */
void GeometryRenderer::Render(std::vector<Geometry*> &Geometries,
                        const VectorStyle* pDefaultStyle,
                        const Mask* pMask) {

   if (Geometries.empty() || !pWorldWindow_ || !pDC_)
      return;

   CoordinatesTransformation *pct = NULL;
   if (!pWorldWindow_->GetSpatialReference().empty()
                         && !Geometries[0]->GetSpatialReference().empty()) {
      ParameterCollection params;
      params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                   Geometries[0]->GetSpatialReference());
      params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                   pWorldWindow_->GetSpatialReference());
      TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
      pct = pfactory->Create(TransformationFactory::kExact, params);
      TransformationFactoryBuilder::Release(pfactory);
      if (!pct) {
         REPORT_DEBUG("D: No se pudo renderizar geometria en srs del mundo");
         return;
      }
   }

   // Listado de puntos que se envian a renderizar.
   std::vector<GeometryData*> points;
   std::vector<GeometryData*> lines;
   std::vector<GeometryData*> rings;

   std::vector<Geometry*>::iterator it = Geometries.begin();


   for (; it != Geometries.end(); ++it) {
      SimpleGeometryEditor* pgeomeditor = SimpleGeometryEditor::InitializeEditor(*it);
      std::vector<SuriObject::UuidType> ids = pgeomeditor->GetPoints();

      // Si la coleccion tiene seteado estilo, se lo pasamos al render.
      if ( pDefaultStyle != NULL ) {
         VectorStyle *vStyle = pDefaultStyle->Clone();
         pgeomeditor->SetStyle(vStyle);
      }
      else {
         VectorStyle *vStyle = NULL;
         std::string vectorStyle = Configuration::GetParameter(
               "geo_creation_style", DEFAULT_GEO_CREATION_STYLE);
         if ( !vectorStyle.empty() )
            vStyle = VectorStyle::Create(vectorStyle.c_str());
         else
         // Si no fue seteado ningun estilo previamente, ponemos este por defecto
            vStyle = VectorStyle::Create();
         pgeomeditor->SetStyle(vStyle);
      }

      // Configuro datos de geometria
      GeometryData* pgeomdata = new GeometryData(ids.size(),
                                      pgeomeditor->GetStyle(pDefaultStyle));

      std::vector<SuriObject::UuidType>::iterator it2 = ids.begin();
      for (int i = 0; it2 != ids.end(); ++it2, i++) {
         Coordinates pointcoordinate;
         pgeomeditor->GetPointCoordinate(*it2, pointcoordinate);

         // Transformo los puntos en geometria a coordenadas de viewport
         int x, y;
         if (pct)
            pct->Transform(pointcoordinate);
         pWorldWindow_->W2VTransform(pointcoordinate.x_, pointcoordinate.y_, x, y);


         // Agrego punto a listado de puntos a renderizar
         GeometryData* ppointdata = new GeometryData(1,
                                                     pgeomeditor->GetStyle(pDefaultStyle, *it2));

         ppointdata->points_[0] = wxPoint(x, y);
         points.push_back(ppointdata);

         // Configuro las coordenadas en datos de geometria
         pgeomdata->points_[i] = wxPoint(x, y);
      }

      // Agrego datos de geometria a vector correspondiente
      if (pgeomeditor->GetType() == SimpleGeometryEditor::SimpleLine)
         lines.push_back(pgeomdata);
      if (pgeomeditor->GetType() == SimpleGeometryEditor::SimpleRing) {
         rings.push_back(pgeomdata);
         lines.push_back(pgeomdata);
      }

      delete pgeomeditor;
   }

   // Envio a renderizar los vectores
   Paint(rings, pMask);
   Trace(lines, pMask);
   Mark(points, pMask);

   // Elimino referencias
   delete pct;
   std::vector<GeometryData*>::iterator geomdatait = lines.begin();
   for (; geomdatait != lines.end(); ++geomdatait)
      delete *geomdatait;
   geomdatait = points.begin();
   for (; geomdatait != points.end(); ++geomdatait)
      delete *geomdatait;
}

/**
 * Verifica que tiene los elementos para realizar el Paint
 * Para cada anillo configura el brush en pDC_ y envia a renderizar.
 * @param[in] Rings vector con los datos necesarios para renderizar los anillos
 * @param[in] pMask puntero a la mascara raster binaria.
 * @return informa si pudo rellenar
 */
bool GeometryRenderer::Paint(std::vector<GeometryRenderer::GeometryData*> &Rings,
                                                                 const Mask* &pMask) {
   if (pDC_ == NULL || Rings.size() == 0) {
      REPORT_DEBUG("D:DC invalido.");
      return false;
   }

   for (size_t i = 0; i < Rings.size(); i++) {
      if (Rings[i]->pStyle_ && ConfigureDC(Rings[i]->pStyle_->GetBrush(), pDC_)) {
         ConfigureDC(Rings[i]->pStyle_->GetPen(), pDC_);
         pDC_->DrawPolygon(Rings[i]->pointCount_, Rings[i]->points_);
      }
   }

   return true;
}

/**
 * Verifica que tiene los elementos para realizar el Trace
 * Para cada linea configura el pen en pDC_ y envia a renderizar.
 * @param[in] Lines vector con los datos necesarios para renderizar las lineas
 * @param[in] pMask puntero a la mascara raster binaria.
 * @return informa si pudo dibujar
 */
bool GeometryRenderer::Trace(std::vector<GeometryRenderer::GeometryData*> &Lines,
                                                            const Mask * &pMask) {
   if (pDC_ == NULL || Lines.size() == 0) {
      REPORT_DEBUG("D:No se pudo renderizar lineas");
      return false;
   }

   for (size_t i = 0; i < Lines.size(); i++) {
      if (Lines[i]->pStyle_ && Lines[i]->pStyle_->GetPen()) {
         ConfigureDC(Lines[i]->pStyle_->GetPen(), pDC_);
         pDC_->DrawLines(Lines[i]->pointCount_, Lines[i]->points_);
      }
   }

   return true;
}

/**
 * Verifica que tiene los elementos para poner marcas
 * Para cada punto configura el simbolo en pDC_ y envia a renderizar.
 * @param[in] Points vector con los datos necesarios para renderizar los puntos
 * @param[in] pMask puntero a la mascara raster binaria.
 * @return informa si pudo dibujar
 */
bool GeometryRenderer::Mark(
            std::vector<GeometryRenderer::GeometryData*> &Points,
                                                const Mask* &pMask) {
   if (pDC_ == NULL || Points.size() == 0) {
      REPORT_DEBUG("D:DC invalido.");
      return false;
   }

   for (size_t i = 0; i < Points.size(); i++) {
      if (Points[i]->pStyle_ && Points[i]->pStyle_->GetSymbol()) {
         VectorStyle::Symbol* psymbol = Points[i]->pStyle_->GetSymbol();
         wxPoint *pwxpoly = NULL;
         int polypoints = 0;

         ConfigureDC(psymbol, pDC_);
         GetSymbolPoints(psymbol, pwxpoly, polypoints);

         if (psymbol->id_ == VectorStyle::Symbol::Circle
               || psymbol->id_ == VectorStyle::Symbol::EmptyCircle)
            pDC_->DrawCircle(Points[i]->points_->x, Points[i]->points_->y, psymbol->size_);
         else
            pDC_->DrawPolygon(polypoints, pwxpoly, Points[i]->points_->x,
                              Points[i]->points_->y);
         delete[] pwxpoly;
      }
   }

#if defined(__DEBUG__) && defined(__ENUMERATE_VERTEX__)
   for (size_t i = 0; i < Points.size() - 1; i++) {
      pDC->DrawText(wxString::Format(PERCENT_U, i), *(Points[i]));
   }
#endif

   return true;
}


} /* namespace suri */
