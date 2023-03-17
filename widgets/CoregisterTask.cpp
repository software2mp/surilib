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

// Includes Estandar

// Includes Suri
#include "suri/CoregisterTask.h"
#include "suri/GcpDriver.h"
#include "VectorDataLayer.h"
#include "GuiGeometryEditor.h"
#include "GuiGeometryCreator.h"
#include "SimpleGeometryEditor.h"
#include "Point.h"
#include "CoregisterSelectionSource.h"
#include "GeometryColumnEditionState.h"

// Includes Wx

// Defines

// forwards

namespace suri {

/** Nombre asignado a la tarea */
const std::string CoregisterTask::TASKNAME = "CoregisterTask";
const std::string CoregisterTask::GCP_SOURCE_COLUMN_NAME = "the_geom1";
const std::string CoregisterTask::GCP_DESTINATION_COLUMN_NAME = "the_geom2";

/**
 * Constructor
 * @param pTable tabla en donde se almacenaran los puntos de control
 * @param pFeatureSelection clase encargada del manejo de la seleccion
 */
CoregisterTask::CoregisterTask() {
   pWorldMap_[SOURCE] = NULL;
   pWorldMap_[DESTINATION] = NULL;
   pRasterModelMap_[SOURCE] = NULL;
   pRasterModelMap_[DESTINATION] = NULL;
}

/**
 * Destructor
 */
CoregisterTask::~CoregisterTask() {
}

/**
 * Configura el mundo que se usa para editar
 * @param[in] pWorld mundo donde se van a editar los puntos
 * @param[in] WorkingImage imagen que corresponde al mundo
 * @return bool que indica si se pudo configurar la tabla.
 */
bool CoregisterTask::SetWorld(World* pWorld, WorkingImageType WorkingImage) {
   if (IsActive() || !pWorld)
      return false;
   pWorldMap_[WorkingImage] = pWorld;
   return true;
}

/** 
 * Retorna el mundo que se usa para editar 
 * @param[in] WorkingImage imagen que corresponde al mundo
 * @return mundo asociado a imagen.
 */
World* CoregisterTask::GetWorld(WorkingImageType WorkingImage) const {
   std::map<WorkingImageType, World*>::const_iterator it;
   it = pWorldMap_.find(WorkingImage);
   return it != pWorldMap_.end() ? it->second : NULL;
}

/**
 * Retorna el tipo de imagen asociado al mundo (origen o destino)
 * @param[in] pWorld world a analizar
 * @param[out] pImageType imagen asociada a viewer(origen o destino)
 * @return bool que indica si el world tenia una imagen asociada. Si es
 * false pWorld no se modifico.
 */
bool CoregisterTask::GetImageType(const World* pWorld,
                                       WorkingImageType &ImageType) const {
   // Identifico si el visualizador corresponde a origen o al destino
   CoregisterTask::WorkingImageType workingimage = SOURCE;
   if (pWorld == GetWorld(SOURCE))
      workingimage = SOURCE;
   else if (pWorld == GetWorld(DESTINATION))
      workingimage = DESTINATION;
   else
      return false;
   ImageType = workingimage;
   return true;
}

/**
 * Retorna el SpatialModel asociada a la imagen
 * @param[in] WorkingImage imagen de la que quiero el modelo raster.
 * @return RasterSpatialModel asociado a la imagen
 */
RasterSpatialModel* CoregisterTask::GetSpatialModel(
                                       WorkingImageType WorkingImage) {
   return pRasterModelMap_[WorkingImage];
}

/**
 * Configura el SpatialModel asociado a la imagen
 * @param[in] WorkingImage imagen que quiero configurar
 * @param[in] pRasterModel modelo raster de la imagen donde se cargan gcps,
 *  el puntero pasa a ser responsabilidad de esta clase.
 */
void CoregisterTask::SetSpatialModel(WorkingImageType WorkingImage,
                                    RasterSpatialModel* pRasterModel) {
   RasterSpatialModel::Destroy(pRasterModelMap_[WorkingImage]);
   pRasterModelMap_[WorkingImage] = pRasterModel;
}

/** 
 * Le agrega el sistema de referencia de la imagen a la geoemtria 
 * La geometria no debe tener un srs configurado.
 * @param[in] pImageType imagen cuyo srs quiero configurar
 * @param[in] pGeometry geometria sin srs a configurar.
 * @param[out] pGeometry geometria con srs de imagen
 * @return bool que indica si tuvo exito.
 */
bool CoregisterTask::AddSpatialReference(WorkingImageType pImageType,
                                                   Geometry* pGeometry) {
   SimpleGeometryEditor* geomeditor =
                  SimpleGeometryEditor::InitializeEditor(pGeometry);
   bool result = geomeditor ? geomeditor->AddSpatialReference(pRasterModelMap_[pImageType],
                                       pWorldMap_[pImageType]->GetSpatialReference()) : false;
   delete geomeditor;
   return result;
}

/** Retorna el nombre de la columna en tabla de gcps asociado a la imagen(origen o destino) */
std::string CoregisterTask::GetGcpColumnName(WorkingImageType &ImageType) const {
   switch (ImageType) {
   case SOURCE:
      return GCP_SOURCE_COLUMN_NAME;
   case DESTINATION:
      return GCP_DESTINATION_COLUMN_NAME;
   }
   return "";
}

/**
 * Inicia la creacion de la geometria asociada al feature
 * @param[in] NewCoordinate coordenada en pixel y linea de imagen.
 * @param[in] WorkingImage imagen donde esta el punto a editar
 */
bool CoregisterTask::SetGcpCoordinate(WorkingImageType ImageType,
                                          const Coordinates &NewCoordinate) {
   if (!IsEditingFeature())
      return false;

   std::string geomcolumnname = GetGcpColumnName(ImageType);
   Geometry* pgeometry = GetEditedGeometry(geomcolumnname);
   if (pgeometry) {
      SimpleGeometryEditor* peditor =
            SimpleGeometryEditor::InitializeEditor(pgeometry);
      std::vector<SuriObject::UuidType> points = peditor->GetPoints();
      peditor->MovePoint(*points.begin(), NewCoordinate);
      delete peditor;
   } else {
      SetEditedGeometry(geomcolumnname, new Point(NewCoordinate));
   }

   return true;
}

/** 
 * Retorna la coordenada del gcp asociada al feature en edicion 
 * @param[in] WorkingImage imagen de donde quiero el punto
 * @param[out] Coordinate coordenada del gcp
 * @return true si habia un feature en edicion y tenia una 
 * coordenada asignada
 * @return false en otro caso.
 */
bool CoregisterTask::GetGcpCoordinate(WorkingImageType ImageType,
                                                     Coordinates &Coordinate) {
   if (!IsEditingFeature())
      return false;

   std::string geomcolumnname = GetGcpColumnName(ImageType);
   Geometry* pgeometry = GetEditedGeometry(geomcolumnname);
   if (!pgeometry)
      return false;

   SimpleGeometryEditor* peditor =
               SimpleGeometryEditor::InitializeEditor(pgeometry);
   std::vector<SuriObject::UuidType> points = peditor->GetPoints();
   bool result = peditor->GetPointCoordinate(*points.begin(), Coordinate);
   delete peditor;
   return result;
}

/**
 * Configura las columnas de tipo geometria que estan en la tabla.
 * @return bool que indica si tuvo exito
 */
bool CoregisterTask::ConfigureGeometryColumnEditors() {
   pGeometryColumnEditorMap_.insert(
         std::pair<std::string, GeometryColumnEditionState*>(GCP_SOURCE_COLUMN_NAME,
                new GeometryColumnEditionState(pTable_, "", GCP_SOURCE_COLUMN_NAME)));
   pGeometryColumnEditorMap_.insert(
         std::pair<std::string, GeometryColumnEditionState*>(GCP_DESTINATION_COLUMN_NAME,
                new GeometryColumnEditionState(pTable_, "", GCP_DESTINATION_COLUMN_NAME)));
   return true;
}

/** Funcion que indica si se puede iniciar la edicion */
bool CoregisterTask::CanStart() {
   return TableEditionTask::CanStart() && pWorldMap_[SOURCE] != NULL
                                       && pWorldMap_[DESTINATION] != NULL
                                       && pRasterModelMap_[SOURCE] != NULL
                                       && pRasterModelMap_[DESTINATION] != NULL;
}

/** Crea el SelectionSource que hay que configurar al iniciar la tarea */
FeatureSelectionSource* CoregisterTask::CreateFeatureSelectionSource() {
   return new CoregisterSelectionSource(this);
}

} /** namespace suri */
