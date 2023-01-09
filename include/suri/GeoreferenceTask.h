/** \file GeoreferenceTask.h
 *
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
