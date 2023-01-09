/** \file GeoreferenceTask.cpp
 *
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <map>
#include <string>
#include <vector>
#include <utility>

#include "suri/GeoreferenceTask.h"
#include "suri/World.h"
#include "suri/RasterSpatialModel.h"
#include "Geometry.h"
#include "SimpleGeometryEditor.h"
#include "suri/Coordinates.h"
#include "suri/SuriObject.h"
#include "suri/TableEditionTask.h"
#include "suri/FeatureSelectionSource.h"
#include "GeoreferenceSelectionSource.h"
#include "GeometryColumnEditionState.h"
#include "Point.h"

namespace suri {

const std::string GeoreferenceTask::kGcpDestinationColumnName = "the_geom";
const std::string GeoreferenceTask::kTaskName = "GeoreferenceTask";

/**
 * Ctor.
 */
GeoreferenceTask::GeoreferenceTask() : pRasterSpatialModel_(NULL) {
}

/**
 * Dtor.
 */
GeoreferenceTask::~GeoreferenceTask() {

}

/**
 * Retorna el nombre de la columna en tabla de gcps asociado a la imagen(origen o destino)
 */
std::string GeoreferenceTask::GetGcpColumnName() const {
   return kGcpDestinationColumnName;
}

/**
 * Configura la coordenada del gcp asociado al feature en edicion
 */
bool GeoreferenceTask::SetGcpCoordinate(const Coordinates& NewCoordinate) {
   if (!IsEditingFeature())
      return false;

   Geometry* pgeometry = GetEditedGeometry(kGcpDestinationColumnName);
   if (pgeometry) {
      SimpleGeometryEditor* peditor = SimpleGeometryEditor::InitializeEditor(pgeometry);
      std::vector<SuriObject::UuidType> points = peditor->GetPoints();
      peditor->MovePoint(*points.begin(), NewCoordinate);
      delete peditor;
   } else {
      SetEditedGeometry(kGcpDestinationColumnName, new Point(NewCoordinate));
   }

   return true;
}

/**
 * Retorna la coordenada del gcp asociad al feature en edicion
 */
bool GeoreferenceTask::GetGcpCoordinate(Coordinates& Coordinate) {
   if (!IsEditingFeature())
      return false;

   Geometry* pgeometry = GetEditedGeometry(kGcpDestinationColumnName);
   if (!pgeometry)
      return false;

   SimpleGeometryEditor* peditor = SimpleGeometryEditor::InitializeEditor(pgeometry);
   std::vector<SuriObject::UuidType> points = peditor->GetPoints();
   bool result = peditor->GetPointCoordinate(*points.begin(), Coordinate);
   delete peditor;
   return result;
}

/**
 * Funcion que indica si se puede iniciar la edicion
 */
bool GeoreferenceTask::CanStart() {
   return TableEditionTask::CanStart();
}

/**
 * Configura las columnas de tipo geometria que estan en la tabla.
 */
bool GeoreferenceTask::ConfigureGeometryColumnEditors() {
   pGeometryColumnEditorMap_.insert(
         std::pair<std::string, GeometryColumnEditionState*>(
               kGcpDestinationColumnName,
               new GeometryColumnEditionState(pTable_, "", kGcpDestinationColumnName)));
   return true;
}

/**
 * Crea el SelectionSource que hay que configurar al iniciar la tarea
 */
FeatureSelectionSource* GeoreferenceTask::CreateFeatureSelectionSource() {
   return new GeoreferenceSelectionSource(this);
}

/**
 * Retorna el SpatialModel asociada a la imagen
 * @return RasterSpatialModel asociado a la imagen
 */
RasterSpatialModel* GeoreferenceTask::GetSpatialModel() {
   return pRasterSpatialModel_;
}

/**
 * Configura el SpatialModel asociado a la imagen
 * @param[in] pRasterModel modelo raster de la imagen donde se cargan gcps,
 *  el puntero pasa a ser responsabilidad de esta clase.
 */
void GeoreferenceTask::SetSpatialModel(RasterSpatialModel* pRasterModel) {
   if (pRasterSpatialModel_)
      RasterSpatialModel::Destroy(pRasterSpatialModel_);
   pRasterSpatialModel_ = pRasterModel;
}

/**
 * Le agrega el sistema de referencia de la imagen a la geometria
 * La geometria no debe tener un srs configurado.
 * @param[in] pGeometry geometria sin srs a configurar.
 * @param[out] pGeometry geometria con srs de imagen
 * @return bool que indica si tuvo exito.
 */
bool GeoreferenceTask::AddSpatialReference(Geometry* pGeometry) {
   SimpleGeometryEditor* geomeditor =
                  SimpleGeometryEditor::InitializeEditor(pGeometry);
   bool result = geomeditor ? geomeditor->AddSpatialReference(pRasterSpatialModel_,
                                       pWorldMap_->GetSpatialReference()) : false;
   delete geomeditor;
   return result;
}

/**
 * Configura el mundo que se usa para editar
 * @param[in] pWorld mundo donde se van a editar los puntos
 * @return bool que indica si se pudo configurar la tabla.
 */
bool GeoreferenceTask::SetWorld(World* pWorld) {
   if (IsActive() || !pWorld)
      return false;
   pWorldMap_ = pWorld;
   return true;
}

/**
 * Retorna el mundo que se usa para editar
 * @return mundo asociado a imagen.
 */
World* GeoreferenceTask::GetWorld() const {
   return pWorldMap_;
}
}  // namespace suri
