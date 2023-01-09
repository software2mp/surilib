/** \file Polygon.cpp */
/*!
 *  Biblioteca de procesamiento GeoEspacial SuriLib.
 *  Copyright 2011-2007 CONAE - SUR Emprendimientos Tecnologicos S.R.L.
 *
 *  Este producto contiene software desarrollado por
 *  SUR Emprendimientos Tecnologicos S.R.L. (http://www.suremptec.com/)
 *
 */

// Includes standard
#include <string>
#include <vector>

// Includes suri
#include "Polygon.h"

namespace suri {

/** Ctor */
Polygon::Polygon(const std::string &SpatialReference) : Geometry(SpatialReference) {
}

/** Ctor */
Polygon::Polygon(const Polygon &APolygon) : Geometry(APolygon) {
   RingIterator it = APolygon.ringVector_.begin();
   for (; it != APolygon.ringVector_.end(); ++it)
      AppendRing(new Ring(*(*it)));
}

/** Dtor */
Polygon::~Polygon() {
   std::vector<Ring*>::iterator it;
   for (it = ringVector_.begin(); it != ringVector_.end(); ++it)
      delete *it;
   ringVector_.clear();
}

/**
 * Indica si la geometria es valida
 * @return true si los anillos que contiene estan cerrados
 */
bool Polygon::IsValid() const {
   bool isvalid = true;
   RingIterator it = ringVector_.begin();
   for (; it != ringVector_.end(); ++it)
      isvalid = isvalid && (*it)->IsValid();
   return isvalid;
}

/** Retorna una copia de la geometria. Tambie copia subgeometrias */
Geometry* Polygon::Clone() const {
   Polygon* pnewpolygon = new Polygon(*this);
   return pnewpolygon;
}

/**
 * Configura el srs de la geometria
 * @param[in] SpatialReference srs a configurar
 */
void Polygon::AssignSpatialReference(const std::string &SpatialReference) {
   RingIterator it = ringVector_.begin();
   for (; it != ringVector_.end(); ++it)
      (*it)->AssignSpatialReference(SpatialReference);
   Geometry::AssignSpatialReference(SpatialReference);
}

/** Indica si la geometria esta cerrada, solo valida si es poligono */
bool Polygon::IsClosed() const {
   bool isclosed = true;
   RingIterator it = ringVector_.begin();
   for (; it != ringVector_.end() && isclosed; ++it)
      isclosed = (*it)->IsClosed();
   return isclosed;
}

/** Fuerza a que se cierre la geometria. Solo para poligonos */
bool Polygon::CloseRings() {
   return true;
}

/**
 * Agrega un punto al vector
 * @param[in] pPoint punto que quiero agregar.
 * \attention El punto pasa a ser responsabilidad de esta clase. No eliminar.
 */
bool Polygon::AppendRing(Ring* pRing) {
   if (!ValidateRing(pRing))
      return false;
   ringVector_.push_back(pRing);
   return true;
}

/**
 * Elimina un anillo del vector
 * @param[in] Id del anillo que quiero eliminar
 * @return bool que indica si se pudo eliminar el anillo
 */
bool Polygon::RemoveRing(SuriObject::UuidType RingId) {
   std::vector<Ring*>::iterator it = ringVector_.begin();
   while (it != ringVector_.end() && (*it)->GetId() != RingId)
      ++it;
   if (it != ringVector_.end()) {
      delete *it;
      ringVector_.erase(it);
   }
   return it != ringVector_.end();
}

/**
 * Retorla listado de anillos en vector
 * @return vector con el id de los puntos que contiene
 */
std::vector<SuriObject::UuidType> Polygon::GetRings() const {
   std::vector<SuriObject::UuidType> returnvector;
   RingIterator it = ringVector_.begin();
   for (; it != ringVector_.end(); ++it)
      returnvector.push_back((*it)->GetId());
   return returnvector;
}

/**
 * Retorna el anillo con el id indicado
 * @param[in] RingId id del anillo que busco
 * @return anillo que busco o NULL si no lo encuentra.
 */
Ring* Polygon::GetRing(SuriObject::UuidType RingId) {
   RingIterator it = FindRing(RingId);
   return (it != ringVector_.end()) ? *it : NULL;
}

/**
 * Retorna el anillo con el id indicado
 * @param[in] RingId id del anillo que busco
 * @return anillo que busco o NULL si no lo encuentra.
 */
const Ring* Polygon::GetRing(SuriObject::UuidType RingId) const {
   RingIterator it = FindRing(RingId);
   return (it != ringVector_.end()) ? *it : NULL;
}

/**
 * Busca un anillo a partir del id
 * @param[in] RindId id del anillo buscado
 * @return iterador apuntando a anillo con id correspondiente
 */
Polygon::RingIterator Polygon::FindRing(SuriObject::UuidType RingId) const {
   RingIterator it = ringVector_.begin();
   while (it != ringVector_.end() && (*it)->GetId() != RingId)
      ++it;
   return it;
}

/**
 * Valida el anillo
 * @param[in] pRing anillo a validar
 * @return bool que indica si se puede agregar el anillo al poligono
 */
bool Polygon::ValidateRing(const Ring* pRing) {
   if (!pRing)
      return false;
   RingIterator it = FindRing(pRing->GetId());
   return it == ringVector_.end() &&
            pRing->GetSpatialReference() == GetSpatialReference();
}

} /* namespace suri */
