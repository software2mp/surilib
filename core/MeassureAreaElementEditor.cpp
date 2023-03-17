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

#include <limits>
#include <memory>

#include "MeassureAreaElementEditor.h"
#include "SpatialReference.h"
#include "suri/VectorEditor.h"
#include "suri/VectorElement.h"
#include "suri/messages.h"
#include "resources.h"
#include "suri/ExactCoordinatesTransformation.h"
#include "suri/CoordinatesTransformation.h"
#include "suri/ParameterCollection.h"
#include "suri/TransformationFactory.h"
#include "suri/TransformationFactoryBuilder.h"
#include "MeasureChangedNotification.h"
#include "suri/ViewportManagerInterface.h"
#include "suri/ViewcontextManagerInterface.h"
#include "GuiGeometryCreator.h"
#include "GeometryCollection.h"
#include "GeometryRenderer.h"
#include "suri/PaintEventInterface.h"
#include "suri/ObserverInterface.h"
#include "suri/Subject.h"
#include "MeasureDeletionRequestedNotification.h"
#include "suri/Configuration.h"
#include "suri/DynamicViewerWidget.h"

/** Primer estilo para dibujar poligono */
#define MEASSURE_POLYGON_1 \
   "VECTORSTYLE[3,\"Poligono rojo\",BRUSH[\"Suri-brush\",2,COLOR[255,0,0,255]],PEN[\"Suri-pen\",1,COLOR[255,0,0,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",1,COLOR[128,255,0,255],SIZE[2]],LABEL[\"Suri-label\",0,EXPRESSION[],COLOR[0,0,0,255],BACKCOLOR[0,0,0,0],SIZE[8],ITALIC[0],BOLD[0],UNDERLINED[0],ANGLE[0],OFFSET[0,0],ANCHOR[4]]]"
/** Segundo estilo para dibujar poligono */
#define MEASSURE_POLYGON_2 \
   "VECTORSTYLE[3,\"Poligono verde\",BRUSH[\"Suri-brush\",2,COLOR[128,255,128,255]],PEN[\"Suri-pen\",2,COLOR[0,128,0,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",2,COLOR[128,255,0,255],SIZE[2]]]"
/** TErcer estilo para dibujar poligono */
#define MEASSURE_POLYGON_3 \
   "VECTORSTYLE[3,\"Poligono azul\",BRUSH[\"Suri-brush\",2,COLOR[0,255,255,255]],PEN[\"Suri-pen\",2,COLOR[0,0,255,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",3,COLOR[128,255,255,255],SIZE[2]]]"

/** Proyeccion mundial con conservacion de area */
// \todo Verificar si hay proyeccion que sea mejor para medir areas a nivel mundial
#define _AREA_MEASURE_PROJECTION_ \
   "PROJCS[\"World_Sinusoidal\",GEOGCS[\"GCS_WGS_1984\",DATUM[\"D_WGS_1984\",SPHEROID[\"WGS_1984\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]],PROJECTION[\"Sinusoidal\"],PARAMETER[\"false_easting\",0],PARAMETER[\"false_northing\",0],PARAMETER[\"longitude_of_center\",0],UNIT[\"Meter\",1]]"

#define LATLON_SPATIAL_REFERENCE \
   "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.2572235630016,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433],AUTHORITY[\"EPSG\",\"4326\"]]"

#define POLYGON_FILE_NAME "Polygon_1"

