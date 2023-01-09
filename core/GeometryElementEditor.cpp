/*! \file GeometryElementEditor.cpp */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2013-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes Estandar
#include <string>
#include <vector>
// Includes Suri
#include "GeometryElementEditor.h"
#include "suri/VectorEditor.h"
#include "suri/VectorEditionTool.h"
#include "suri/VectorStyleManager.h"
#include "suri/ElementManager.h"
#include "suri/Vector.h"
#include "GeometryTranslator.h"


// Includes Wx
// Defines
// forwards

// #ifdef __OLD_CODE__
#define LAYER_POLYGON_RED \
   "VECTORSTYLE[3,\"Poligono rojo\",PEN[\"Suri-pen\",1,COLOR[255,0,0,255],WIDTH[1]]]"
/** Segundo estilo para dibujar poligono */
#define LAYER_POLYGON_MODIFIED \
   "VECTORSTYLE[3,\"Poligono amarillo\",PEN[\"Suri-pen\",1,COLOR[255,255,0,255],WIDTH[1]]]"
/** TErcer estilo para dibujar poligono */
#define LAYER_POLYGON_SELECTED \
   "VECTORSTYLE[3,\"Poligono azul\",PEN[\"Suri-pen\",1,COLOR[0,0,255,255],WIDTH[1]]]"

#define LATLON_SPATIAL_REFERENCE \
"GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.2572235630016,AUTHORITY[\"EPSG\",\"7030\"]],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433],AUTHORITY[\"EPSG\",\"4326\"]]"


#define POLYGON_FILE_NAME "Polygon_1"

namespace suri {

int GeometryElementEditor::GeoElementCounter_ = 0;

GeometryElementEditor::GeometryElementEditor(
      GuiGeometryCreator::GeometryType Type, ViewerWidget* pViewer,
      ViewportManagerInterface* pViewportManager,
      ViewcontextManagerInterface* pViewcontextManager) :
      ViewportEvent(pViewportManager, pViewcontextManager), pGuiGeomCreator_(NULL),
      type_(Type), pVectorEditor_(new VectorEditor()), pViewer_(pViewer),
      active_(false) {
}

GeometryElementEditor::~GeometryElementEditor() {
   delete pGuiGeomCreator_;
   delete pVectorEditor_;
}

/**
 * Se ejecuta cuando el usuario hace click sobre el visualizador.
 */
void GeometryElementEditor::SetPoint(double Pixel, double Line,
                                         wxMouseEvent& Event) {
   if (!IsEditing())
      return;

   if (Event.RightUp()) {
      pGuiGeomCreator_->HidePhantomPoint();
      Event.Skip(false);
      Stop();
      editing_ = false;
      pViewer_->GetWindow()->Refresh();
      return;
   }

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
   }
   pViewer_->GetWindow()->Refresh();
}


/**
 * Se ejecuta cuando el usuario se mueve en el visualizador.
 */
