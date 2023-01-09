/** \file GcpList.cpp
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

#include <string>
#include <vector>

#include "suri/GcpList.h"
#include "suri/RasterSpatialModel.h"
#include "suri/Coordinates.h"
#include "SpatialReference.h"

namespace suri {

GcpList::GcpList (const GcpList& gcpList) {
   this->srcInPixelLine_ = gcpList.srcInPixelLine_;
   this->destInPixelLine_ = gcpList.destInPixelLine_;
   this->spatialReferenceSrc_ = gcpList.spatialReferenceSrc_;
   this->spatialReferenceDest_ = gcpList.spatialReferenceDest_;
   this->rasterModelSrc_ = gcpList.rasterModelSrc_;
   this->rasterModelDest_ = gcpList.rasterModelDest_;
   this->transformationType_ = gcpList.transformationType_;
   
   this->destinationDatasourceUrl_ = gcpList.sourceDatasourceUrl_;
   this->destinationDatasourceId_ = gcpList.destinationDatasourceId_;
   this->sourceDatasourceUrl_ = gcpList.sourceDatasourceUrl_;
   this->sourceDatasourceId_ = gcpList.sourceDatasourceId_;
   std::vector<GroundControlPointExtended>::const_iterator it;
   for (it = gcpList.gcps_.begin(); it != gcpList.gcps_.end(); ++it){
      GroundControlPointExtended gcpe = *it;;
      this->gcps_.push_back(gcpe);
   }
}


/** * Construye una lista de GCPs.
 *
 * @param[in] SourceInPixelLineP Indica si las coordenadas de origen estan en formato Pixel/Linea.
 * @param[in] DestinationInPixelLineP Indica si las coordenadas de destino estan en formato Pixel/Linea
 */
GcpList::GcpList(bool SourceInPixelLineP, bool DestinationInPixelLineP) :
		srcInPixelLine_(SourceInPixelLineP), destInPixelLine_(
				DestinationInPixelLineP) {
}

/**
 * Construye una lista de GCPs.
 *
 * @param[in] SourceInPixelLineP Indica si las coordenadas de origen estan en formato Pixel/Linea.
 * @param[in] DestinationInPixelLineP Indica si las coordenadas de destino estan en formato Pixel/Linea.
 * @param[in] SpatialReferenceSourceP Sistema de referencia de origen.
 * @param[in] SpatialReferenceDestinationP Sistema de referencia de destino.
 * @param[in] RasterModelSourceP Modelo raster de origen.
 * @param[in] RasterModelDestinationP Modelo raster de destino.
 * @param[in] TransformationTypeP Tipo de transformacion.
 * @param[in] pParameterCollectionP Parametros para la transformacion.
 */
GcpList::GcpList(bool SourceInPixelLineP, bool DestinationInPixelLineP,
		const std::string& SpatialReferenceSourceP,
		const std::string& SpatialReferenceDestinationP,
		const std::string& RasterModelSourceP,
		const std::string& RasterModelDestinationP,
		const std::string& TransformationTypeP) :
		srcInPixelLine_(SourceInPixelLineP), destInPixelLine_(
				DestinationInPixelLineP), spatialReferenceSrc_(
				SpatialReferenceSourceP), spatialReferenceDest_(
				SpatialReferenceDestinationP), rasterModelSrc_(
				RasterModelSourceP), rasterModelDest_(RasterModelDestinationP), transformationType_(
				TransformationTypeP) {}

/**
 * Destructor.
 */
GcpList::~GcpList() {
	// Release GCPs
	// for (int ix = 0, len = gcps_.size(); ix < len; ++ix)
		// delete gcps_[ix];
	gcps_.clear();
}

/**
 * Devuelve el sistema de referencia espacial de origen.
 *
 * @return String con el SR de origen.
 */
std::string GcpList::GetSpatialReferenceSource() const {
	return spatialReferenceSrc_;
}

/**
 * Devuelve el sistema de referencia espacial de destino.
 *
 * @return String con el SR de destino.
 */
std::string GcpList::GetSpatialReferenceDestination() const {
	return spatialReferenceDest_;
}

/**
 * Devuelve el modelo raster de origen.
 *
 * @return String con el MR de origen.
 */
std::string GcpList::GetRasterModelSource() const {
	return rasterModelSrc_;
}

/**
 * Devuelve el modelo raster de destino.
 *
 * @return String con el MR de destino.
 */
std::string GcpList::GetRasterModelDestination() const {
	return rasterModelDest_;
}

/**
 * Devuelve el tipo de transformacion
 */
std::string GcpList::GetTransformacionType() const {
	return transformationType_;
}

/**
 * Configura los datos para el Destination datasource.
 */
void GcpList::SetDestinationDatasource(std::string UrlP, std::string IdP) {
	destinationDatasourceUrl_ = UrlP;
	destinationDatasourceId_ = IdP;
}

/**
 * Obtiene la url para el Destination datasource.
 */
std::string GcpList::GetDestinationDatasourceUrl() const {
	return destinationDatasourceUrl_;
}

/**
 * Obtiene el id para el Destination datasource.
 */
