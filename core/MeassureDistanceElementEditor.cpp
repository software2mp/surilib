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

#include <memory>
#include <string>
#include <vector>

#include "MeassureDistanceElementEditor.h"
#include "SpatialReference.h"
#include "suri/MemoryVectorElement.h"
#include "suri/CoordinatesTransformInterface.h"
#include "suri/VectorEditor.h"
#include "suri/Configuration.h"
#include "resources.h"
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
#include "suri/DynamicViewerWidget.h"

// Defines
/** Proyeccion en lat/long para medir distancias */
// \todo Implementar posibilidad de elegir el Datum deseado
#define _DISTANCE_MEASURE_PROJECTION_ \
   "GEOGCS[\"GCS_WGS_1984\",DATUM[\"D_WGS_1984\",SPHEROID[\"WGS_1984\",6378137,298.257223563]],PRIMEM[\"Greenwich\",0],UNIT[\"Degree\",0.017453292519943295]]"
/** Primer estilo para dibujar linea */
#define MEASSURE_LINE_1 \
   "VECTORSTYLE[2,\"Linea rojo\",PEN[\"Suri-pen\",1,COLOR[255,0,0,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",1,COLOR[255,128,0,255],SIZE[2]]]"
/** Segundo estilo para dibujar linea */
#define MEASSURE_LINE_2 \
   "VECTORSTYLE[2,\"Linea verde\",PEN[\"Suri-pen\",1,COLOR[0,128,0,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",2,COLOR[128,255,0,255],SIZE[2]]]"
/** Tercer estilo para dibujar linea */
#define MEASSURE_LINE_3 \
   "VECTORSTYLE[2,\"Linea azul\",PEN[\"Suri-pen\",1,COLOR[0,0,255,255],WIDTH[1]],SYMBOL[\"Suri-symbol\",3,COLOR[128,255,255,255],SIZE[2]]]"

#define LATLON_SPATIAL_REFERENCE \
   "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.2572235630016,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433],AUTHORITY[\"EPSG\",\"4326\"]]"

#define LINE_FILE_NAME "Line_1"