void GeometryElementEditor::UpdatePoint(double Pixel, double Line,
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
void GeometryElementEditor::OnStartDrag(const Coordinates& InitialCoordinate) {
}

/**
 * Se ejecuta cuando el usuario finaliza drag.
 */
void GeometryElementEditor::OnEndDrag(const Coordinates& InitialCoordinate,
                                          const Coordinates& CurrentCoordinate,
                                          wxMouseEvent& Event) {
}

/**
 * Se ejecuta cada vez que el usuario mueve el mouse mientras realiza drag.
 */
void GeometryElementEditor::OnDrag(const Coordinates& InitialCoordinate,
                                       const Coordinates& CurrentCoordinate,
                                       wxMouseEvent& Event) {
}

/**
 * Al pintar se dibuja geometria en edicion.
 */
void GeometryElementEditor::OnPaint(wxPaintEvent& Event, wxDC &Dc) {
   GeometryCollection* pmodifgeomcoll = new GeometryCollection(LATLON_SPATIAL_REFERENCE);
   std::vector<Geometry*>::iterator git = modifgeoms_.begin();

   /** Agrego a la coleccion las geometrias modificadas para pasarselas al render */
   for (; git != modifgeoms_.end(); ++git)
      pmodifgeomcoll->AppendGeometry(*git);

   VectorStyle *vStyle = VectorStyle::Create(LAYER_POLYGON_MODIFIED);
   pmodifgeomcoll->SetStyle(vStyle);
   GeometryRenderer renderer(&Dc, pViewer_->GetWorld());
   renderer.Render(modifgeoms_, vStyle, NULL);

   Geometry* pgeom = pGuiGeomCreator_ ? pGuiGeomCreator_->GetCreatedGeometry() : NULL;
   if ( !pgeom )
      return;
   vStyle = VectorStyle::Create(LAYER_POLYGON_RED);
   GeometryCollection* pgeomcoll = new GeometryCollection(LATLON_SPATIAL_REFERENCE);
   pgeomcoll->AppendGeometry(pgeom);
   renderer.Render(pgeomcoll, NULL);
}

/**
 * Indica que quiere recibir el evento OnPaint.
 */
PaintEventInterface::PaintEventFlags GeometryElementEditor::GetPaintFlags() const {
   return PaintEventInterface::Paint;
}

/**
 *
 */
bool GeometryElementEditor::CanProcessEvent(wxEvent &Event) const {
   wxWindow* pwindow = GetEventWindow(Event);
   ViewerWidget* pviewer =
         GetViewportManager() ? AS_VW(GetViewportManager()->GetSelectedViewport()): pViewer_;
   return pwindow && pviewer && (pviewer->GetWindow() == pwindow);
}

/**
 *
 */
void GeometryElementEditor::Start() {
   if (!IsActive()) {
      // Inicializo el editor de vectores
      // Inicializo el editor de geometrias para la creacion
      wxString vectorname;
      vectorname.sprintf("Elem_%d", GeoElementCounter_);
      elementUrl_ = wxT("shpmemory:") + vectorname;

      if (elementUrl_.empty()) {
         SHOW_ERROR(message_ADD_ELEMENT_ERROR_s);
         return;
      }

      if (pGuiGeomCreator_)
         delete pGuiGeomCreator_;
      pGuiGeomCreator_ = new GuiGeometryCreator();
      pGuiGeomCreator_->Start(type_, LATLON_SPATIAL_REFERENCE,
                              pViewer_->GetWorld());
      // Agrego seteo el objeto que va a recibir notificacion de los eventos
      // del visualizador
      pViewer_->PushPaintEvent(this);
      pViewer_->PushMouseEvent(this);

      active_ = true;
      editing_ = true;
   }
}

/** Finaliza y actualiza la edicion de la geometria en curso */
void GeometryElementEditor::EndFeatureEdition() {
   Geometry* pgeom = pGuiGeomCreator_->GetCreatedGeometry();
   if (pgeom != NULL) {
      pgeom->SetStyle(NULL);
      modifgeoms_.push_back(pgeom);

      // Obtengo el wkt
      std::string geowkt = GeometryTranslator::ExportToWkt(pgeom);
   }
}

/**
 *
 */
void GeometryElementEditor::Stop() {
   if (IsActive()) {
      // Cierro el vector
      EndFeatureEdition();
      // Libero el editor para la creacion de geometrias
      pGuiGeomCreator_->End();
      delete pGuiGeomCreator_;
      pGuiGeomCreator_ = NULL;

      // Agrego seteo el objeto que va a recibir notificacion de los eventos
      // del visualizador
      pViewer_->PopMouseEvent(this);

      pViewer_->GetWindow()->Refresh();
      // Resetea el estado interno
      active_ = false;
      editing_ = false;
   }
}

/**
 *
 *
 *
 * Cierra el vector en edicion.
 * Elimina de la lista a pCurrentVectorElement_
 */
bool GeometryElementEditor::CloseVector() {
   // no esta en la lista
   pVectorEditor_->CloseVector();
   return true;
}



/**
 * Este metodo salva la medicion, esto se hace porque esta herramienta se
 * comporta distinto al resto de las herramientas de edicion de vector:
 * solo permite guardar una geometria.
 */
void GeometryElementEditor::CloseFeature() {
   if (IsActive()) {
      Stop();
   }
}

/**
 * Analiza si se puede eliminar el shapefile y sus archivos asociados
 * Trata de abrir el datasource con el filename recibido, si no lo puede abrir
 * retorna false.
 * Recorre las capas, si en alguna no tiene permiso de escritura retorna false
 * @param[in] Filename nombre del archivo que se desea eliminar
 * @return informa si se puede eliminar archivo
 */


bool GeometryElementEditor::CanDeleteShapefile(const std::string &Filename) {
   // Verifico que sea un vector valido
   OGRSFDriver *pdriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
         "ESRI Shapefile");
   if (!pdriver || !pdriver->TestCapability(ODrCDeleteDataSource)) {
      return false;
   }
   OGRDataSource *pdatasource = OGRSFDriverRegistrar::Open(Filename.c_str());
   if (!pdatasource) {
      return false;
   }
   OGRDataSource::DestroyDataSource(pdatasource);
   // Verifico que el elemento no este abierto
   VectorEditor vectoreditor;
   vectoreditor.OpenVector(Filename);
   int layerindex = 0;
   while (vectoreditor.OpenLayer(layerindex)) {
      if (!vectoreditor.CanWrite()) {
         vectoreditor.CloseVector();
         return false;
      }
      layerindex++;
   }
   vectoreditor.CloseVector();
   return true;
}

bool GeometryElementEditor::DeleteShapefile(const std::string &Filename) {
   /** Elimino shapefile */
   if (!CanDeleteShapefile(Filename)) {
      return false;
   }

   /** Elimino archivo */
   OGRSFDriver *pdriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(
         "ESRI Shapefile");
   if (pdriver->DeleteDataSource(Filename.c_str()) != OGRERR_NONE) {
      return false;
   }

   return true;
}