std::string GcpList::GetDestinationDatasourceId() const {
	return destinationDatasourceId_;
}

/**
 * Configura los datos para el Source datasource.
 */
void GcpList::SetSourceDatasource(std::string UrlP, std::string IdP) {
	sourceDatasourceUrl_ = UrlP;
	sourceDatasourceId_ = IdP;
}

/**
 * Obtiene la url para el Source datasource.
 */
std::string GcpList::GetSourceDatasourceUrl() const {
	return sourceDatasourceUrl_;
}

/**
 * Obtiene el id para el Source datasource.
 */
std::string GcpList::GetSourceDatasourceId() const {
	return sourceDatasourceId_;
}

/**
 * Genera un vector de GCPs con las conversiones realizadas cuando corresponda.
 * Devuelve solo los puntos activos.
 * @return Un vertor con los GCPs listo para usar en la transformacion.
 */
std::vector<GroundControlPoint> GcpList::CreateAdaptedGcps(bool Inverse) const {
	std::vector<GroundControlPoint> returnVector;
	for (int ix = 0, len = gcps_.size(); ix < len; ++ix) {
		Coordinates src, dest;
		//if (gcps_[ix]->IsEnabled()) {
			if (!Inverse) {
				src = gcps_[ix].GetSource();
				dest = gcps_[ix].GetDestination();
			} else {
				dest = gcps_[ix].GetSource();
				src = gcps_[ix].GetDestination();
			}
			returnVector.push_back(suri::GroundControlPoint(src, dest));
		//}
	}
	return returnVector;
}

/**
 * Devuelve el vector de GCPs tal cual se levanto del origen de datos.
 */
std::vector<GroundControlPointExtended> GcpList::GetAsExtendedGcps() const {
	return gcps_;
}

/**
 * Agrega un nuevo GCP a la lista.
 *
 * @param[in] GcpP Punto de control a agregar.
 */
void GcpList::Add(const GroundControlPointExtended& pGcpP) {
	gcps_.push_back(pGcpP);
}

/**
 * Elimina un GCP de la lista.
 *
 * @param[in] IndexP Indice del punto de control a remover.
 */
void GcpList::Remove(int IndexP) {
	gcps_.erase(gcps_.begin() + IndexP);
}

/**
 * Devuelve un GCP de la lista.
 *
 * @param[in] IndexP Indice del elemento que se quiere obtener.
 * @return Punto de control.
 */
GroundControlPointExtended& GcpList::Get(int IndexP) {
	return gcps_[IndexP];
}

/**
 * Devuelve la cantidad de GCPs en la lista.
 *
 * @return Cantidad de elementos en la lista.
 */
unsigned int GcpList::Size() const {
	return gcps_.size();
}

/**
 * Informa si la lista esta vacia.
 *
 * @return true si la lista esta vacia.
 */
bool GcpList::Empty() const {
	return gcps_.empty();
}

/**
 * Informa si las coordenadas de origen estan en formato Raster.
 *
 * @return true si las coordenadas de origen estan en formato raster.
 */
bool GcpList::IsSourceInPixelLine() const {
	return srcInPixelLine_;
}

/**
 * Informa si las coordenadas de destino estan en formato Raster.
 *
 * @return true si las coordenadas de destino estan en formato raster.
 */
bool GcpList::IsDestinationInPixelLine() const {
	return destInPixelLine_;
}

/**
 * Indica el sistema de referencia espacial de origen.
 */
bool GcpList::SetSpatialReferenceSource(const std::string& SpatialReference) {
	bool valid = SpatialReference::IsValidSpatialReference(SpatialReference);
	if (valid)
		spatialReferenceSrc_ = SpatialReference;
	return valid;

}
/**
 * Indica el sistema de referencia espacial de destino.
 */
bool GcpList::SetSpatialReferenceDestination(
		const std::string& SpatialReference) {
	bool valid = SpatialReference::IsValidSpatialReference(SpatialReference);
	if (valid)
		spatialReferenceDest_ = SpatialReference;
	return valid;
}

/**
 * Indica el modelo raster de origen.
 */
bool GcpList::SetRasterModelSource(const std::string& RasterModel) {
	bool valid = false;
	RasterSpatialModel* praster = RasterSpatialModel::Create(RasterModel);
	if (praster != NULL){
		rasterModelSrc_ = RasterModel;
		RasterSpatialModel::Destroy(praster);
		valid = true;
	}
	return valid;
}

/**
 * Indica el modelo raster de destino.
 */
bool GcpList::SetRasterModelDestination(const std::string& RasterModel) {
	bool valid = false;
	RasterSpatialModel* praster = RasterSpatialModel::Create(RasterModel);
	if (praster != NULL){
		valid = true;
		rasterModelDest_ = RasterModel;
		RasterSpatialModel::Destroy(praster);
	}
	return valid;
}

/**
 * Indica el tipo de transformacion
 */
void GcpList::SetTransformacionType(const std::string& TransformationType) {
	transformationType_ = TransformationType;
}

/** Vacia la lista de puntos de control */
void GcpList::Clear() {
	gcps_.clear();
}

}  // namespace suri