namespace suri {

/**
 * Ctor.
 *
 * @param[in]	pViewer: visualizador sobre el que se realizaran las mediciones.
 * @param[in]	ListGroup: Grupo de boton al que pertenece
 * @param[in]  pVectorEditor: Permite editar vectores
 * @param[in]	pParent: puntero a la herramienta de medicion que lo contiene
 */
MeassureAreaElementEditor::MeassureAreaElementEditor(
      suri::ViewerWidget* pViewer, int ListGroup,
      ViewportManagerInterface* pViewportManager,
      ViewcontextManagerInterface* pViewcontextManager) :
      Meassure(true), ViewportEvent(pViewportManager, pViewcontextManager),
      editing_(false), pViewer_(pViewer), pVectorEditor_(new VectorEditor) {
   Initialize();
}

/**
 * Resetea el estado del editor.
 */
void MeassureAreaElementEditor::Reset() {
}

/**
 * Se ejecuta cuando el usuario hace click sobre el visualizador.
 */
void MeassureAreaElementEditor::SetPoint(double Pixel, double Line,
                                         wxMouseEvent& Event) {
   RefreshViewer();
   if (pViewer_ != AS_VW(GetViewportManager()->GetSelectedViewport()))
      return;
   if (!IsEditing())
      return;

   if (Event.RightUp()) {
      Event.Skip(false);
      pGuiGeomCreator_->HidePhantomPoint();
      editing_ = false;
      pViewer_->GetWindow()->Refresh();
      return;
   }

   // Verifico el feature
   if (pVectorEditor_->GetOpenLayer() < 0
         || (pVectorEditor_->GetOpenFeature() < 0 && pVectorEditor_->OpenFeature() < 0))
      return;

   Coordinates pl(Pixel, Line);

   // Transformo los valores pixel/linea
   double wx, wy;
   pViewer_->GetWorld()->V2WTransform(Pixel, Line, wx, wy);
   Coordinates point(wx, wy);

   // verifico que el punto entre en el mundo
   Subset world;
   pViewer_->GetWorld()->GetWorld(world);
   if (!world.IsInside(point)) {
      SHOW_STATUS(message_POINT_OUTSIDE_ALLOWED_AREA_ERROR);
   } else {
      // Agrego las coordenadas
      pGuiGeomCreator_->AppendPoint(pl);

      pVectorEditor_->AddPoint(point);
      pVectorEditor_->SyncToDisk();

      CalculateMeassurement();
      if (area_ > 0) {
         SendMeasureChangedNotifcation(area_, perimeter_);
      }
   }
   pViewer_->GetWindow()->Refresh();
}

/**
 * Se ejecuta cuando el usuario se mueve en el visualizador.
 */
void MeassureAreaElementEditor::UpdatePoint(double Pixel, double Line,
                                            wxMouseEvent& Event) {
   if (IsEditing()) {
      Coordinates pl(Pixel, Line);
      pGuiGeomCreator_->MovePhantomPoint(pl);
      pViewer_->GetWindow()->Refresh();
   }
}

/**
 * Se ejecuta cuando el usuario comienza drag sobre visualizador.
 */
void MeassureAreaElementEditor::OnStartDrag(const Coordinates& InitialCoordinate) {

}

/**
 * Se ejecuta cuando el usuario finaliza drag.
 */
void MeassureAreaElementEditor::OnEndDrag(const Coordinates& InitialCoordinate,
                                          const Coordinates& CurrentCoordinate,
                                          wxMouseEvent& Event) {

}

/**
 * Se ejecuta cada vez que el usuario mueve el mouse mientras realiza drag.
 */
void MeassureAreaElementEditor::OnDrag(const Coordinates& InitialCoordinate,
                                       const Coordinates& CurrentCoordinate,
                                       wxMouseEvent& Event) {

}

/**
 * Al pintar se dibuja geometria en edicion.
 */
void MeassureAreaElementEditor::OnPaint(wxPaintEvent& Event, wxDC &Dc) {
   if (pViewer_ != AS_VW(GetViewportManager()->GetSelectedViewport()))
      return;
   Geometry* pgeom = pGuiGeomCreator_->GetCreatedGeometry();

   GeometryCollection* pgeomcoll = new GeometryCollection(LATLON_SPATIAL_REFERENCE);

   std::string vectorStyle = Configuration::GetParameter("geo_area_style", DEFAULT_GEO_AREA_STYLE);

   VectorStyle* pgeomstyle = NULL;

   if ( !vectorStyle.empty() )
      pgeomstyle = VectorStyle::Create(vectorStyle.c_str());
   else
      pgeomstyle = VectorStyle::Create(MEASSURE_POLYGON_1);

   pgeomcoll->SetStyle(pgeomstyle);
   pgeomcoll->AppendGeometry(pgeom);

   GeometryRenderer renderer(&Dc, pViewer_->GetWorld());
   renderer.Render(pgeomcoll, NULL);
}

/**
 * Indica que quiere recibir el evento OnPaint.
 */
PaintEventInterface::PaintEventFlags MeassureAreaElementEditor::GetPaintFlags() const {
   return PaintEventInterface::Paint;
}

/**
 *
 */
bool MeassureAreaElementEditor::CanProcessEvent(wxEvent &Event) const {
   wxWindow* pwindow = GetEventWindow(Event);
   ViewerWidget* pviewer =
         GetViewportManager() ? AS_VW(GetViewportManager()->GetSelectedViewport()) : pViewer_;
   DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(pviewer);
   if (pdynamic)
      pviewer = pdynamic->GetViewer();
   return pwindow && pviewer && (pviewer->GetWindow() == pwindow);
}

/**
 * Recibe notificaciones del control contenedor.
 */
void MeassureAreaElementEditor::Update(NotificationInterface* pNotification) {
   MeasureDeletionRequestedNotification* pmnot =
         dynamic_cast<MeasureDeletionRequestedNotification*>(pNotification);
   if (pmnot != NULL) {
      CloseFeature();
   }
}

/**
 *
 */
void MeassureAreaElementEditor::Start() {
   if (!IsActive()) {
      // Inicializo el editor de vectores
      pVectorEditor_->OpenVector("shpmemory:tmp_measure_vector_poly.shp");
      pVectorEditor_->OpenLayer(POLYGON_FILE_NAME, LATLON_SPATIAL_REFERENCE, Vector::Polygon);

      // Inicializo el editor de geometrias para la creacion
      pGuiGeomCreator_ = new GuiGeometryCreator();
      pGuiGeomCreator_->Start(GuiGeometryCreator::Polygon, LATLON_SPATIAL_REFERENCE,
                              pViewer_->GetWorld());

      // Agrego seteo el objeto que va a recibir notificacion de los eventos
      // del visualizador
      std::vector<SuriObject::UuidType> ids = GetViewportManager()->GetViewportIds();
      for (size_t i = 0; i < ids.size(); ++i) {
         ViewerWidget* pviewer = AS_VW(GetViewportManager()->GetViewport(ids[i]));
         pviewer->PushMouseEvent(this);
         pviewer->PushPaintEvent(this);
      }

      active_ = true;
      editing_ = true;
   }
}

/**
 *
 */
void MeassureAreaElementEditor::Stop() {
   if (IsActive()) {
      // Cierro el vector
      pVectorEditor_->CloseFeature();
      pVectorEditor_->CloseLayer();
      pVectorEditor_->CloseVector();

      // Libero el editor para la creacion de geometrias
      pGuiGeomCreator_->End();
      delete pGuiGeomCreator_;
      pGuiGeomCreator_ = NULL;

      // Agrego seteo el objeto que va a recibir notificacion de los eventos del visualizador
      std::vector<SuriObject::UuidType> ids = GetViewportManager()->GetViewportIds();
      for (size_t i = 0; i < ids.size(); ++i) {
         ViewerWidget* pviewer = AS_VW(GetViewportManager()->GetViewport(ids[i]));
         pviewer->PopMouseEvent(this);
         pviewer->PopPaintEvent(this);
      }

      pViewer_->GetWindow()->Refresh();

      // Resetea el estado interno
      active_ = false;
      editing_ = false;
      area_ = 0;
      perimeter_ = 0;
      distance_ = 0;

      // Manda una notificacion.
      SendMeasureChangedNotifcation(0, 0);
   }
}

/**
 * Este metodo salva la medicion, esto se hace porque esta herramienta se
 * comporta distinto al resto de las herramientas de edicion de vector:
 * solo permite guardar una geometria.
 */
void MeassureAreaElementEditor::CloseFeature() {
   if (IsActive()) {
      Stop();
      Start();
   }
}

/**
 * Inicializa el editor.
 */
void MeassureAreaElementEditor::Initialize() {
}

/**
 * Calcula el area determinada por los distintos puntos que conforman el poligono.
 * \pre debe existir el vector. (pCurrentVectorElement_ != NULL)
 * \post se setea el valor de la propiedad meassurement_ con el valor de la
 * medicion realizada.
 */
void MeassureAreaElementEditor::CalculateMeassurement() {
   // ---------------------- CODIGO DE MEDICION ---------------------
   suri::CoordinatesTransformation *pct = NULL;
   std::string vectorproj(pVectorEditor_->GetLayerSpatialReference().c_str());
   if (!suri::SpatialReference::IsProjectedSpatialRef(vectorproj)) {
      ParameterCollection params;
      params.AddValue<std::string>(TransformationFactory::kParamWktIn, vectorproj);
      params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                   _AREA_MEASURE_PROJECTION_);
      TransformationFactory* pfactory = TransformationFactoryBuilder::Build();
      pct = pfactory->Create(TransformationFactory::kExact, params);
      TransformationFactoryBuilder::Release(pfactory);
      if (pct == NULL) {
         return;
      }
   }