bool GeometryElementEditor::ConfigureSavedElement(VectorElement* pVectorElement) {
   wxFileName wxfilename(pVectorElement->GetUrl().c_str());
   pVectorElement->SetName(wxfilename.GetName());
   return true;
}

bool GeometryElementEditor::SaveVector(const std::string &FileName) {
   return SaveFeatures(FileName);
}


/** Guardado "manual" de geometrias en los features del vector destino **/
bool GeometryElementEditor::SaveFeatures(const std::string& FileName) {
   VectorEditor* pdestv = new VectorEditor();
   Vector::VectorType vtype = Vector::Undefined;

   if (!pdestv->OpenVector(FileName))
      return false;
   switch (type_) {
      case GuiGeometryCreator::Line:
         vtype = Vector::Line;
         break;
      case GuiGeometryCreator::Polygon:
         vtype = Vector::Polygon;
         break;
      case GuiGeometryCreator::Point:
         vtype = Vector::Point;
         break;
      default:
         break;
   }
   pdestv->OpenLayer(FileName, LATLON_SPATIAL_REFERENCE, vtype);
   std::vector<Geometry*>::iterator it = modifgeoms_.begin();
   bool ok = true;
   for (;it != modifgeoms_.end() && ok; ++it) {
      long fid = pdestv->OpenFeature();
      std::string geowkt = GeometryTranslator::ExportToWkt((*it));
      ok |= pdestv->SetGeometryByWkt(fid, geowkt);
      if (ok)
         pdestv->CloseFeature();
   }
   pdestv->SyncToDisk();
   return ok;
}

/**
 * Indica si se termino de editar el elemento.
 * @return bool que indica si hay un vector editandose
 */
bool GeometryElementEditor::IsVectorOpen() {
   // retorna true si estoy editando elemento y este esta abierto
   return pVectorEditor_->IsVectorOpen();
}

/**
 * Inicializa el editor.
 */
void GeometryElementEditor::Initialize() {
}

/**
 * Retorna la cantidad de puntos
 * @return cantidad de puntos en vector en edicion. -1 si no hay ninguno
 */
int GeometryElementEditor::GetPointCount() {
   if (IsVectorOpen()) {
      return pVectorEditor_->GetPointCount();
   } else {
      return -1;
   }
}

/**
 * Elimina el feature de la posicion indicada. Si es el ultimo feature del
 * vector, cierra el vector.
 * @param[in] FeatureID identificador del feature que se quiere eliminar
 * @return true si pudo eliminar el feature, y si era el ultimo si pudo cerrar
 * el vector.
 */
bool GeometryElementEditor::DeleteFeature(long FeatureID) {
   bool returnvalue = false;
   CloseFeature();
   returnvalue = pVectorEditor_->DeleteFeature(FeatureID);
   if (pVectorEditor_->GetFeatureCount() <= 0) {
      returnvalue = returnvalue && CloseVector();
   }
   return returnvalue;
}


/**
 * Elimina el ultimo punto ingresado al vector.
 * @param[in] Position posicion(dentro del vector de puntos) donde esta
 * el punto a eliminar.
 * @return informa si se pudo eliminar el punto
 */
bool GeometryElementEditor::DeletePoint(int Position) {
   bool returnvalue = false;

   // Si el elemento se elimino externamente
/*   if (pCurrentVectorElement_
         && !pViewer_->GetList()->GetElement(pCurrentVectorElement_)) {
      CloseVector();
   }*/

   // Si la cantidad de puntos > 0 puedo eliminar uno.
   if ((pVectorEditor_->GetOpenFeature() >= 0) && (GetPointCount() > 0)) {
      if (pVectorEditor_->DeletePoint(Position)) {
         pVectorEditor_->SyncToDisk();
         returnvalue = true;
      }
   }
   // Si se elimino el ultimo punto, se cierra el feature.
   if (GetPointCount() <= 0) {
      DeleteFeature(pVectorEditor_->GetFeatureCount() - 1);
   }
   return returnvalue;
}

/**
 * Resetea el estado del editor.
 */
void GeometryElementEditor::Reset() {
}

void GeometryElementEditor::OnLeftUp(wxMouseEvent &Event) {
      double x = Event.GetX(), y = Event.GetY();
      if (isDragging_) {
         OnEndDrag(startDrag_, Coordinates(x, y), Event);
      }

      if (inside_) {
         SetPoint(x, y, Event);
      }

      isDragging_ = false;
      Reset();
}

void GeometryElementEditor::Finish() {
   pViewer_->PopPaintEvent(this);
   pViewer_->PopMouseEvent(this);
   pVectorEditor_->CloseVector();
   GeoElementCounter_++;  // Sumo uno para que no se use el mismo archivo temporal.

   if ( !modifgeoms_.empty())
      modifgeoms_.clear();
}
}  /** namespace suri **/