namespace suri {

/** Calcula la distancia entre dos puntos en coordenadas geograficas */
/**
 * @param[in] Point1 Punto 1 con coordenadas geograficas
 * @param[in] Point2 Punto 2 con coordenadas geograficas
 * @return La distancia calculada en metros, negativo en caso de error
 * TODO Soportar otros elipsoides ademas de WGS-84
 */
double VincentyDistance(const Coordinates &Point1, const Coordinates &Point2) {
   double a = 6378137, b = 6356752.3142, f = 1 / 298.257223563;  // WGS-84
   double l = (Point1.x_ - Point2.x_) * M_PI / 180.0;
   double u1 = atan((1 - f) * tan(Point1.y_ * M_PI / 180.0));
   double u2 = atan((1 - f) * tan(Point2.y_ * M_PI / 180.0));
   double sinu1 = sin(u1);
   double cosu1 = cos(u1);
   double sinu2 = sin(u2);
   double cosu2 = cos(u2);
   double cossqalpha, sinsigma, cos2sigmaM, cossigma, sigma;
   double lambda = l;
   double lambdap;
   int iterlimit = 100;
   do {
      double sinlambda = sin(lambda);
      double coslambda = cos(lambda);
      sinsigma = sqrt(
            (cosu2 * sinlambda) * (cosu2 * sinlambda)
                  + (cosu1 * sinu2 - sinu1 * cosu2 * coslambda)
                        * (cosu1 * sinu2 - sinu1 * cosu2 * coslambda));
      if (sinsigma == 0) {
         return 0;  // puntos coincidentes
      }
      cossigma = sinu1 * sinu2 + cosu1 * cosu2 * coslambda;
      sigma = atan2(sinsigma, cossigma);
      double sinalpha = cosu1 * cosu2 * sinlambda / sinsigma;
      cossqalpha = 1 - sinalpha * sinalpha;
      cos2sigmaM = cossigma - 2 * sinu1 * sinu2 / cossqalpha;
      if (isnan(cos2sigmaM)) {
         cos2sigmaM = 0;  // linea ecuatorial: cossqalpha=0
      }
      double c = f / 16 * cossqalpha * (4 + f * (4 - 3 * cossqalpha));
      lambdap = lambda;
      lambda =
            l
                  + (1 - c) * f * sinalpha
                        * (sigma
                              + c * sinsigma
                                    * (cos2sigmaM
                                          + c * cossigma
                                                * (-1 + 2 * cos2sigmaM * cos2sigmaM)));
   } while (fabs(lambda - lambdap) > 1e-12 && --iterlimit > 0);

   if (iterlimit == 0) {
      return -1.0;  // fallo convergencia de formula
   }
   double usq = cossqalpha * (a * a - b * b) / (b * b);
   double A = 1 + usq / 16384 * (4096 + usq * (-768 + usq * (320 - 175 * usq)));
   double B = usq / 1024 * (256 + usq * (-128 + usq * (74 - 47 * usq)));
   double deltasigma = B * sinsigma
         * (cos2sigmaM
               + B / 4
                     * (cossigma * (-1 + 2 * cos2sigmaM * cos2sigmaM)
                           - B / 6 * cos2sigmaM * (-3 + 4 * sinsigma * sinsigma)
                                 * (-3 + 4 * cos2sigmaM * cos2sigmaM)));
   double dist = b * A * (sigma - deltasigma);

   return dist;
}

/**
 * Ctor.
 */
MeassureDistanceElementEditor::MeassureDistanceElementEditor(
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
void MeassureDistanceElementEditor::Reset() {

}

/**
 * Se ejecuta cuando el usuario hace click sobre el visualizador.
 */
void MeassureDistanceElementEditor::SetPoint(double Pixel, double Line, wxMouseEvent& Event) {
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
      if (distance_ > 0) {
         SendMeasureChangedNotifcation(distance_);
      }
   }
   pViewer_->GetWindow()->Refresh();
}

/**
 * Se ejecuta cuando el usuario se mueve en el visualizador.
 */
void MeassureDistanceElementEditor::UpdatePoint(double Pixel, double Line,
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
void MeassureDistanceElementEditor::OnStartDrag(const Coordinates& InitialCoordinate) {

}

/**
 * Se ejecuta cuando el usuario finaliza drag.
 */
void MeassureDistanceElementEditor::OnEndDrag(const Coordinates& InitialCoordinate,
                                              const Coordinates& CurrentCoordinate,
                                              wxMouseEvent& Event) {

}

/**
 * Se ejecuta cada vez que el usuario mueve el mouse mientras realiza drag.
 */
void MeassureDistanceElementEditor::OnDrag(const Coordinates& InitialCoordinate,
                                           const Coordinates& CurrentCoordinate,
                                           wxMouseEvent& Event) {

}

/**
 * Al pintar se dibuja geometria en edicion.
 */
void MeassureDistanceElementEditor::OnPaint(wxPaintEvent& Event, wxDC &Dc) {
   if (pViewer_ != AS_VW(GetViewportManager()->GetSelectedViewport()))
      return;
   Geometry* pgeom = pGuiGeomCreator_->GetCreatedGeometry();

   GeometryCollection* pgeomcoll = new GeometryCollection(LATLON_SPATIAL_REFERENCE);

   std::string vectorStyle = Configuration::GetParameter("geo_distance_style",
                                                         DEFAULT_GEO_DISTANCE_STYLE);

   VectorStyle* pgeomstyle = NULL;

   if ( !vectorStyle.empty() )
      pgeomstyle = VectorStyle::Create(vectorStyle.c_str());
   else
      pgeomstyle = VectorStyle::Create(MEASSURE_LINE_1);



   pgeomcoll->SetStyle(pgeomstyle);
   pgeomcoll->AppendGeometry(pgeom);

   GeometryRenderer renderer(&Dc, pViewer_->GetWorld());
   renderer.Render(pgeomcoll, NULL);
}

/**
 * Indica que quiere recibir el evento OnPaint.
 */
PaintEventInterface::PaintEventFlags MeassureDistanceElementEditor::GetPaintFlags() const {
   return PaintEventInterface::Paint;
}

/**
 *
 */
bool MeassureDistanceElementEditor::CanProcessEvent(wxEvent &Event) const {
   wxWindow* pwindow = GetEventWindow(Event);
   ViewerWidget* pviewer =
         GetViewportManager() ? AS_VW(GetViewportManager()->GetSelectedViewport()): pViewer_;
   DynamicViewerWidget* pdynamic = dynamic_cast<DynamicViewerWidget*>(pviewer);
   if (pdynamic)
      pviewer = pdynamic->GetViewer();
   return pwindow && pviewer && (pviewer->GetWindow() == pwindow);
}

/**
 * Recibe notificaciones del control contenedor.
 */
void MeassureDistanceElementEditor::Update(NotificationInterface* pNotification) {
   MeasureDeletionRequestedNotification* pmnot =
         dynamic_cast<MeasureDeletionRequestedNotification*>(pNotification);
   if (pmnot != NULL) {
      CloseFeature();
   }
}

/**
 *
 */
void MeassureDistanceElementEditor::Start() {
   if (!IsActive()) {
      // Inicializo el editor de vectores
      pVectorEditor_->OpenVector("shpmemory:tmp_measure_vector_line.shp");
      pVectorEditor_->OpenLayer(LINE_FILE_NAME, LATLON_SPATIAL_REFERENCE, Vector::Line);

      // Inicializo el editor de geometrias para la creacion
      pGuiGeomCreator_ = new GuiGeometryCreator();
      pGuiGeomCreator_->Start(GuiGeometryCreator::Line, LATLON_SPATIAL_REFERENCE,
                              pViewer_->GetWorld());

      // Agrego seteo el objeto que va a recibir notificacion de los eventos del visualizador
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
void MeassureDistanceElementEditor::Stop() {
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
      SendMeasureChangedNotifcation(0);
   }
}

/**
 * Este metodo salva la medicion, esto se hace porque esta herramienta se
 * comporta distinto al resto de las herramientas de edicion de vector:
 * solo permite guardar una geometria.
 */
void MeassureDistanceElementEditor::CloseFeature() {
   if (IsActive()) {
      Stop();
      Start();
   }
}

/**
 * Inicializa el editor.
 */
void MeassureDistanceElementEditor::Initialize() {
}

/**
 * Calcula la distancia entre los distintos puntos que conforman la linea.
 * \pre debe existir el vector. (pCurrentVectorElement_ != NULL)
 * \post se setea el valor de la propiedad distance_ con el valor de la
 * medicion realizada.
 */
void MeassureDistanceElementEditor::CalculateMeassurement() {
   Vector *pvector = pVectorEditor_->GetReadOnlyVector();
   if (pvector && pvector->GetLayer(pVectorEditor_->GetOpenLayer())) {
      // Toma el feature actual
      OGRFeature *pfeature =
            pvector->GetLayer(pVectorEditor_->GetOpenLayer())->GetFeature(
                  pVectorEditor_->GetOpenFeature());
      // Roba la geometria y la castea a poligono
      OGRLineString *pline =
            pfeature ? dynamic_cast<OGRLineString *>(pfeature->GetGeometryRef()) : NULL;
      if (pline != NULL && pline->getNumPoints() > 1) {
         std::string vectorproj(pViewer_->GetWorld()->GetSpatialReference().c_str());
         // La distancia la calcula con Vincenty para sistemas de representacion proyectados
         // Para sistemas de representacion georeferenciados utiliza get_Length
         if (SpatialReference::IsProjectedSpatialRef(vectorproj)) {
            distance_ = pline->get_Length();
         } else {
            ParameterCollection params;
            params.AddValue<std::string>(TransformationFactory::kParamWktIn,
                                         vectorproj);
            params.AddValue<std::string>(TransformationFactory::kParamWktOut,
                                         _DISTANCE_MEASURE_PROJECTION_);
            TransformationFactory* pfactory =
                  TransformationFactoryBuilder::Build();
            CoordinatesTransformation *pct = pfactory->Create(TransformationFactory::kExact,
                                                     params);
            TransformationFactoryBuilder::Release(pfactory);

            if (pct == NULL) {
               OGRGeometryFactory::destroyGeometry(pline);
               return;
            }
            distance_ = 0;
            Coordinates vertex1(pline->getX(0), pline->getY(0));
            pct->Transform(vertex1);
            for (int i = 1; i < pline->getNumPoints(); i++) {
               Coordinates vertex2(pline->getX(i), pline->getY(i));
               pct->Transform(vertex2);
               distance_ += VincentyDistance(vertex1, vertex2);
               vertex1 = vertex2;
            }
            delete pct;
         }
      }
   }
}

/** Envia una notificacion de que las medidas cambiaron junto con los valores deseados. */
void MeassureDistanceElementEditor::SendMeasureChangedNotifcation(double Distance) {
   std::auto_ptr<MeasureChangedNotification> notification(
         new MeasureChangedNotification());
   notification.get()->SetDistance(Distance);
   notification.get()->SetType(MeasureChangedNotification::kDistance);
   Vector *pvector = pVectorEditor_->GetReadOnlyVector();
   if (pvector && pvector->GetLayer(pVectorEditor_->GetOpenLayer())) {
      // Toma el feature actual
      OGRFeature *pfeature =
            pvector->GetLayer(pVectorEditor_->GetOpenLayer())->GetFeature(
                  pVectorEditor_->GetOpenFeature());
      OGRGeometry* pgeom = pfeature->GetGeometryRef();
      char* pwkt = NULL;
      pgeom->exportToWkt(&pwkt);
      notification.get()->SetMeassureGeometry(pwkt);
   }
   Notify(notification.get());
}

/** Actualiza el visualizador activo */
void MeassureDistanceElementEditor::RefreshViewer() {
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
   SendMeasureChangedNotifcation(0);

   // Inicializo el editor de vectores
   pVectorEditor_->OpenVector("shpmemory:tmp_measure_vector_line.shp");
   pVectorEditor_->OpenLayer(LINE_FILE_NAME, LATLON_SPATIAL_REFERENCE, Vector::Line);

   // Inicializo el editor de geometrias para la creacion
   pGuiGeomCreator_ = new GuiGeometryCreator();
   pGuiGeomCreator_->Start(GuiGeometryCreator::Line, LATLON_SPATIAL_REFERENCE,
                           pViewer_->GetWorld());

   active_ = true;
   editing_ = true;

   pViewer_ = pselectedviewer;
}

}  // namespace suri
