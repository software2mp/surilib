/** \file GcpList.h
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#ifndef GCPLIST_H_
#define GCPLIST_H_

#include <string>
#include <vector>

#include "suri/GroundControlPoint.h"
#include "suri/ParameterCollection.h"

namespace suri {

/**
 * Clase encargada de administrar una lista de puntos de control (GCP). Ademas es la encargada
 * de la adecuacion de los puntos de controlpara el uso en las transformaciones de coordenadas.
 */
class GcpList {
public:
   /**
    * Constructor copia
    */
   GcpList(const GcpList& gcpList);
   /** Construe una lista de GCPs. **/
   GcpList(bool SourceInPixelLineP = true, bool DestinationInPixelLineP = true);
   /**
    * Construye una lista de GCPs.
    */
   GcpList(bool SourceInPixelLineP, bool DestinationInPixelLineP,
           const std::string& SpatialReferenceSourceP,
           const std::string& SpatialReferenceDestinationP,
           const std::string& RasterModelSourceP,
           const std::string& RasterModelDestinationP,
           const std::string& TransformationTypeP);

   /**
    * Destructor.
    */
   virtual ~GcpList();

   /**
    * Devuelve el sistema de referencia espacial de origen.
    */
   std::string GetSpatialReferenceSource() const;

   /**
    * Devuelve el sistema de referencia espacial de destino.
    */
   std::string GetSpatialReferenceDestination() const;

   /**
    * Devuelve el modelo raster de origen.
    */
   std::string GetRasterModelSource() const;

   /**
    * Devuelve el modelo raster de destino.
    */
   std::string GetRasterModelDestination() const;

   /**
    * Devuelve el tipo de transformacion
    */
   std::string GetTransformacionType() const;

   /**
    * Valida e indica el sistema de referencia espacial de origen.
    */
   bool SetSpatialReferenceSource(const std::string& SpatialReference);
   /**
    * Valida e indica el sistema de referencia espacial de destino.
    */
   bool SetSpatialReferenceDestination(const std::string& SpatialReference);

   /**
    * Valida e indica el modelo raster de origen.
    */
   bool SetRasterModelSource(const std::string& RasterModel);

   /**
    * Valida e indica el modelo raster de destino.
    */
   bool SetRasterModelDestination(const std::string& RasterModel);

   /**
    * Valida e indica el tipo de transformacion
    */
   void SetTransformacionType(const std::string& TransformationType);

   /**
    * Configura los datos para el destination datasource.
    */
   void SetDestinationDatasource(std::string UrlP, std::string IdP);

   /**
    * Obtiene la url para el destination datasource.
    */
   std::string GetDestinationDatasourceUrl() const;

   /**
    * Obtiene el id para el destination datasource.
    */
   std::string GetDestinationDatasourceId() const;

   /**
    * Configura los datos para el source datasource.
    */
   void SetSourceDatasource(std::string UrlP, std::string IdP);

   /**
    * Obtiene la url para el source datasource.
    */
   std::string GetSourceDatasourceUrl() const;

   /**
    * Obtiene el id para el source datasource.
    */
   std::string GetSourceDatasourceId() const;

   /**
    * Genera un vector de GCPs con las conversiones realizadas cuando corresponda.
    */
   std::vector<GroundControlPoint> CreateAdaptedGcps(bool Inverse = false) const;

   /**
    * Devuelve el vector de GCPs tal cual se levanto del origen de datos.
    */
   std::vector<GroundControlPointExtended> GetAsExtendedGcps() const;

   /**
    * Agrega un nuevo GCP a la lista.
    */
   void Add(const GroundControlPointExtended& pGcpP);

   /**
    * Elimina un GCP de la lista.
    */
   void Remove(int IndexP);

   /**
    * Devuelve un GCP de la lista.
    */
   GroundControlPointExtended& Get(int IndexP);

   /**
    * Devuelve la cantidad de GCPs en la lista.
    */
   unsigned int Size() const ;

   /**
    * Informa si la lista esta vacia.
    */
   bool Empty() const ;

   /**
    * Informa si las coordenadas de origen estan en formato Raster.
    */
   bool IsSourceInPixelLine() const;

   /**
    * Informa si las coordenadas de destino estan en formato Raster.
    */
   bool IsDestinationInPixelLine() const;

   /** Vacia la lista de puntos de control */
   void Clear();

private:
   bool srcInPixelLine_;
   bool destInPixelLine_;
   std::string spatialReferenceSrc_; /** sistema de referencia espacial de origen */
   std::string spatialReferenceDest_; /** sistema de referencia espacial de destino */
   std::string rasterModelSrc_; /** modelo raster de origen */
   std::string rasterModelDest_; /** modelo raster de destino */
   std::string transformationType_;
   std::string destinationDatasourceUrl_;
   std::string destinationDatasourceId_;
   std::string sourceDatasourceUrl_;
   std::string sourceDatasourceId_;
   // TODO Podrian ser instancias y no punteros
   std::vector<GroundControlPointExtended> gcps_; /** lista de GCPs */
};

}  // namespace suri
#endif  // GCPLIST_H_