   suri::Vector *pvector = pVectorEditor_->GetReadOnlyVector();
   if (pvector && pvector->GetLayer(pVectorEditor_->GetOpenLayer())) {
      // Toma el feature actual
      OGRFeature *pfeature =
            pvector->GetLayer(pVectorEditor_->GetOpenLayer())->GetFeature(
                  pVectorEditor_->GetOpenFeature());
      // Roba la geometria y la castea a poligono
      OGRPolygon *ppolygon =
            pfeature ? dynamic_cast<OGRPolygon*>(pfeature->GetGeometryRef()) : NULL;
      if (ppolygon) {
         // Reproyecta si es necesario
         // TODO Segmentar antes de hacer la reproyeccion
         // TODO(Gabriel - TCK #2324): Ver porque se necesita este metodo puntual
         ExactCoordinatesTransformation* pctcoord =
               dynamic_cast<ExactCoordinatesTransformation*>(pct);
         if (pctcoord != NULL && pctcoord->GetOGRCT() != NULL) {
            // Analizo si el area es valida
            OGRPolygon *ppolygonaux = dynamic_cast<OGRPolygon*>(ppolygon->clone());
            ppolygonaux->closeRings();
            OGRGeometry* pboundary = ppolygonaux->Boundary();
            if (pboundary) {
               OGRGeometry* intersection = ppolygonaux->Intersection(pboundary);
               if (intersection) {
                  ppolygonaux->transform(pctcoord->GetOGRCT());
                  area_ = ppolygonaux->get_Area();
                  perimeter_ = ppolygonaux->getExteriorRing()->get_Length();
                  delete pct;
                  OGRGeometryFactory::destroyGeometry(intersection);
               } else {
                  area_ = std::numeric_limits<double>::quiet_NaN();
                  perimeter_ = 0;
               }

               OGRGeometryFactory::destroyGeometry(pboundary);
            }
            OGRGeometryFactory::destroyGeometry(ppolygonaux);
         } else {
            area_ = ppolygon->get_Area();
            perimeter_ = ppolygon->getExteriorRing()->get_Length();
         }
      }
   }
}

