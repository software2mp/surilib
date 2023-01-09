/*! \file CoregisterTask.h */
/**
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2012-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

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
