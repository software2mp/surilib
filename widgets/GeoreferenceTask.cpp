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
