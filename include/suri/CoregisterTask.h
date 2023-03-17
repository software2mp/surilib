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

#ifndef COREGISTERTASK_H_
#define COREGISTERTASK_H_

// Includes Estandar
#include <map>

// Includes Suri
#include "suri/TableEditionTask.h"
#include "suri/World.h"
#include "suri/Coordinates.h"
#include "suri/RasterSpatialModel.h"

// Includes Wx
// Defines
// forwards

namespace suri {

/*!
 * Tarea que administra la tarea de corregistro (toma de puntos)
 */
class CoregisterTask : public TableEditionTask {
   /** constructor copia **/
   CoregisterTask(const CoregisterTask&);

public:
   enum WorkingImageType {SOURCE, DESTINATION};
   static const std::string GCP_SOURCE_COLUMN_NAME;
   static const std::string GCP_DESTINATION_COLUMN_NAME;

   /** Constructor */
   CoregisterTask();
   /** Destructor **/
   virtual ~CoregisterTask();

   /** Configura el mundo que se usa para editar */
   bool SetWorld(World* pWorld, WorkingImageType WorkingImage);
   /** Retorna el mundo que se usa para editar */
   World* GetWorld(WorkingImageType WorkingImage) const;
   /** Retorna el tipo de imagen asociado al mundo (origen o destino) */
   bool GetImageType(const World* pWorld,
                        WorkingImageType &ImageType) const;

   /** Retorna el SpatialModel asociada a la imagen */
   RasterSpatialModel* GetSpatialModel(WorkingImageType WorkingImage);
   /** Configura el SpatialModel asociado a la imagen */
   void SetSpatialModel(WorkingImageType WorkingImage,
                              RasterSpatialModel* pRasterModel);
   /** Le agrega el sistema de referencia de la imagen a la geoemtria */
   bool AddSpatialReference(CoregisterTask::WorkingImageType pImageType,
                                                      Geometry* pGeometry);

   // Metodos que permiten modificar el feature en edicion
   /** Retorna el nombre de la columna en tabla de gcps asociado a la imagen(origen o destino) */
   std::string GetGcpColumnName(WorkingImageType &ImageType) const;
   /** Configura la coordenada del gcp asociado al feature en edicion */
   bool SetGcpCoordinate(WorkingImageType ImageType,
                                       const Coordinates &NewCoordinate);
   /** Retorna la coordenada del gcp asociad al feature en edicion */
   bool GetGcpCoordinate(WorkingImageType ImageType,
                                       Coordinates &Coordinate);

   /** Nombre asignado a la tarea. */
   static const std::string TASKNAME;

   /** Funcion que indica si se puede iniciar la edicion */
   virtual bool CanStart();

protected:
   /** Configura las columnas de tipo geometria que estan en la tabla. */
   virtual bool ConfigureGeometryColumnEditors();

   /** Crea el SelectionSource que hay que configurar al iniciar la tarea */
   virtual FeatureSelectionSource* CreateFeatureSelectionSource();

private:
   /** Mapa que contiene mundos asociados a origen y destino */
   std::map<WorkingImageType, World*> pWorldMap_;
   /** Mapa que contiene el modelo raster de las imagenes editadas */
   std::map<WorkingImageType, RasterSpatialModel*> pRasterModelMap_;
};

} /** namespace suri */

#endif /* COREGISTERTASK_H_ */
