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

#ifndef GEOREFERENCETASK_H_
#define GEOREFERENCETASK_H_

#include "suri/TableEditionTask.h"

namespace suri {

class World;
class RasterSpatialModel;
class Geometry;
class Coordinates;
class FeatureSelectionSource;

/**
 * Clase que administra la tarea de georreferenciacion (toma de puntos)
 */
class GeoreferenceTask : public TableEditionTask {
public:
   static const std::string kGcpDestinationColumnName;
   static const std::string kTaskName;

   /** Ctor. */
   GeoreferenceTask();
   /** Dtor.*/
   ~GeoreferenceTask();
   /** Retorna el nombre de la columna en tabla de gcps asociado a la
    * imagen(origen o destino) */
   std::string GetGcpColumnName() const;
   /** Configura la coordenada del gcp asociado al feature en edicion */
   bool SetGcpCoordinate(const Coordinates& NewCoordinate);
   /** Retorna la coordenada del gcp asociad al feature en edicion  */
   bool GetGcpCoordinate(Coordinates& Coordinate);
   /** Funcion que indica si se puede iniciar la edicion */
   virtual bool CanStart();
   /** Configura el mundo que se usa para editar */
   bool SetWorld(World* pWorld);
   /** Retorna el mundo que se usa para editar */
   World* GetWorld() const;
   /** Retorna el SpatialModel asociada a la imagen */
   RasterSpatialModel* GetSpatialModel();
   /** Configura el SpatialModel asociado a la imagen */
   void SetSpatialModel(RasterSpatialModel* pRasterModel);
   /**
    * Le agrega el sistema de referencia de la imagen a la geometria
    * La geometria no debe tener un srs configurado.
    * @param[in] pGeometry geometria sin srs a configurar.
    * @param[out] pGeometry geometria con srs de imagen
    * @return bool que indica si tuvo exito.
    */
   bool AddSpatialReference(Geometry* pGeometry);

protected:
   /**
    * Configura las columnas de tipo geometria que estan en la tabla.
    */
   virtual bool ConfigureGeometryColumnEditors();

   /**
    * Crea el SelectionSource que hay que configurar al iniciar la tarea
    */
   virtual FeatureSelectionSource* CreateFeatureSelectionSource();
   /** modelo espacial del raster **/
   RasterSpatialModel* pRasterSpatialModel_;
   /** mundo del raster **/
   World* pWorldMap_;
};

}  // namespace suri

#endif  // GEOREFERENCETASK_H_