/**
 * Envia una notificacion de que las medidas cambiaron junto con los valores deseados.
 */
void MeassureAreaElementEditor::SendMeasureChangedNotifcation(double Area,
                                                              double Perimeter) {
   std::auto_ptr<MeasureChangedNotification> notification(
         new MeasureChangedNotification());
   notification.get()->SetArea(Area);
   notification.get()->SetPerimeter(Perimeter);
   notification.get()->SetType(
         MeasureChangedNotification::kArea | MeasureChangedNotification::kPerimeter);
   /** Configura el wkt de la geometria que corresponde a la medicion **/
   suri::Vector *pvector = pVectorEditor_->GetReadOnlyVector();
   if (pvector && pvector->GetLayer(pVectorEditor_->GetOpenLayer())) {
      // Toma el feature actual
      OGRFeature *pfeature =
            pvector->GetLayer(pVectorEditor_->GetOpenLayer())->GetFeature(
                  pVectorEditor_->GetOpenFeature());
      // Roba la geometria y la castea a poligono
      OGRPolygon *ppolygon =
            pfeature ? dynamic_cast<OGRPolygon*>(pfeature->GetGeometryRef()) : NULL;
      if (ppolygon) {
         char* pwkt = NULL;
         ppolygon->exportToWkt(&pwkt);
         notification.get()->SetMeassureGeometry(pwkt);
      }
   }
   Notify(notification.get());
}

/** Actualiza el visualizador activo */
void MeassureAreaElementEditor::RefreshViewer() {
   ViewerWidget* pselectedviewer = AS_VW(GetViewportManager()->GetSelectedViewport());
   if (pViewer_ == pselectedviewer)
      return;

   // Reinicio los valores del widget sin la conexion/desconexion de eventos que poseen los
   // metodos Start() y Stop()
   pVectorEditor_->CloseFeature();
   pVectorEditor_->CloseLayer();
   pVectorEditor_->CloseVector();

   // Libero el editor para la creacion de geometrias
   pGuiGeomCreator_->End();
   delete pGuiGeomCreator_;
   pGuiGeomCreator_ = NULL;

   pViewer_->GetWindow()->Refresh();

   // Resetea el estado interno
   area_ = 0;
   perimeter_ = 0;
   distance_ = 0;

   // Manda una notificacion.
   SendMeasureChangedNotifcation(0, 0);

   // Inicializo el editor de vectores
   pVectorEditor_->OpenVector("shpmemory:tmp_measure_vector_poly.shp");
   pVectorEditor_->OpenLayer(POLYGON_FILE_NAME, LATLON_SPATIAL_REFERENCE, Vector::Polygon);

   // Inicializo el editor de geometrias para la creacion
   pGuiGeomCreator_ = new GuiGeometryCreator();
   pGuiGeomCreator_->Start(GuiGeometryCreator::Polygon, LATLON_SPATIAL_REFERENCE,
                           pViewer_->GetWorld());

   active_ = true;
   editing_ = true;

   pViewer_ = pselectedviewer;
}

} // namespace suri
