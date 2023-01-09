/** \file GeometryCollection.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <vector>
#include <string>

// Includes suri
#include "GeometryCollection.h"

namespace suri {

/** Ctor */
GeometryCollection::GeometryCollection(const GeometryCollection &ACollection) : 
                                                        Geometry(ACollection) {
   std::vector<Geometry*>::const_iterator it = geometries_.begin();
   for (; it != ACollection.geometries_.end(); ++it)
      AppendGeometry((*it)->Clone());
}

/** Ctor */
GeometryCollection::GeometryCollection(const std::string &SpatialReference) :
                                                   Geometry(SpatialReference) {
}

/** Ctor */
GeometryCollection::~GeometryCollection () {
   Clear();
}

/**
 * Indica si la geometria es valida
 * @return true si todas las geometrias contenidas son validas.
 * @return false en otro caso.
 */
bool GeometryCollection::IsValid() const {
   bool isvalid = true;
   std::vector<Geometry*>::const_iterator it = geometries_.begin();
   for (; it != geometries_.end() && isvalid; ++it)
      isvalid = (*it)->IsValid();
   return isvalid;
}

/**
 * Retorna una copia de la geometria. Tambien copia subgeometrias
 * @return retorna una copia de la coleccion.
 */
Geometry* GeometryCollection::Clone() const {
   GeometryCollection* pcollection = new GeometryCollection(*this);
   return pcollection;
}

/**
 * Configura el srs de la geometria
 * @param[in] SpatialReference srs a configurar
 */
void GeometryCollection::AssignSpatialReference(const std::string &SpatialReference) {
   std::vector<Geometry*>::const_iterator it = geometries_.begin();
   for (; it != geometries_.end(); ++it)
      (*it)->AssignSpatialReference(SpatialReference);
   Geometry::AssignSpatialReference(SpatialReference);
}

/**
 * Indica si todas las geometrias contenidas estan cerradas.
 * @return true si todas las gemetrias contenidas estan cerradas
 * @return false en otro caso.
 */
bool GeometryCollection::IsClosed() const {
   bool isclosed = true;
   std::vector<Geometry*>::const_iterator it = geometries_.begin();
   for (; it != geometries_.end() && isclosed; ++it)
      isclosed = (*it)->IsClosed();
   return isclosed;
}

/** Elimina todas las geometrias contenidas */
void GeometryCollection::Clear() {
   std::vector<Geometry*>::iterator it = geometries_.begin();
   for (; it != geometries_.end(); ++it)
      delete *it;
   geometries_.clear();
}

/**
 * Retorna las geometrias desplegadas
 * @return vector con el id de las geomtrias contenidas.
 */
std::vector<SuriObject::UuidType> GeometryCollection::GetGeometries() const {
   std::vector<SuriObject::UuidType> geometries;
   std::vector<Geometry*>::const_iterator it = geometries_.begin();
   for (; it != geometries_.end(); ++it)
      geometries.push_back((*it)->GetId());
   return geometries;
}

/**
 * Retorna la geometria a partir de un id
 * @param[in] GeometryId id de la geometria que me interesa.
 * @return puntero a la geometria.
 */
Geometry* GeometryCollection::GetGeometry(SuriObject::UuidType GeometryId) const {
   std::vector<Geometry*>::const_iterator it = geometries_.begin();
   for (; it != geometries_.end() && (*it)->GetId() != GeometryId; ++it);
   return it != geometries_.end() ? *it : NULL;
}

/**
 * Saca la geometria con el id GeometryId(No elimina el puntero)
 * @param[in] GeometryId id de la geometria a remover.
 * @return referencia a la geometria removida
 * @return NULL si no pudo encontrar la geometria.
 */
Geometry* GeometryCollection::RemoveGeometry(SuriObject::UuidType GeometryId) {
   std::vector<Geometry*>::iterator it = geometries_.begin();
   for (; it != geometries_.end() && (*it)->GetId() != GeometryId; ++it);
   geometries_.erase(it);
   return it != geometries_.end() ? *it : NULL;
}

/**
 * Elimina la geometria con el id GeometryId
 * @param[in] GeoemtryId id de la geometria a eliminar
 * @return bool que indica si se pudo eliminar la geometria
 */
bool GeometryCollection::DeleteGeometry(SuriObject::UuidType GeometryId) {
   Geometry* pgeom = RemoveGeometry(GeometryId);
   delete pgeom;
   return pgeom == NULL;
}

/**
 * Agrega una geometria a la coleccion
 * @param[in] pGeometry geometria que quiero agregar
 * @param[in] ReferenceGeometryId id de la geometria siguiente a donde
 * quiero agregar pGeometry.
 * @return bool que indica si pudo agregar la geometria a la coleccion.
 */
bool GeometryCollection::InsertGeometryBefore(Geometry* pGeometry,
                                 SuriObject::UuidType ReferenceGeometryId) {
   if (!ValidateGeometry(pGeometry) || GetGeometry(pGeometry->GetId()) != NULL)
      return false;
   std::vector<Geometry*>::iterator it = geometries_.begin();
   for (; it != geometries_.end() && (*it)->GetId() != ReferenceGeometryId; ++it);
   if (it != geometries_.end())
      geometries_.insert(it, pGeometry);
   return it != geometries_.end();
}

/**
 * Agrega una geometria al final de la coleccion
 * @param[in] pGeometry geometria que quiero agregar
 * @return true si pudo agregar la geometria
 * @return false si la geometria ya existia en coleccion.
 */
bool GeometryCollection::AppendGeometry(Geometry* pGeometry) {
   if (!ValidateGeometry(pGeometry) || GetGeometry(pGeometry->GetId()) != NULL)
      return false;
   geometries_.push_back(pGeometry);
   return true;
}

/**
 * Valida si la geometria se puede agregar a la coleccion
 * @param[in] pGeometry geometria que quiero validar
 * @return bool que indica si la geometria se puede agregar.
 */
bool GeometryCollection::ValidateGeometry(const Geometry* pGeometry) {
   return pGeometry && pGeometry->GetSpatialReference() == GetSpatialReference();
}

} /* namespace suri */
